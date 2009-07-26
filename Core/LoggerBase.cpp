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
// Filename:	LoggerBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:54:24 AM
//
//////////////////////////////////////////////////////////////////////////

#include "Core/LoggerBase.h"

namespace PGCore 
{

CRITICAL_SECTION PGCore::LoggerBase::m_cs = CRITICAL_SECTION();

LoggerBase::LoggerBase(/*PGCore::Logger *ipLogger = ppgLogger*/) 
{
	/* Initialize the critical section -- This must be done before locking */
	InitializeCriticalSection(&PGCore::LoggerBase::m_cs);

	m_ppgLogger = 0;		
}; 
	
LoggerBase::~LoggerBase() 
{ 
	/* Release system object when all finished -- usually at the end of the cleanup code */
	DeleteCriticalSection(&PGCore::LoggerBase::m_cs); 
};

void LoggerBase::SetLogger(PGCore::Logger *ipLogger) 
{
	//EnterCriticalSection(&PGCore::LoggerBase::m_cs);				
	ppgLogger = ipLogger;
	//LeaveCriticalSection(&PGCore::LoggerBase::m_cs);	
};

void LoggerBase::SetLocalLogger(PGCore::Logger *ipLogger) 
{
	m_ppgLogger = ipLogger;	
};


PGCore::Logger * LoggerBase::GetLocalLogger() 
{		
	return m_ppgLogger;
};


PGCore::Logger * LoggerBase::GetLogger() 
{		
	return ppgLogger;
};

};

//////////////////////////////////////////////////////////////////////////
// EOF

