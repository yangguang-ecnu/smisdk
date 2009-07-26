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
// Filename:	BaseProcessObject.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:12:57 PM
//

#ifndef _PGBaseProcessObject_H_
#define _PGBaseProcessObject_H_
//////////////////////////////////////////////////////////////////////////
#ifndef _PG_Core_H_
	#include "Core/Core.h"
#endif
#include "Core/BaseDataObject.h"
#include "Core/LoggerBase.h"

namespace PGAlgs
{

template <class T, class U>
class BaseProcessObject : public PGCore::LoggerBase
{
public:
	BaseProcessObject(void);	

	bool SetInput(PGCore::BaseDataObject *iDataObject);

	bool AttachCB(PGAlgs::BaseProcessObject<T, U> *iProcessCB);

	virtual bool UpdateCB(void	*iDataObject);

	bool UpdateAttachedCB(void	*iDataObject);
	
	// abstract methods
	virtual bool Execute()=0;
	virtual bool GetOutput(PGCore::BaseDataObject *oDataObject)  = 0;
	
	static void StatusCallBackFn (double ioProgressCounter, std::string &ioStatusMsg, void *ioSrcProcess, void *ioData);
		
public:	
	virtual ~BaseProcessObject(void);

protected:
	PGCore::BaseDataObject	*m_iDataObject;
	PGCore::BaseDataObject	*m_oDataObject;

	PGAlgs::BaseProcessObject<T, U>		*m_iProcessCB;
};
};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGBaseProcessObject_H_
