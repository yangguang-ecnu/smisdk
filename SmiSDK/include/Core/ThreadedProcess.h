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
