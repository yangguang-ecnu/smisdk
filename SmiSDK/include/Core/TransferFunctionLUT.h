// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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
