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
