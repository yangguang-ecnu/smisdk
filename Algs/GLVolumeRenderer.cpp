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
// Filename:	GLVolumeRenderer.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLVolumeRenderer_HPP_
#define _PGGLVolumeRenderer_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/GLVolumeRenderer.h"

namespace PGAlgs
{	
	
	template <class T, class U>
	GLVolumeRenderer<T, U>::GLVolumeRenderer() 
	{
		m_coronalCutPlanePosition = 0.0f;
		m_rendererType = kPgRendererType3D;
		m_ready = false;
		
		m_LuT.clear();

		for (int i=0; i<2; i++)
		{
			gTranslation[i] = PGMath::Vector3D<float>(0, 0, 0);
			gRotation[i] = PGMath::Vector3D<float>(0, 0, 0);
		}

		gRotate = 0.5f;
		
		gXMin = -1.0; gXMax = 1.0; gYMin = -1.0; gYMax =1.0; gZMin = -1.0; gZMax = 1.0; 
		

		gSparseMode=1;	
		gRotate = 5.0f;
		gLowerBound = 60; gLowerEndNoiseBound = 25; gUpperBound = 100;
		gGradLowerBound=60, gGradUpperBound=75;
		gSuperSampligFactorZ = 4.0f;	

		Clear();		
	};

	template <class T, class U>
	GLVolumeRenderer<T, U>::~GLVolumeRenderer(void) 
	{ 

	};

	template <class T, class U>
	void  GLVolumeRenderer<T, U>::Clear() 
	{ 
		gVolumeScope.clear();
		m_voxelDims.clear();
		m_voxelDimsSkipZ.clear();
		
		m_LuT.clear();
		m_volumeAccessor.clear();
		m_voxel3DList.clear();
		m_MaskLUT.clear();	
		m_polyMeshList.clear();

		GLRendererBase::Clear();
	}

	//iCursor3D has xyz in [-0.5...0.5] range
	template <class T, class U>
	bool GLVolumeRenderer<T, U>::SetCursorPosition(const PGMath::Point3D<float>& iCursor3D) 
	{ 
		m_cursor3D = iCursor3D; return true; 
	};		

	// xyz in [-0.5...0.5] range
	template <class T, class U>
	bool GLVolumeRenderer<T, U>::SetSlicerPosition(double iPosition) 
	{ 
		m_coronalCutPlanePosition = iPosition; return true; 
	};		
	
	template <class T, class U>
	const PGMath::AffineTransform<float>& GLVolumeRenderer<T, U>::GetViewTransform() 
	{ 
		assert(0 && "Not implemented");
		return PGMath::AffineTransform<float>(); 
	};
		
	template <class T, class U>
	bool GLVolumeRenderer<T, U>::SetViewTransform(const PGMath::AffineTransform<float>&	iTransform) 
	{ 
		assert(0 && "Not implemented");
		return false; 
	} 

	template <class T, class U>
	bool GLVolumeRenderer<T, U>::UpdateCB(void	*iDataObject) 
	{ 
		return false; 
	};

	template <class T, class U>
	bool GLVolumeRenderer<T, U>::SetInput(PGCore::BaseDataObject *iDataObject)
	{
		bool rv = ProcessBase::SetInput(iDataObject);
		if (!rv) return false;

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) 
		{
			LOG0("GLVolumeRenderer:SetInput Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount(kPgDataObjectTypeIsotropicVoxel3D);
		int iMultiMeshCount   = inMultiVolume->GetDataCount(kPgDataObjectTypePolygonMesh);
		if (iMultiVolumeCount < 1 && iMultiMeshCount < 1)		
		{
			LOG0("GLVolumeRenderer:SetInput Error: Needs at least one rendering target");
			return false;
		}

		if (m_LuT.size() < iMultiVolumeCount)
		{
			m_LuT.resize(iMultiVolumeCount);
			m_MaskLUT.resize(iMultiVolumeCount);
			m_volumeAccessor.resize(iMultiVolumeCount);
			m_voxel3DList.resize(iMultiVolumeCount);
			
			m_voxelDims.resize(iMultiVolumeCount);
			m_voxelDimsSkipZ.resize(iMultiVolumeCount);
			gVolumeScope.resize(iMultiVolumeCount);			
			m_blendRatio.resize(iMultiVolumeCount);

			for (int i=0; i<iMultiVolumeCount; i++)
			{
				m_blendRatio[i] = (float(1.0f/float(iMultiVolumeCount)));
				m_voxelDimsSkipZ[i] = 1;
			}
		}

		if (m_polyMeshList.size() < iMultiMeshCount)
		{
			m_polyMeshList.resize(iMultiMeshCount);
		}
		
		return true;		
	};
	
	template <class T, class U>
	bool GLVolumeRenderer<T, U>::UpdateLUTParams(const int iLowIncr, const int iHighIncr, const int iLowGradIncr, const int iHighGradIncr)
	{
		gLowerBound += iLowIncr;
		gUpperBound += iHighIncr;

		gGradLowerBound += iLowGradIncr;
		gGradUpperBound += iHighGradIncr;

		LOG4("GLVolumeRenderer::UpdateLUTParams: %d, %d, %d, %d\n", gLowerBound, gUpperBound, gGradLowerBound, gGradUpperBound); 
				
		// upper bound ignored for now		
		return this->reloadLUT();
	}

	template <class T, class U>
	bool GLVolumeRenderer<T, U>::SetTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0) 
	{ 
		//	m_LUT = iLUT; return true; 
		if (!iLUT || iRendererIndex>m_LuT.size()-1)
			return false;

		//trying to add multiple volumes to the Renderer->. search for m_LUT then make it work

		int indexBegin= (iRendererIndex==-1) ? 0 : iRendererIndex;
		int indexEnd= (iRendererIndex==-1) ?  m_volumeAccessor.size()-1: iRendererIndex;
		int okCount=0;

#define _USE_3D_LUT
#ifdef _USE_3D_LUT
		for (int i=indexBegin; i<=indexEnd; i++)
		{
			m_LuT[i] = iLUT;
			okCount+= (m_LuT[i]->GetBuffer()!=0);
		}

#else
		(okCount=(indexEnd-indexBegin+1));
#endif
		return (okCount==(indexEnd-indexBegin+1));
	};

	template <class T, class U>
	bool GLVolumeRenderer<T, U>::SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0)
	{ 
		if (iRendererIndex > m_MaskLUT.size()-1) return false;
		m_MaskLUT[iRendererIndex] = iLUT; 
		return true; 
	};

	template <class T, class U>
	void GLVolumeRenderer<T, U>::clampToNextHigherNumber(PGMath::Point3D<long>& ioDim)
	{
		for (int i=0; i<=2; i++)
		{
			int j=0;

			while (pow((float)2.0f, (float)j) < ioDim[i])
			{
				j++;
			}

			ioDim[i] = pow(2.0f, j);
		}

		return;
	}


	template <class T, class U>
	bool GLVolumeRenderer<T, U>::ImageSizeUpdated()
	{
		//return PrepareRenderer();
		return true;
	}

	template <class T, class U>
	bool GLVolumeRenderer<T, U>::SetContext(HDC iContext)
	{
		if (!GLRendererBase::SetContext(iContext))
		{
			return false;
		}

		if(!initGLthread()) 
		{
			LOG1("GLVolumeRenderer: Error: failure to initGLthread. 3D texture memory available: %ld", m_max3DTextureSize);
			return false;    
		}	

		return true;
	}
 
	

	template <class T, class U>
	bool GLVolumeRenderer<T, U>::Execute() 
	{		
		return false;
	}

	



	


	
	template <class T, class U>
	void GLVolumeRenderer<T, U>::renderAxisPlanes()
	{
		glDisable(GL_TEXTURE_3D);

		glMatrixMode(GL_MODELVIEW);

		glTranslatef(gTranslation[0].X(), gTranslation[0].Y(), gTranslation[0].Z());
		
		glPushMatrix();
		glMatrixMode(GL_PROJECTION); 
		glLoadIdentity(); 		
		glFrustum(-0.5/m_zoomFactor, 0.5/m_zoomFactor, -0.5/m_zoomFactor, 0.5/m_zoomFactor, 4, 10);
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 
		gluLookAt(0.0, 0.0, 9, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
		

		// draw cut plane
		float cutZ = (2.0f*m_coronalCutPlanePosition);
		glColor4f(0, 0, 1, 1);
		glBegin(GL_LINE_LOOP);
			glVertex3f(-1, -1, cutZ);
			glVertex3f(-1, 1, cutZ);
			glVertex3f(1, 1, cutZ);
			glVertex3f(1, -1, cutZ);
		glEnd();


		//if (m_skipFactor!=1)
		/*
		glRotatef(-gRotation.Z(), 0, 0, 1);
		glRotatef(-gRotation.Y(), 0, 1, 0);		
		glRotatef(gRotation.X(), 1, 0, 0);		

		{
			// x axis
			glColor4f(1, 0, 0, 1);
			glBegin(GL_LINES);//_LOOP);				
			glVertex3f(2*m_cursor3D.X(), 2*m_cursor3D.Z(), -2*m_cursor3D.Y());
			glVertex3f(1, 2*m_cursor3D.Z(), -2*m_cursor3D.Y());
			glEnd();

			// y axis
			glColor4f(0, 1, 0, 1);
			glBegin(GL_LINES);//_LOOP);				
			glVertex3f(2*m_cursor3D.X(), 2*m_cursor3D.Z(), -2*m_cursor3D.Y());
			glVertex3f(2*m_cursor3D.X(), 2*m_cursor3D.Z(), -1);
			glEnd(); 


			// XY plane - Z axis
			glColor4f(0, 0, 1, 1);
			glBegin(GL_LINES);//_LOOP);		
			glVertex3f(2*m_cursor3D.X(), 2*m_cursor3D.Z(), -2*m_cursor3D.Y());
			glVertex3f(2*m_cursor3D.X(), 1, -2*m_cursor3D.Y());
			glEnd(); 

		}	
		*/
		/*
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, (short) 0x1C47); 


		// bbox

		glColor4f(1,1,0,0.3);
		glBegin(GL_LINE_LOOP);
		//glBegin(GL_QUADS);
		glVertex3f( gXMin, gYMin, gZMin);
		glVertex3f( gXMin, gYMax, gZMin); 
		glVertex3f( gXMin, gYMax, gZMax); 
		glVertex3f( gXMin, gYMin, gZMax);
		glEnd(); 
		glBegin(GL_LINE_LOOP);
		//glBegin(GL_QUADS);
		glVertex3f( gXMax, gYMin, gZMin);
		glVertex3f( gXMax, gYMax, gZMin); 
		glVertex3f( gXMax, gYMax, gZMax); 
		glVertex3f( gXMax, gYMin, gZMax);
		glEnd(); 
		glBegin(GL_LINE_LOOP);
		//glBegin(GL_QUADS);
		glVertex3f( gXMin, gYMin, gZMin);
		glVertex3f( gXMin, gYMax, gZMin); 
		glVertex3f( gXMax, gYMax, gZMin); 
		glVertex3f( gXMax, gYMin, gZMin);
		glEnd(); 
		glBegin(GL_LINE_LOOP);
		//glBegin(GL_QUADS);
		glVertex3f( gXMin, gYMin, gZMax);
		glVertex3f( gXMin, gYMax, gZMax); 
		glVertex3f( gXMax, gYMax, gZMax); 
		glVertex3f( gXMax, gYMin, gZMax);
		glEnd(); 
		glDisable(GL_LINE_STIPPLE);
*/

		glPopMatrix();
	}
	

	

	template <class T, class U>
	void GLVolumeRenderer<T, U>::getGLViewParams (PGMath::Vector3D<float> &oMinVec,
		PGMath::Vector3D<float> &oMaxVec,							
		PGMath::Vector3D<float> &oCameraCenter,
		float &oCameraEye,
		float &oCameraNear,
		float &oCameraFar, const int iVolumeIndex/*=0*/
		) {	

			oMinVec = PGMath::Vector3D<float>(-((float) m_voxelDims[iVolumeIndex].X() / 2.0f),
				-((float) m_voxelDims[iVolumeIndex].Y() / 2.0f), -((float) m_voxelDims[iVolumeIndex].Z() / 2.0f));

			oMaxVec = PGMath::Vector3D<float>(((float) m_voxelDims[iVolumeIndex].X() / 2.0f),
				((float) m_voxelDims[iVolumeIndex].Y() / 2.0f), ((float) m_voxelDims[iVolumeIndex].Z() / 2.0f));

			// determine center of data
			oCameraCenter = PGMath::Vector3D<float>(0.0f, 0.0f, 0.0f);

			// Why these values are set this way is unclear to me, but it works
			oCameraNear = (float) m_voxelDims[iVolumeIndex].Z() / 5.0f;
			oCameraFar =  m_voxelDims[iVolumeIndex].Z() * 1000.0f;
			oCameraEye = 0.5f * m_voxelDims[iVolumeIndex].Y() * oCameraNear;       		
	};

#ifdef _PG_GENERATE_SDK_LIBS_
	template class GLVolumeRenderer<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 		
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLVolumeRenderer_HPP_
