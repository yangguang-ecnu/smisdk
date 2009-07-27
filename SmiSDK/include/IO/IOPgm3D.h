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
// Filename:	IOPgm3D.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IOPgm3D.h: interface for the IOPgm3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOPgm3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
#define AFX_IOPgm3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _DRAW_SYNTH_SHAPES_ 0

#include "IOPgm.h"
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
class IOPgm3D : public IOBase<T>, public PGCore::ThreadedProcess
{
public:
	IOPgm3D(void);
	
	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);	

	~IOPgm3D(void);

	virtual void StartExecution(void *iParams);

private:


	bool ReadFromFolder(const std::vector<std::string>& iFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);	
	bool ReadFromFolderMT(const std::vector<std::string>& iFileList);
	
	bool WriteToFolder(const std::vector<std::string>& oFileList,	PGCore::BaseDataObject *iMetaDataObject,
		PGCore::Volume<T> *oDataObject);	
	
	bool RearrangeSlices(const std::vector<SlicePosition>& iSortedPositions, 
		std::vector<PGCore::Image<T> >& iVolume, PGCore::Volume<T>& oVolume,
		std::vector<PGMath::Vector3D<float> >& ioSortedPositions, std::vector<PGMath::Vector3D<float> >& ioSortedOrX, 
		std::vector<PGMath::Vector3D<float> >& ioSortedOrY);



};


};


#ifndef _PG_GENERATE_SDK_LIBS_
	#include "IOPgm3D.hpp"
#endif


#endif // !defined(AFX_IOPgm3D_H__C4EA1B91_0881_44C7_927E_3CD2769137C9__INCLUDED_)
