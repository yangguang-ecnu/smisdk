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
