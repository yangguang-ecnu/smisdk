// -*- C++ -*-
// � [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
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
// Filename:	AsyncProcess.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//

// AsyncProcess.cpp: implementation for the AsyncProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "Core/AsyncProcess.h"


namespace PGCore
{

	AsyncProcessParams::AsyncProcessParams() 
	{
		iParentThreadPtr	=	0;
		statusCB			=	0;
		ioLogger			=	0;		
		m_asyncEnabled		=	false;
	};


	AsyncProcess::AsyncProcess()
	{		
		m_pProcess = 0;
		m_threadsRemaining=0; 
		m_numThreadsMT = _PG_MAX_THREADS_; 
		InitializeCriticalSection(&m_csMT);
	}

	AsyncProcess::~AsyncProcess()
	{		
		DeleteCriticalSection(&m_csMT);
	}

	bool AsyncProcess::CreateNewThread(PGCore::ThreadedProcess* iParentProcessPtr, void *iParams)
	{
		if (m_threadsRemaining>=_PG_MAX_THREADS_)
		{
			return false;
		}

		LOG1("Creating thread %d...", m_threadsRemaining);

		m_pProcess = iParentProcessPtr;    
		if (!m_pProcess) return false;

		m_pProcess->SetParams(iParams);

		m_hThreadMT[m_threadsRemaining] = m_thread[m_threadsRemaining].CreateThread(iParentProcessPtr);
		
		LOG3("Created thread %d with threadId [%d], handle [%d]", m_threadsRemaining, m_threadIDMT[m_threadsRemaining], m_hThreadMT[m_threadsRemaining]);

		if (m_pProcess->GetAsyncEnabled()) m_threadsRemaining++;

		//if (ioAsyncLoadMT) m_threadsRemaining++;

		return true;		
	}

	// -1: threads remain
	//  0: false, not in async mode
	//  1: true
	int AsyncProcess::NotifyFinished()
	{
		if (!m_pProcess) return false;
		if (!m_pProcess->GetAsyncEnabled()) return false;
		//if (!ioAsyncLoadMT) return 0;

		EnterCriticalSection(&m_csMT);
		m_threadsRemaining--;
		LeaveCriticalSection(&m_csMT);

		if (m_threadsRemaining>0) return -1;	

		::Sleep(100);

		for (int i=0; i<m_numThreadsMT; i++)
		{				
			//if (m_thread[i].IsAlive())
			{
				m_thread[i].CloseThread();
				LOG1("Closed thread with threadHandle [%d]", m_hThreadMT[i]);
			}
		}

		return ConsolidateWhenDone();
	}

	
	//  0: false, not in async mode
	//  1: true
	int AsyncProcess::NotifyError(const char *iMsg/*=0*/)
	{
		if (!m_pProcess) return false;
		if (!m_pProcess->GetAsyncEnabled()) return false;
		//if (!ioAsyncLoadMT) return 0;

		EnterCriticalSection(&m_csMT);
		m_threadsRemaining--;
		LeaveCriticalSection(&m_csMT);
		
		if (m_threadsRemaining>0) 
		{
			LOG1("%d threads remaining. One of them reported error. Killing all..", m_threadsRemaining);
		} else
		{
			LOG0("A thread reported error. Killing all..");
		}

		for (int i=0; i<m_numThreadsMT; i++)
		{				
			//if (m_thread[i].IsAlive())
			{					
				m_thread[i].CloseThread();
				LOG1("Closed thread with threadHandle [%d]", m_hThreadMT[i]);
			}
		}

		return 1;
	}

	bool AsyncProcess::WaitToFinishIfNeeded()
	{
		bool rv = false;
		if (!m_pProcess) return false;
		if (!m_pProcess->GetAsyncEnabled())
		{	
			for (int i=0; i<m_numThreadsMT; i++)
			{
				m_thread[i].WaitToFinish();
				LOG1("Closed thread with threadHandle [%d]", m_hThreadMT[i]);
			}

			rv = ConsolidateWhenDone();
		} else
		{
			::Sleep(1000); // allow the sub thread to copy data structures
			rv = true;
		}

		return rv;
	}

	const int& AsyncProcess::GetAliveThreadCount() const { return m_threadsRemaining; }
	
	const int& AsyncProcess::GetMaxThreadCount() const { return m_numThreadsMT; }

	void AsyncProcess::SetMaxThreadCount(const int& iMaxCount) { m_numThreadsMT = iMaxCount; }

};

