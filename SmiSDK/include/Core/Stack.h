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
// Filename:	Stack.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Stack_H_
#define _PG_Stack_H_
//////////////////////////////////////////////////////////////////////////
#include <list>
#include "ObjectBase.h"
#include "BaseDataObject.h"


namespace PGCore
{
	template <class T>
	class Stack : public ObjectBase, public BaseDataObject
	{
	public:

		Stack(const long iMaxSize=PG_USHORT_MAX); 
		
		virtual ~Stack(void);	

		void Clear();

		bool Push(const T& iElement);		

		const T Pop();
		
		T Top(); 
		
		const long Size() const;
		
	protected: 
		std::vector<T>			m_buffer;
		long					m_maxSize;	
				
	private:	

	};

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Stack_H_