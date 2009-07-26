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
