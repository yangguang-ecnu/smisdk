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
// Filename:	BaseProcessObject.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:12:57 PM
//

#ifndef _PGBaseProcessObject_H_
#define _PGBaseProcessObject_H_
//////////////////////////////////////////////////////////////////////////
#ifndef _PG_Core_H_
	#include "Core/Core.h"
#endif
#include "Core/BaseDataObject.h"
#include "Core/LoggerBase.h"

namespace PGAlgs
{

template <class T, class U>
class BaseProcessObject : public PGCore::LoggerBase
{
public:
	BaseProcessObject(void);	

	bool SetInput(PGCore::BaseDataObject *iDataObject);

	bool AttachCB(PGAlgs::BaseProcessObject<T, U> *iProcessCB);

	virtual bool UpdateCB(void	*iDataObject);

	bool UpdateAttachedCB(void	*iDataObject);
	
	// abstract methods
	virtual bool Execute()=0;
	virtual bool GetOutput(PGCore::BaseDataObject *oDataObject)  = 0;
	
	static void StatusCallBackFn (double ioProgressCounter, std::string &ioStatusMsg, void *ioSrcProcess, void *ioData);
		
public:	
	virtual ~BaseProcessObject(void);

protected:
	PGCore::BaseDataObject	*m_iDataObject;
	PGCore::BaseDataObject	*m_oDataObject;

	PGAlgs::BaseProcessObject<T, U>		*m_iProcessCB;
};
};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGBaseProcessObject_H_
