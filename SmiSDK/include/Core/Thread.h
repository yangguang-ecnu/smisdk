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