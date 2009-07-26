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
// Filename:	Kernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Kernel_HPP_
#define _PG_Kernel_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Kernel.h"

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
	T* Kernel<T, U>::GetBuffer() const {
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

		const T *iBuf = iKernel.GetBuffer();
		int i=0, kSize=m_dimension;
		
		if (!m_separable) kSize = m_dimension*m_dimension;

		while (i<kSize)
		{
			m_buffer[i] -= iBuf[i];
			i++;
		}

		return *this;
	}


	template <class T, class U>
	bool Kernel<T, U>::Plot(Image<U>& oImage, unsigned int iScale/*=1*/) const
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
	bool Kernel<T, U>::Render(Image<U>& oImage, unsigned int iScale/*=1*/) const
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

	/*	if (m_separable)
		{

		// rows first
		for (j=0; j<rows; j++)
		{
			for (i=0; i<cols; i++)
			{
				//if ( IN_CIRCLE(cols, j, i, oSigma) )			
				//if ( IN_SQUARE(cols, j, i, oSigma/2) )	
				//if (IN_RANDOM(cols, j, i, oSigma) )	
				{
					oBuf[j*cols + i] = 255;//(unsigned char)(((double)255.0f)*(double)(m_buffer[i/iScale]) + 0.5);
				} else 
				{
					oBuf[j*cols + i] = 0;
				}
			}
		}

		} else
		{
			

		}
*/
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
					oBuf[j*cols + i] *= (U)(m_buffer[j/iScale]);
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
		PGAlgs::DumpImageAsPGM(oImage, std::string("C:\\Data\\KernelRender.pgm"));
#endif

		return true;
	}


	template <class T, class U>
	bool Kernel<T, U>::Convolve(const Image<U>& iImage, Image<U>& oImage) const
	{
#ifdef _DEBUG
		//Image<U> oCharImage;
		//Render(oCharImage, 16);
		//Plot(oCharImage, 16);
#endif
		if (m_separable)
		{
			return convolveSeparable(iImage, oImage);
		}
		return convolve(iImage, oImage);
	}

	template <class T, class U>
	bool Kernel<T, U>::Convolve(const BitImage<U>& iImage, BitImage<U>& oImage) const
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
	bool Kernel<T, U>::Convolve1D_X(const Image<U>& iImage, Image<U>& oImage) const
	{
		return convolveX(iImage, oImage);
	}

	template <class T, class U>
	bool Kernel<T, U>::Convolve1D_Y(const Image<U>& iImage, Image<U>& oImage) const
	{
		return convolveY(iImage, oImage);
	}

	template <class T, class U>
	bool Kernel<T, U>::convolveX(const Image<U>& iImage, Image<U>& oImage) const
	{
		// perform 2D convolution
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, cc, center;            
		U dot;
		T sum;               

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		oImage = iImage;
		const U* iBuf	= (const U*)iImage.GetBuffer();
		U* oBuf			= (U*)oImage.GetBuffer();		

		T kernelValue = 0;
		U imageValue = 0;


		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++)
		{
			for(c=0;c<cols;c++)
			{
				dot = U(0);
				sum = T(0);
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
								imageValue = (U)(iImage.GetValue(r, (c+cc)));
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

				U oValue =  ( dot );

				if (sum>kPgFineEpsilon)
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

	template <class T, class U>
	bool Kernel<T, U>::convolveY(const Image<U>& iImage, Image<U>& oImage) const
	{
		// perform 2D convolution
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, rr, center;            
		U dot;
		T sum;               

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		oImage = iImage;
		const U* iBuf	= (const U*)iImage.GetBuffer();
		U* oBuf			= (U*)oImage.GetBuffer();		

		T kernelValue = 0;
		U imageValue = 0;


		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++)
		{
			for(c=0;c<cols;c++)
			{
				dot = U(0);
				sum = T(0);
				rr = 0;
				for(rr=(-center);rr<=center;rr++)
				{
					if(((r+rr) >= 0) && ((r+rr) < rows))
					{
						//for(cc=(-center);cc<=center;cc++)
						{
							//if(((c+cc) >= 0) && ((c+cc) < cols))
							kernelValue = GetValue(0, (center+rr));
							imageValue = (U)(iImage.GetValue(r+rr, c));
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

				U oValue =  ( dot );

				if (sum>kPgFineEpsilon)
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
	bool Kernel<T, U>::convolve(const BitImage<U>& iImage, BitImage<U>& oImage) const
	{
		assert(0 && "Not implemented yet");
		return false;
	}

	template <class T, class U>
	bool Kernel<T, U>::convolve(const Image<U>& iImage, Image<U>& oImage) const
	{
		// perform 2D convolution
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, rr, cc, center;            
		//long double dot, sum;               
		U dot;
		T sum;

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		Image<U> tempImage(rows, cols);
		oImage = iImage;

		const U* iBuf	= (const U*)iImage.GetBuffer();
		U* oBuf			= (U*)oImage.GetBuffer();	
		U* tempBuf = (U *)tempImage.GetBuffer();

		if (!tempBuf) return false;

		T kernelValue = 0;
		U imageValue = 0;
		U maxVal(-99), minVal(99);

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
					//if(((r+rr) >= 0) && ((r+rr) < rows))
					{
						for(cc=(-center);cc<=center;cc++)
						{
							//if(((c+cc) >= 0) && ((c+cc) < cols))
							kernelValue = GetValue((center+rr), (center+cc));
							imageValue = (U)(iImage.GetValue((r+rr), (c+cc)));
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

				U oValue =  ( dot );

				if (sum>kPgFineEpsilon)
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

		// quantize values to between 0 & 255
		long imCount=0;
		U oRange = (maxVal-minVal);
		if (oRange > U(0)) oRange = U(255)/oRange;
		else oRange = U(255);

		while (imCount < rows*cols)
		{
			U outVal = (tempBuf[imCount]);//-minVal)*oRange;
			oBuf[imCount] = (U)outVal;
			imCount++;
		}

		return true;
	}

	template <class T, class U>
	bool Kernel<T, U>::convolveSeparable(const Image<U>& iImage, Image<U>& oImage) const
	{	
		//LOG1("Begin Convolve %d.", &iImage);
		int r, c, rr, cc, center;            
		U dot;
		T sum;               

		center = m_dimension / 2;

		long rows=0, cols=0;
		if (!iImage.GetDimensions(rows, cols))
		{
			return false;
		}

		Image<double> tempImage(rows, cols);
		oImage = iImage;

		U* iBuf			= (U*)iImage.GetBuffer();
		U* oBuf			= (U*)oImage.GetBuffer();
		U* tempBuf		= (U *)tempImage.GetBuffer();
		
		//LOG0("Blurring the image in the X-direction.");
		for(r=0;r<rows;r++){
			for(c=0;c<cols;c++){
				dot = U(0);
				sum = T(0);
				for(cc=(-center);cc<=center;cc++)
				{
					if(((c+cc) >= 0) && ((c+cc) < cols))
					{
						dot += iBuf[r*cols+(c+cc)] * m_buffer[center+cc];
						sum += m_buffer[center+cc];
					}
				}
				if (sum>kPgFineEpsilon)
					tempBuf[r*cols+c] = dot/sum;
				else 
					tempBuf[r*cols+c] = dot;
			}
		}

		Image<double> tempImage1(rows, cols);
		U* tempBuf1 = (U *)tempImage1.GetBuffer();
		U  maxVal = U(-99), minVal = U(99);

		//LOG0("Blurring the image in the Y-direction.");
		for(c=0;c<cols;c++){
			for(r=0;r<rows;r++){
				sum = 0.0f;
				dot = 0.0f;
				for(rr=(-center);rr<=center;rr++){
					if(((r+rr) >= 0) && ((r+rr) < rows)){
						dot += tempBuf[(r+rr)*cols+c] * m_buffer[center+rr];
						sum += m_buffer[center+rr];
					}
				}
				if (sum>kPgFineEpsilon)
					tempBuf1[r*cols+c] =(dot/sum);
				else
					tempBuf1[r*cols+c] =(dot);

				U oValue = tempBuf1[r*cols+c];
				maxVal = maxVal<oValue ? oValue : maxVal;
				minVal = minVal>oValue ? oValue : minVal;
			}
		}  

		// quantize values to between 0 & 255
		long imCount=0;
		U oRange = (maxVal-minVal);
		if (oRange>U(0)) oRange = U(255)/oRange;
		else oRange = U(255);

		while (imCount < rows*cols)
		{
			U outVal = (tempBuf1[imCount]);//-minVal)*oRange;
			oBuf[imCount] = (U)outVal;
			imCount++;
		}
		//LOG1("End Convolve %d.", &iImage);
		return true;
	}



	template <class T, class U>
	bool Kernel<T, U>::Clear() {		
		bool cleared = ObjectBase::Clear();
		m_buffer = 0;	
		m_dimension = 0;		
		return cleared;
	}


	template <class T, class U>
	bool Kernel<T, U>::SetValue(const unsigned int &iRow, const unsigned int &iCol, const T iValue) {
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
	const T Kernel<T, U>::GetValue(const unsigned int &iRow, const unsigned int &iCol) const {
		if (iRow<m_dimension && iCol<m_dimension) 
		{		
			if (!m_separable)
				return *(m_buffer + iRow*m_dimension + iCol);
			else
				return *(m_buffer + iCol);
		}	
		return (T)0;	
	}


	template <class T, class U>
	const unsigned int & Kernel<T, U>::GetDimension() const 
	{	
		return m_dimension;		
	}

	template <class T, class U>
	bool Kernel<T, U>::Reset(const T iValue/*=0*/)
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

		long pSize = iDimension*iDimension*sizeof(T);

		if (m_separable)
		{
			pSize = iDimension*sizeof(T);
		}

		m_buffer = (T *)Alloc(pSize);
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




};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Kernel_HPP_