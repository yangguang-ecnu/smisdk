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
// Filename:	GLCubeSliceVolumeRenderer.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLCubeSliceVolumeRenderer_HPP_
#define _PGGLCubeSliceVolumeRenderer_HPP_
//////////////////////////////////////////////////////////////////////////


#include "GLCubeSliceVolumeRenderer.h"

namespace PGAlgs
{	
	
	template <class T, class U>
	GLCubeSliceVolumeRenderer<T, U>::GLCubeSliceVolumeRenderer() 
	{
		

	
		gDrSparse = 1.0f; gDrFull = 1.0f;  
		gDz = 1.0f;//, gDzFull = 1.0f, gDzSparse = 8.0f; 	

		
		
		
	};

	template <class T, class U>
	GLCubeSliceVolumeRenderer<T, U>::~GLCubeSliceVolumeRenderer(void) 
	{ 

	};


	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::SetInput(PGCore::BaseDataObject *iDataObject)
	{
		bool rv = GLVolumeRenderer::SetInput(iDataObject);
		if (!rv) return false;

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) 
		{
			LOG0("GLCubeSliceVolumeRenderer:SetInput Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount();
		if (iMultiVolumeCount < 1)
		{
			LOG0("GLCubeSliceVolumeRenderer:SetInput Error: Needs at least one volume");
			return false;
		}



		return true;		
	};
	
	
 
	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::PrepareRenderer()
	{
		if (m_externalContext)
		{
			if (m_hdContext == (HDC)0 || m_glContext == (HGLRC)0)
			{
				LOG0("GLCubeSliceVolumeRenderer: Error: no context");
				return false;
			}
		}

		if (!m_iDataObject || (m_rows*m_columns)<=0) 
		{
			LOG0("GLCubeSliceVolumeRenderer: Error: no input");
			return false;
		}		

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) {
			LOG0("GLCubeSliceVolumeRenderer:initVolumes Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount();
		if (iMultiVolumeCount < 1)
		{
			LOG0("GLCubeSliceVolumeRenderer:initVolumes Error: Needs at least one volume");
			return false;
		}	

		for (int i=0; i<iMultiVolumeCount; i++)
		{
			// get the input volume
			PGCore::Voxel3D< T > *inVolume = (static_cast<PGCore::Voxel3D < T > *>(inMultiVolume->GetDataObject(i)));
			if (!inVolume) 
			{
				LOG1("GLCubeSliceVolumeRenderer: Error: failure to fetch input volume %d", i);
				return false;
			}

			m_voxel3DList[i] = inVolume;

			PGMath::Point3D<float> pDimensions;
			pDimensions = inVolume->GetDimensions();
			if (pDimensions.Z()<=0) 
			{
				LOG1("GLCubeSliceVolumeRenderer: Error: empty input volume %d", i);
				return false;
			}

			PGCore::Volume< T > *volumeAccessor = inVolume->GetVolumeAccessor();

			if (!volumeAccessor) 
			{
				LOG0("GLCubeSliceVolumeRenderer: Error: failure to fetch input volume accessor");
				return false;
			}

			m_volumeAccessor[i] = volumeAccessor;
			m_volumeAccessor[i]->GetDimensions(m_voxelDims[i]);	

			gVolumeScope[i] = m_voxelDims[i];

			clampToNextHigherNumber(m_voxelDims[i]);

			LOG3("GLCubeSliceVolumeRenderer: Dimensions (padded): %d / %d / %d", m_voxelDims[i][0], m_voxelDims[i][1], m_voxelDims[i][2]);

			int maxVoxelDim = max(m_voxelDims[i].X(), m_voxelDims[i].Y());
			maxVoxelDim = max(maxVoxelDim, m_voxelDims[i].Z());

			if (!initGLthread())
			{
				LOG0("GLCubeSliceVolumeRenderer: Error: failed to initialize GL machine.");
				m_ready = false;
				return false;
			}						

			
			if(!init(i)) 
			{
				LOG1("GLCubeSliceVolumeRenderer: Error: failure to init for volume %d", i);
				m_ready = false;
				return false;    
			}

			m_ready = true;
			
		}

		return GLRendererBase::PrepareRenderer();		
	}

	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::Execute() 
	{		
		return false;
	}

	

	

	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::loadVolume(const int iVolumeIndex/*=-1*/)
	{
		if (iVolumeIndex!=-1 && iVolumeIndex>m_volumeAccessor.size()-1)
			return false;

		
		// this is where to fuse the volumes ? 
		int iBeginIndex = iVolumeIndex==-1 ? 0 : iVolumeIndex;
		int iEndIndex = iVolumeIndex==-1 ? (m_volumeAccessor.size()-1) : iVolumeIndex;	

		bool rv = false;		
		for (int vIndex=iBeginIndex; vIndex<=iEndIndex; vIndex++)
		{
			
			bool maskInit = m_volumeAccessor[vIndex]->GetBitVolume().IsInitialized();
			bool rv0 = maskInit ?  loadVolumeWithMask(vIndex) : loadVolumeWithoutMask(vIndex);    
			rv = rv0||rv;
		}
		
		return rv;		
	}



	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::loadVolumeWithMask(const int iVolumeIndex/*=0*/)
	{
		if (!m_ready) return false;

		if (iVolumeIndex > m_volumeAccessor.size()-1) return false; 

		if (iVolumeIndex > m_voxel3DList.size()-1) return false; 
	
		m_mesh.Clear();


		

#ifdef _TEST_POLY_
		PGCore::TreeNode<T> pNode(static_cast<PGCore::TreeNode<T> *>(m_volumeAccessor[0]));
		pNode.AddChild(0);
		Octree<T> pTree(&pNode);


		PGCore::Polygon<float, 3> triad;
		triad.AddVertex(PGMath::Point3D<float>(-1, -1, -1));
		triad.AddVertex(PGMath::Point3D<float>(0, 1, -1));
		triad.AddVertex(PGMath::Point3D<float>(1, -1, -1));

		m_mesh.AddTriangle(triad);
#endif

		

		LOG0("} GLCubeSliceVolumeRenderer::loadVolumeWithMask");
		return 1;
	}


	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::loadVolumeWithoutMask(const int iVolumeIndex/*=0*/)
	{	

		if (!m_ready) return false;

		if (iVolumeIndex > m_volumeAccessor.size()-1) return false; 

		m_mesh.Clear();

#ifdef _TEST_POLY_
		PGCore::Polygon<float, 3> triad;
		triad.AddVertex(PGMath::Point3D<float>(-1, -1, -1));
		triad.AddVertex(PGMath::Point3D<float>(0, 1, 0));
		triad.AddVertex(PGMath::Point3D<float>(1, -1, 1));

		m_mesh.AddTriangle(triad);
#endif

		UpdateProgress(100);

		LOG0("} GLCubeSliceVolumeRenderer::loadVolumeWithoutMask");
		return 1;
	}

	

	/* 
	* Method:
	* 
	* Input Parameters : 
	* Output Parameters:
	* 
	* Comments : 
	*
	*/          
	template <class T, class U>
	int GLCubeSliceVolumeRenderer<T, U>::init(const int iVolumeIndex/*=0*/)
	{


		getGLViewParams(gMinVec, gMaxVec, gCameraCenter, gCameraEyeZ, gCameraNear, gCameraFar, iVolumeIndex);

		// init params
		// m_voxelDims : power of 2, texture dimemsions
		// gVolumeScope: actual volume dimensions 
		{
			gDrSparse = 1.0f/m_voxelDims[iVolumeIndex][2], gDrFull = 1.0/m_voxelDims[iVolumeIndex][2];
			gDz = 1.0f/((float)m_voxelDims[iVolumeIndex][2]/*m_max3DTextureSize*/);  
			gDzFull = gDz/(gSuperSampligFactorZ); 	
			gDzSparse = 8.0f*gDzFull; 	
			gDrSparse = gDzSparse;
			gDrFull = gDzFull;
		}

		


		return 1;	
	}



	/* 
	* Method: display
	* 
	* Input Parameters : 
	* Output Parameters:
	* 
	* Comments : callback function for opengl based display of the rendered image
	*
	*/
	template <class T, class U>
	void GLCubeSliceVolumeRenderer<T, U>::display()
	{
		if (!m_ready) return;

	

#if (_TEST_SCENE)
		glClearColor( 1.0, 0.0, 0.0, 1.0 );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//***************************
		// DRAWING CODE
		//***************************

		//cout << "Rot: (" << gRotation.X() << "," << gRotation.Y() <<", "<< gRotation.Z() << endl;

		//glTranslatef(0.0f,0.0f,-5.0f);
		//glRotatef(0,0.0f,1.0f,0.0f);

		//glRotatef(gRotation.X(), 1, 0, 0);
		glRotatef(gRotation.Y(), 0, 1, 0);
		//glRotatef(gRotation.Z(), 0, 0, 1);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,-1.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,0.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glEnd();

		/*	glBegin(GL_QUADS);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-1.1f,-1.1,0.0f);
		glVertex3f(1.1f,-1.1f,0.0f);
		glVertex3f(1.1f,1.1f,0.0f);
		glVertex3f(-1.1f,1.1f,0.0f);
		glEnd();*/	
#else

		glClearColor( 0.88, 0.88, 0.88, 0.0 );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION); 
		glLoadIdentity(); 
		//gluPerspective(100.0, 1.0, 3, 10.0); 
		glFrustum(-0.5/m_zoomFactor, 0.5/m_zoomFactor, -0.5/m_zoomFactor, 0.5/m_zoomFactor, 4, 10);
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 
		gluLookAt(0.0, 0.0, 7, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

		//glMatrixMode(GL_MODELVIEW);

		//glLoadIdentity();
		//glFrustum(-3*m_zoomFactor, 3*m_zoomFactor, -3*zoomFm_zoomFactoractor, 3*m_zoomFactor, 10, 100);

		//gluLookAt (0.0, 0.0, 10 + m_zoomFactor, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		//	gluLookAt(gCameraCenter.X(), gCameraCenter.Y(), 
		//		//gCameraEyeZ, 
		//		2,
		//		gCameraCenter.X(), gCameraCenter.Y(), 
		//		0.0f,
		//		//-gVolumeScope.Z(), 
		//		0, 1, 0);	
		//	//glPushMatrix();


		//glRotatef(gRotation.X(), gRotation.Y(), gRotation.Z());
		glTranslatef(gTranslation.X(), gTranslation.Y(), gTranslation.Z());

		////glTranslatef(-0.5f, -0.5f, -0.5f);
		//glRotatef(gRotation.X(), 1, 0, 0);
		//glRotatef(gRotation.Y(), 0, 1, 0);
		//	glRotatef(gRotation.Z(), 0, 0, 1);
		//glTranslatef(0.5f, 0.5f, 0.5f);

		renderVolume();

		renderAxisPlanes();

		//glPopMatrix();
#endif

		

	}

	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::UpdateRender(int iIndex=-1, bool iUpdateDisplay=0)
	{
		if (!m_ready) return false;
	
		if (GetAsyncEnabled())
		{
			return updateRenderAsync();
		}

		return updateRenderSync();			
	};	

	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::updateRenderSync(int iIndex=-1, bool iUpdateDisplay=0)
	{
		bool res = loadVolume(iIndex); 
		if (!res) return false;

		Render();

		if (!m_externalContext)
		{
			bool res = GLRendererBase<T, U>::UpdateRender(iIndex, iUpdateDisplay);
			if (!res) return false;
		}

		return true;
	}

	template <class T, class U>
	bool GLCubeSliceVolumeRenderer<T, U>::updateRenderAsync(int iIndex=-1, bool iUpdateDisplay=0)
	{
		LOG0("{ updateRenderAsync");

		if (m_voxel3DList.empty()) return false;

		SetMaxThreadCount(1);

		bool rv = false;
		char szPath[MAX_PATH]; 
		DWORD dwBufferSize = MAX_PATH;
		GetTempPath( dwBufferSize, szPath );	
		std::string pStr = std::string(szPath) + kPgApplicationName + "\\";

		// fill params and send to thread
		std::string fName = pStr + std::string("UpdateVR") + std::string(".txt");
		
		m_threadLogger.SetLogFile(fName.c_str());

		// data
		m_updateVRParams.m_pIVolume = m_voxel3DList[0];
		m_updateVRParams.m_MaskLUT = m_MaskLUT[0];
		m_updateVRParams.m_LuT = m_LuT[0];
		m_updateVRParams.m_blendRatio = m_blendRatio[0];
		m_updateVRParams.m_hdContext = m_hdContext;
		m_updateVRParams.m_rendererIndex = iIndex;
		m_updateVRParams.m_updateDisplay = iUpdateDisplay;
		m_updateVRParams.m_rows = m_rows;
		m_updateVRParams.m_columns = m_columns;



		// base params settings
		m_updateVRParams.statusCB = GetProgressCB();
		m_updateVRParams.ioLogger = &(m_threadLogger);			
		m_updateVRParams.iParentThreadPtr = GetAsyncEnabled() ? (void *)this : 0;
		
		// set params
		if (!CreateNewThread((ThreadedProcess *)this, (void *) (&(m_updateVRParams))))
		{
			LOG1("ERROR: Failed to spawn new thread %d.", GetAliveThreadCount()); 
			return false;
		}	

		rv = WaitToFinishIfNeeded();	

		LOG1("} updateRenderAsync : %d threads spawned", GetAliveThreadCount());

		return rv;	
	}

	template <class T, class U>
	void GLCubeSliceVolumeRenderer<T, U>::StartExecution(void *iParam)
	{
		if (!iParam) return;

		//using namespace PGIO;
		PGAlgs::ThreadedUpdateVRParams<T> * iParams = static_cast<PGAlgs::ThreadedUpdateVRParams<T> *>(iParam);

		iParams->ioLogger->Log("{ VR update begin");

		if (iParams->m_asyncEnabled)
		{
			iParams->ioLogger->Log("} StartExecution: Error: async flag enabled");
			if (iParams->iParentThreadPtr)
			{
				GLCubeSliceVolumeRenderer<T, U>* iParentThreadPtr = (GLCubeSliceVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: async flag enabled");
			}
			return;
		}

		GLCubeSliceVolumeRenderer<T, U> VREngine;
		bool rv = VREngine.SetContext(iParams->m_hdContext);
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to set context");
			if (iParams->iParentThreadPtr)
			{
				GLCubeSliceVolumeRenderer<T, U>* iParentThreadPtr = (GLCubeSliceVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to set context");
			}
			return;
		}

		// set data
		PGCore::MultiDataBase<T>* multiVolume = new PGCore::MultiDataBase<T>();
		if (!multiVolume)
		{
			iParams->ioLogger->Log("} StartExecution: Error: no data");
			if (iParams->iParentThreadPtr)
			{
				GLCubeSliceVolumeRenderer<T, U>* iParentThreadPtr = (GLCubeSliceVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: no data");
			}
			return;
		}

		multiVolume->AddDataObject(static_cast<PGCore::BaseDataObject *> (iParams->m_pIVolume));
		rv = VREngine.SetInput(static_cast<PGCore::BaseDataObject *> (multiVolume));
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to set input");
			if (iParams->iParentThreadPtr)
			{
				GLCubeSliceVolumeRenderer<T, U>* iParentThreadPtr = (GLCubeSliceVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to set input");
			}
			delete multiVolume;
			return;
		}


		//  mask will be implicit to the volume
		VREngine.SetProgressCB(iParams->statusCB);
		VREngine.SetImageSize(iParams->m_rows, iParams->m_columns);
		
		
		rv = VREngine.PrepareRenderer();
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to PrepareRenderer");
			if (iParams->iParentThreadPtr)
			{
				GLCubeSliceVolumeRenderer<T, U>* iParentThreadPtr = (GLCubeSliceVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to PrepareRenderer");
			}
			delete multiVolume;
			return;
		}		
		
		VREngine.SetMaskTransferFunctionLUT(iParams->m_MaskLUT);
		VREngine.SetTransferFunctionLUT(iParams->m_LuT);
		VREngine.SetBlendWeight(iParams->m_blendRatio, 0);
		//VREngine.SetBlendWeight(1.0f-iParams->m_blendRatio, 1);

		rv = VREngine.UpdateRender(iParams->m_rendererIndex, iParams->m_updateDisplay);
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to UpdateRender");
			if (iParams->iParentThreadPtr)
			{
				GLCubeSliceVolumeRenderer<T, U>* iParentThreadPtr = (GLCubeSliceVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to UpdateRender");
			}
			delete multiVolume;
			return;
		}


		if (iParams->iParentThreadPtr)
		{
			GLCubeSliceVolumeRenderer<T, U>* iParentThreadPtr = (GLCubeSliceVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
			iParentThreadPtr->NotifyFinished();
		}
	
		iParams->ioLogger->Log("} GLCubeSliceVolumeRenderer end");

		return;
	}


	/* 
	* Method: initGLthread(
	* 
	* Input Parameters : int argc, char **argv
	* Output Parameters:
	* 
	* Comments : method to initialise the openGl state machine for interactive display
	*
	*/
	template <class T, class U>
	int GLCubeSliceVolumeRenderer<T, U>::initGLthread()
	{
		

		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_TRUE);
			glShadeModel(GL_SMOOTH);
		}

		return 1;
	}


	template <class T, class U>
	void GLCubeSliceVolumeRenderer<T, U>::renderSingleVolume(const int iVolumeIndex/*=0*/)
	{
		if (iVolumeIndex>m_volumeAccessor.size()-1)
			return;

		int iVIndex = iVolumeIndex;
		
		register float r=0.0f, zz=gZMin;
		register float zSparse = (m_skipFactor==1) ? gDzFull : gDzSparse;	

		gDrSparse = zSparse/2;	

		
		const PGCore::Polygon<float, 3>& triad = m_mesh[0];

		glBegin(GL_LINE_LOOP);
		glColor3f(1, 0, 0);
		glVertex3f(triad[0].X(), triad[0].Y(), triad[0].Z());

		glColor3f(0, 1, 0);
		glVertex3f(triad[1].X(), triad[1].Y(), triad[1].Z());

		glColor3f(0, 0, 1);
		glVertex3f(triad[2].X(), triad[2].Y(), triad[2].Z());
		glEnd();
	
	}



};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLCubeSliceVolumeRenderer_HPP_
