// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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