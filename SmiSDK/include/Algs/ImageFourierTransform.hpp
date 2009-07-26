// -*- C++ -*-
// © [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS TECHNICAL AND INTELLECTUAL PROPERTY OF
// SCIENCE.MEDICAL.IMAGING (groups.google.com/group/medicalimagingscience). 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND SCIENCE.MEDICAL.IMAGING GROUP
// MAKES NO WARRANTY REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY
// SPECIFIC USE. ENTIRE RISK TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	ImageFourierTransform.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageFourierTransform_HPP_
#define _PGImageFourierTransform_HPP_
//////////////////////////////////////////////////////////////////////////


#include "ImageFourierTransform.h"

namespace PGAlgs
{

	template <class T, class U>
	bool ImageFourierTransform<T, U>::Execute() 
	{

		if (!m_iDataObject) {
			LOG0("ImageFourierTransform: Error: no input");
			return false;
		}

		// get first image
		PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
		if (!inImage) {
			LOG0("ImageFourierTransform: Error: failure to fetch input image");
			return false;
		}

		long iRows=0, iColumns=0;
		inImage->GetDimensions(iRows, iColumns);
		if (iRows*iColumns <=0) {
			LOG0("ImageFourierTransform: Error: invalid image size");
			return false;
		}

		const T* inBuf = inImage->GetBuffer();
		if (!inBuf) 
		{
			LOG0("ImageFourierTransform: Error: invalid image");
			return false;
		}

		if (m_oImageObject == NULL)
			m_oImageObject = new PGCore::Image<U>(iRows, iColumns);
		U* outBuf = m_oImageObject->GetBuffer();
		if (!outBuf) 
		{
			LOG0("ImageFourierTransform: Error: invalid out image");
			return false;
		}

		// if inverse flag is set, pass in incoming real/imaginary images
		bool rv = false;
		if (m_inverseTransform)
		{
			PGCore::Image<U> pImageReal(iRows, iColumns), pImageImaginary(iRows, iColumns);
			//PGCore::Image<U> pImageReal = m_ImageReal, pImageImaginary = m_ImageImaginary;
			rv = performFFT2D(&m_ImageReal, &m_ImageImaginary, &pImageReal, &pImageImaginary);
			//rv = performFFT2D(&pImageReal, &pImageImaginary, &pImageReal, &pImageImaginary);
			m_ImageReal = pImageReal; m_ImageImaginary = pImageImaginary;
		} else 
		{
			PGCore::Image<U> pImageImaginary(iRows, iColumns);
			m_ImageReal.SetDimensions(iRows, iColumns);
			m_ImageImaginary.SetDimensions(iRows, iColumns);
			rv = performFFT2D(inImage, &pImageImaginary, &m_ImageReal, &m_ImageImaginary);
		}

		// this one doesnt work properly
		//bool rv = performDFT2D(inImage, &oImageReal, &oImageImaginary);					
		if (!rv) return false;

		// fill m_oImageObject now
		rv = rootSquaredSum(m_ImageReal, m_ImageImaginary, *m_oImageObject);
		if (!rv) return false;

		m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

		return rv;
	}

	template <class T, class U>
	bool ImageFourierTransform<T, U>::rootSquaredSum(const PGCore::Image<U> &iImage1, const PGCore::Image<U> &iImage2, 
			PGCore::Image<T> &oImage)
	{
		long nx1=0, ny1=0, nx2=0, ny2=0, nx3=0, ny3=0;
		iImage1.GetDimensions(ny1, nx1);
		iImage2.GetDimensions(ny2, nx2);
		oImage.GetDimensions(ny3, nx3);

		if (nx1!=nx2 || ny1!=ny2 || nx1!=nx3 || ny1!=ny3)
		{
			return false;
		}

		U *iBuf1 = (U *)iImage1.GetBuffer();
		U *iBuf2 = (U *)iImage2.GetBuffer();
		U *oBuf  = (U *)oImage.GetBuffer();
		
		if (!iBuf1 || !iBuf2 || !oBuf)
			return false;

		long imSize = nx1*ny1, i=0, offsetSrc=0, offsetDst=0;
		int rSrc=0, cSrc=0;

		int fX=-nx1/2, fY=-ny1/2;
		
		fX = m_inverseTransform ? 0 : -nx1/2;
		fY = m_inverseTransform ? 0 : -ny1/2;

		//while (i<imSize)
		for (int r=0; r<ny1; r++)
		{
			rSrc = abs(r+fY);//m_inverseTransform ? abs(r+fY) : abs(r+fY);
			for (int c=0; c<nx1; c++)
			{
				cSrc = abs(c+fX);//m_inverseTransform ? abs(c+fX) : abs(c+fX);
				offsetSrc = rSrc*nx1 + cSrc;
				offsetDst = r*nx1 + c;

				double iSqVal = (double)(iBuf1[offsetSrc]*iBuf1[offsetSrc]) + 
					(double)(iBuf2[offsetSrc]*iBuf2[offsetSrc]);
				oBuf[offsetDst] =  m_inverseTransform ? (U)( sqrt(iSqVal) + 0.5f) :
						(U)( log(1 + sqrt(iSqVal)) + 0.5f);
				//i++;

			}
		}

		oImage.Update();
		return true;
	}


	/*-------------------------------------------------------------------------
	Perform a 2D FFT inplace given a complex 2D array
	The direction dir, 0 for forward, 1 for reverse
	The size of the array (nx,ny)
	Return false if there are memory problems or
	the dimensions are not powers of 2
	*/

	template <class T, class U>
	bool ImageFourierTransform<T, U>::performFFT2D(PGCore::Image<T> *iInImageReal, 
		PGCore::Image<T> *iInImageImaginary, 
		PGCore::Image<U> *oImageReal,
		PGCore::Image<U> *oImageImaginary)
		//int FFT2D(COMPLEX **c,int nx,int ny,int dir)
	{
		if (!iInImageReal || !oImageReal || !oImageImaginary || !iInImageImaginary)
			return false;

		long nx=0, ny=0;
		iInImageReal->GetDimensions(ny, nx);

		int m=0;
		if (!(IsPowerOfTwo(nx, m) || IsPowerOfTwo(ny, m)))
		{
			return false;
		}

		T* iBufReal = iInImageReal->GetBuffer();
		T* iBufImaginary = iInImageImaginary->GetBuffer();
		if (!iBufReal || !iBufImaginary)
			return false;

		
		U* oBufReal = oImageReal->GetBuffer();
		U* oBufImaginary = oImageImaginary->GetBuffer();
		if (!oBufReal || !oBufImaginary)
			return false;

		int i,j;
		//int twopm;

		/* Transform the rows */
		long unitDim = 1;
		PGCore::Image<U> tempBufferReal(unitDim, nx), tempBufferImaginary(unitDim, nx);
		U *real = tempBufferReal.GetBuffer(), *imaginary = tempBufferImaginary.GetBuffer();
		if (!real || !imaginary) return false;

		if (IsPowerOfTwo(nx, m))
		{

			for (j=0;j<ny;j++) 
			{
				for (i=0;i<nx;i++) 
				{
					real[i] = (U)(iBufReal[j*nx + i]);         
					imaginary[i] = (U)(iBufImaginary[j*nx + i]);         
				}
				
				if (!performFFT1D(nx, real, imaginary))
					return false;

				for (i=0;i<nx;i++) 
				{
					oBufReal[j*nx + i] = (U)(real[i]);
					oBufImaginary[j*nx + i] = (U)(imaginary[i]);
				}
			}
		}


		/* Transform the columns */

		tempBufferReal.SetDimensions(unitDim, ny);
		tempBufferImaginary.SetDimensions(unitDim, ny);
		real = tempBufferReal.GetBuffer();
		imaginary = tempBufferImaginary.GetBuffer();
		if (!real || !imaginary) return false;

		if (IsPowerOfTwo(ny, m))
		{
			for (i=0;i<nx;i++) 
			{
				for (j=0;j<ny;j++) 
				{
					real[j] = (U)(oBufReal[j*nx + i]);                  
					imaginary[j] = (U)(oBufImaginary[j*nx + i]);                  
				}

				if (!performFFT1D(ny, real, imaginary))
					return false;

				for (j=0;j<ny;j++) 
				{
					oBufReal[j*nx + i] = (U)(real[j]);
					oBufImaginary[j*nx + i] = (U)(imaginary[j]);
				}
			}
		}
		return true;
	}



	/*-------------------------------------------------------------------------
	This computes an in-place complex-to-complex FFT
	x and y are the real and imaginary arrays of 2^m points.
	dir =  1 gives forward transform
	dir = -1 gives reverse transform

	Formula: forward
	N-1
	---
	1   \          - j k 2 pi n / N
	X(n) = ---   >   x(k) e                    = forward transform
	N   /                                n=0..N-1
	---
	k=0

	Formula: reverse
	N-1
	---
	\          j k 2 pi n / N
	X(n) =       >   x(k) e                    = forward transform
	/                                n=0..N-1
	---
	k=0
	*/
	/*
	This computes an in-place complex-to-complex FFT 
	x and y are the real and imaginary arrays of 2^m points.
	dir =  1 gives forward transform
	dir = -1 gives reverse transform 
	*/
	template <class T, class U>
	bool ImageFourierTransform<T, U>::performFFT1D(int iElements, U *ioBufferReal, U* ioBufferImaginary)
	{
		int m=0;
		if (!IsPowerOfTwo(iElements, m))
		{
			return false; // need a power of 2
		}

		long n,i,i1,j,k,i2,l,l1,l2;
		double c1,c2,tx,ty,t1,t2,u1,u2,z;

		/* Calculate the number of points */
		n=iElements;//1, m=0;
		

		/* Do the bit reversal */
		i2 = n > > 1;
		j = 0;
		for (i=0;i<n-1;i++) 
		{
			if (i < j) 
			{
				tx = ioBufferReal[i];
				ty = ioBufferImaginary[i];
				ioBufferReal[i] = ioBufferReal[j];
				ioBufferImaginary[i] = ioBufferImaginary[j];
				ioBufferReal[j] = (U)tx;
				ioBufferImaginary[j] = (U)ty;
			}
			k = i2;
			while (k <= j) 
			{
				j -= k;
				k > >= 1;
			}
			j += k;
		}

		/* Compute the FFT */
		c1 = -1.0; 
		c2 = 0.0;
		l2 = 1;
		for (l=0;l<m;l++) 
		{
			l1 = l2;
			l2 <<= 1;
			u1 = 1.0; 
			u2 = 0.0;
			for (j=0;j<l1;j++) 
			{
				for (i=j;i<n;i+=l2) 
				{
					i1 = i + l1;
					t1 = u1 * ioBufferReal[i1] - u2 * ioBufferImaginary[i1];
					t2 = u2 * ioBufferReal[i1] + u1 * ioBufferImaginary[i1];
					ioBufferReal[i1] = ioBufferReal[i] - (U)t1; 
					ioBufferImaginary[i1] = ioBufferImaginary[i] - (U)t2;
					ioBufferReal[i] += (U)t1;
					ioBufferImaginary[i] += (U)t2;
				}
				z =  u1 * c1 - u2 * c2;
				u2 = u1 * c2 + u2 * c1;
				u1 = z;
			}
			c2 = sqrt((1.0 - c1) / 2.0);
			if (!m_inverseTransform) 
				c2 = -c2;
			c1 = sqrt((1.0 + c1) / 2.0);
		}

		/* Scaling for forward transform */
		if (!m_inverseTransform) 
		{
			for (i=0;i<n;i++) 
			{
				ioBufferReal[i] /= (U)n;
				ioBufferImaginary[i] /= (U)n;
			}
		}

		return true;
	}

};


/*
   Direct fourier transform
*/
	/*
template <class T, class U>
bool ImageFourierTransform<T, U>::performDFT1D(int iElements, U *ioBufferReal, U *ioBufferImaginary)
{
	 long i,k;
   double arg;
   double cosarg,sinarg;

   int unitDim = 1;
    PGCore::Image<U> tempBuffer(unitDim, iElements);
	U *real = tempBuffer.GetBuffer();
	if (!real) return false;

   //double *x2=NULL,*y2=NULL;

   //x2 = malloc(m*sizeof(double));
   //y2 = malloc(m*sizeof(double));
   //if (x2 == NULL || y2 == NULL)
   //   return(FALSE);

   int dir = (!m_inverseTransform) ? 1 : 0;
   for (i=0;i<iElements;i++) {
      real[i] = 0;
      //y2[i] = 0;
      arg = - dir * 2.0f * PG_PI * (double)i / (double)iElements;
      for (k=0;k<iElements;k++) {
         cosarg = cos(k * arg);
         sinarg = sin(k * arg);
         real[i] += (ioBuffer[k] * cosarg);// - y1[k] * sinarg);
         //y2[i] += (x1[k] * sinarg + y1[k] * cosarg);
      }
   }

   //Copy the data back 
   if (dir == 1) 
   {
      for (i=0;i<iElements;i++) {
         ioBuffer[i] = real[i]/(double)iElements;
         //y1[i] = y2[i] / (double)m;
      }
   } else
	   {
      for (i=0;i<iElements;i++) {
         ioBuffer[i] = real[i];
         //y1[i] = y2[i] / (double)m;
      }
   }

   


		return true;
}

template <class T, class U>
	bool ImageFourierTransform<T, U>::performDFT2D(PGCore::Image<T> *iInImage, PGCore::Image<U> *oImageReal,
		PGCore::Image<U> *oImageImaginary)
		//int FFT2D(COMPLEX **c,int nx,int ny,int dir)
	{
		if (!iInImage || !oImage)
			return false;

		long nx=0, ny=0;
		iInImage->GetDimensions(ny, nx);

		//if (nx%2 || ny%2)
		int m=0;
		//if (!(IsPowerOfTwo(nx, m) || IsPowerOfTwo(ny, m)))
		{
		//	return false;
		}

		T* iBuf = iInImage->GetBuffer();
		if (!iBuf)
			return false;

		//oImage->SetDimensions(ny, nx);
		//*oImage = *iInImage;


		U* oBuf = oImage->GetBuffer();
		if (!oBuf)
			return false;

		int i,j;
		int twopm;

		// Transform the rows 
		long unitDim = 1;
		PGCore::Image<U> tempBuffer(unitDim, nx);
		U *real = tempBuffer.GetBuffer();
		if (!real) return false;

		//if (IsPowerOfTwo(nx, m))
		{

			for (j=0;j<ny;j++) 
			{
				for (i=0;i<nx;i++) 
				{
					real[i] = (U)(iBuf[j*nx + i]);         
				}

				if (!performDFT1D(nx, real))
					return false;

				for (i=0;i<nx;i++) 
				{
					oBuf[j*nx + i] = (U)(real[i]);
				}
			}
		}
	
		//return true;

		// Transform the columns 

		tempBuffer.SetDimensions(unitDim, ny);
		real = tempBuffer.GetBuffer();
		if (!real) return false;

		//if (IsPowerOfTwo(ny, m))
		{
			for (i=0;i<nx;i++) 
			{
				for (j=0;j<ny;j++) 
				{
					real[j] = (U)(oBuf[j*nx + i]);                  
				}

				if (!performDFT1D(ny, real))
					return false;

				for (j=0;j<ny;j++) 
				{
					oBuf[j*nx + i] = (U)(real[j]);
				}
			}
		}
		return true;
	}

};

*/

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageFourierTransform_HPP_
