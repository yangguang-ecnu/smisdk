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
// Filename:	Stack3D.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Core/Stack3D.h"

namespace PGCore
{
		template <class T>
		Stack3D<T>::Stack3D(const int iZSize=256) 
		{
			SetZSize(iZSize);
		};	

		template <class T>
		Stack3D<T>::~Stack3D(void) {};	

		template <class T>
		void Stack3D<T>::SetZSize(const int iZSize)
		{
			Clear();
			m_sliceStacks.resize(iZSize);

			//for (int i=0; i<iZSize; i++)
			//	m_sliceStackPtrs[i].Clear();
		}

		template <class T>
		void Stack3D<T>::Clear() 
		{ 
			m_stacks.Clear(); 
			m_sliceStacks.clear();
		};

		template <class T>
		bool Stack3D<T>::Push(const PGMath::Point3D<T>& iPoint)
		{
			if (iPoint.Z() > m_sliceStacks.size()-1)			
			{
				GetLogger()->Log("Stack3D[%d]:Push inZ: %d, size: %d", this, iPoint.Z(), m_sliceStacks.size());
				return false;
			}

			if (m_sliceStacks[iPoint.Z()].Size()==0)
			{
				// add new slice stack
				//Stack<PGMath::Point3D<T>> newStack;
				if (!m_stacks.Push(&m_sliceStacks[iPoint.Z()]))
				//m_sliceStackPtrs[iPoint.Z()] = m_stacks.Top();

				//if (m_sliceStackPtrs[iPoint.Z()]==NULL)
				{
					GetLogger()->Log("Stack3D[%d]:Push: Error adding new stackslice for Z %d", this, iPoint.Z());
					return false;
				}
			}

			return m_sliceStacks[iPoint.Z()].Push(iPoint);
		}

		template <class T>
		bool Stack3D<T>::Pop(PGMath::Point3D<T>& oPoint)
		{
			if (Size()==0)
			{
				GetLogger()->Log("Stack3D[%d]:Pop size: %d", this, Size());
				return false;
			}

			Stack<PGMath::Point3D<T> >* oTopStack = m_stacks.Top();
			if (oTopStack->Size()==0)
			{
				GetLogger()->Log("Stack3D[%d]:Top is empty. Something wrong", this);
				return false;
			}
			
			oPoint = oTopStack->Pop();

			if (oTopStack->Size()==0) 
			{
				m_stacks.Pop();
				//m_sliceStackPtrs[oPoint.Z()] = 0;
			}

			return true;
		}

		template <class T>
		const int Stack3D<T>::Size() const 
		{ 
			int oSize = m_stacks.Size(); 
			return oSize;
		}
		
			

#ifdef _PG_GENERATE_SDK_LIBS_
	template class Stack3D<int>;	
#endif
	

};
//////////////////////////////////////////////////////////////////////////
// EOF
