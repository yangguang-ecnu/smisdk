// -*- C++ -*-
// � [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
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
		
		glClearColor( 0.568, 0.568, 0.568, 1.0 );

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
		
		
		std::vector<float> dims;	
		{
			dims.push_back(skelSpan.X());
			dims.push_back(skelSpan.Y());
			dims.push_back(skelSpan.Z());	
			std::sort(dims.begin(), dims.end());		
		}	

		// 1.0 unit = dims[2] mm
		// 1 mm = 1.0/dims[2] units		
		float oneMmUnit = 1.0f;//1.0f;
		if (dims[2]>0.0f) 
		{
			//meshScFactor/=dims[2];
			// 1.0 unit = dims[2] mm
			// 1 mm = 1.0/dims[2] units
			oneMmUnit = 1.0f/dims[2];
		}
		
		float cursorSz = 10.0f*oneMmUnit;
		
		//glClearColor(0, 0, 0, 0);		
		
		glTranslatef(gTranslation[iMeshIndex].X(), gTranslation[iMeshIndex].Y(), -gTranslation[iMeshIndex].Z());
			glRotatef(-90, 1, 0, 0);
			glRotatef(gRotation[iMeshIndex].X(), 1, 0, 0);
			glRotatef(gRotation[iMeshIndex].Y(), 0, 0, 1);
			glRotatef(gRotation[iMeshIndex].Z(), 0, 1, 0);
		
		
		// cursor
		glPushMatrix();
			glScalef(1.0, 1.0, 1.0);
			// x axis
			glColor4f(1, 0, 0, 0.5);
			glBegin(GL_LINES);
				glVertex3f(m_cursor3D.X(), m_cursor3D.Y(), m_cursor3D.Z());				
				glVertex3f(m_cursor3D.X()+cursorSz, m_cursor3D.Y(), m_cursor3D.Z());
			glEnd();

			// y axis
			glColor4f(0, 1, 0, 0.5);			
			glBegin(GL_LINES);
				glVertex3f(m_cursor3D.X(), m_cursor3D.Y(), m_cursor3D.Z());
				glVertex3f(m_cursor3D.X(), m_cursor3D.Y()+cursorSz, m_cursor3D.Z());
			glEnd();
			
			// XY plane - Z axis			
			glColor4f(0.7, 0.7, 1, 0.5);
			glBegin(GL_LINES);
				glVertex3f(m_cursor3D.X(), m_cursor3D.Y(), m_cursor3D.Z());
				glVertex3f(m_cursor3D.X(), m_cursor3D.Y(), m_cursor3D.Z()+cursorSz);
			glEnd();
		glPopMatrix();

		
		float gXMin=-0.5, gYMin=-0.5, gZMin=-0.5;
		float gXMax=0.5, gYMax=0.5, gZMax=0.5;
		glPushMatrix();
		{
			glColor4f(0.2,0.2,0.2,0.2);
			glBegin(GL_LINE_LOOP);			
				glVertex3f( gXMin, gYMin, gZMin);
				glVertex3f( gXMin, gYMax, gZMin); 
				glVertex3f( gXMin, gYMax, gZMax); 
				glVertex3f( gXMin, gYMin, gZMax);
				glEnd(); 
			glBegin(GL_LINE_LOOP);			
				glVertex3f( gXMax, gYMin, gZMin);
				glVertex3f( gXMax, gYMax, gZMin); 
				glVertex3f( gXMax, gYMax, gZMax); 
				glVertex3f( gXMax, gYMin, gZMax);
			glEnd(); 
			glBegin(GL_LINE_LOOP);			
				glVertex3f( gXMin, gYMin, gZMin);
				glVertex3f( gXMin, gYMax, gZMin); 
				glVertex3f( gXMax, gYMax, gZMin); 
				glVertex3f( gXMax, gYMin, gZMin);
			glEnd(); 
			glBegin(GL_LINE_LOOP);			
				glVertex3f( gXMin, gYMin, gZMax);
				glVertex3f( gXMin, gYMax, gZMax); 
				glVertex3f( gXMax, gYMax, gZMax); 
				glVertex3f( gXMax, gYMin, gZMax);
			glEnd(); 
			glDisable(GL_LINE_STIPPLE);
		}
		glPopMatrix();
		


		// mesh first
		glPushMatrix();
		{		
			glRotatef(-90, 1, 0, 0);
			int polySkip = 16;//36;//
			glScalef(meshScFactor, meshScFactor, meshScFactor);							
			glTranslatef(-meshOrigin.X(), -meshOrigin.Y(), -meshOrigin.Z());			
			glColor4f(1, 1, 1, 0.1f);
			unsigned int polyCount = m_polyMeshList[iMeshIndex]->GetDimensions();
		
			for (int i=0; i<polyCount; i+=polySkip)
			{
				const PGCore::Polygon<float, 3>& nextPoly = m_polyMeshList[iMeshIndex]->GetPolygon(i);
				
				glBegin(GL_POINTS);
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

		// clouds next
		glPushMatrix();
		{		
			float skelScFactor=1.0f;		
			skelScFactor = meshScFactor;			
			glScalef(1, 1, 1);					
			glRotatef(-90, 1, 0, 0);

			glScalef(skelScFactor, skelScFactor, skelScFactor);								
			glTranslatef(-meshOrigin.X(), -meshOrigin.Y(), -meshOrigin.Z());

			// point, tool, target - ref
			// point, tool, target - sec
			float treeColors[2][3][4] = {
										{{1.0,  0.3,  0.2, 0.7}, {0.3, 1.0, 1.0, 1.0},  {0.0, 1, 0, 0.5} }, // Orange, cyan, green - BIG
										{{0.1,  1,  0.5, 0.7}, {0.3, 1.0, 1, 0.8},  {1.0, 1, 1, 0.8}} // green, white, white - faded
									}; 
			float geomPtSz[2][2] = { {1, 2}, {1, 2} };   
			float sphereDivn[2] = {16, 8};
			float sphereRadius[2] = {0.001, 0.001}; // ~1mm radius
			int pSkip=1;

			for (int j=0; j<2; j++)
			{	
				// pt cloud
				const PGMath::AffineTransform<float> &regTrans =  m_polyMeshList[iMeshIndex]->GetRegistrationTransform(j);
				const Matrix4x4<float>& regMat = regTrans.ConstMatrix4x4();
				glScalef(regMat[0], regMat[5], regMat[10]);			

				glPointSize(geomPtSz[j][0]);
				glColor4fv(treeColors[j][0]);
				std::vector<PGMath::Point3D<float> >& ptCloud = m_polyMeshList[iMeshIndex]->GetPointCloud(j);
				unsigned int pointCount = ptCloud.size();		
				glBegin(GL_POINTS);
				for (int i=0; i<pointCount; i+=pSkip)
				{
					const PGMath::Point3D<float>& nextPoint = ptCloud[i];						
					glVertex3f(nextPoint.X(), nextPoint.Y(), nextPoint.Z());								
				}	
				if (ptCloud.size()>0)
				{
					const PGMath::Point3D<float>& nextPoint = ptCloud[ptCloud.size()-1];						
					glVertex3f(nextPoint.X(), nextPoint.Y(), nextPoint.Z());	
				}
				glEnd();

				// tool				
				glPointSize(geomPtSz[j][1]);
				glColor4fv(treeColors[j][1]);
				std::vector<PGMath::Point3D<float> >& tlCloud = m_polyMeshList[iMeshIndex]->GetToolCloud(j);
				pointCount = tlCloud.size();		
				//if (pointCount>1) glBegin(GL_LINES);
				glBegin(GL_POINTS);
				for (int i=0; i<pointCount; i+=pSkip)
				{
					const PGMath::Point3D<float>& nextPoint = tlCloud[i];						
					glVertex3f(nextPoint.X(), nextPoint.Y(), nextPoint.Z());				
					//renderSphere_convenient(nextPoint.X(), nextPoint.Y(), nextPoint.Z(), oneMmUnit*0.25f, sphereDivn[j]);
				}	
				if (tlCloud.size()>0)
				{
					const PGMath::Point3D<float>&nextPoint = tlCloud[tlCloud.size()-1];						
					glVertex3f(nextPoint.X(), nextPoint.Y(), nextPoint.Z());	
				//	renderSphere_convenient(nextPoint.X(), nextPoint.Y(), nextPoint.Z(), oneMmUnit*0.25f, sphereDivn[j]);
				}
				glEnd();
				

				// target
				if (j==0) glEnable(GL_LIGHTING);
				glPointSize(geomPtSz[j][2]);
				glColor4fv(treeColors[j][2]);				
				std::vector<PGMath::Point3D<float> >& tgCloud = m_polyMeshList[iMeshIndex]->GetTargetCloud(j);
				pointCount = tgCloud.size();		
				//glBegin(GL_POINTS);
				glScalef(1, 1, 1);
				for (int i=0; i<pointCount; i+=pSkip)
				{
					const PGMath::Point3D<float>& nextPoint = tgCloud[i];						
					//glVertex3f(nextPoint.X(), nextPoint.Y(), nextPoint.Z());								
					renderSphere_convenient(nextPoint.X(), nextPoint.Y(), nextPoint.Z(), oneMmUnit, sphereDivn[j]);
				}	
				//glEnd();
				glDisable(GL_LIGHTING);
				
			}

			// shape cloud
			glPointSize(1.0f);
			glScalef(1, 1, 1);
			glColor4f(1.0, 1.0, 0.0, 0.75);
			std::vector<PGMath::Point3D<float> >& ptCloud = m_polyMeshList[iMeshIndex]->GetShapeCloud();
			unsigned int pointCount = ptCloud.size();		
			if (pointCount>1)
			{
				glBegin(GL_LINES);
				for (int i=0; i<pointCount; i++)
				{
					const PGMath::Point3D<float>& nextPoint = ptCloud[i];						
					glVertex3f(nextPoint.X(), nextPoint.Y(), nextPoint.Z());								
				}	
			}
			if (ptCloud.size()>0)
			{
				glEnable(GL_LIGHTING);
				const PGMath::Point3D<float>& nextPoint = ptCloud[ptCloud.size()-1];										
				renderSphere_convenient(nextPoint.X(), nextPoint.Y(), nextPoint.Z(), oneMmUnit, sphereDivn[0]);
				glDisable(GL_LIGHTING);
			}

			glEnd();

			glPointSize(1.0);
			
		} 
		glPopMatrix();

		// Now the tree
		glPushMatrix();
		{ 
			float skelScFactor=1.0f;		
			skelScFactor = meshScFactor;			
			glScalef(1, 1, 1);					
			glRotatef(-90, 1, 0, 0);

			glScalef(skelScFactor, skelScFactor, skelScFactor);								
			glTranslatef(-meshOrigin.X(), -meshOrigin.Y(), -meshOrigin.Z());

			
			float treeColors[2][4] = {{0.196, 0.82, 0.20, 1.0}, {0.32,  0.36,  0.85, 1.0}}; // 
			float lWidths[2] = {1, 1}; // ref: 1, sub: 2
			int beginIdx[2] = {1, 1};
			int pSkip=1;
		
			for (int k=0; k<2; k++)
			{
				const PGMath::AffineTransform<float> &regTrans =  m_polyMeshList[iMeshIndex]->GetRegistrationTransform(k);
				const Matrix4x4<float>& regMat = regTrans.ConstMatrix4x4();
				glScalef(regMat[0], regMat[5], regMat[10]);			
				glLineWidth(lWidths[k]);
				glPointSize(2);
				glColor4fv(treeColors[k]);
				unsigned int clineCount = m_polyMeshList[iMeshIndex]->GetSkeletonDimensions(k);
							
				for (int i=0; i<clineCount; i++)
				{				
					const std::vector<PGMath::Point3D<float> >& nextCenterline = m_polyMeshList[iMeshIndex]->GetSkeletonElement(i, k); 
				
					glBegin(GL_LINE_STRIP);
					for (int j=beginIdx[k]; j<nextCenterline.size(); j+=pSkip)
					{				
						glVertex3f(nextCenterline[j-1].X(), nextCenterline[j-1].Y(), nextCenterline[j-1].Z());
						glVertex3f(nextCenterline[j].X(), nextCenterline[j].Y(), nextCenterline[j].Z());
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
			glLineWidth(1.0f);
			glPointSize(1);
		}
		glPopMatrix();			
	}

	template <class T, class U>
	void GLSurfaceMeshRenderer<T, U>::renderSphere(float x, float y, float z, float radius,int subdivisions,GLUquadricObj *quadric)

	{

		glPushMatrix();

		glTranslatef( x,y,z );

		gluSphere(quadric, radius, subdivisions,subdivisions);

		glPopMatrix();

	}

	template <class T, class U>
	void GLSurfaceMeshRenderer<T, U>::renderSphere_convenient(float x, float y, float z, float radius,int subdivisions)

	{

		//the same quadric can be re-used for drawing many spheres

		GLUquadricObj *quadric=gluNewQuadric();

		gluQuadricNormals(quadric, GLU_SMOOTH);

		renderSphere(x,y,z,radius,subdivisions,quadric);

		gluDeleteQuadric(quadric);

	}




#ifdef _PG_GENERATE_SDK_LIBS_
	template class GLSurfaceMeshRenderer<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 		
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLSurfaceMeshRenderer_HPP_
