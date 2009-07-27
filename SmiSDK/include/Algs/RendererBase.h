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
// Filename:	RendererBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGRendererBASE_H_
#define _PGRendererBASE_H_
//////////////////////////////////////////////////////////////////////////

#include "ProcessBase.h"
#include "Core/MultiDataBase.h"
#include "Core/Image.h"

namespace PGAlgs
{

typedef enum ePgRendererType
{
	kPgRendererTypeStatic = 0,
	kPgRendererTypeMPR = 1,
	kPgRendererType3D = 2,
	kPgRendererTypeUnknown = -1
};


template <class T, class U>
class RendererBase : public ProcessBase
{
public:
	RendererBase(void);	

	virtual bool Execute() = 0;
	
	bool SetImageSize(unsigned int& iRows, unsigned int& iColumns);

	bool GetOutput(PGCore::BaseDataObject *oDataObject);

	virtual bool PrepareRenderer() = 0;

	ePgRendererType GetRendererType();	

	virtual ~RendererBase(void);

protected:
	ePgRendererType m_rendererType;
	unsigned int	m_rows, m_columns;
	bool			m_imageSizeChanged;

	virtual bool ImageSizeUpdated() = 0;
};


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGRendererBASE_H_
