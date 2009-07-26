// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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

