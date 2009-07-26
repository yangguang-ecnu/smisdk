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