// -*- C++ -*-
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
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
// Filename:	IODef3D.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IODef3D.h: interface for the IODef3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IODef3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IODef3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Using more threads causes missing slices. 5/7/08
#define _DRAW_SYNTH_SHAPES_ 0

#include "IO/IOPgm3D.h"
#include "IO/IOPpm3D.h"
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
class IODef3D : public IOBase<T>, public PGCore::ThreadedProcess
{
public:

	IODef3D(void);
	
	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);	

	virtual ~IODef3D(void);

	virtual void StartExecution(void *iParams);
	void StartExecution_PGM(void *iParams);
	void StartExecution_PPM(void *iParams);

private:
	
	bool	m_autoSkip;
	PGIO::ePgIOSourceFormat				m_fileFormat;

	bool ReadFromFolderPGM(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);	
	bool ReadFromFolderPPM(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);	
	
	bool ReadFromFolderMT_PGM(const std::vector<std::string>& iFileList);
	bool ReadFromFolderMT_PPM(const std::vector<std::string>& iFileList);
	
	bool WriteToFolder(const std::vector<std::string>& oFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);	
	
	bool RearrangeSlices(const std::vector<SlicePosition>& iSortedPositions, 
		std::vector<PGCore::Image<T> >& iVolume, PGCore::Volume<T>& oVolume,
		std::vector<PGMath::Vector3D<float> >& ioSortedPositions, std::vector<PGMath::Vector3D<float> >& ioSortedOrX, 
		std::vector<PGMath::Vector3D<float> >& ioSortedOrY);

	bool ParseDefinitionFile(const std::string &iDefSourceName, std::vector<std::string>& ioFileNames, 
		int& ioRows, int& ioCols, int& ioBppCode,
		float& ioZSpacing, std::string& ioFormatString);

};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IO/IODef3D.hpp"
#endif


#endif // !defined(AFX_IODef3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
