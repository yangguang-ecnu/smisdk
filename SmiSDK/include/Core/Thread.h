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