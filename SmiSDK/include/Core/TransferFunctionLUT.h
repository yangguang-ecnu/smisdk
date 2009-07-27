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
// Filename:	TransferFunctionLUT.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_TransferFunctionLUT_H_
#define _PG_TransferFunctionLUT_H_
//////////////////////////////////////////////////////////////////////////


#include "ObjectBase.h"
#include "BaseDataObject.h"
#include "Image.h"
#include "Math/Point3D.h"

namespace PGCore
{

	typedef enum 
	{
		kPgLUTTypeUnknown=-1,
		kPgLUTTypeVIBGYOR=0,
		kPgLUTTypeGrayScale=1,
		kPgLUTTypeIronHot=2
	} ePgLUTType;

	template <class T>
	class TransferFunctionLUT : public ObjectBase, public BaseDataObject
	{

	public:

		TransferFunctionLUT(ePgLUTType iType=kPgLUTTypeGrayScale, const long iDimension=PG_LUT_SIZE);

		virtual ~TransferFunctionLUT(void);

		bool SetWindow(const float iLowLevel, const float iHighLevel);

		Point3D<T>* GetBuffer();

		bool Clear();

		bool Reset(const T iValue=0);		

		const long  GetDimension() const;		

		bool SetType(ePgLUTType iType);

		const ePgLUTType GetType() const;	

		bool Dump() const;

	protected: 
		ePgLUTType					m_LutType;
		PGCore::Image<Point3D<T> >	m_image;
		float						m_lowLevel, m_highLevel;

		bool SetDimension(const long &iDimension);

		bool UpdateDims();

		bool FillLuT();

	private:

		int		m_maxOutValue;
		int		m_dimBy6, m_windowLow;
		float	m_step;

		bool fillVIBGYOR();
		bool fillGrayScale();
		bool fillHotIron();

	};

};
//////////////////////////////////////////////////////////////////////////
// EOF

#ifndef _PG_GENERATE_SDK_LIBS_
#include "TransferFunctionLUT.hpp"
#endif

#endif	// _PG_TransferFunctionLUT_H_
