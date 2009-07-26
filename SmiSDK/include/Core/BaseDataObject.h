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
// Filename:	BaseDataObject.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:54:24 AM
//

#ifndef _PG_BaseDataObject_H_
#define _PG_BaseDataObject_H_
//////////////////////////////////////////////////////////////////////////

#ifndef _PG_Core_H_
	#include "Core.h"
#endif
#include "LoggerBase.h"

namespace PGCore 
{

typedef enum {
	kPgDataObjectTypeUnknown=-1,
	kPgDataObjectTypePixel=0,
	kPgDataObjectTypeImage=1,
	kPgDataObjectTypeVolume=2,
	kPgDataObjectTypeIsotropicVoxel3D=3,
	kPgDataObjectTypeKernel=4,
	kPgDataObjectTypeTransferFunctionLUT=5,
	kPgDataObjectTypeBitImage=6,
	kPgDataObjectTypeBitVolume=7
} ePgDataObjectType;


class BaseDataObject : public LoggerBase
{
public:
	BaseDataObject();
	
	virtual ~BaseDataObject();

	ePgDataObjectType Type() const;
	
protected:
	ePgDataObjectType	m_type;		

private:
	
};

};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_BaseDataObject_H_
