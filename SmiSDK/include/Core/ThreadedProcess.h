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
// Filename:	ThreadedProcess.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//

// ThreadedProcess.h: interface for the ThreadedProcess class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PG_ThreadedProcess_H 
#define _PG_ThreadedProcess_H "_PG_ThreadedProcess_H"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>
#include "LoggerBase.h"

namespace PGCore
{

DWORD WINAPI EntryPoint(void *iParentThreadedProcess);
DWORD WINAPI ThreadEntryPoint(void *iParentThreadedProcess);

class ThreadedProcess : public PGCore::LoggerBase
{
public:
	ThreadedProcess();

	~ThreadedProcess();
	
	void SetParams(void *iParams);

	void *GetParams();

	// overload this method to allow threaded execution
	virtual void StartExecution(void *iParams) = 0;

	void SetAsyncEnabled(const bool iFlag);

	bool GetAsyncEnabled() const;

protected:

private:
	void *m_params;
	bool m_asyncThreadedProcessingEnabled;

} ;



};

#endif // !defined(AFX_Thread_H__9B651563_FA0B_4911_8024_918D60EB56AD__INCLUDED_)
