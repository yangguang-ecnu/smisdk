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
// Filename:	MultiDataBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:19:34 PM
//

#ifndef _MultiDataBase_H_
#define _MultiDataBase_H_
//////////////////////////////////////////////////////////////////////////

#include "BaseDataObject.h"

namespace PGCore 
{

template <class T>
class MultiDataBase : public BaseDataObject
{
public:
	MultiDataBase(void);	

    MultiDataBase(const MultiDataBase<T>& iMultiObject);

    MultiDataBase<T> & operator = (const MultiDataBase<T> & iMultiImageObject);

	bool AddDataObject(PGCore::BaseDataObject * iDataObject);	

	BaseDataObject* GetDataObject(unsigned int iIndex) const;	

	unsigned short GetDataCount(ePgDataObjectType iQueryType=kPgDataObjectTypeUnknown) const;

	void Clear();

	virtual ~MultiDataBase(void);

protected:
	std::vector <BaseDataObject *>	m_dataObjects;		
	
};

};

#ifndef _PG_GENERATE_SDK_LIBS_	
	#include "MultiDataBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _MultiDataBase.H_H_
