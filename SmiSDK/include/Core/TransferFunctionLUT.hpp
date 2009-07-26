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
// Filename:	TransferFunctionLUT.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_TransferFunctionLUT_HPP_
#define _PG_TransferFunctionLUT_HPP_
//////////////////////////////////////////////////////////////////////////


#include "TransferFunctionLUT.h"


namespace PGCore
{
	
	

	template <class T>
	bool TransferFunctionLUT<T>::Dump() const
	{
#ifdef _DEBUG0
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
#endif
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
					posValue,
					posValue, 
					posValue, 
					posValue);							
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
					m_maxOutValue
					);
		}

		return true;
	}

	template <class T>
	bool TransferFunctionLUT<T>::fillHotIron() 
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

		float tstep = (1.0f)/(float)(m_dimBy6);
		float invExp = float(m_maxOutValue) / (exp(1.0f) - 1.0f);
		float tstepBig = (1.0f)/(float)(24.0f*m_dimBy6);;

		for (i=0; i<m_dimBy6; i++)
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
				float eb = exp(((float)i)*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset] = Point3D<T>( 
					posValue,
					0, 
					0, 
					//m_maxOutValue
					posValueAlpha//i
					);
			}



			// red to orange
			if (i+offset1 < GetDimension())
			{
				float eb = exp(((float)offsetA1+i)*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset1] = Point3D<T>( 
					m_maxOutValue,
					posValue/2, 
					0, 
					//m_maxOutValue
					posValueAlpha//offsetA1+i
					);
			}

			// orange to yellow
			if (i+offset2 < GetDimension())
			{
				float eb = exp(((float)offsetA2+i)*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset2] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue/2 + posValue/2, 
					0,
					//m_maxOutValue
					posValueAlpha//offsetA2+i
					);
			}

			// yellow to pink
			if (i+offset3 < GetDimension())
			{
				float eb = exp(((float)offsetA3+i)*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset3] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					posValue/2,
					//m_maxOutValue
					posValueAlpha//offsetA3+i
					);
			}

			// pink to white
			if (i+offset4 < GetDimension())
			{
				float eb = exp(((float)offsetA4+i)*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset4] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					m_maxOutValue/2 + posValue/2,
					//m_maxOutValue
					posValueAlpha//offsetA4+i
					);
			}

			if (i+offset5 < GetDimension())
			{
				float eb = exp(((float)offsetA5+i)*tstepBig) - 1.0f; eb *= invExp;
				int posValueAlpha = (int)( eb );
				posValueAlpha = posValueAlpha < 0 ? 0 : posValueAlpha;
				posValueAlpha = posValueAlpha > m_maxOutValue ? m_maxOutValue : posValueAlpha;

				buf[i+offset5] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					m_maxOutValue,
					//m_maxOutValue
					posValueAlpha//offsetA5+i
					);
			}
		
		}

		
		for (i=offset5+m_dimBy6; i<GetDimension(); i++)
		{
			buf[i] = Point3D<T>( 
					m_maxOutValue, 
					m_maxOutValue, 
					m_maxOutValue,
					//m_maxOutValue
					m_maxOutValue
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
					//m_maxOutValue
					i
					);
			}

			if (i+offset1 < GetDimension())
			{
				buf[i+offset1] = Point3D<T>( 
					negValue ,
					0, 
					m_maxOutValue, 
					//m_maxOutValue
					offsetA1+i
					);
			}

			if (i+offset2 < GetDimension())
			{
				buf[i+offset2] = Point3D<T>( 
					0, 
					posValue, 
					negValue,
					//m_maxOutValue
					offsetA2+i
					);
			}

			if (i+offset3 < GetDimension())
			{
				buf[i+offset3] = Point3D<T>( 
					posValue, 
					m_maxOutValue, 
					0,
					//m_maxOutValue
					offsetA3+i
					);
			}

			if (i+offset4 < GetDimension())
			{
				buf[i+offset4] = Point3D<T>( 
					m_maxOutValue, 
					negValue, 
					0,
					//m_maxOutValue
					offsetA4+i
					);
			}

			if (i+offset5 < GetDimension())
			{
				buf[i+offset5] = Point3D<T>( 
					m_maxOutValue, 
					posValue, 
					posValue,
					//m_maxOutValue
					offsetA5+i
					);
			}
		
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

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_TransferFunctionLUT_HPP_
