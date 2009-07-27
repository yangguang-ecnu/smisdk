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
// Filename:	IOMhd3D.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IOMhd3D.h: interface for the IOMhd3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOMhd3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IOMhd3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _DRAW_SYNTH_SHAPES_ 0

#include "IO/IORaw3D.h"
#include "Core/GaussianKernel.h"
#include "Core/DilationKernel.h"
#include "Core/ErosionKernel.h"
#include "Core/Volume.h"
#include "Core/BitImage.h"
#include "Core/DrawHelper.h"
#include "Algs/ImageResampler.h"
#include "Algs/ImageBinarizer.h"
#include "Algs/ImageFourierTransform.h"
#include "Algs/ImageTypeConverter.h"
#include "Algs/ImageBitClipper.h"
#include "Algs/ImageEndianSwapper.h"

namespace PGIO
{	
	
template <class T>
class IOMhd3D : public IOBase<T>, public PGCore::ThreadedProcess
{
public:
	IOMhd3D(void) { SetAsyncEnabled(false); m_fileFormat = kPgIOSourceFormatUnknown; };
	
	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);	

	~IOMhd3D(void) { };

	virtual void StartExecution(void *iParams);

private:
	
	PGIO::ePgIOSourceFormat				m_fileFormat;	
	PGIO::IORaw3D<short>				m_rawImgIO; // keep a member to keep in scope for threads

		
	bool WriteToFolder(const std::vector<std::string>& oFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);	

	bool ParseDefinitionFile(const std::string &iDefSourceName, 
	    ePgIOSourceType& ioSrcType, std::vector<std::string>& ioFileNames, 
		std::string& ioFormatString,
		PGCore::MetaData<T> &ioMetaData);


};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IO/IOMhd3D.hpp"
#endif


#endif // !defined(AFX_IOMhd3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
