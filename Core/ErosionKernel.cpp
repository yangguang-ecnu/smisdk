// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	ErosionKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_ErosionKernel_HPP_
#define _PG_ErosionKernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/ErosionKernel.h"

// http://www.cee.hw.ac.uk/hipr/html/Erode.html
// http://www.cee.hw.ac.uk/hipr/html/erode.html


/*
		// Example
		PGCore::ErosionKernel<char, T> erosionKernel(1);
		{			
			PGAlgs::ImageBinarizer<T, char> binarizer;
			binarizer.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			binarizer.SetThresholdRange(900, 1024);
			binarizer.Execute();
	
			binarizer.GetOutput(static_cast<PGCore::BitImage *>(&bImage));
			bImage.UnPack(nextImage);

			char fileName[256] = {0};
			_snprintf(fileName, 255, "C:\\Data\\Dump\\OriginalImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));
			PGCore::Image<T> sImage;
			erosionKernel.Convolve(nextImage, sImage);	
			_snprintf(fileName, 255, "C:\\Data\\Dump\\DilatedImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(sImage, std::string(fileName));
		}
*/
namespace PGCore
{
	
/******public
 * NAME
 *    ErosionKernel
 * SYNOPSIS
 *    ErosionKernel<T, U>::ErosionKernel(const double iSigma, const int iDimension) 
 * DESCRIPTION
 *    Class constructor
 * PARAMETERS
 *	  iSigma		:	Standard deviation value. Double precision. Optional. Default value: 1.0f
 *	  iDimension	:	Spread of the kernel in one dimension. Integer. Optional. Default value: -1.
 *							If iDimension==-1, the dimension is computed off the given sigma value.
 * OUTPUT
 *    An instance of the class
 * RETURN VALUE
 *    None.
 * EXAMPLE
 *    ErosionKernel<double, double> gKernel(1.5f, 64);    
 ***/
	template <class T, class U>
	ErosionKernel<T, U>::ErosionKernel(const int iRadius/*=1*/)
	{	
		m_radius = iRadius;
		m_separable = false;

		// compute default dimension
		unsigned int pDimension = 1+2*m_radius;	

		// set dimensions
		SetDimension(pDimension);	

		// initialize the buffer, fill kernel values
		Initialize_();
	}

	
/******public
 * NAME
 *    ~ErosionKernel
 * SYNOPSIS
 *    ErosionKernel<T, U>::~ErosionKernel(void) 
 * DESCRIPTION
 *    Class destructor
 * PARAMETERS
 *	  None.
 * OUTPUT
 *    None.
 * RETURN VALUE
 *    None.
 * EXAMPLE
 *     
 ***/	
	template <class T, class U>
	ErosionKernel<T, U>::~ErosionKernel(void)
	{	
		// no memory allocated, nothing to delete
	}

	
/******public
 * NAME
 *    ErosionKernel
 * SYNOPSIS
 *    ErosionKernel<T, U>::ErosionKernel(const ErosionKernel<T, U>& iKernelObject) 
 * DESCRIPTION
 *    Copy constructor
 * PARAMETERS
 *	  iKernelObject	:	an instance of a ErosionKernel object
 * OUTPUT
 *    None.
 * RETURN VALUE
 *    None.
 * EXAMPLE
 *    ErosionKernel<double, double> gKernel1(1.5f, 32); 
 *	  ErosionKernel<double, double> gKernel2(gKernel1); 
 ***/	
	template <class T, class U>
	ErosionKernel<T, U>::ErosionKernel(const ErosionKernel<T, U>& iKernelObject) 
	{
		// use assignment operator
		*this = iKernelObject;
	}


/******public
 * NAME
 *    operator=
 * SYNOPSIS
 *    ErosionKernel<T, U> &ErosionKernel<T, U>::operator=(const ErosionKernel<T, U> &iKernelObject) 
 * DESCRIPTION
 *    Assignment operator
 * PARAMETERS
 *	  iKernelObject	:	an instance of a ErosionKernel object
 * OUTPUT
 *    None.
 * RETURN VALUE
 *    An instance of the class.
 * EXAMPLE
 *    ErosionKernel<double, double> gKernel1(1.5f, 32); 
 *	  ErosionKernel<double, double> gKernel2 = gKernel1; 
 ***/	
	template <class T, class U>
	ErosionKernel<T, U> &ErosionKernel<T, U>::operator=(const ErosionKernel<T, U> &iKernelObject) 
	{	
		unsigned int iDimension = iKernelObject.GetDimension();		

		if(SetDimension(iDimension)) 
		{
			memcpy(m_buffer, iKernelObject.GetBuffer(), iDimension*iDimension*sizeof(float));					
		}

		return *this;
	}


/******protected
 * NAME
 *    Initialize
 * SYNOPSIS
 *    bool ErosionKernel<T, U>::Initialize_()
 * DESCRIPTION
 *    Initialization method. It computes and fills in the kernel values.
 * PARAMETERS
 *	  iKernelObject	:	an instance of a ErosionKernel object
 * OUTPUT
 *    None.
 * RETURN VALUE
 *    An instance of the class.
 * EXAMPLE
 *     
 ***/
	template <class T, class U>
	bool ErosionKernel<T, U>::Initialize_()
	{
		if (m_radius<=0.0f)
		{
			return false;
		}

		// fill up the buffer here
		int i=0;

		// fill circle

		
		/*for (i=0;i<(m_dimension);i++) 
		{
			SetValue(0, i, 1);
		}*/
		
		bool res = drawCircle(m_dimension/2, m_dimension/2, m_radius, m_buffer);
		if (!res) return false;

#ifdef _DEBUG
		if (0)
		{
			LOG1("The filter coefficients are (radius: %d):", m_radius);
			for(int j=0;j<(m_dimension);j++)
			{
				LOG0("\n");
				for(i=0;i<(m_dimension);i++)
					LOG3("Kernel[%d][%d] = %f", j, i, (double)GetValue(j, i));
			}
		}
#endif
		return true;
	}



	template <class T, class U>
	bool ErosionKernel<T, U>::drawCircle(int iXc, int iYc, int iRadius, float* ioBuf)
	{
		if (!ioBuf) return false;

		memset(ioBuf, 0, m_dimension*m_dimension*sizeof(float));
		if (m_dimension==3)
		{
						  ioBuf[1] = 1;
			ioBuf[3] = 1; ioBuf[4] = 1; ioBuf[5] = 1;
						  ioBuf[7] = 1;
			
			return true;

		} else
		if (m_dimension==5)
		{
											ioBuf[2] = 1;
							ioBuf[6] = 1;	ioBuf[7] = 1;	 ioBuf[8] = 1;
			ioBuf[10] = 1;	ioBuf[11] = 1;	ioBuf[12] = 1;   ioBuf[13] = 1;   ioBuf[14] = 1;
							ioBuf[16] = 1;	ioBuf[17] = 1;	 ioBuf[18] = 1;
											ioBuf[22] = 1;			
			return true;
		}


		int x=0, y=iRadius;				

		ioBuf[m_dimension*iYc + iXc] = 1;

		double p=1-iRadius;

		while(x<y)
		{
			if(p<0)
			{
				x=x+1;
				p=p+2*x+1;
			}
			else
			{
				x=x+1;
				y=y-1;
				p=p+2*(x-y)+1;
			}
			ioBuf[m_dimension*x + y] = 1;			
		}
		return true;
	}


	template <class T, class U>
	bool ErosionKernel<T, U>::convolve(const Image<T>& iImage, Image<U>& oImage) const
	{	
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, cc, rr, center;            

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		PGCore::PixelBase<T> tPixel;
		T minVal = tPixel.GetMaxValue(), maxVal = tPixel.GetMinValue();
		if (!iImage.GetDataRange(&minVal, &maxVal))
		{
			return false;
		}

		Image<U> tempImage(rows, cols);
		//tempImage = iImage;

		const U* iBuf	= (const U*)iImage.GetBuffer();
		U* tempBuf = (U *)tempImage.GetBuffer();
		bool okErode=false;
		long totalDim = rows*cols;
		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++)
		{
			for(c=0;c<cols;c++)
			{
				okErode=false;
				for(rr=(-center);rr<=center;rr++)
				{
					for(cc=(-center);cc<=center;cc++)
					{
						//if(((c+cc) >= 0) && ((c+cc) < cols) && ((r+rr) >= 0) && ((r+rr) < rows))
						{
							long iIndex = (r+rr)*cols+(c+cc);							
							if (iIndex>=0 && iIndex<totalDim)
							{
								if (m_buffer[(center+rr)*m_dimension + center+cc]) 
									//&& iBuf[iIndex]==minVal)
								{
									okErode=true;
									break;
								}
							}
						}
					}
				}

				// it is not separable

				if (okErode)
				{
					for(rr=(-center);rr<=center;rr++)
					{
						for(cc=(-center);cc<=center;cc++)
						{
							long iIndex = (r+rr)*cols+(c+cc);							
							if (iIndex>=0 && iIndex<totalDim)
							{
								if (m_buffer[(center+rr)*m_dimension + center+cc] )
									tempBuf[iIndex] = minVal;
							}
						}
					}
				} else 
				{
					tempBuf[(r)*cols+(c)] = iBuf[(r)*cols+(c)];
				}
			}
		}		

		// copy
		oImage = tempImage;

		//LOG1("End Convolve %d.", &iImage);
		return true;
	}


	
	template <class T, class U>
	bool ErosionKernel<T, U>::convolve(const BitImage& iImage, BitImage& oImage) const
	{	
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, cc, rr, center;            

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		
		BitImage tempImage(rows, cols);
		//tempImage = iImage;

		//const U* iBuf	= (const U*)iImage.GetBuffer();
		//U* tempBuf = (U *)tempImage.GetBuffer();
		bool okErode=false;
		long totalDim = rows*cols;
		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++)
		{
			for(c=0;c<cols;c++)
			{
				okErode=false;
				for(rr=(-center);rr<=center;rr++)
				{
					for(cc=(-center);cc<=center;cc++)
					{
						if(((c+cc) >= 0) && ((c+cc) < cols) && ((r+rr) >= 0) && ((r+rr) < rows))
						{
							//long iIndex = (r+rr)*cols+(c+cc);							
							//if (iIndex>=0 && iIndex<totalDim)
							{
								if (m_buffer[(center+rr)*m_dimension + center+cc]>0 
									&& iImage.GetValue(r+rr, c+cc)==false)
								{
									okErode=true;
									break;
								}
							}
						}
					}
				}

				// it is not separable

				if (okErode)
				{
					for(rr=(-center);rr<=center;rr++)
					{
						for(cc=(-center);cc<=center;cc++)
						{
							//long iIndex = (r+rr)*cols+(c+cc);							
							//if (iIndex>=0 && iIndex<totalDim)
							if(((c+cc) >= 0) && ((c+cc) < cols) && ((r+rr) >= 0) && ((r+rr) < rows))
							{
								if (m_buffer[(center+rr)*m_dimension + center+cc] )
									//tempBuf[iIndex] = minVal;
									tempImage.SetValue(r+rr, c+cc, false);
							}
						}
					}
				} else 
				{
					tempImage.SetValue(r, c, iImage.GetValue(r, c));					
					//tempImage.SetValue(r, c, false);
					//tempBuf[(r)*cols+(c)] = iBuf[(r)*cols+(c)];
				}
			}
		}		

		// copy
		oImage = tempImage;

		//LOG1("End Convolve %d.", &iImage);
		return true;
	}



	
#ifdef _PG_GENERATE_SDK_LIBS_
	template class ErosionKernel<unsigned short, unsigned short>; 
	template class ErosionKernel<short, short>; 
	template class ErosionKernel<unsigned char, unsigned char>; 
	template class ErosionKernel<char, char>; 
	template class ErosionKernel<char, unsigned short>; 
	template class ErosionKernel<char, unsigned char>; 
	//template class DilationKernel<double, double>; 
	template class ErosionKernel<char, short>; 
	//template class DilationKernel<double, short>; 
	template class ErosionKernel<char, PGMath::Point3D<unsigned char>>; 
	template class ErosionKernel<char, PGMath::Point3D<unsigned short>>; 
#endif


};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_ErosionKernel_HPP_