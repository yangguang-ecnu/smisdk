// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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
