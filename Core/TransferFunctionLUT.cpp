// -*- C++ -*-
// � [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
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
// Filename:	TransferFunctionLUT.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_TransferFunctionLUT_HPP_
#define _PG_TransferFunctionLUT_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Core/TransferFunctionLUT.h"


namespace PGCore
{

	template <class T>
	TransferFunctionLUT<T>::TransferFunctionLUT(ePgLUTType iType/*=kPgLUTTypeGrayScale*/, const long iDimension/*=PG_LUT_SIZE*/)
	{		
		m_lowLevel = 0; m_highLevel = 1;
		m_LutType = iType;
		m_dimBy6 = iDimension/6;
		m_windowLow = 0;
		m_step=1.0f/m_dimBy6;
		m_maxOutValue = 255;

		m_transparencyFlag = false;

#if (PG_USE_RGBA16)
		m_maxOutValue = (PG_USHORT_MAX-1);
#endif

		SetDimension(iDimension);
		SetType(iType);
		//SetBlendWeight(1.0f);
	}

	template <class T>
	void TransferFunctionLUT<T>::SetTransparencyFlag(const bool iFlag)
	{
		m_transparencyFlag = iFlag;
	}

	template <class T>
	bool TransferFunctionLUT<T>::GetTransparencyFlag() const
	{
		return m_transparencyFlag;
	}

	template <class T>
	bool TransferFunctionLUT<T>::SetWindow(const float iLowLevel, const float iHighLevel)
	{

		if(iHighLevel<=iLowLevel)
			return false;

		m_lowLevel = iLowLevel; m_highLevel = iHighLevel;

		if (m_lowLevel<0) m_lowLevel = 0;
		if (m_highLevel>1.0) m_highLevel = 1;

		//return (m_highLevel>m_lowLevel && m_highLevel<=1 && m_lowLevel>=0);
		UpdateDims();
		return FillLuT();
	}


	template <class T>
	Point3D<T>* TransferFunctionLUT<T>::GetBuffer() 
	{ 
		return m_image.GetBuffer(); 
	};

	template <class T>
	bool TransferFunctionLUT<T>::SetType(ePgLUTType iType)
	{
		m_LutType = iType;
		return FillLuT();
	}


	template <class T>
	const ePgLUTType TransferFunctionLUT<T>::GetType() const
	{
		return m_LutType;
	}

	template <class T>
	bool TransferFunctionLUT<T>::Dump() const
	{
#ifdef _DEBUG
		if (0)
		{
			LOG1("The TransferFunctionLUT coefficients: [%d]", m_LutType);		
			{
				for(int i=0;i<(GetDimension());i++)
				{
					LOG4("TransferFunctionLUT[%d] = %3.4f, %3.4f, %3.4f", 
						i, 
						(double)((m_image.GetBuffer())[i].X()),
						(double)((m_image.GetBuffer())[i].Y()),
						(double)((m_image.GetBuffer())[i].Z()));
				}

			} 
		}
#endif
		return true;
	}

	template <class T>
	bool TransferFunctionLUT<T>::UpdateDims()
	{
		int dim = GetDimension();
		m_windowLow = (int)floor((float)dim * (m_lowLevel));
		int windowDim = (int)floor((float)dim * (m_highLevel-m_lowLevel));

		m_dimBy6 = windowDim/6;;
		m_step=1.0f/(float)m_dimBy6;

		return true;
	}



	template <class T>
	bool TransferFunctionLUT<T>::Clear() 
	{		
		return m_image.Clear();
	}


	template <class T>
	const long TransferFunctionLUT<T>::GetDimension() const 
	{	
		long iDimension=0, iT=0;
		m_image.GetDimensions(iT, iDimension);
		return iDimension;		
	}

	template <class T>
	bool TransferFunctionLUT<T>::Reset(const T iValue/*=0*/)
	{		
		return m_image.Reset(iValue);
	}


	template <class T>
	bool TransferFunctionLUT<T>::SetDimension(const long &iDimension) 
	{
		// clear previous memory
		long iDimCols = iDimension, iDimRows=1;
		bool rv = m_image.SetDimensions(iDimRows, iDimCols);		
		if (!rv) return false;

		rv = UpdateDims();
		if (!rv) return false;

		return FillLuT();
	}

	template <class T>
	bool TransferFunctionLUT<T>::FillLuT() 
	{
		long dim = GetDimension();
		//if (dim<=0)	return false;

		bool res = false;

		switch(m_LutType)
		{
		case kPgLUTTypeVIBGYOR:
			Reset(0);
			res = fillVIBGYOR();
			break;

		case kPgLUTTypeIronHot:
			Reset(0);
			res = fillHotIron();
			break;


		case kPgLUTTypeGrayScale:


		default: 
			Reset(0);
			res = fillGrayScale();
			break;
		}

		Dump();

		return res;
	}



	template <class T>
	TransferFunctionLUT<T>::~TransferFunctionLUT(void)
	{	
		if (!Clear())
		{
			LOG1("Failed to free memory for TransferFunctionLUT: %d", this);
		}
	}

	template <class T>
	bool TransferFunctionLUT<T>::fillGrayScale() 
	{
		int i=0;
		Point3D<T> *buf = m_image.GetBuffer();

		for (i=0; i<GetDimension(); i++)
		{
			buf[i] = Point3D<T>( 
				i, 
				i, 
				i,				
				i	
				);
		}

		return true;


		int window=6*m_dimBy6, 
			offset = m_windowLow,
			offset1 = m_windowLow + window/2;

		//here.......
		float tstep = (1.0f)/(float)(window);;//(2.0f)/(float)(window);//float(m_maxOutValue)*m_step;
		float invExp = float(m_maxOutValue) / (exp(1.0f) - 1.0f);
		for (i=0; i<window; i++)
		{
			float e = exp(((float)i)*tstep) - 1.0f;
			e *= invExp;
			int posValue = (int)( e );
			posValue = posValue < 0 ? 0 : posValue;
			posValue = posValue > m_maxOutValue ? m_maxOutValue : posValue;

			//here.......
			//int negValue = posValue;//(int)(m_maxOutValue-(int)posValue);

			if (i+offset < GetDimension())
			{
				buf[i+offset] = Point3D<T>( 
					i+offset, // posValue,
					i+offset, // posValue, 
					i+offset, // posValue, 
					i+offset // m_transparencyFlag ? i : posValue		
					);							
			}

			/*
			if (i+offset1 < GetDimension())
			{
			buf[i+offset1] = Point3D<T>( 
			negValue,
			negValue, 
			negValue, 
			negValue);	
			}
			*/
		}

		//here.......
		// set all teh ones above 
		//int offset2 = m_windowLow+window+1, remainingVals = GetDimension() - offset2;
		//memset(buf+offset2, posValue, remainingVals*sizeof(T));

		for (i=offset+window-1; i<GetDimension(); i++)
		{
			buf[i] = Point3D<T>( 
				m_maxOutValue, 
				m_maxOutValue, 
				m_maxOutValue,
				//m_maxOutValue
				m_transparencyFlag ? i : m_maxOutValue	
				);
		}

		return true;
	}

	template <class T>
	bool TransferFunctionLUT<T>::fillHotIron() 
	{
		int i=0;
		Point3D<T> *buf = m_image.GetBuffer();

		
		int m_dimBy6By2 = m_dimBy6/2;

		int offsetA1 = m_dimBy6By2,
			offsetA2 = 2*m_dimBy6By2,
			offsetA3 = 3*m_dimBy6By2,
			offsetA4 = 4*m_dimBy6By2,
			offsetA5 = 5*m_dimBy6By2,
			offsetA6 = 6*m_dimBy6By2,
			offsetA7 = 7*m_dimBy6By2,
			offsetA8 = 8*m_dimBy6By2,
			offsetA9 = 9*m_dimBy6By2,
			offsetA10 = 10*m_dimBy6By2,
			offsetA11 = 11*m_dimBy6By2;

		int offset = m_windowLow,
			offset1 = m_windowLow+offsetA1,
			offset2 = m_windowLow+offsetA2,
			offset3 = m_windowLow+offsetA3,
			offset4 = m_windowLow+offsetA4,
			offset5 = m_windowLow+offsetA5,
			offset6 = m_windowLow+offsetA6,
			offset7 = m_windowLow+offsetA7,
			offset8 = m_windowLow+offsetA8,
			offset9 = m_windowLow+offsetA9,
			offset10 = m_windowLow+offsetA10,
			offset11 = m_windowLow+offsetA11;		

		float tstep = (1.0f)/(float)(m_dimBy6By2);
		float invExp = float(0.005*m_maxOutValue) / (exp(1.0f) - 1.0f);
		float tstepBig = (1.0f)/(float)(m_dimBy6By2);;

		for (i=0; i<m_dimBy6By2; i++)
		{
			float e = exp(((float)i)*tstep) - 1.0f;
			e *= invExp;
			int posValue = (int)( e );
			posValue = posValue < 0 ? 0 : posValue;
			posValue = posValue > m_maxOutValue ? m_maxOutValue : posValue;
			int negValue = (int)(m_maxOutValue -(int)posValue);

			//T posValue = (T)((((float)i)*tstep)),
			//	negValue = (T)(m_maxOutValue-(T)posValue);
			
			// black to red
			if (i+offset < GetDimension())
			{
				float eb = exp( ( (float)i )*tstepBig ) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset] = Point3D<T>( 
					posValue,
					0, 
					0, 					
					m_transparencyFlag ? i : posValueAlpha		
					);
			}



			// red to orange
			if (i+offset1 < GetDimension())
			{
				float eb = exp((float)(offsetA1+i)*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset1] = Point3D<T>( 
					m_maxOutValue,
					posValue/2, 
					0, 
					//m_maxOutValue
					m_transparencyFlag ? offsetA1+i : posValueAlpha		
					);
			}

			// orange to yellow
			if (i+offset2 < GetDimension())
			{
				float eb = exp((float)(offsetA2+i)*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset2] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue/2 + posValue/2, 
					0,					
					m_transparencyFlag ? offsetA2+i : posValueAlpha		
					);
			}

			// yellow to pink
			if (i+offset3 < GetDimension())
			{
				float eb = exp((float)((offsetA3+i))*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset3] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					posValue/2,					
					m_transparencyFlag ? offsetA3+i : posValueAlpha		
					);
			}

			// pink to white
			if (i+offset4 < GetDimension())
			{
				float eb = exp((float)((offsetA4+i))*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset4] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					m_maxOutValue/2 + posValue/2,					
					m_transparencyFlag ? offsetA4+i : posValueAlpha		
					);
			}

			// full white
			if (i+offset5 < GetDimension())
			{
				float eb = exp((float)((offsetA5+i))*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset5] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					m_maxOutValue,
					m_transparencyFlag ? offsetA5+i : posValueAlpha										
					);
			}

			// white to pink - reverse
			if (i+offset6 < GetDimension())
			{
				float eb = exp((float)(GetDimension()-(offsetA6+i))*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset6] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					m_maxOutValue, //m_maxOutValue/2 + (m_maxOutValue-posValue)/2,					
					m_transparencyFlag ? offsetA6+i : posValueAlpha		
					);
			}

			// pink to yellow - reverse
			if (i+offset7 < GetDimension())
			{
				float eb = exp((float)(GetDimension()-(offsetA7+i))*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset7] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					m_maxOutValue, // (m_maxOutValue-posValue)/2,
					m_transparencyFlag ? offsetA7+i : posValueAlpha		
					);
			}

			// yellow to orange - reverse
			if (i+offset8 < GetDimension())
			{
				float eb = exp((float)(GetDimension()-(offsetA8+i))*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset8] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, //m_maxOutValue/2 + (m_maxOutValue-posValue)/2, 
					m_maxOutValue, //0,					
					m_transparencyFlag ? offsetA8+i : posValueAlpha		
					);
			}

			// orange to red - reverse
			if (i+offset9 < GetDimension())
			{
				float eb = exp((float)(GetDimension()-(offsetA9+i))*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset9] = Point3D<T>( 
					m_maxOutValue,
					m_maxOutValue, //(m_maxOutValue-posValue)/2, 
					m_maxOutValue, //0, 					
					m_transparencyFlag ? offsetA9+i : posValueAlpha		
					);
			}

			// red to black - reverse
			if (i+offset10 < GetDimension())
			{
				float eb = exp( (float)(GetDimension()-(offsetA10+i))*tstepBig ) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset10] = Point3D<T>( 
					m_maxOutValue, //m_maxOutValue-posValue,
					m_maxOutValue, //0, 
					m_maxOutValue, //0, 					
					m_transparencyFlag ? offsetA10+i : posValueAlpha		
					);
			}


		}


		for (i=offset11+m_dimBy6By2; i<GetDimension(); i++)
		{
			buf[i] = Point3D<T>( 
				m_maxOutValue, 
				m_maxOutValue, 
				m_maxOutValue,
				// m_maxOutValue
				m_transparencyFlag ? offset11+i : 0					
				);
		}

		return true;

	}

	template <class T>
	bool TransferFunctionLUT<T>::fillVIBGYOR() 
	{
		int i=0;
		Point3D<T> *buf = m_image.GetBuffer();

		// black->violet
		int offsetA1 = m_dimBy6,
			offsetA2 = 2*m_dimBy6,
			offsetA3 = 3*m_dimBy6,
			offsetA4 = 4*m_dimBy6,
			offsetA5 = 5*m_dimBy6;
		int offset = m_windowLow,
			offset1 = m_windowLow+offsetA1,
			offset2 = m_windowLow+offsetA2,
			offset3 = m_windowLow+offsetA3,
			offset4 = m_windowLow+offsetA4,
			offset5 = m_windowLow+offsetA5;

		float tstep = float(m_maxOutValue)*m_step;

		for (i=0; i<m_dimBy6; i++)
		{
			T posValue = (T)((((float)i)*tstep)),
				negValue = (T)(m_maxOutValue-(T)posValue);

			if (i+offset < GetDimension())
			{
				buf[i+offset] = Point3D<T>( 
					posValue,
					0, 
					posValue, 
					m_transparencyFlag ? i+offset : m_maxOutValue					
					);
			}

			if (i+offset1 < GetDimension())
			{
				buf[i+offset1] = Point3D<T>( 
					negValue ,
					0, 
					m_maxOutValue, 					
					m_transparencyFlag ? i+offset1 : m_maxOutValue					
					);
			}

			if (i+offset2 < GetDimension())
			{
				buf[i+offset2] = Point3D<T>( 
					0, 
					posValue, 
					negValue,
					//m_maxOutValue
					m_transparencyFlag ? i+offset2 : m_maxOutValue					
					);
			}

			if (i+offset3 < GetDimension())
			{
				buf[i+offset3] = Point3D<T>( 
					posValue, 
					m_maxOutValue, 
					0,
					//m_maxOutValue
					m_transparencyFlag ? i+offset3 : m_maxOutValue					
					);
			}

			if (i+offset4 < GetDimension())
			{
				buf[i+offset4] = Point3D<T>( 
					m_maxOutValue, 
					negValue, 
					0,
					//m_maxOutValue
					m_transparencyFlag ? i+offset4 : m_maxOutValue					
					);
			}

			if (i+offset5 < GetDimension())
			{
				buf[i+offset5] = Point3D<T>( 
					m_maxOutValue, 
					posValue, 
					posValue,					
					m_transparencyFlag ? i+offset5 : m_maxOutValue					
					);
			}

		}

		for (i=offset5+m_dimBy6; i<GetDimension(); i++)
		{
			buf[i] = Point3D<T>( 
				m_maxOutValue, 
				m_maxOutValue, 
				m_maxOutValue,				
				m_transparencyFlag ? 0 : m_maxOutValue		
				);
		}

		/*
		for (i=offset5+m_dimBy6; i<GetDimension(); i++)
		{
		buf[i] = Point3D<T>( 
		m_maxOutValue, 
		m_maxOutValue, 
		m_maxOutValue,
		//m_maxOutValue
		m_maxOutValue
		);
		}*/

		return true;
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class TransferFunctionLUT<unsigned short>; 
	template class TransferFunctionLUT<unsigned char>; 
	template class TransferFunctionLUT<char>; 
#endif

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_TransferFunctionLUT_HPP_
