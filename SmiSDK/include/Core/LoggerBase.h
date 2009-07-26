// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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
