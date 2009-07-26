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
