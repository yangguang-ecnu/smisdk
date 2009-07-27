// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
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
// Filename:	Kernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Kernel_HPP_
#define _PG_Kernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/Kernel.h"

//template PGCore::Kernel<short, short>;
//template PGCore::Kernel<char, char>;

namespace PGCore
{	

/******public
 * NAME
 *    Kernel
 * SYNOPSIS
 *    Kernel<T, U>::Kernel() 
 * DESCRIPTION
 *    Class constructor
 * PARAMETERS
 *	  None.
 * OUTPUT
 *    It is an abstract class, cannot be instantiated.
 * RETURN VALUE
 *    None.
 * EXAMPLE
 *    It is an abstract class, cannot be instantiated.
 ***/
	template <class T, class U>
	Kernel<T, U>::Kernel()
	{		
		m_type = kPgDataObjectTypeKernel;
		m_separable = false;
		//m_kernelType = kPgKernelType1D;
	}


	
/******public
 * NAME
 *    GetBuffer
 * SYNOPSIS
 *    Kernel<T, U>::GetBuffer() const
 * DESCRIPTION
 *    Returns address of the kernel's buffer in memory.
 * PARAMETERS
 *	  None.
 * OUTPUT
 *    Address of the kernel's buffer in memory.
 * RETURN VALUE
 *    A pointer of type T.
 * EXAMPLE
 *    const T* oKernelPtr = gKernel.GetBuffer();
 ***/
	template <class T, class U>
	float* Kernel<T, U>::GetBuffer() const {
		return m_buffer;
	}


	//#define IN_CIRCLE( x ) { return (T)(x+0.5f) > (T)(x)  ?  (T)(x+0.5f) : (T)(x) ; }
	//#define IN_CIRCLE( DIAMETER, ROW, COL, INNERRADIUS ) { return 1; };
	//( ROW - DIAMETER*0.5f ) < ( INNERRADIUS*INNERRADIUS ) ; }; 
	//\
	//( (double)ROW - (double)DIAMETER*0.5f) * (double)ROW - (double)DIAMETER*0.5f)) < ( (double)(INNERRADIUS*INNERRADIUS) ) \
	//	?	1 : 0; \
	//)

		
/******public
 * NAME
 *    GetBuffer
 * SYNOPSIS
 *    Kernel<T, U>::GetBuffer() const
 * DESCRIPTION
 *    Returns address of the kernel's buffer in memory.
 * PARAMETERS
 *	  None.
 * OUTPUT
 *    Address of the kernel's buffer in memory.
 * RETURN VALUE
 *    A pointer of type T.
 * EXAMPLE
 *    const T* oKernelPtr = gKernel.GetBuffer();
 ***/
	template <class T, class U>
	Kernel<T, U>& Kernel<T, U>::operator-=(const Kernel<T, U>& iKernel)
	{
		if (iKernel.GetDimension() != m_dimension
				|| iKernel.IsSeparable()!=m_separable
				|| !iKernel.GetBuffer())
			return *this;

		const float *iBuf = iKernel.GetBuffer();
		int i=0, kSize=m_dimension;
		
		if (!m_separable) kSize = m_dimension*m_dimension;

		while (i<kSize)
		{
			m_buffer[i] -= iBuf[i];
			i++;
		}

		return *this;
	}

/*
	template <class T, class U>
	bool Kernel<T, U>::Plot(Image<U>& oImage, unsigned int iScale) const
	{
		if (iScale<1)
		{
			return false;
		}

		long rows=m_dimension*iScale, cols=m_dimension*iScale;
		if (!oImage.SetDimensions(rows, cols))
		{
			return false;
		}

		U* oBuf			= (U*)oImage.GetBuffer();
		if (!oBuf)
		{
			return false;
		}		
		
		bool res = false;
		if (m_separable)
		{
			int i=0;
			T minValue = 99, maxValue = -99;
			while (i<m_dimension)
			{
				T iVal = m_buffer[i];
				minValue = minValue > iVal ? iVal : minValue;
				maxValue = maxValue < iVal ? iVal : maxValue;
				i++;
			}

			float iRange= (float)(maxValue-minValue), invRange=1.0f;
			if (iRange>0.0f)
				invRange = 1.0f/iRange;

			for (i=0; i<cols; i++)
			{
				int oHeight = (int)((float)rows*(float)(1.0f - invRange*(m_buffer[i/iScale] - minValue)));
				oBuf[oHeight*cols + i] =  (U)255;	
			}
			res = true;			
			
		} else
		{			
			res = Render(oImage, iScale);
		}		

#ifdef _DEBUG
		//PGAlgs::DumpImageAsPGM(oImage, std::string("C:\\Data\\KernelPlot.pgm"));
#endif

		return res;
	}



	template <class T, class U>
	bool Kernel<T, U>::Render(Image<U>& oImage, unsigned int iScale) const
	{
		if (iScale<1)
		{
			return false;
		}

		long rows=m_dimension*iScale, cols=m_dimension*iScale;
		if (!oImage.SetDimensions(rows, cols))
		{
			return false;
		}

		U* oBuf			= (U*)oImage.GetBuffer();
		if (!oBuf)
		{
			return false;
		}

		unsigned int oSigma = (rows-1.0)/2.1f;///(2.0f *2.5f);	

		long i=0, j=0;

	
		if (m_separable)
		{
			for (j=0; j<rows; j++)
			{
				for (i=0; i<cols; i++)
				{
//#define _SYNTH_IMAGE
#ifdef _SYNTH_IMAGE
					if ( IN_CIRCLE(cols, j, i, oSigma) )			
					//if ( IN_SQUARE(cols, j, i, oSigma) )	
					//if (IN_RANDOM(cols, j, i, oSigma) )	
					{
						oBuf[j*cols + i] = 1;//(T)sqrt((double)((rows-j)*(rows-j) + (cols-i)*(cols-i)));//(unsigned char)(((double)255.0f)*(double)(m_buffer[i/iScale]) + 0.5);
					} else 
					{
						oBuf[j*cols + i] = 0;
					}
#else
					oBuf[j*cols + i] = (U)(m_buffer[i/iScale]);
#endif

				}
			}

			for (j=0; j<rows; j++)
			{
				for (i=0; i<cols; i++)
				{
#ifdef _SYNTH_IMAGE
					if ( IN_CIRCLE(cols, j, i, oSigma) )			
					//if ( IN_SQUARE(cols, j, i, oSigma) )	
					//if (IN_RANDOM(cols, j, i, oSigma) )	
					{
						oBuf[j*cols + i] = 1;//(T)sqrt((double)((rows-j)*(rows-j) + (cols-i)*(cols-i)));//(unsigned char)(((double)255.0f)*(double)(m_buffer[i/iScale]) + 0.5);
					} else 
					{
						oBuf[j*cols + i] = 0;
					}
#else
					oBuf[j*cols + i] = oBuf[j*cols + i] * (m_buffer[j/iScale]);
#endif
				}
			}
			
		} else
		{

			for (j=0; j<rows; j++)
			{
				for (i=0; i<cols; i++)
				{
					oBuf[j*cols + i] = (U)(m_buffer[(i/iScale)*m_dimension + j/iScale]);

				}
			}

		}

		//PGCore::Image<U> tImage = oImage;
		//return Convolve(tImage, oImage);
#ifdef _DEBUG
		//PGAlgs::DumpImageAsPGM(oImage, std::string("C:\\Data\\KernelRender.pgm"));
#endif

		return true;
	}
*/

	template <class T, class U>
	bool Kernel<T, U>::Convolve(const Image<T>& iImage, Image<U>& oImage) const
	{
		if (m_separable)
		{
			return convolveSeparable(iImage, oImage);
		}
		return convolve(iImage, oImage);
	}

	template <class T, class U>
	bool Kernel<T, U>::Convolve(const BitImage& iImage, BitImage& oImage) const
	{
#ifdef _DEBUG
		//Image<U> oCharImage;
		//Render(oCharImage, 16);
		//Plot(oCharImage, 16);
#endif
		/*
		if (m_separable)
		{
			return convolveSeparable(iImage, oImage);
		}*/
		return convolve(iImage, oImage);
	}

	template <class T, class U>
	bool Kernel<T, U>::Convolve1D_X(const Image<T>& iImage, Image<U>& oImage) const
	{
		return convolveX(iImage, oImage);
	}

	template <class T, class U>
	bool Kernel<T, U>::Convolve1D_Y(const Image<T>& iImage, Image<U>& oImage) const
	{
		return convolveY(iImage, oImage);
	}

	template <>
	bool Kernel<unsigned char, PGMath::Point3D<unsigned char>>::convolveX(const Image<unsigned char>& iImage, Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned char, PGMath::Point3D<unsigned short>>::convolveX(const Image<unsigned char>& iImage, Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<char, PGMath::Point3D<unsigned short>>::convolveX(const Image<char>& iImage, Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;

	}

	template <>
	bool Kernel<char, PGMath::Point3D<unsigned char>>::convolveX(const Image<char>& iImage, Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;

	}

	template <>
	bool Kernel<unsigned short, PGMath::Point3D<unsigned short>>::convolveX(const Image<unsigned short>& iImage, Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;

	}

	template <>
	bool Kernel<unsigned short, PGMath::Point3D<unsigned char>>::convolveX(const Image<unsigned short>& iImage, Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;

	}


	template <class T, class U>
	bool Kernel<T, U>::convolveX(const Image<T>& iImage, Image<U>& oImage) const
	{
		// perform 2D convolution
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, cc, center;            
		long double dot, sum;               

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		oImage.SetDimensions(rows, cols);
		const T* iBuf	= (const T*)iImage.GetBuffer();
		U* oBuf			= (U*)oImage.GetBuffer();		

		float kernelValue = 0, imageValue = 0;


		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++)
		{
			for(c=0;c<cols;c++)
			{
				dot = 0.0f;
				sum = 0.0f;
				//rr = 0;
				//for(rr=(-center);rr<=center;rr++)
				{
					//if(((r+rr) >= 0) && ((r+rr) < rows))
					{
						for(cc=(-center);cc<=center;cc++)
						{
							if(((c+cc) >= 0) && ((c+cc) < cols))
							{
								kernelValue = GetValue(0, (center+cc));
								imageValue = (float)(iImage.GetValue(r, (c+cc)));
								//if (imageValue) 
								{
									//imageValue = imageValue;
									//}
									//{
									dot += imageValue * kernelValue;//7//iBuf[(r+rr)*cols+(c+cc)] * kernelValue;//m_buffer[(center+rr)*m_dimension + (center+cc)];
									sum += kernelValue;//m_buffer[(center+rr)*m_dimension + (center+cc)];
								}
							}
						}
					}
				}

				long double oValue =  ( dot );

				if (sum>kPgCoarseEpsilon)
				{
					oValue /= sum;			
				}

				oBuf[r*cols+c] = oValue;

			}
		}

		//LOG0("The filter coefficients after derivative unquantized are:");
		//for(int i=0;i<(cols);i++)
		//	LOG2("Kernel[%d] = %f", i, (double)oBuf[i]);


		return true;

	}

	template <class T, class U>
	bool Kernel<T, U>::Dump() const
	{
		LOG0("The kernel coefficients:");
		
		if (m_separable)
		{
			for(int i=0;i<(m_dimension);i++)
				LOG2("Kernel[%d] = %3.4f", i, (double)(m_buffer[i]));

		} else
		{

			for(int i=0;i<(m_dimension);i++)
				for(int j=0;j<(m_dimension);j++)
					LOG3("Kernel[%d][%d] = %3.4f", i, j, (double)(m_buffer[i*m_dimension + j]));

		}

		return true;
	}

	template <>
	bool Kernel<char, PGMath::Point3D<unsigned short>>::convolveY(const Image<char>& iImage, 
		Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<char, PGMath::Point3D<unsigned char>>::convolveY(const Image<char>& iImage, 
		Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned char, PGMath::Point3D<unsigned char>>::convolveY(const Image<unsigned char>& iImage, 
		Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned short, PGMath::Point3D<unsigned char>>::convolveY(const Image<unsigned short>& iImage, 
		Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned char, PGMath::Point3D<unsigned short>>::convolveY(const Image<unsigned char>& iImage, 
		Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned short, PGMath::Point3D<unsigned short>>::convolveY(const Image<unsigned short>& iImage, 
		Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <class T, class U>
	bool Kernel<T, U>::convolveY(const Image<T>& iImage, Image<U>& oImage) const
	{
		// perform 2D convolution
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, rr, center;            
		long double dot, sum;               

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		//oImage = iImage;
		oImage.SetDimensions(rows, cols);
		const T* iBuf	= (const T*)iImage.GetBuffer();
		U* oBuf			= (U*)oImage.GetBuffer();		

		float kernelValue = 0, imageValue = 0;


		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++)
		{
			for(c=0;c<cols;c++)
			{
				dot = 0.0f;
				sum = 0.0f;
				rr = 0;
				for(rr=(-center);rr<=center;rr++)
				{
					if(((r+rr) >= 0) && ((r+rr) < rows))
					{
						//for(cc=(-center);cc<=center;cc++)
						{
							//if(((c+cc) >= 0) && ((c+cc) < cols))
							kernelValue = GetValue(0, (center+rr));
							imageValue = (float)(iImage.GetValue(r+rr, c));
							//if (imageValue) 
							{
								//imageValue = imageValue;
								//}
								//{
								dot += imageValue * kernelValue;//7//iBuf[(r+rr)*cols+(c+cc)] * kernelValue;//m_buffer[(center+rr)*m_dimension + (center+cc)];
								sum += kernelValue;//m_buffer[(center+rr)*m_dimension + (center+cc)];
							}
						}
					}
				}

				long double oValue =  ( dot );

				if (sum>kPgCoarseEpsilon)
				{
					oValue /= sum;			
				}

				oBuf[r*cols+c] = oValue;

			}
		}

		//LOG0("The filter coefficients after derivative unquantized are:");
		//for(int i=0;i<(cols);i++)
		//	LOG2("Kernel[%d] = %f", i, (double)oBuf[i]);


		return true;

	}

	template <class T, class U>
	bool Kernel<T, U>::convolve(const BitImage& iImage, BitImage& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<char, PGMath::Point3D<unsigned short>>::convolve(const Image<char>& iImage, 
		Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<char, PGMath::Point3D<unsigned char>>::convolve(const Image<char>& iImage, 
		Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned short, PGMath::Point3D<unsigned short>>::convolve(const Image<unsigned short>& iImage, 
		Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned short, PGMath::Point3D<unsigned char>>::convolve(const Image<unsigned short>& iImage, 
		Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned char, PGMath::Point3D<unsigned short>>::convolve(const Image<unsigned char>& iImage, 
		Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned char, PGMath::Point3D<unsigned char>>::convolve(const Image<unsigned char>& iImage, 
		Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <class T, class U>
	bool Kernel<T, U>::convolve(const Image<T>& iImage, Image<U>& oImage) const
	{
		// perform 2D convolution
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, rr, cc, center;            
		long double dot, sum;               

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		Image<double> tempImage(rows, cols);
		//oImage = iImage;

		oImage.SetDimensions(rows, cols);

		const T* iBuf	= (const T*)iImage.GetBuffer();
		U* oBuf			= (U*)oImage.GetBuffer();	
		double* tempBuf = (double *)tempImage.GetBuffer();

		if (!tempBuf) return false;

		float kernelValue = 0;
		double imageValue = 0;
		double maxVal = -9999, minVal = 9999;

		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++)
		{
			for(c=0;c<cols;c++)
			{
				dot = 0.0f;
				sum = 0.0f;
				rr = 0;
				for(rr=(-center);rr<=center;rr++)
				{
					if(((r+rr) >= 0) && ((r+rr) < rows))
					{
						for(cc=(-center);cc<=center;cc++)
						{
							if(((c+cc) >= 0) && ((c+cc) < cols))
							{
								kernelValue = GetValue((center+rr), (center+cc));
								imageValue = (double)(iImage.GetValue((r+rr), (c+cc)));
								//if (imageValue) 
								{
									//imageValue = imageValue;
									//}
									//{
									dot += imageValue * kernelValue;//7//iBuf[(r+rr)*cols+(c+cc)] * kernelValue;//m_buffer[(center+rr)*m_dimension + (center+cc)];
									sum += kernelValue;//m_buffer[(center+rr)*m_dimension + (center+cc)];
								}
							}
						}
					}
				}

				long double oValue =  ( dot );

				if (sum>kPgCoarseEpsilon)
				{
					oValue /= sum;			
				}

				tempBuf[r*cols+c] = oValue;
				maxVal = maxVal<oValue ? oValue : maxVal;
				minVal = minVal>oValue ? oValue : minVal;

			}
		}

		//LOG0("The filter coefficients after derivative unquantized are:");
		//for(int i=0;i<(cols);i++)
		//	LOG2("Kernel[%d] = %f", i, (double)tempBuf[i]);

		// quantize values to between 0 & 255 ? 
		long imCount=0;
		double oRange = (maxVal-minVal);
		if (oRange) oRange = 255.0f/oRange;
		else oRange = 255.0f;

		while (imCount < rows*cols)
		{
			double outVal = (tempBuf[imCount]);//-minVal)*oRange;
			oBuf[imCount] = (U)(outVal+0.5f);
			imCount++;
		}

		return true;
	}

	template <>
	bool Kernel<unsigned short, PGMath::Point3D<unsigned char>>::convolveSeparable(const Image<unsigned short>& iImage, Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned short, PGMath::Point3D<unsigned short>>::convolveSeparable(const Image<unsigned short>& iImage, Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned char, PGMath::Point3D<unsigned char>>::convolveSeparable(const Image<unsigned char>& iImage, Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<unsigned char, PGMath::Point3D<unsigned short>>::convolveSeparable(const Image<unsigned char>& iImage, Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<char, PGMath::Point3D<unsigned char>>::convolveSeparable(const Image<char>& iImage, Image<PGMath::Point3D<unsigned char>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <>
	bool Kernel<char, PGMath::Point3D<unsigned short>>::convolveSeparable(const Image<char>& iImage, Image<PGMath::Point3D<unsigned short>>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <class T, class U>
	bool Kernel<T, U>::convolveSeparable(const Image<T>& iImage, Image<U>& oImage) const
	{	
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, rr, cc, center;            
		long double dot, sum;               

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		Image<double> tempImage(rows, cols);
		//oImage = iImage;
		oImage.SetDimensions(rows, cols);

		const T* iBuf	= (const T*)iImage.GetBuffer();
		U* oBuf			= (U*)oImage.GetBuffer();
		double* tempBuf = (double *)tempImage.GetBuffer();
		
		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++)
		{
			for(c=0;c<cols;c++)
			{
				dot = 0.0f;
				sum = 0.0f;
				for(cc=(-center);cc<=center;cc++)
				{
					if(((c+cc) >= 0) && ((c+cc) < cols))
					{
						dot += iBuf[r*cols+(c+cc)] * m_buffer[center+cc];
						sum += m_buffer[center+cc];
					}
				}
				if (sum>kPgCoarseEpsilon)
					tempBuf[r*cols+c] = dot/sum;
				else 
					tempBuf[r*cols+c] = dot;
			}
		}

		Image<double> tempImage1(rows, cols);
		double* tempBuf1 = (double *)tempImage1.GetBuffer();
		double maxVal = -9999, minVal = 9999;

		//LOG0("Blurring the image in the Y-direction.");
		for(c=0;c<cols;c++)
		{
			for(r=0;r<rows;r++)
			{
				sum = 0.0f;
				dot = 0.0f;
				for(rr=(-center);rr<=center;rr++)
				{
					if(((r+rr) >= 0) && ((r+rr) < rows))
					{
						dot += tempBuf[(r+rr)*cols+c] * m_buffer[center+rr];
						sum += m_buffer[center+rr];
					}
				}
				if (sum>kPgCoarseEpsilon)
					tempBuf1[r*cols+c] =(dot/sum);
				else
					tempBuf1[r*cols+c] =(dot);

				double oValue = tempBuf1[r*cols+c];
				maxVal = maxVal<oValue ? oValue : maxVal;
				minVal = minVal>oValue ? oValue : minVal;
			}
		}  

		// quantize values to between 0 & 255
		long imCount=0;
		double oRange = (maxVal-minVal);
		if (oRange) oRange = 255.0f/oRange;
		else oRange = 255.0f;

		while (imCount < rows*cols)
		{
			double outVal = (tempBuf1[imCount]);//-minVal)*oRange;
			oBuf[imCount] = (U)(outVal+0.5f);
			imCount++;
		}
		//LOG1("End Convolve %d.", &iImage);
		return true;
	}



	template <class T, class U>
	bool Kernel<T, U>::Clear()
	{		
		bool cleared = ObjectBase::Clear();
		m_buffer = 0;	
		m_dimension = 0;		
		return cleared;
	}


	template <class T, class U>
	bool Kernel<T, U>::SetValue(const unsigned int &iRow, const unsigned int &iCol, const float iValue)
	{
		bool res = false;
		if (iRow<m_dimension && iCol<m_dimension) 
		{
			if (!m_separable)
				*(m_buffer + iRow*m_dimension + iCol) = iValue;
			else
				*(m_buffer + iCol) = iValue;
		}		
		return res;	
	}


	template <class T, class U>
	const float Kernel<T, U>::GetValue(const unsigned int &iRow, const unsigned int &iCol) const 
	{
		if (iRow<m_dimension && iCol<m_dimension) 
		{		
			if (!m_separable)
				return *(m_buffer + iRow*m_dimension + iCol);
			else
				return *(m_buffer + iCol);
		}	
		return (float)0;	
	}


	template <class T, class U>
	const unsigned int & Kernel<T, U>::GetDimension() const 
	{	
		return m_dimension;		
	}

	template <class T, class U>
	bool Kernel<T, U>::Reset(const float iValue/*=0*/)
	{
		if (!m_buffer) 
		{
			return false;
		}

		long pSize = m_dimension*m_dimension;

		if (m_separable)
		{
			pSize = m_dimension;
		}

		int i=0;
		while (i<pSize)
		{
			m_buffer[i] = iValue;
			i++;
		}

		return true;

	}


	template <class T, class U>
	bool Kernel<T, U>::SetDimension(const unsigned int &iDimension) 
	{
		// clear previous memory
		Clear();

		long pSize = iDimension*iDimension*sizeof(float);

		if (m_separable)
		{
			pSize = iDimension*sizeof(float);
		}

		m_buffer = (float *)Alloc(pSize);
		if (!m_buffer) 
		{
			LOG0("Error allocating Kernel.");
			return false;
		}

		m_dimension = iDimension;

		return Reset();
	}



	template <class T, class U>
	Kernel<T, U>::~Kernel(void)
	{	
		Clear();
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class Kernel<unsigned short, unsigned short>; 
	template class Kernel<short, short>; 
	template class Kernel<unsigned char, unsigned char>; 
	template class Kernel<char, short>; 
	template class Kernel<double, short>; 
	template class Kernel<char, char>; 
	template class Kernel<char, unsigned char>; 
	template class Kernel<char, unsigned short>; 
	template class Kernel<double, unsigned char>; 
	template class Kernel<float, float>; 
	template class Kernel<double, double>; 
	template class Kernel<char, PGMath::Point3D<unsigned char>>; 
	template class Kernel<char, PGMath::Point3D<unsigned short>>; 
	//template class Kernel<double, PGMath::Point3D<unsigned short>>; 
	//template class Kernel<double, PGMath::Point3D<unsigned char>>; 	
#endif


};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Kernel_HPP_