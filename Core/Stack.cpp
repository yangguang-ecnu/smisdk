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
// Filename:	Stack.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
//////////////////////////////////////////////////////////////////////////
#include "Core/Stack.h"
#include "Math/Point3D.h"

namespace PGCore
{
	template <class T>
	Stack<T>::Stack(const long iMaxSize/*=PG_USHORT_MAX*/) 
	{
		m_maxSize = iMaxSize;
	};	

	template <class T>
	Stack<T>::~Stack(void) 
	{
	};	

	template <class T>
	void Stack<T>::Clear() 
	{ 
		m_buffer.clear(); 
	};

	template <class T>
	bool Stack<T>::Push(const T& iElement)
	{
		if (Size() >= m_maxSize)
		{
			//GetLogger()->Log("Stack[%d]:Push size: %d", this, Size());
			return false;
		}

		m_buffer.push_back(iElement);
		return true;
	}

	template <class T>
	const T Stack<T>::Pop()
	{
		if (Size() ==0)
		{
			//GetLogger()->Log("Stack[%d]:Pop size: %d", this, Size());
			return T();
		}

		T pElement = m_buffer.back();
		m_buffer.pop_back();			
		return pElement;
	}

	template <class T>
	T Stack<T>::Top() 
	{
		if (Size() ==0)
		{
			//GetLogger()->Log("Stack[%d]:Pop size: %d", this, Size());
			return 0;
		}

		return (m_buffer.back());
	};

	template <class T>
	const long Stack<T>::Size() const 
	{ 
		long oSize = m_buffer.size(); 
		return oSize;
	};

#ifdef _PG_GENERATE_SDK_LIBS_
	template class Stack<int>;	
	template class Stack<PGMath::Point3D< int >>;		
	template class Stack<Stack<PGMath::Point3D< int >>*>;		
#endif
	

};
//////////////////////////////////////////////////////////////////////////
// EOF
