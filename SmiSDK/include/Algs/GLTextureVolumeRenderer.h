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
