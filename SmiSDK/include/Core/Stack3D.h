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