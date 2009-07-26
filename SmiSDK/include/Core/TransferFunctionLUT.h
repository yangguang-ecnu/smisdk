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
