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
// Filename:	IOPgm.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IOPgm.h: interface for the IOPgm class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOPgm_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IOPgm_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOBase.h"
#include "Core/PixelBase.h"

namespace PGIO
{

template <class T>
class IOPgm : public IOBase<T> 
{
public:
	IOPgm(void);

	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);		

	virtual bool GetAttributes(PGCore::MetaData<T>& oMetaDataObject, const std::string &iFilePath);	

	~IOPgm(void);	

	virtual void StartExecution(void *iParams);
	virtual int NotifyFinished();
	virtual int NotifyError(const char *iMsg=0);

protected:
	
	virtual bool WriteMetaData(PGCore::BaseDataObject *iMetaDataObject, const IOParams &iParams);

	virtual bool ReadMetaData(PGCore::BaseDataObject *oMetaDataObject, const IOParams &iParams);	

	
};

};


#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IOPgm.hpp"
#endif

#endif // !defined(AFX_IOPgm_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
