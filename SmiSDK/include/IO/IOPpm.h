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
// Filename:	IOPpm.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IOPpm.h: interface for the IOPpm class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PPM_H_)
#define _PPM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IO/IOBase.h"
#include "Core/PixelBase.h"

namespace PGIO
{

template <class T>
class IOPpm : public IOBase<T> 
{
public:
	IOPpm(void);

	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);		

	virtual bool GetAttributes(PGCore::MetaData<T>& oMetaDataObject, const std::string &iFilePath);	

	~IOPpm(void);	

	virtual void StartExecution(void *iParams);
	virtual int NotifyFinished();
	virtual int NotifyError(const char *iMsg=0);


protected:

	virtual bool WriteMetaData(PGCore::BaseDataObject *iMetaDataObject, const IOParams &iParams);

	virtual bool ReadMetaData(PGCore::BaseDataObject *oMetaDataObject, const IOParams &iParams);	

};



};


#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IO/IOPpm.hpp"
#endif

#endif // !defined(_PPM_H_)
