// -*- C++ -*-
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
// Filename:	ProcessBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:12:57 PM
//

#ifndef _PGProcessBase_H_
#define _PGProcessBase_H_
//////////////////////////////////////////////////////////////////////////
#ifndef _PG_Core_H_
	#include "Core/Core.h"
#endif
#include "Core/BaseDataObject.h"
#include "Core/LoggerBase.h"
#include "Core/ThreadedProcess.h"
#include "Core/AsyncProcess.h"


namespace PGAlgs
{

//template <class T, class U>
class ProcessBase : public PGCore::AsyncProcess
{
public:
	ProcessBase(void) 
	{
		m_CBFn = 0; 
		m_CBInitialized = false;
		m_iDataObject = 0;
		m_oDataObject = 0;
		m_iProcessCB.clear();
	};

	virtual bool SetInput(PGCore::BaseDataObject *iDataObject)
	{
		m_iDataObject = iDataObject;
		return true;
	}

	bool AttachCB(std::vector<PGAlgs::ProcessBase *> iProcessCB)
	{
		m_iProcessCB = iProcessCB;
		return true;
	}

	void UpdateProgress(float iProgress)
	{
		if (m_CBInitialized)
		{
			m_CBFn(iProgress);
		}
	}

	virtual bool UpdateCB(void	*iDataObject) 
	{ 
		return false; 
	};

	bool UpdateAttachedCB(void	*iDataObject, float iProgress=0) 
	{ 
		for (int i=0; i<m_iProcessCB.size(); i++)
		{
			if (m_iProcessCB[i]) m_iProcessCB[i]->UpdateCB(iDataObject);
		}

		UpdateProgress(iProgress);

		return this->UpdateCB(iDataObject);
	};
	

	void SetProgressCB(StatusCallBackFunctionD iCB)
	{
		m_CBFn = iCB;
		m_CBInitialized = true;
	}

	StatusCallBackFunctionD GetProgressCB()
	{
		return m_CBFn;
	}

	virtual bool Execute()=0;
	virtual bool GetOutput(PGCore::BaseDataObject *oDataObject)  = 0;
	
	// overload for AsyncProcess inheritance
	virtual int NotifyFinished();
	virtual int NotifyError(const char *iMsg=0);
	virtual bool ConsolidateWhenDone() { return true; };

	
	static void StatusCallBackFn (double ioProgressCounter, std::string &ioStatusMsg, void *ioSrcProcess, void *ioData) 
	{
		LOG2("AffineRegistration: Progress [%3.2f]: [%s]", ioProgressCounter, ioStatusMsg.c_str());	
		if (ioSrcProcess)
		{
			PGAlgs::ProcessBase *srcProcess = static_cast<PGAlgs::ProcessBase	*>(ioSrcProcess);
			if (srcProcess)
			{
				//srcProcess->UpdateAttachedCB(static_cast<PGCore::BaseDataObject	*>(ioData));
				srcProcess->UpdateAttachedCB(ioData, (float)ioProgressCounter);
			}
		}
	}

	


	//virtual float ObjectiveFn(const std::vector<float>& iParams, StatusCallBackFunction iFn) { return 0; };
	//virtual float ObjectiveGradientFn(const std::vector<float>& iParams, StatusCallBackFunction) { return 0; };

public:	
	virtual ~ProcessBase(void) {m_iProcessCB.clear();};

protected:
	bool					m_CBInitialized;
	PGCore::BaseDataObject	*m_iDataObject;
	PGCore::BaseDataObject	*m_oDataObject;

	std::vector<PGAlgs::ProcessBase *>		m_iProcessCB;
	StatusCallBackFunctionD m_CBFn;

	virtual bool executeSync() { return false; }
	virtual bool executeAsync() { return false; }

};



inline int ProcessBase::NotifyError(const char *iMsg/*=0*/)
{		
	LOG0("Begin NotifyError");

	if (AsyncProcess::NotifyError() <= 0) return 0;

	int progressValue = -1;

	UpdateProgress(progressValue);

	LOG0("End NotifyError");

	return true;
}


inline int ProcessBase::NotifyFinished()
{		
	LOG0("IO/IODicom3D::Begin NotifyFinished");
	
	if (AsyncProcess::NotifyFinished()<=0)
	{
		return 0;
	}

	int progressValue = 100;
	UpdateProgress(progressValue);
	
	return 1;
}

};




//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGProcessBase_H_
