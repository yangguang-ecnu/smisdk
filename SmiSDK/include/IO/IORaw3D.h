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
// Filename:	IORaw3D.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IORaw3D.h: interface for the IORaw3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IORAW3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IORAW3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IORaw.h"
#include "Core/GaussianKernel.h"

namespace PGIO
{

template <class T>
class IORaw3D : public IOBase<T>, public PGCore::ThreadedProcess
{
public:
	IORaw3D(void);
	
	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);	

	~IORaw3D(void);

	virtual void StartExecution(void *iParam);

private:
	bool ReadFromFile(const std::string& iFileName,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);	
	bool ReadFromFileMT(const std::string& iFileName);	
	
	bool WriteToFile(const std::string& iFolder,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);		
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IORaw3D.hpp"
#endif

#endif // !defined(AFX_IORAW3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
