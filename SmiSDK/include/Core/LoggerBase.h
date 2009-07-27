// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
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
