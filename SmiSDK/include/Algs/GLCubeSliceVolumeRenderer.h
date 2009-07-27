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
// Filename:	GLCubeSliceVolumeRenderer->h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLCubeSliceVolumeRenderer_H_
#define _PGGLCubeSliceVolumeRenderer_H_
//////////////////////////////////////////////////////////////////////////

#define _TEST_POLY_ 1

#include "GLVolumeRenderer.h"
#include "Core/TriangleMesh.h"
#include "Core/Octree.h"


namespace PGAlgs
{

	template <class T, class U>
	class GLCubeSliceVolumeRenderer : public GLVolumeRenderer<T, U>, public PGCore::ThreadedProcess
	{
	public:
		GLCubeSliceVolumeRenderer(void);

		virtual bool SetInput(PGCore::BaseDataObject *iDataObject);

		virtual bool Execute();

		virtual bool UpdateRender(int iIndex=-1, bool iUpdateDisplay=0);
		
		virtual bool PrepareRenderer();

		virtual ~GLCubeSliceVolumeRenderer(void); 	

		// overload for threadedprocess inheritance
		virtual void StartExecution(void *iParams);

	protected:

		virtual void display();

		virtual bool updateRenderSync(int iIndex=-1, bool iUpdateDisplay=0);
		virtual bool updateRenderAsync(int iIndex=-1, bool iUpdateDisplay=0);

	
	private:		

		float					gDrSparse, gDrFull;  
		float					gDz;
		unsigned int			gSparseMode;	
		
		// add mesh structure here

		PGCore::TriangleMesh<float>	m_mesh;

				
		void	renderSingleVolume(const int iVolumeIndex=0);		
		
		int		initGLthread(); 
		int		init(const int iVolumeIndex=0);

		bool   loadVolume(const int iVolumeIndex=-1);
		bool   loadVolumeWithMask(const int iVolumeIndex=0);
		bool   loadVolumeWithoutMask(const int iVolumeIndex=0);
		

	};
};

#include "GLCubeSliceVolumeRenderer.hpp"

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLCubeSliceVolumeRenderer_H_
