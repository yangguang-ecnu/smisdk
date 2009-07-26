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
// Filename:	LoggerBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:54:24 AM
//

#ifndef _PG_LoggerBase_H_
#define _PG_LoggerBase_H_
//////////////////////////////////////////////////////////////////////////
#ifndef _PG_Core_H_
	#include "Core.h"
#endif
#include "Logger.h"

namespace PGCore 
{

static PGCore::Logger *ppgLogger = 0;

class LoggerBase 
{
public: 
	LoggerBase(); 
	
	virtual ~LoggerBase(); 

	static void SetLogger(PGCore::Logger *ipLogger); 
	
	void SetLocalLogger(PGCore::Logger *ipLogger); 

	PGCore::Logger * GetLocalLogger(); 

protected:
	static PGCore::Logger * GetLogger(); 

	
	
protected:
	

private:
	static	CRITICAL_SECTION  m_cs;

	PGCore::Logger *m_ppgLogger;
	
};

};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_LoggerBase_H_
