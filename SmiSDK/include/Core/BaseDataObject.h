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
// Filename:	BaseDataObject.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:54:24 AM
//

#ifndef _PG_BaseDataObject_H_
#define _PG_BaseDataObject_H_
//////////////////////////////////////////////////////////////////////////

#ifndef _PG_Core_H_
	#include "Core.h"
#endif
#include "LoggerBase.h"

namespace PGCore 
{

typedef enum {
	kPgDataObjectTypeUnknown=-1,
	kPgDataObjectTypePixel=0,
	kPgDataObjectTypeImage=1,
	kPgDataObjectTypeVolume=2,
	kPgDataObjectTypeIsotropicVoxel3D=3,
	kPgDataObjectTypeKernel=4,
	kPgDataObjectTypeTransferFunctionLUT=5,
	kPgDataObjectTypeBitImage=6,
	kPgDataObjectTypeBitVolume=7
} ePgDataObjectType;


class BaseDataObject : public LoggerBase
{
public:
	BaseDataObject();
	
	virtual ~BaseDataObject();

	ePgDataObjectType Type() const;
	
protected:
	ePgDataObjectType	m_type;		

private:
	
};

};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_BaseDataObject_H_
