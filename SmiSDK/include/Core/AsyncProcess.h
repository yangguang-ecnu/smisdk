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
// Filename:	AsyncProcess.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//

// AsyncProcess.h: interface for the AsyncProcess class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PG_AsyncProcess_H 
#define _PG_AsyncProcess_H "_PG_AsyncProcess_H"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _PG_MAX_THREADS_ 8


#include "Thread.h"


namespace PGCore
{

class AsyncProcessParams
{
public: 
	AsyncProcessParams();

	void*							iParentThreadPtr;		
	StatusCallBackFunctionD			statusCB;
	PGCore::Logger*					ioLogger;	// need to check. this doesnt work too well	
	bool							m_asyncEnabled;
};


class AsyncProcess : public PGCore::LoggerBase
{
public:
	AsyncProcess();

	~AsyncProcess();

	bool CreateNewThread(PGCore::ThreadedProcess* iParentProcessPtr, void *iParams);
	
	// -1: threads remain
	//  0: false, not in async mode
	//  1: true
	virtual int NotifyFinished();

	
	//  0: false, not in async mode
	//  1: true
	virtual int NotifyError(const char *iMsg=0);

	virtual bool ConsolidateWhenDone() = 0;

	bool WaitToFinishIfNeeded();

	const int& GetAliveThreadCount() const;
	
	const int& GetMaxThreadCount() const;

	void SetMaxThreadCount(const int& iMaxCount);

private:
	PGCore::ThreadedProcess*	m_pProcess;
	PGCore::Thread				m_thread[_PG_MAX_THREADS_];
	HANDLE						m_hThreadMT[_PG_MAX_THREADS_];
    unsigned					m_threadIDMT[_PG_MAX_THREADS_];	
	int							m_threadsRemaining;
	int							m_numThreadsMT;	
	CRITICAL_SECTION			m_csMT;

} ;


};

#endif // !defined(AFX_Thread_H__9B651563_FA0B_4911_8024_918D60EB56AD__INCLUDED_)