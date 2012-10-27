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
	unsigned short MultiDataBase<T>::GetDataCount(ePgDataObjectType iQueryType/*=kPgDataObjectTypeUnknown*/) const
	{
		if (iQueryType==kPgDataObjectTypeUnknown) return m_dataObjects.size();	

		int objCount=0;
		for (int i=0; i<m_dataObjects.size(); i++)
		{
			if (m_dataObjects[i]->Type()==iQueryType)
			{
				objCount++;
			}
		}

		return objCount;
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
