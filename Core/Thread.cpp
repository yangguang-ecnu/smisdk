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
// Filename:	Thread.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//
//
//////////////////////////////////////////////////////////////////////

#include "Core/Thread.h"

namespace PGCore
{	

	Thread::Thread()
	{		
		m_alive = false;
	}

	Thread::~Thread()
	{		
		CloseThread();
	}

	bool Thread::IsAlive()
	{
		return m_alive;
	}

	HANDLE Thread::CreateThread(PGCore::ThreadedProcess *iParentProcessPtr)
	{		
		void *iPtr = static_cast<void *>(iParentProcessPtr);  
		m_threadHandle = ::CreateThread(NULL,	// default security
			0,							// default stack size
			&ThreadEntryPoint,			// thread function
			iPtr,						// thread function argument: parent process ptr
			0,							// default creation flags
			0);							// don't want thread id

		m_alive = true;
		return m_threadHandle;
	}


	void Thread::WaitToFinish()
	{
		WaitForSingleObject( m_threadHandle, INFINITE );
		CloseThread();	
	}

	void Thread::CloseThread()
	{
		if (m_alive)
		{
			::CloseHandle(m_threadHandle);
			m_alive = false;
		}
	}	



};

