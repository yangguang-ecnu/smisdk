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
// Filename:	IOPpm3D.h
// Author:		Prashant Chopra
// Created:		Saturday, November 11, 2006 at 11:41:53 PM
//
// IOPpm3D.h: interface for the IOPpm3D class. toread raw binary images
//
//////////////////////////////////////////////////////////////////////

#if !defined(_PPM3D_HPP_)
#define _PPM3D_HPP_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Using more threads causes missing slices. 5/7/08
#define _DRAW_SYNTH_SHAPES_ 0

#include "IO/IOPpm.h"
#include "Core/GaussianKernel.h"
#include "Core/DilationKernel.h"
#include "Core/ErosionKernel.h"
#include "Core/Volume.h"
#include "Core/BitImage.h"
#include "Core/PixelBase.h"
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
class IOPpm3D : public IOBase<T>, public PGCore::ThreadedProcess
{
public:
	IOPpm3D(void);
	
	virtual bool Write(PGCore::BaseDataObject *iDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *iMetaDataObject = 0);

	virtual bool Read(PGCore::BaseDataObject *oDataObject, const IOParams &iParams,
		PGCore::BaseDataObject *oMetaDataObject = 0);	

	~IOPpm3D(void);

	virtual void StartExecution(void *iParams);

private:
		

	


	//unsigned int __stdcall  LoadFilesDICOMThread(const ThreadedIOParams<T>& iParams);

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
	#include "IO/IOPpm3D.hpp"
#endif


#endif // !defined(_PPM3D_HPP_)
