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
// Filename:	BaseProcessObject.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:12:57 PM
//

//////////////////////////////////////////////////////////////////////////
#include "Algs/BaseProcessObject.h"

namespace PGAlgs
{ 

	template <class T, class U>
	BaseProcessObject<T, U>::BaseProcessObject(void) 
	{
		m_iDataObject = 0;
		m_oDataObject = 0;
		m_iProcessCB = 0;
	};

	template <class T, class U>
	bool BaseProcessObject<T, U>::SetInput(PGCore::BaseDataObject *iDataObject)
	{
		m_iDataObject = iDataObject;
		return true;
	}

	template <class T, class U>
	bool BaseProcessObject<T, U>::AttachCB(PGAlgs::BaseProcessObject<T, U> *iProcessCB)
	{
		m_iProcessCB = iProcessCB;
		return true;
	}

	template <class T, class U>
	bool BaseProcessObject<T, U>::UpdateCB(void	*iDataObject) { return false; };

	template <class T, class U>
	bool BaseProcessObject<T, U>::UpdateAttachedCB(void	*iDataObject) 
	{ 
		if (m_iProcessCB) return m_iProcessCB->UpdateCB(iDataObject);
		return false; 
	};
	
	template <class T, class U>
	bool BaseProcessObject<T, U>::GetOutput(PGCore::BaseDataObject *oDataObject)  = 0;
	
	template <class T, class U>
	void BaseProcessObject<T, U>::StatusCallBackFn (double ioProgressCounter, std::string &ioStatusMsg, void *ioSrcProcess, void *ioData) 
	{
		//LOG2("AffineRegistration: Progress [%3.2f]: [%s]", ioProgressCounter, ioStatusMsg.c_str());	
		if (ioSrcProcess)
		{
			PGAlgs::BaseProcessObject<T, U> *srcProcess = static_cast<PGAlgs::BaseProcessObject<T, U>	*>(ioSrcProcess);
			if (srcProcess)
			{
				srcProcess->UpdateAttachedCB(static_cast<PGCore::BaseDataObject	*>(ioData));
			}
		}
	}

	template <class T, class U>
	BaseProcessObject<T, U>::~BaseProcessObject(void) {};

};

//////////////////////////////////////////////////////////////////////////
// EOF

