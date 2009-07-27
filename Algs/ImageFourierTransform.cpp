// -*- C++ -*-
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License. You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// More details about the license can be found here: http://www.apache.org/licenses
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is 
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
//
// Filename:	ImageFourierTransform.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//
// Comments:	http://www.dspguide.com/ch24/5.htm

#ifndef _PGImageFourierTransform_HPP_
#define _PGImageFourierTransform_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageFourierTransform.h"

namespace PGAlgs
{


	template <class T, class U>
	ImageFourierTransform<T, U>::ImageFourierTransform() 
	{ 
		m_oImageObject = 0; 
		SetInversionFlag(false); 
	};


	template <class T, class U>
	void ImageFourierTransform<T, U>::SetInversionFlag(bool iInverseTransform) 
	{ 
		m_inverseTransform = iInverseTransform; 
	}


	template <class T, class U>
	bool ImageFourierTransform<T, U>::GetInversionFlag() const 
	{ 
		return m_inverseTransform; 
	}


	template <class T, class U>
	bool ImageFourierTransform<T, U>::GetFourierImages(PGCore::Image<U> &oImageReal, PGCore::Image<U> &oImageImaginary) const
	{
		oImageReal = m_ImageReal;
		oImageImaginary = m_ImageImaginary;
		return true;
	}


	template <class T, class U>
	bool ImageFourierTransform<T, U>::SetFourierImages(const PGCore::Image<U> &iImageReal, const PGCore::Image<U> &iImageImaginary)
	{
		m_ImageReal = iImageReal;
		m_ImageImaginary = iImageImaginary;
		return true;
	}

	template <class T, class U>
	ImageFourierTransform<T, U>::~ImageFourierTransform(void) 
	{ 
		if (m_oImageObject) 
		{
			delete m_oImageObject;
			m_oImageObject = 0;
		}
	}

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
			PGCore::Image<T> pImageReal(iRows, iColumns), pImageImaginary(iRows, iColumns);
			//PGCore::Image<U> pImageReal = m_ImageReal, pImageImaginary = m_ImageImaginary;
			rv = performFFT2DInv(&m_ImageReal, &m_ImageImaginary, &pImageReal, &pImageImaginary);
			//rv = performFFT2D(&pImageReal, &pImageImaginary, &pImageReal, &pImageImaginary);

			if (!rv) return false;

			//m_ImageReal = pImageReal; m_ImageImaginary = pImageImaginary;
			// fill m_oImageObject now
			rv = rootSquaredSumInv(pImageReal, pImageImaginary, *m_oImageObject);
			if (!rv) return false;

		} else 
			
		{
			PGCore::Image<T> pImageImaginary(iRows, iColumns);
			m_ImageReal.SetDimensions(iRows, iColumns);
			m_ImageImaginary.SetDimensions(iRows, iColumns);
			rv = performFFT2D(inImage, &pImageImaginary, &m_ImageReal, &m_ImageImaginary);

			if (!rv) return false;

			// fill m_oImageObject now
			rv = rootSquaredSum(m_ImageReal, m_ImageImaginary, *m_oImageObject);
			if (!rv) return false;
		}

		// this one doesnt work properly
		//bool rv = performDFT2D(inImage, &oImageReal, &oImageImaginary);					
		

		m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

		return rv;
	}

	template <class T, class U>
	bool ImageFourierTransform<T, U>::rootSquaredSum(const PGCore::Image<U> &iImage1, const PGCore::Image<U> &iImage2, 
		PGCore::Image<U> &oImage)
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

	template <class T, class U>
	bool ImageFourierTransform<T, U>::rootSquaredSumInv(const PGCore::Image<T> &iImage1, const PGCore::Image<T> &iImage2, 
		PGCore::Image<U> &oImage)
	{
		long nx1=0, ny1=0, nx2=0, ny2=0, nx3=0, ny3=0;
		iImage1.GetDimensions(ny1, nx1);
		iImage2.GetDimensions(ny2, nx2);
		oImage.GetDimensions(ny3, nx3);

		if (nx1!=nx2 || ny1!=ny2 || nx1!=nx3 || ny1!=ny3)
		{
			return false;
		}

		T *iBuf1 = (T *)iImage1.GetBuffer();
		T *iBuf2 = (T *)iImage2.GetBuffer();
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

				double iSqVal = (double)(iBuf1[offsetSrc]*iBuf1[offsetSrc]);// + 
					//(double)(iBuf2[offsetSrc]*iBuf2[offsetSrc]);
				iSqVal = sqrt(iSqVal);
				iSqVal = iSqVal > 255 ? 255 : iSqVal;
				iSqVal = iSqVal < 0 ? 0 : iSqVal;

				oBuf[offsetDst] =  m_inverseTransform ? (U)( (iSqVal) + 0.5f) :
					(U)( log(1 + (iSqVal)) + 0.5f);
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
	Perform a 2D FFT inplace given a complex 2D array
	The direction dir, 0 for forward, 1 for reverse
	The size of the array (nx,ny)
	Return false if there are memory problems or
	the dimensions are not powers of 2
	*/

	template <class T, class U>
	bool ImageFourierTransform<T, U>::performFFT2DInv(PGCore::Image<U> *iInImageReal, 
		PGCore::Image<U> *iInImageImaginary, 
		PGCore::Image<T> *oImageReal,
		PGCore::Image<T> *oImageImaginary)		
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

		U* iBufReal = iInImageReal->GetBuffer();
		U* iBufImaginary = iInImageImaginary->GetBuffer();
		if (!iBufReal || !iBufImaginary)
			return false;


		T* oBufReal = oImageReal->GetBuffer();
		T* oBufImaginary = oImageImaginary->GetBuffer();
		if (!oBufReal || !oBufImaginary)
			return false;

		int i,j;
		
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
		i2 = n >> 1;
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
				k >>= 1;
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
		float mFac = m_inverseTransform ? float (n) : 1.0f/float(n);
		if (!m_inverseTransform) 
		{
			for (i=0;i<n;i++) 
			{
				ioBufferReal[i] *= mFac;///= (U)n;
				ioBufferImaginary[i] *= mFac;///= (U)n;
			}
		}

		return true;
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageFourierTransform<short, short>; 
	template class ImageFourierTransform<short, float>; 
	template class ImageFourierTransform<float, float>; 
	template class ImageFourierTransform<unsigned char, unsigned char>; 
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageFourierTransform_HPP_
