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
// Filename:	IORaw3D.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IORaw3D.h: interface for the IORaw3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IORAW3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IORAW3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IORaw.h"
#include "Core/GaussianKernel.h"

namespace PGIO
{

template <class T>
class IORaw3D : public IOBase<T>, public PGCore::ThreadedProcess
{
public:
	IORaw3D(void);
	
	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);	

	~IORaw3D(void);

	virtual void StartExecution(void *iParam);

private:
	bool ReadFromFile(const std::string& iFileName,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);	
	bool ReadFromFileMT(const std::string& iFileName);	
	
	bool WriteToFile(const std::string& iFolder,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);		
};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IORaw3D.hpp"
#endif

#endif // !defined(AFX_IORAW3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
