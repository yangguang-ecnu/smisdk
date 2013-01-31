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
// Filename:	GLSurfaceMeshRenderer.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLSurfaceMeshRenderer_HPP_
#define _PGGLSurfaceMeshRenderer_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/GLSurfaceMeshRenderer.h"

namespace PGAlgs
{	
	
	template <class T, class U>
	GLSurfaceMeshRenderer<T, U>::GLSurfaceMeshRenderer() 
	{	
		Clear();		
	};

	template <class T, class U>
	GLSurfaceMeshRenderer<T, U>::~GLSurfaceMeshRenderer(void) 
	{ 
		Clear();
	};


	template <class T, class U>
	bool GLSurfaceMeshRenderer<T, U>::SetInput(PGCore::BaseDataObject *iDataObject)
	{
		bool rv = GLVolumeRenderer::SetInput(iDataObject);
		if (!rv) return false;

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) 
		{
			LOG0("GLSurfaceMeshRenderer:SetInput Error: failure to fetch input multivolume");
			return false;
		}
		
		int iMultiMeshCount   = inMultiVolume->GetDataCount(kPgDataObjectTypePolygonMesh);
		if (iMultiMeshCount < 1)
		{
			LOG0("GLSurfaceMeshRenderer:SetInput Error: Needs at least one rendering target!");
			return false;
		}		

		return true;		
	};
	
	template <class T, class U>
	void  GLSurfaceMeshRenderer<T, U>::Clear() 
	{ 		
		GLVolumeRenderer::Clear();
	}
	
 
	template <class T, class U>
	bool GLSurfaceMeshRenderer<T, U>::PrepareRenderer()
	{
		if (m_externalContext)
		{
			if (m_hdContext == (HDC)0 || m_glContext == (HGLRC)0)
			{
				LOG0("GLSurfaceMeshRenderer: Error: no context");
				return false;
			}
		}

		if (!m_iDataObject) 
		{
			LOG0("GLSurfaceMeshRenderer: Error: no input");
			return false;
		}		

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) {
			LOG0("GLSurfaceMeshRenderer:initVolumes Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiMeshCount   = inMultiVolume->GetDataCount(kPgDataObjectTypePolygonMesh);
		if (iMultiMeshCount < 1)		
		{
			LOG0("GLSurfaceMeshRenderer:initVolumes Error: Needs at least one volume");
			return false;
		}	
	
		int iMultiDataCount   = inMultiVolume->GetDataCount();
		int polyMeshIndex=0;
		for (int i=0; i<iMultiDataCount && polyMeshIndex<iMultiMeshCount; i++)
		{
			if (!inMultiVolume->GetDataObject(i)) continue;

			if (inMultiVolume->GetDataObject(i)->Type() != kPgDataObjectTypePolygonMesh) continue;
			
			PGCore::PolygonMesh<float, 3> *inPolyMesh = (static_cast<PGCore::PolygonMesh<float, 3> *>(inMultiVolume->GetDataObject(i)));
			if (!inPolyMesh) 
			{
				LOG1("GLSurfaceMeshRenderer: Error: failure to fetch input poly mesh %d", i);
				continue;
			}

			m_polyMeshList[polyMeshIndex] = inPolyMesh;
			polyMeshIndex++;			
			m_ready = true;		

			if (!initGLthread())
			{
				LOG0("GLSurfaceMeshRenderer: Error: failed to initialize GL machine.");
				return false;
			}	
			
			if(!init(i)) 
			{
				LOG1("GLSurfaceMeshRenderer: Error: failure to init for volume %d", i);
				return false;    
			}

		}			

		return GLRendererBase::PrepareRenderer();		
	}

	template <class T, class U>
	bool GLSurfaceMeshRenderer<T, U>::Execute() 
	{		
		return false;
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
	int GLSurfaceMeshRenderer<T, U>::init(const int iVolumeIndex/*=0*/)
	{		
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
	void GLSurfaceMeshRenderer<T, U>::display()
	{
		if (!m_ready) return;
		
		glClearColor( 0.1, 0.1, 0.1, 1.0 );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION); 
		glLoadIdentity(); 
		gluPerspective(90.0, 1.0, 0.0001, 20.0); 
		
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 
		gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 	
		
		renderVolume();	
	}

	template <class T, class U>
	void GLSurfaceMeshRenderer<T, U>::renderVolume()
	{
		int i=0;			
		for (i=0; i<m_polyMeshList.size(); i++)
		{
			renderSingleVolume(i);
		}	
	}

	template <class T, class U>
	bool GLSurfaceMeshRenderer<T, U>::UpdateRender(int iIndex=-1, bool iUpdateDisplay=0)
	{
		if (!m_ready) return false;
	
		if (!m_externalContext)
		{
			bool res = GLRendererBase<T, U>::UpdateRender(iIndex, iUpdateDisplay);
			if (!res) return false;
		}

		return true;			
	};		
	

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
	int GLSurfaceMeshRenderer<T, U>::initGLthread()
	{
		
		if (m_externalContext)
		{
			if((wglMakeCurrent(m_hdContext, m_glContext)) == NULL)
			{
				////MessageBox::Show("wglMakeCurrent Failed");
				LOG0("GLSurfaceMeshRenderer::wglMakeCurrent Failed");
				//return false;
			}
		}

		{
			glEnable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL); 
			glDepthMask(GL_TRUE);
			glShadeModel(GL_SMOOTH);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
			glPolygonMode(GL_POLYGON_SMOOTH, GL_FILL);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
		}

		
		return 1;
	}

	template <class T, class U>
	void GLSurfaceMeshRenderer<T, U>::StartExecution(void *iParam)
	{		
		return; 
	}

	template <class T, class U>
	void GLSurfaceMeshRenderer<T, U>::renderSingleVolume(const int iMeshIndex/*=0*/)
	{
		if (m_polyMeshList.empty()) return;

		if (iMeshIndex>m_polyMeshList.size()-1)
			return;		

		if (!m_polyMeshList[iMeshIndex]) 
			return;
	
		PGMath::Point3D<float> meshOrigin(m_polyMeshList[iMeshIndex]->GetOrigin());
		PGMath::Point3D<float> meshSpan(m_polyMeshList[iMeshIndex]->GetSpan());

		PGMath::Point3D<float> skelOrigin(m_polyMeshList[iMeshIndex]->GetSkeletonOrigin());
		PGMath::Point3D<float> skelSpan(m_polyMeshList[iMeshIndex]->GetSkeletonSpan());

		// draw the mesh here
		// scale accordingly
		

		glMatrixMode(GL_MODELVIEW);

		float lighPosFac=1.0f;

		GLfloat light_position0[] = { lighPosFac, lighPosFac, lighPosFac, 1 };
		GLfloat light_position1[] = { -lighPosFac, -lighPosFac, -lighPosFac, 1 };
		GLfloat light_position2[] = { 0, 0,  lighPosFac, 0 };
		GLfloat light_position3[] = { 0, 0, -lighPosFac, 0 };

		GLfloat light_ambient[] = { 0.4, 0.2, 0.1, 0.1 };
		GLfloat light_diffuse[] = { 0.7, 0.7, 0.6, 1 };
		GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1 };


		glEnable(GL_LIGHTING);

		glShadeModel(GL_SMOOTH);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
			glPolygonMode(GL_POLYGON_SMOOTH, GL_FILL);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);		
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

		
		glEnable(GL_LIGHT1);		
		//glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
		//glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
		//glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);		
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	
	  
		glEnable(GL_LIGHT2);		
		//glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
		//glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
		//glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);		
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

		glEnable(GL_LIGHT3);		
		//glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
		//glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
		//glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);		
		glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

		float trans = 1.0f;

		GLfloat matAmbientTrans[4] = {0.600000, 0.6000000, 0.600000, trans};
        GLfloat matDiffuseTrans[4] = {0.717977, 0.428208, 0.312866, trans};
        GLfloat matSpecularTrans[4] = {1.00000, 1.000000, 1.000000, trans};
        GLfloat matShiniTrans =  96.078431;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matAmbientTrans);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matDiffuseTrans);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecularTrans);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShiniTrans);

		
		glDisable(GL_LIGHTING);


		float meshScFactor = 1.0f;
		
		if (meshSpan.Z()>0.0f) meshScFactor/=meshSpan.Z();
		
		//glClearColor(0, 0, 0, 0);		
		
		glTranslatef(gTranslation.X(), gTranslation.Y(), -gTranslation.Z());
			glRotatef(90, 1, 0, 0);
			glRotatef(gRotation.X(), 1, 0, 0);
			glRotatef(gRotation.Y(), 0, 0, 1);
			glRotatef(gRotation.Z(), 0, 1, 0);
		
		
		// mesh first
		glPushMatrix();
		{		
			
			int polySkip = 16;//36;//
			glScalef(meshScFactor, meshScFactor, meshScFactor);				
			glTranslatef(-meshOrigin.X(), -meshOrigin.Y(), -meshOrigin.Z());			
			glColor4f(1, 1, 1, 0.1f);
			unsigned int polyCount = m_polyMeshList[iMeshIndex]->GetDimensions();
		
			for (int i=0; i<polyCount; i+=polySkip)
			{
				const PGCore::Polygon<float, 3>& nextPoly = m_polyMeshList[iMeshIndex]->GetPolygon(i);
				
				glBegin(GL_POINTS);//LINE_LOOP);//TRIANGLES);
				for (int j=0; j<3; j++)
				{						
					const PGMath::Point3D<float>& nextNormal = nextPoly.GetNormal(j);
	
					if (nextNormal.Length()>0)
					{	
						glNormal3f(nextNormal.X(), nextNormal.Y(), nextNormal.Z());
					}		
					
					glVertex3f(nextPoly.GetVertex(j).X(), nextPoly.GetVertex(j).Y(), nextPoly.GetVertex(j).Z());
				}
				glEnd();
			}				
			
		} 
		glPopMatrix();

		// Now the tree
		glPushMatrix();
		{ 
			float skelScFactor=1.0f;		
			skelScFactor = meshScFactor;
			//glRotatef(180, 0, 0, 1);
			glScalef(1, 1, 1);		
			//glRotatef(180, 1, 0, 0);

			glScalef(skelScFactor, skelScFactor, skelScFactor);					
			//glTranslatef(-skelOrigin.X(), -skelOrigin.Y(), -skelOrigin.Z());		
			glTranslatef(-meshOrigin.X(), -meshOrigin.Y(), -meshOrigin.Z());


			float treeColors[2][4] = {{0,  0.7,  0, 0.5}, {0.8, 0, 0.3, 0.5} }; // 
			float lWidths[2] = {2, 2}; // ref: 1, sub: 2
			int beginIdx[2] = {1, 1};
			int pSkip=1;

			//glEnable(GL_LINE_STIPPLE);
			//glLineStipple(1, 0xAAAA);
			for (int k=0; k<2; k++)
			{
				glLineWidth(lWidths[k]);//
				glPointSize(2);//
				glColor4fv(treeColors[k]);
				unsigned int clineCount = m_polyMeshList[iMeshIndex]->GetSkeletonDimensions(k);
							
				for (int i=0; i<clineCount; i++)
				{				
					const std::vector<PGMath::Point3D<float> >& nextCenterline = m_polyMeshList[iMeshIndex]->GetSkeletonElement(i, k); 
				
					glBegin(GL_LINE_STRIP);
					for (int j=beginIdx[k]; j<nextCenterline.size(); j+=pSkip)
					{					
						//glPushMatrix();
						//glTranslatef(nextCenterline[j-1].X(), nextCenterline[j-1].Y(), nextCenterline[j-1].Z());
						//gluCylinder(m_quadric, 0.01, 0.01, 0.01, 32, 32);					
						glVertex3f(nextCenterline[j-1].X(), nextCenterline[j-1].Y(), nextCenterline[j-1].Z());
						glVertex3f(nextCenterline[j].X(), nextCenterline[j].Y(), nextCenterline[j].Z());
						//glPopMatrix();
					}
					glEnd();

					glBegin(GL_POINTS);
					for (int j=0; j<nextCenterline.size(); j++)
					{	
						glVertex3f(nextCenterline[j].X(), nextCenterline[j].Y(), nextCenterline[j].Z());						
					}
					glEnd();
				}
			}
			glLineWidth(1.0f);//
			glPointSize(1);//
			//glDisable(GL_LINE_STIPPLE);
		}
		glPopMatrix();	
		//glPopMatrix();
	}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class GLSurfaceMeshRenderer<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 		
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLSurfaceMeshRenderer_HPP_
