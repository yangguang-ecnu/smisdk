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
