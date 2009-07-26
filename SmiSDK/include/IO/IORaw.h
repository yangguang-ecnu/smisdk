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
// Filename:	IORaw.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IORaw.h: interface for the IORaw class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IORAW_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IORAW_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOBase.h"
#include "Algs/ImageEndianSwapper.h"
#include "Algs/ImageBitClipper.h"

namespace PGIO
{

template <class T>
class IORaw : public IOBase<T> 
{
public:
	IORaw(void);
	
	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);	


	~IORaw(void);
	
	virtual int NotifyFinished();
	virtual int NotifyError(const char *iMsg=0);

};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IORaw.hpp"
#endif


#endif // !defined(AFX_IORAW_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
