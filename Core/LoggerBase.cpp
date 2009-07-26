// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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

