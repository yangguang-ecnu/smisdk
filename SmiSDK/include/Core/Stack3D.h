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
// Filename:	Stack3D.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Stack3D_H_
#define _PG_Stack3D_H_
//////////////////////////////////////////////////////////////////////////

#include "Stack.h"
#include "Math/Point3D.h"

namespace PGCore
{
	template <class T>
	class Stack3D : public ObjectBase, public BaseDataObject
	{

	public:

		Stack3D(const int iZSize=256);
			
		virtual ~Stack3D(void);	

		void SetZSize(const int iZSize);
		
		void Clear();

		bool Push(const PGMath::Point3D<T>& iPoint);
		
		bool Pop(PGMath::Point3D<T>& oPoint);
	
		const int Size() const;
		
	protected: 
		Stack< Stack< PGMath::Point3D< T > > *>			m_stacks;	
		std::vector<Stack<PGMath::Point3D<T> > >		m_sliceStacks;
				
	private:

		

	};

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Stack3D_H_