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
// Filename:	ObjectBase.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 5:46:58 PM
//


// ObjectBase.h: interface for the ObjectBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTBASE_H__00DB3C84_0038_4CB6_9B29_8DB569BBA642__INCLUDED_)
#define AFX_OBJECTBASE_H__00DB3C84_0038_4CB6_9B29_8DB569BBA642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _PG_USE_VIRTUAL_ALLOCATION_ 0

#ifndef _PG_Core_H_
	#include "Core.h"
#endif
#include <assert.h>
#include "BaseDataObject.h"


namespace PGCore 
{
//////////////////////////////////////////////////////////////////////////
	
typedef enum ePgAlignmentPolicy
{
		kPgAlignmentPolicyUnknown = 0,
		kPgAlignmentPolicyDefault = 1,
		kPgAlignmentPolicy16BitsAligned = 2
};



//////////////////////////////////////////////////////////////////////////
class ObjectBase  : public LoggerBase
{
	public:
		ObjectBase() 
		{ 
			m_size =0; 
			m_buffer = 0; 		
			/* Initialize the critical section -- This must be done before locking */
			InitializeCriticalSection(&m_cs);
		};

		// the consumer class is responsible for cleanup
		virtual ~ObjectBase() 
		{ 	
			try {
				//LOG2("ObjectBase: [%d] Now remaining unfreed MegaBytes: %6.4f", this, ((float)kPgTotalBytesAllocated)/(1024.0f*1024.0f));			
				/* Release system object when all finished -- usually at the end of the cleanup code */
				//DeleteCriticalSection(&m_cs);
			} catch (...) 
			{
				LOG1("ObjectBase: [%d] Failed to delete critical section", this);			
			}
			
		};
		
		static long int GetAvailableMainMemory();

		void* GetBuffer() { return (void *)m_buffer; }

		void* Alloc(const long &iSize,				    					
					ePgAlignmentPolicy iPolicy = kPgAlignmentPolicyDefault);
	
		bool Clear();		

	private:		
		CRITICAL_SECTION	m_cs;
		void*				m_buffer;
		long				m_size;		

		void* allocateChunk(void *oChunkPointer, const long &iSize);
		bool  freeChunk(void *iChunk, const long &iSize);		
};
//////////////////////////////////////////////////////////////////////////

inline void* ObjectBase::Alloc(
					const long &iSize,				    
					ePgAlignmentPolicy iPolicy/* = kPgAlignmentPolicyDefault*/)
{
	if (iSize <=0) 
	{		
		//LOG0("Invalid element size/count");
		return 0;
	}

	// clear previous memory
	Clear();
	
	m_buffer = (void *)allocateChunk((void *)m_buffer, iSize);
	if (!m_buffer)		
	{		
		LOG2("<%d>[PG--] Failed to allocate pointer of size [%d].", this, iSize );		
		assert(0 &&  "Failed to allocate memory");
		return 0;	
	}

	m_size = iSize;

	/* Enter the critical section -- other threads are locked out */
	EnterCriticalSection(&m_cs);	
	kPgTotalBytesAllocated+=iSize;
	kPgPeakBytesAllocated = kPgPeakBytesAllocated<kPgTotalBytesAllocated ? kPgTotalBytesAllocated : kPgPeakBytesAllocated;
	

	//#ifdef _DEBUG_DUMP
	#if (_TRACK_MEMORY)
	LOG3("[C++] Allocated [%d] [Size:%d] [Peak: %d KB]", m_buffer, iSize, kPgPeakBytesAllocated/1024);		
	#endif		

#if (_TRACK_MEMORY)
		memInfo[(int)m_buffer] = PGMemInfo(false, (int)m_buffer, iSize);
		pointerIndex.push_back((int)m_buffer);
#endif
	LeaveCriticalSection(&m_cs);	

	return m_buffer;
}

//////////////////////////////////////////////////////////////////////////
inline bool ObjectBase::Clear() 
{
	bool res = freeChunk(m_buffer, m_size);

	if (res) m_buffer = 0;
	
	return res;
}

//////////////////////////////////////////////////////////////////////////
inline void* ObjectBase::allocateChunk(void *oChunk, const long &iSize) 
{
		
#if _PG_USE_VIRTUAL_ALLOCATION_	
	return VirtualAlloc(
			oChunk, 
			iSize,
			MEM_COMMIT,
			PAGE_EXECUTE_READWRITE
	);
#else 	
	char *pChunk = (char *)calloc(iSize, sizeof(char));
	oChunk = pChunk;
	return (void *)pChunk;
#endif
}
//////////////////////////////////////////////////////////////////////////
inline bool ObjectBase::freeChunk(void *iChunk, const long &iSize) {

	if (iChunk==NULL || iSize<=0) 
	{				
		return false;
	}

		
#if (_TRACK_MEMORY)
	EnterCriticalSection(&m_cs);
		memInfo[(int)iChunk].freed = true;
		memInfo[(int)iChunk].numberOfBytes = 0;
		memInfo[(int)iChunk].pointerToBlock = 0;
	LeaveCriticalSection(&m_cs);	
#endif
	


	try {		
		#if _PG_USE_VIRTUAL_ALLOCATION_
		if (iChunk)
		{
			VirtualFree(iChunk, 
				iSize, MEM_COMMIT);
		}
		#else
		if (iChunk)
		{
			free(iChunk);		
		}
		#endif
	}
	catch (...) {		
		LOG3("<%d>[PG--] Failed to free pointer [%d] of size [%d].", this, iChunk, iSize );		
		return false;	
	}	

	EnterCriticalSection(&m_cs);	
	kPgTotalBytesAllocated-=iSize;
	#if (_TRACK_MEMORY)
	LOG3("[C++] Freed [%d] [Size:%d] [Remaining: %d]", m_buffer, iSize, kPgTotalBytesAllocated);		
	#endif
	LeaveCriticalSection(&m_cs);	

	return true;
}


// in KBytes
inline long int ObjectBase::GetAvailableMainMemory()
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	long res = statex.ullAvailPhys/kPgBytesInKB;
	
	LOG1("Available physical memory: %d MB", int(res/1024));

	return res;
}


};
#endif // !defined(AFX_OBJECTBASE_H__00DB3C84_0038_4CB6_9B29_8DB569BBA642__INCLUDED_)
