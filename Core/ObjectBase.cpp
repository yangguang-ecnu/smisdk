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
// Filename:	ObjectBase.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 5:46:58 PM
//
//
//////////////////////////////////////////////////////////////////////

#include "Core/ObjectBase.h"


namespace PGCore 
{

	ObjectBase::ObjectBase() 
	{ 
		m_size =0; 
		m_buffer = 0; 		
		/* Initialize the critical section -- This must be done before locking */
		InitializeCriticalSection(&m_cs);
	};

	// the consumer class is responsible for cleanup
	ObjectBase::~ObjectBase() 
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

	void* ObjectBase::GetBuffer() { return (void *)m_buffer; }


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

		if (iChunk==NULL) {				
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

		LOG1("Available physical memory: %d MB", res/1024);

		return res;
	}


};

