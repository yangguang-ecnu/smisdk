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