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
// Filename:	RendererBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Algs/RendererBase.h"

namespace PGAlgs
{

	template <class T, class U>
	RendererBase<T, U>::RendererBase(void) 
	{
		m_rows		= 0;
		m_columns	= 0;
		m_imageSizeChanged = false;
	};



	template <class T, class U>
	bool RendererBase<T, U>::SetImageSize(unsigned int& iRows, unsigned int& iColumns)
	{
		m_imageSizeChanged = (m_rows!=iRows || m_columns!=iColumns);	

		m_rows		= iRows;
		m_columns	= iColumns;

		return ImageSizeUpdated();
	}


	template <class T, class U>
	bool RendererBase<T, U>::GetOutput(PGCore::BaseDataObject *oDataObject)  
	{
		if (!oDataObject || !m_oDataObject) 
		{
			LOG2("GetOutput: NullPtr %d, %d", oDataObject, m_oDataObject);
			return false;
		}

		PGCore::Image<PGMath::Point3D<U>> *tempImgPtr	= dynamic_cast<PGCore::Image < PGMath::Point3D<U> > *>(m_oDataObject);
		PGCore::Image<PGMath::Point3D<U>> *tempOutImgPtr = dynamic_cast<PGCore::Image< PGMath::Point3D<U> > *>(oDataObject);

		if (!tempImgPtr || !tempOutImgPtr) 
		{
			LOG2("GetOutput: NullPtr %d, %d", tempImgPtr, tempOutImgPtr);
			return false;
		} else
		{
			long iRow=0, iCol=0, oRow=0, oCol=0;
			tempImgPtr->GetDimensions(iRow, iCol);
			tempOutImgPtr->GetDimensions(oRow, oCol);
			//LOG4("GetOutput: About to copy (%d, %d) -> (%d, %d)", iRow, iCol, oRow, oCol);
		}

		LOG0("{ RendererBase<T, U>::GetOutput..");
		*tempOutImgPtr = *tempImgPtr;
		LOG0("} RendererBase<T, U>::GetOutput..");
		return true;
	}	




	template <class T, class U>
	ePgRendererType RendererBase<T, U>::GetRendererType() 
	{ 
		return m_rendererType; 
	}

	template <class T, class U>
	RendererBase<T, U>::~RendererBase(void) 
	{

	};

	
#ifdef _PG_GENERATE_SDK_LIBS_
	template class RendererBase<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 	
#endif


};



//////////////////////////////////////////////////////////////////////////
// EOF

