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

	unsigned short GetDataCount() const;

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
