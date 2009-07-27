// -*- C++ -*-
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License. You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// More details about the license can be found here: http://www.apache.org/licenses
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is 
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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

