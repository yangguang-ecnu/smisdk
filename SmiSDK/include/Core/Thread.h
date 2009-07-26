// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
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
// Filename:	Thread.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//

// Thread.h: interface for the Thread class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PG_Thread_H 
#define _PG_Thread_H "_PG_Thread_H"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadedProcess.h"
#include <process.h>



namespace PGCore
{	

class Thread
{
public:
	Thread();

	~Thread();

	HANDLE CreateThread(PGCore::ThreadedProcess *iParentProcessPtr);
	
	void WaitToFinish();

	void CloseThread();

	bool IsAlive();

private:
    HANDLE	m_threadHandle;
	bool	m_alive;

} ;
	

};

#endif // !defined(AFX_Thread_H__9B651563_FA0B_4911_8024_918D60EB56AD__INCLUDED_)