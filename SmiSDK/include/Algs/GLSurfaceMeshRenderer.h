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
// Filename:	GLSurfaceMeshRenderer->h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLSurfaceMeshRenderer_H_
#define _PGGLSurfaceMeshRenderer_H_
//////////////////////////////////////////////////////////////////////////


#include "GLVolumeRenderer.h"

namespace PGAlgs
{

	template <class T, class U>
	class GLSurfaceMeshRenderer : public GLVolumeRenderer<T, U>, public PGCore::ThreadedProcess
	{
	public:
		GLSurfaceMeshRenderer(void);

		virtual bool SetInput(PGCore::BaseDataObject *iDataObject);

		virtual bool Execute();

		virtual bool UpdateRender(int iIndex=-1, bool iUpdateDisplay=0);
		
		virtual bool PrepareRenderer();

		virtual ~GLSurfaceMeshRenderer(void); 	

		// overload for threadedprocess inheritance
		virtual void StartExecution(void *iParams);

		virtual void Clear();

	protected:

		virtual void display();
		virtual void renderVolume();

	private:				
		
		void	renderSingleVolume(const int iVolumeIndex=0);
		
		
		int		initGLthread(); 
		int		init(const int iVolumeIndex=0);
	};
};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "GLSurfaceMeshRenderer.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLSurfaceMeshRenderer_H_
