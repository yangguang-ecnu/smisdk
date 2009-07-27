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
// Filename:	MultiDataBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:19:34 PM
//

#ifndef _MultiDataBase_HPP_
#define _MultiDataBase_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/MultiDataBase.h"

namespace PGCore 
{

	template<class T>
	MultiDataBase<T>::MultiDataBase(void) 
	{
	};	

	template<class T>
	unsigned short MultiDataBase<T>::GetDataCount() const
	{
		return m_dataObjects.size();	
	}

	template<class T>
	void MultiDataBase<T>::Clear() {
		m_dataObjects.clear();	
	}

	template<class T>
	MultiDataBase<T>::~MultiDataBase(void) { Clear();	};

	template <class T>
	bool MultiDataBase<T>::AddDataObject(BaseDataObject* iDataObject) {
		m_dataObjects.push_back(iDataObject);
		return true;
	}


	template <class T>
	BaseDataObject* MultiDataBase<T>::GetDataObject(unsigned int iIndex) const {
		if (iIndex < m_dataObjects.size()) {		
			return m_dataObjects[iIndex];;
		}
		return NULL;
	}

	template <class T>	
	MultiDataBase<T>::MultiDataBase(const MultiDataBase<T>& iMultiObject)
	{
		*this = iMultiObject;
		//return *this;
	}

	template <class T>
	MultiDataBase<T> &MultiDataBase<T>::operator=(const MultiDataBase<T> &iMultiObject) {	
		m_dataObjects.clear();	

		// copy objects
		for (int i=0; i<iMultiObject.GetDataCount(); i++) {
			BaseDataObject *pDataObject = iMultiObject.GetDataObject(i);		
			AddDataObject(pDataObject);		
		}
		return *this;
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class MultiDataBase<short>; 
	template class MultiDataBase<unsigned short>; 
	template class MultiDataBase<unsigned char>; 
#endif


};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _MultiDataBase.H_H_
