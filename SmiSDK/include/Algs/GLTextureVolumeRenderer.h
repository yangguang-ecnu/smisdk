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
// Filename:	GLTextureVolumeRenderer->h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLTextureVolumeRenderer_H_
#define _PGGLTextureVolumeRenderer_H_
//////////////////////////////////////////////////////////////////////////


#include "GLVolumeRenderer.h"

namespace PGAlgs
{

	template <class T, class U>
	class GLTextureVolumeRenderer : public GLVolumeRenderer<T, U>, public PGCore::ThreadedProcess
	{
	public:
		GLTextureVolumeRenderer(void);

		virtual bool SetInput(PGCore::BaseDataObject *iDataObject);

		virtual bool Execute();

		virtual bool UpdateRender(int iIndex=-1, bool iUpdateDisplay=0);
		
		virtual bool PrepareRenderer();

		virtual ~GLTextureVolumeRenderer(void); 	

		// overload for threadedprocess inheritance
		virtual void StartExecution(void *iParams);

	protected:

		virtual void display();

		virtual bool updateRenderSync(int iIndex=-1, bool iUpdateDisplay=0);
		virtual bool updateRenderAsync(int iIndex=-1, bool iUpdateDisplay=0);

	
	private:

		int						m_textureFormat, m_rawFormat;
		int						m_numChannels;		

		float					gDrSparse, gDrFull;  
		float					gDz;

		unsigned int			gLUTName, gSparseMode;	
		std::vector<GLuint>		gTextureName;
		std::vector<PGMath::Vector3D<float> >	gTexScale;			
		std::vector<float>		gXTexRatio, gYTexRatio, gZTexRatio;		
		std::vector<float>		gXTexTrans, gYTexTrans, gZTexTrans; 

		ObjectBase						m_voxelChunk[2];

		
		void	renderSingleVolume(const int iVolumeIndex=0);
		
		
		int		initGLthread(); 
		int		init(const int iVolumeIndex=0);

		bool   loadVolume(const int iVolumeIndex=-1);
		bool   loadVolumeWithMask(const int iVolumeIndex=0);
		bool   loadVolumeWithoutMask(const int iVolumeIndex=0);
		bool   loadLookupTable();

#ifdef _USE_CG
		// cg specific code
		int		enableFragmentShader ();
		void	disableFragmentShader ();
#endif
	};
};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "GLTextureVolumeRenderer.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLTextureVolumeRenderer_H_
