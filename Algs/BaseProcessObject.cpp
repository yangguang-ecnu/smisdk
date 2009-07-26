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

