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
// Filename:	GLMPRRenderer.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLMPRRenderer_HPP_
#define _PGGLMPRRenderer_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/GLMPRRenderer.h"

namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;

	template <class T>
	static void makeCheckImage(int iX, int iY, T *ioImg)
	{
		int i, j, k, c;
		T tPixel;
		for (i = 0; i < iY; i++) 
		{
			for (j = 0; j < iX; j++) 
			{
				ioImg[i * iX + j] = i;//tPixel;//(GLubyte) tPixel.X();				
			}
		}
	}

	template <class T>
	static void makeCheckImageRGBA(int iX, int iY, T *ioImg)
	{
		int i, j, k, c;
		T tPixel;
		for (i = 0; i < iY; i++) 
		{
			for (j = 0; j < iX; j++) 
			{
				//c = ((((i&0x8)==0)^((j&0x8))==0))*255;
				c = ((((i&64)==0)^((j&64))==0))*255;
				tPixel = T(c, j, i, 255);

				ioImg[i * iX + j] = tPixel;//(GLubyte) tPixel.X();
				
				/*
				ioImg[i * iX + j + 0] = (GLubyte) tPixel.X();
				ioImg[i * iX + j + 1] = (GLubyte) tPixel.Y();
				ioImg[i * iX + j + 2] = (GLubyte) tPixel.Z();
				ioImg[i * iX + j + 3] = (GLubyte) tPixel.W();*/
				//for (k=1; k < 3; k++)
				//	ioImg[i * iX + j + k] = (GLubyte) 0;
				//ioImg[i * iX + j + k] = 127;//0.5f;
			}
		}
	}

	template <class T>
	static void makeCheckImageRGBA4(int iX, int iY, T *ioImg)
	{
		int i, j, k, c;
		T tPixel;
		for (i = 0; i < iY; i++) 
		{
			for (j = 0; j < iX*4; j+=4) 
			{
				//c = ((((i&0x8)==0)^((j&0x8))==0))*255;
				c = ((((i&64)==0)^((j&64))==0))*255;
				//tPixel = T(c, 0, 0, 127);

				ioImg[i * iX + j] = c;//tPixel;//(GLubyte) tPixel.X();
				ioImg[i * iX + j+1] = c;//tPixel;//(GLubyte) tPixel.X();
				ioImg[i * iX + j+2] = c;//tPixel;//(GLubyte) tPixel.X();
				ioImg[i * iX + j+3] = 255;//tPixel;//(GLubyte) tPixel.X();
				/*
				ioImg[i * iX + j + 0] = (GLubyte) tPixel.X();
				ioImg[i * iX + j + 1] = (GLubyte) tPixel.Y();
				ioImg[i * iX + j + 2] = (GLubyte) tPixel.Z();
				ioImg[i * iX + j + 3] = (GLubyte) tPixel.W();*/
				//for (k=1; k < 3; k++)
				//	ioImg[i * iX + j + k] = (GLubyte) 0;
				//ioImg[i * iX + j + k] = 127;//0.5f;
			}
		}
	}

	template <class T, class U>
	inline GLMPRRenderer<T, U>::GLMPRRenderer() 
	{		
		m_rendererType = kPgRendererTypeMPR;
		m_renderCount = 0;

		m_colorXAxis[0] = 1; m_colorXAxis[1] = 0; m_colorXAxis[2] = 0; m_colorXAxis[3] = 1;
		m_colorYAxis[0] = 0; m_colorYAxis[1] = 1; m_colorYAxis[2] = 0; m_colorYAxis[3] = 1;
	};
	
	template <class T, class U>
	inline bool GLMPRRenderer<T, U>::PrepareRenderer()
	{
		LOG0("{ GLMPRRenderer:PrepareRenderer");

		if (m_externalContext)
		{
			if (m_hdContext == (HDC)0 || m_glContext == (HGLRC)0)
			{
				LOG0("GLMPRRenderer: Error: no context");
				return false;
			}
		}

		if ((!m_iDataObject) || (m_rows*m_columns)<=0) 
		{
			LOG0("GLMPRRenderer: Error: no input OR image size not set yet");
			return false;
		}		

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) {
			LOG0("GLMPRRenderer:initVolumes Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount();
		if (iMultiVolumeCount < 1)
		{
			LOG0("GLMPRRenderer:initVolumes Error: Needs at least one volume");
			return false;
		}

		//if (!m_imageSizeChanged) 
		Clear();

		int i=0;
		if (m_pTextureName.size()<iMultiVolumeCount)
		{
			m_pTextureName.resize(iMultiVolumeCount);
			m_blendRatio.resize(iMultiVolumeCount);
			for (i=0; i<iMultiVolumeCount; i++)
			{
				m_pTextureName[i] = GLuint(99);//.push_back(GLuint(99));
				m_blendRatio[i] = float(1.0f/float(iMultiVolumeCount));//.push_back(float(1.0f/float(iMultiVolumeCount)));
			}
		}

		if (!initGLthread())
		{
			LOG0("GLMPRRenderer: Error: failed to initialize GL machine.");
			return false;
		}

		if (m_mprRenderer.size()<iMultiVolumeCount)
		{
			m_mprRenderer.resize(iMultiVolumeCount);
			for (i=0; i<iMultiVolumeCount; i++)
			{
				// get the input volume

				PGCore::Voxel3D< T > *inVolume = (static_cast<PGCore::Voxel3D < T > *>(inMultiVolume->GetDataObject(i)));
				if (!inVolume) {
					LOG1("GLMPRRenderer: Error: failure to fetch input volume %d", i);
					return false;
				}

				PGMath::Point3D<float> pDimensions;
				pDimensions = inVolume->GetDimensions();
				if (pDimensions.Z()<=0) {
					LOG1("GLMPRRenderer: Error: empty input volume %d", i);
					return false;
				}

				// ok query 		
				if (m_oImageObject==0)
				{
					//int iRows = (int)(pDimensions.Y()), iColumns = (int)(pDimensions.X());
					m_oImageObject = new PGCore::Image<PGMath::Point3D<U> >(m_rows, m_columns);				

					long ir = m_rows, ic = m_columns;
					m_tempImageObject.SetDimensions(ir, ic);


#if (_DEBUG_DUMP_IMG)
					//PGCore::PixelBase<U> tPixel;
					//U rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
					//m_checkImage.GetDataRange(&rangeMin, &rangeMax);	
					//DumpImageAsPPM(m_checkImage, "C:/Tmp/CheckImage.ppm");
#endif
#if    (_USE_TEXTURE_CHKIMG)
					m_checkImage.SetDimensions(ir, ic);
					makeCheckImageRGBA(ic, ir, (U *)(m_checkImage.GetBuffer()));
					if (1)
					{
						//char chkImg[256][256][4];
						//makeCheckImage(ic, ir, &(chkImg[0][0]));

						//makeCheckImageRGBA4(ic, ir, &(chkImg[0][0][0]));
						glEnable(GL_TEXTURE_2D);
						glBindTexture (GL_TEXTURE_2D, m_pTextureName[1]);

						//errrr
						glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA,
							m_rows, m_columns, 0, GL_RGBA, 
							GL_UNSIGNED_BYTE,
							(GLvoid *)(m_checkImage.GetBuffer()) );
						//(GLvoid *)&(chkImg[0][0][0]));



						//glTexImage2D (GL_TEXTURE_2D, 0, 1,
						//	m_rows, m_columns, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
						//	(GLvoid *)(&(chkImg[0][0])));
					}
#endif
				}

				/*PGCore::Volume< T > *volumeAccessor = inVolume->GetVolumeAccessor();
				if (!volumeAccessor) {
				LOG1("GLMPRRenderer: Error: failure to fetch input volume accessor %d", i);
				return false;
				}*/

				unsigned int rRows = m_rows, rCols = m_columns;
				PGAlgs::VoxelToMPRRenderer<T, U> *mprRenderer = new PGAlgs::VoxelToMPRRenderer<T, U>;
				if (!mprRenderer) return false;
				mprRenderer->SetInput(static_cast<PGCore::BaseDataObject *>(inVolume));//m_iDataObject);
				mprRenderer->SetImageSize(rRows, rCols);
				mprRenderer->PrepareRenderer();
				m_mprRenderer[i] = mprRenderer;//.push_back(mprRenderer);

			}
		}

		bool rv = GLRendererBase::PrepareRenderer();
		//return SetSlicerPositionAndDirection(iPosition, iDirection);
		
		LOG0("} GLMPRRenderer:PrepareRenderer");

		return rv;
	}


	template <class T, class U>
	bool GLMPRRenderer<T, U>::SetBlendWeight(const float iWeight, const int iRendererIndex=0) 
	{ 
		bool rv = DeviceRendererBase::SetBlendWeight(iWeight, iRendererIndex);
		if (!rv)
		{
			return false;
		}
		
		// setthis blend ratio only if a single volume has been uploaded
		if (m_mprRenderer.size()==1)
		{
			rv = m_mprRenderer[iRendererIndex]->SetBlendWeight(iWeight);
		}
		
		return rv;
	}


	template <class T, class U>
	bool GLMPRRenderer<T, U>::ImageSizeUpdated()
	{
		//bool rv = initGLthread();
		//if (rv) return false;

		if (m_mprRenderer.empty()) return true;

		// reallocate img buffer here
		// ok query 		
		if (m_oImageObject)
		{
			delete m_oImageObject;
			m_oImageObject = 0;
		}


		{

			m_oImageObject = new PGCore::Image<PGMath::Point3D<U> >(m_rows, m_columns);				

			long ir = m_rows, ic = m_columns;
			m_tempImageObject.SetDimensions(ir, ic);


#if (_DEBUG_DUMP_IMG)
			//PGCore::PixelBase<U> tPixel;
			//U rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
			//m_checkImage.GetDataRange(&rangeMin, &rangeMax);	
			//DumpImageAsPPM(m_checkImage, "C:/Tmp/CheckImage.ppm");
#endif
#if    (_USE_TEXTURE_CHKIMG)
			m_checkImage.SetDimensions(ir, ic);
			makeCheckImageRGBA(ic, ir, (U *)(m_checkImage.GetBuffer()));
			if (1)
			{
				//char chkImg[256][256][4];
				//makeCheckImage(ic, ir, &(chkImg[0][0]));

				//makeCheckImageRGBA4(ic, ir, &(chkImg[0][0][0]));
				glEnable(GL_TEXTURE_2D);
				glBindTexture (GL_TEXTURE_2D, m_pTextureName[1]);

				//errrr
				glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA,
					m_rows, m_columns, 0, GL_RGBA, 
					GL_UNSIGNED_BYTE,
					(GLvoid *)(m_checkImage.GetBuffer()) );
				//(GLvoid *)&(chkImg[0][0][0]));



				//glTexImage2D (GL_TEXTURE_2D, 0, 1,
				//	m_rows, m_columns, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
				//	(GLvoid *)(&(chkImg[0][0])));
			}
#endif
		}


		for (int i=0; i<m_mprRenderer.size(); i++)
		{
			if (!m_mprRenderer[i]) return false;				

			m_mprRenderer[i]->SetImageSize(m_rows, m_columns);
			m_mprRenderer[i]->PrepareRenderer();				
		}

		return true;
	}


	template <class T, class U>
	inline bool GLMPRRenderer<T, U>::Execute()
	{	
		bool rv = PrepareRenderer();
		if (!rv)
			return false;

		//rv = initGLthread();//1, (char **)this);
		//if (!rv)
		//	return false;

		return rv;
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
	int GLMPRRenderer<T, U>::init(const int iVolumeIndex/*=0*/)// int    argc, char **argv )
	{
#ifdef _USE_TEXTURE
		
		glEnable(GL_TEXTURE_2D);
		glGenTextures (m_pTextureName.size(), &(m_pTextureName[0]));
		
		int rVal = logLastGlError();
		if (rVal<0)
		{
			return 0;
		}

		for (int i=0; i<m_pTextureName.size(); i++)
		{
			glBindTexture (GL_TEXTURE_2D, m_pTextureName[i]);
			bool isTexture = glIsTexture(m_pTextureName[i]);
			int texName = (int)m_pTextureName[i];
			LOG2("<%d>: Texture: %d", this, texName);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}

		/*
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);*/

#endif
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
	void GLMPRRenderer<T, U>::display()
	{
		/*
		if (gFrameCount==0)  
		{
		gTimer.Reset();
		}
		gFrameCount = (gFrameCount+1)%kPgFramesToMeasure;
		*/

		float sY = 2.0f*m_screenY, sX = 2.0f*m_screenX;
		float sYn = 0, sXn = 0;
		float originFacX = 0;//2.0f*m_screenX, 
		float originFacY = 0;//2.0f*m_screenY;

		float boxSize = 1.0f;//m_rows/256.0f;

		originFacX = m_screenLastX; originFacY = m_screenLastY;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(gTranslation.X(), gTranslation.Y(), gTranslation.Z());

		/*
		if (m_centerOnCursor==1)
		{
			//glMatrixMode(GL_MODELVIEW);
			sYn = -2.0f*m_screenY; sXn = -2.0f*m_screenX;

			//glTranslatef(sXn, sYn, 0);
			//glScalef(m_zoomFactor, m_zoomFactor, 1);
			//glTranslatef(-sXn, -sYn, 0);

			glTranslatef(sXn, sYn, 0);
			m_screenLastX+= m_screenX;
			m_screenLastY+= m_screenY;

			//originFacX = m_screenLastX; originFacY = m_screenLastY;

			//m_centerOnCursor = false;
		} else if (m_centerOnCursor==-1)
		{
			//glMatrixMode(GL_MODELVIEW);
			sYn = 2.0f*m_screenLastY; sXn = 2.0f*m_screenLastX;
			glTranslatef(sXn, sYn, 0);
			m_screenLastX = 0;
			m_screenLastY = 0;
			m_centerOnCursor = 0;
		}
		*/

		// zoom re-centering disabled for now
		if (m_centerOnCursor==1)
		{
			glTranslatef(2.0f*m_screenX, 2.0f*m_screenY, 0);		
		}
		
		glScalef(m_zoomFactor, m_zoomFactor, 1);
		
		if (m_centerOnCursor==1)
		{
			glTranslatef(-2.0f*m_screenX, -2.0f*m_screenY, 0);
		}

		

#ifdef _USE_TEXTURE
				//glBindTexture(GL_TEXTURE_2D, m_pTextureName);
				glClearColor( 0, 0, 0,  1);	
				//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer
								
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				
				
				//float m_zFacX = (1.0f - 1.0f/m_zoomFactor) * 0.5f;// * originFacX;
				//float m_zFacY = (1.0f - 1.0f/m_zoomFactor) * 0.5f;// * originFacY;

				float txBegin = 0.0f;// + m_zFacX + originFacX;
				float txEnd   = 1.0f;//- m_zFacX + originFacX;
				float tyBegin = 1.0f;// + m_zFacY + originFacY;
				float tyEnd   = 0.0f;// - m_zFacY + originFacY;

				/*
				if (txBegin < 0)
				{
					txBegin	=	0;
					txEnd	=	txBegin + (1.0f - 2.0f*m_zFacX);
				}
				if (txEnd >1.0f)
				{
					txBegin	=	1.0f - (1.0f - 2.0f*m_zFacX);
					txEnd	=	1.0f;//txBegin + (1.0f - 2.0f*m_zFacX);
				}
				
				if (tyBegin < 0)
				{
					tyBegin	=	0;
					tyEnd	=	tyBegin + (1.0f - 2.0f*m_zFacY);
				}
				if (tyEnd >1.0f)
				{
					tyBegin	=	1.0f - (1.0f - 2.0f*m_zFacY);
					tyEnd	=	1.0f;//txBegin + (1.0f - 2.0f*m_zFacX);
				}
				*/

				// check if multiple data are loaded
				// m_volumeAccessor[vIndex]->GetBitVolume().IsInitialized();
				bool multipleData = (m_pTextureName.size()>1);
				

				for (int i=0; i<m_pTextureName.size(); i++)
				{	
					// bRatio = (int)floor(m_blendRatio[i]);
					// glColor4f(255,255,255,1);//bRatio);
					//glColor4f(m_blendRatio[i], m_blendRatio[i], m_blendRatio[i], m_blendRatio[i]);
					//glColor4f(m_blendRatio[i], m_blendRatio[i], m_blendRatio[i], m_blendRatio[i]);

					//glColor4f(0.5f, 0.5f, 0.5f, m_blendRatio[i]);

					float blendRatio = multipleData ? m_blendRatio[i] : 1.0f;
					glColor4f(1, 1, 1, blendRatio);
					
					glBindTexture(GL_TEXTURE_2D, m_pTextureName[i]);	
					// Front Face
					glBegin(GL_QUADS);
					glTexCoord2f(txBegin, tyBegin); 
					glVertex3f(-boxSize, -boxSize,  boxSize*i);	// Bottom Left Of The Texture and Quad
					glTexCoord2f(txBegin, tyEnd); 
					glVertex3f(-boxSize,  boxSize,  boxSize*i);	// Top Left Of The Texture and Quad
					glTexCoord2f(txEnd, tyEnd); 
					glVertex3f( boxSize,  boxSize,  boxSize*i);	// Top Right Of The Texture and Quad
					glTexCoord2f(txEnd, tyBegin); 
					glVertex3f( boxSize, -boxSize,  boxSize*i);	// Bottom Right Of The Texture and Quad				
					glEnd();				
				}

				glDisable(GL_TEXTURE_2D);
				//glFlush();
#else
		{
			glRasterPos3i(-1, -1, 0);
			//glDrawPixels(m_columns, m_rows, m_imageFormat,  m_imageType, m_oImageBuf);

			// draw check image
			glRasterPos3i(-1, -1, 1);
			glDrawPixels(m_columns, m_rows, m_imageFormat,  m_imageType, m_checkImage.GetBuffer());
			
		}
#endif
		
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glTranslatef(gTranslation.X(), gTranslation.Y(), gTranslation.Z());

		// zoom re-centering disabled for now
		
		{
			//glTranslatef(2.0f*m_screenX, 2.0f*m_screenY, 0);		
		}
		
		//glScalef(m_zoomFactor, m_zoomFactor, 1);
		
		
		//keep cursor gap fixed. use zoom factor
		//also, there is still sync error when in zoomed mode


		//glColor4f(1,0,0,1);
		//glColor4f(1, 0.5f, 0, 1);
		{
			glBegin(GL_LINES);			
			glColor4fv(m_colorXAxis);
			glVertex3f(-1.0f, sY, 0.0f);glVertex3f(sX-0.1f/m_zoomFactor, sY, 0.0f);
			glVertex3f(sX+0.1f/m_zoomFactor, sY, 0.0f);glVertex3f(1.0f, sY, 0.0f);

			glColor4fv(m_colorYAxis);
			glVertex3f(sX, -1.0f, 0.0f);glVertex3f(sX, sY-0.1f/m_zoomFactor,  0.0f);
			glVertex3f(sX, sY+0.1f/m_zoomFactor, 0.0f);glVertex3f(sX, 1.0f,  0.0f);
			glEnd();
		}

		
		{
			//glTranslatef(-2.0f*m_screenX, -2.0f*m_screenY, 0);
		}


		/*
		if (gFrameCount==kPgFramesToMeasure-1)  
		{
		float elapsedTime = gTimer.ElapsedSeconds();
		LOG1("GLMPRRenderer: FPS: %3.6f", ((float)(kPgFramesToMeasure)/elapsedTime));		
		}*/

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
	int GLMPRRenderer<T, U>::initGLthread(const int iVolumeIndex/*=0*/)
	{
		if (m_externalContext)
		{
			if((wglMakeCurrent(m_hdContext, m_glContext)) == NULL)
			{
				LOG0("GLMPRRenderer::wglMakeCurrent Failed");
			}
		} else if (!m_glutInitialized)
		{
			GLRendererBase::initGLthread();
		}
		
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
		}		

		glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// Black Background
		glClearDepth(1.0f);							// Depth Buffer Setup

#ifdef _USE_TEXTURE
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLboolean isIt;
		glGetBooleanv(GL_TEXTURE_2D, &(isIt));
		if (!isIt)
		{
			LOG0("Doesnt support 2D textures...");
		}
#else
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);		
		glClear(GL_COLOR_BUFFER_BIT);
#endif

		return(init(iVolumeIndex));
	}

	template <class T, class U>
	void GLMPRRenderer<T, U>::getGLViewParams (PGMath::Vector3D< float > &oMinVec,
		PGMath::Vector3D< float > &oMaxVec,							
		PGMath::Vector3D< float > &oCameraCenter,
		float &oCameraEye,
		float &oCameraNear,
		float &oCameraFar
		) 
	{	

			oMinVec = PGMath::Vector3D< float >(-((float) m_voxelDims.X() / 2.0f),
				-((float) m_voxelDims.Y() / 2.0f), -((float) m_voxelDims.Z() / 2.0f));

			oMaxVec = PGMath::Vector3D< float >(((float) m_voxelDims.X() / 2.0f),
				((float) m_voxelDims.Y() / 2.0f), ((float) m_voxelDims.Z() / 2.0f));

			// determine center of data
			oCameraCenter = PGMath::Vector3D< float >(0.0f, 0.0f, 0.0f);

			// Why these values are set this way is unclear to me, but it works
			oCameraNear = (float) m_voxelDims.Z() / 5.0f;
			oCameraFar =  m_voxelDims.Z() * 1000.0f;
			oCameraEye = 0.5f * m_voxelDims.Y() * oCameraNear;       		
	};

	

	template <class T, class U>
	inline bool GLMPRRenderer<T, U>::UpdateCB(void	*iDataObject)
		{
			if (m_mprRenderer.size()<2) return false;

			bool res = false;
			int successCount=0;
			char dumpImgPath[256] = {0};
			for (int iRendererIndex=1; iRendererIndex<m_mprRenderer.size(); iRendererIndex++)
			{
				if (!m_mprRenderer[iRendererIndex]) return false;

				bool rv = m_mprRenderer[iRendererIndex]->UpdateCB(iDataObject);
				successCount += (rv ? 1 : 0);

				UpdateRender(iRendererIndex);
				//errr use this code in AffineRegistration
				/*_snprintf(dumpImgPath, 255, "C:/Tmp/Dump/Dir%02d_%01d_%04d.ppm", 
					m_mprRenderer[iRendererIndex]->GetSlicingDirection(), iRendererIndex, m_renderCount);
				dumpImgPath[255] = '\0';
				res = PGAlgs::DumpImageAsPPM(*m_oImageObject, std::string(dumpImgPath));*/			
			}

			//m_renderCount++;

			res = (successCount == m_mprRenderer.size());			

//			UpdateRender(1);
			Render();		

			return res;
		}

		
		
		template <class T, class U>
		inline bool GLMPRRenderer<T, U>::SetTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0)
		{
			LOG2("SetTransferFunctionLUT: %d : %d", iRendererIndex, iLUT);

			if (iRendererIndex < 0 || iRendererIndex > m_mprRenderer.size()-1 || !iLUT)
			{
				return false;
			}

			if (!m_mprRenderer[iRendererIndex]) return false;

			//float lLevel, hiLevel;
			//iLUT->GetWindow(lLevel, hiLevel);			
			//LOG4("SetTransferFunctionLUT: %d : %d : Window: %3.2f %3.2f", iRendererIndex, iLUT, lLevel, hiLevel);

			if (!m_mprRenderer[iRendererIndex]->SetTransferFunctionLUT(iLUT, 0))//iRendererIndex))
				return false;

			return true;//UpdateRender();
		}

	template <class T, class U>
	inline bool GLMPRRenderer<T, U>::SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0)
		{
			if (iRendererIndex < 0 || iRendererIndex > m_mprRenderer.size()-1)
			{
				return false;
			}

			if (!m_mprRenderer[iRendererIndex]) return false;

			if (!m_mprRenderer[iRendererIndex]->SetMaskTransferFunctionLUT(iLUT, 0))//iRendererIndex))
				return false;

			return true;//UpdateRender();
		}

		
		template <class T, class U>
	    inline bool GLMPRRenderer<T, U>::SetSlicerDirection(ePgSlicingDirection iDirection)
		{
			for (int iRendererIndex=0; iRendererIndex<m_mprRenderer.size(); iRendererIndex++)
			{
				if (!m_mprRenderer[iRendererIndex]) return false;

				m_mprRenderer[iRendererIndex]->SetSlicingDirection(iDirection);
			}

			switch (m_mprRenderer[0]->GetSlicingDirection())
			{
			case kPgSlicingDirectionCoronal:
				// XZ plane. draw x, z
				m_colorXAxis[0] = 1; m_colorXAxis[1] = 0; m_colorXAxis[2] = 0; m_colorXAxis[3] = 1;
				m_colorYAxis[0] = 0; m_colorYAxis[1] = 0; m_colorYAxis[2] = 1; m_colorYAxis[3] = 1;
				break;

			case kPgSlicingDirectionSagital:
				// YZ plane. draw y, z
				m_colorXAxis[0] = 0; m_colorXAxis[1] = 1; m_colorXAxis[2] = 0; m_colorXAxis[3] = 1;
				m_colorYAxis[0] = 0; m_colorYAxis[1] = 0; m_colorYAxis[2] = 1; m_colorYAxis[3] = 1;
				break;

			case kPgSlicingDirectionTransverse:
				// XY plane. draw x, y
				m_colorXAxis[0] = 1; m_colorXAxis[1] = 0; m_colorXAxis[2] = 0; m_colorXAxis[3] = 1;
				m_colorYAxis[0] = 0; m_colorYAxis[1] = 1; m_colorYAxis[2] = 0; m_colorYAxis[3] = 1;
				break;

			default: break;

			}

			return true;
		}


		template <class T, class U>
		const PGMath::AffineTransform<float>&	GLMPRRenderer<T, U>::GetViewTransform() const 
		{  
			if (!m_mprRenderer.empty())
			{
				if (!m_mprRenderer[0]) return PGMath::AffineTransform<float>();

				return m_mprRenderer[0]->GetViewTransform(); 
			}			

			return  PGMath::AffineTransform<float>();
		}
		
		template <class T, class U>
		bool GLMPRRenderer<T, U>::SetViewTransform(const PGMath::AffineTransform<float>&	iTransform) 
		{ 
			if (m_mprRenderer.empty()) return false;

			for (int i=0; i<m_mprRenderer.size(); i++)
			{
				if (!m_mprRenderer[i]) return false;

				m_mprRenderer[i]->SetViewTransform(iTransform);				
			}

			return true; 
		}


		
		//iCursor3D has xyz in [-0.5...0.5] range
		template <class T, class U>
		inline bool GLMPRRenderer<T, U>::SetCursorPosition(const PGMath::Point3D<float>& iCursor3D)
		{
			if (m_mprRenderer.empty())
				return false;

			if (!m_mprRenderer[0]) return false;

			// invert view transform
			// invert zoom
			/*
			PGMath::DegreesOfFreedomBase<float> transDoFs;
			transDoFs.SetParameter(kPgDegreeOfFreedomTranslationX, 
			PGMath::AffineTransform<float> invViewTrans(PGMath::Point3D<float>(0, 0, 0));
			invViewTrans.*/
			PGMath::Point3D<float> pCursor3D = iCursor3D;
			//pCursor3D = pCursor3D - m_lastCursor3D;
			//pCursor3D = pCursor3D*(1.0f/m_zoomFactor);
			

			//m_lastCursor3D = iCursor3D;

			// set Y opp to what is passed


			double slicePos = 0.0f;
			switch (m_mprRenderer[0]->GetSlicingDirection())
			{
			case kPgSlicingDirectionCoronal:
				// XZ plane. draw x, z
				m_screenX = pCursor3D.X();
				m_screenY = pCursor3D.Z();
				slicePos =  pCursor3D.Y();
				break;

			case kPgSlicingDirectionSagital:
				// YZ plane. draw y, z
				m_screenX = pCursor3D.Y();
				m_screenY = pCursor3D.Z();
				slicePos =  pCursor3D.X();
				break;

			case kPgSlicingDirectionTransverse:
				// XY plane. draw x, y
				m_screenX = pCursor3D.X();
				m_screenY = -pCursor3D.Y();
				slicePos =  pCursor3D.Z();
				break;

			default: return false;

			}

			return true;
		}

		
		template <class T, class U>
	inline bool GLMPRRenderer<T, U>::SetSkipFactor(int iSkipFactor)
		{
			for (int iRendererIndex=0; iRendererIndex<m_mprRenderer.size(); iRendererIndex++)
			{
				if (!m_mprRenderer[iRendererIndex]) return false;
				
				m_mprRenderer[iRendererIndex]->SetSkipFactor(iSkipFactor);
			}
			return true;
		}

		template <class T, class U>
	inline int GLMPRRenderer<T, U>::GetSkipFactor() const 
		{
			if (m_mprRenderer.empty()) return 0;
			if (!m_mprRenderer[0]) return false;
			return m_mprRenderer[0]->GetSkipFactor();
		}

	template <class T, class U>
	inline bool GLMPRRenderer<T, U>::SetSlicerPosition(double iPosition)
		{
			//LOG0("{ VoxelToMPRRenderer: Begin SetSlicerPosition.");
			for (int iRendererIndex=0; iRendererIndex<m_mprRenderer.size(); iRendererIndex++)
			{
				if (!m_mprRenderer[iRendererIndex]) return false;

				m_mprRenderer[iRendererIndex]->SetSlicerPosition(iPosition);
			}
			//LOG0("} VoxelToMPRRenderer: End SetSlicerPosition.");
			return UpdateRender(-1, 1);//rv;
		}

	/*
		template <class T, class U>
	inline void GLMPRRenderer<T, U>::Render()
		{
			if((wglMakeCurrent(m_hdContext, m_glContext)) == NULL)
			{
				//////MessageBox::Show("wglMakeCurrent Failed");
				return;
			}

			display();
			SwapBuffers(m_hdContext);
		}
*/
		
		
		template <class T, class U>
	inline void GLMPRRenderer<T, U>::Clear() 
		{ 
			int rSize = m_mprRenderer.size();
			for (int i=0; i<rSize; i++)
			{
				if (m_mprRenderer[i]!=0)
				{
					delete m_mprRenderer[i];
					m_mprRenderer[i] = 0;
				}
			}
			m_mprRenderer.clear();

			m_pTextureName.clear();
			m_blendRatio.clear();

			if (m_oImageObject) 
			{ 
				delete m_oImageObject; 
				m_oImageObject=0; 
			}
			
		}

	template <class T, class U>
	inline bool GLMPRRenderer<T, U>::LoadImage(int iBeginIndex, int iEndIndex)
	{
#ifdef _DEBUG
		//LOG0("{ Begin LoadImage");
#endif
		if (m_externalContext)
		{
			if((wglMakeCurrent(m_hdContext, m_glContext)) == NULL)
			{
				////MessageBox::Show("wglMakeCurrent Failed");
				LOG0("GLMPRRenderer::wglMakeCurrent Failed");
				//return false;
			}
		}

		for (int iRendererIndex=iBeginIndex; iRendererIndex<iEndIndex; iRendererIndex++)
		{
			if (!m_mprRenderer[iRendererIndex]) return false;

			m_mprRenderer[iRendererIndex]->Render();
			{

				m_mprRenderer[iRendererIndex]->GetOutput(m_oImageObject);					
#if (_DEBUG_DUMP_IMG)
				{
					char imgPath[255] = {0};
					_snprintf(imgPath, 254, "C:/Tmp/Dump/RenderedImage_%02d_%03d.ppm", iRendererIndex, gDumpedImage);
					imgPath[254] = '\0';						
					DumpImageAsPPM(*m_oImageObject, imgPath);
					gDumpedImage++;
				}
#endif
#ifdef _USE_TEXTURE			
				glBindTexture (GL_TEXTURE_2D, m_pTextureName[iRendererIndex]);					
				glTexImage2D (GL_TEXTURE_2D, 0, 
#if (PG_USE_RGBA16)
					GL_RGBA16,
#else
					GL_RGBA,
#endif
					m_rows, m_columns, 0, GL_RGBA, 
#if (PG_USE_RGBA16)
			GL_UNSIGNED_SHORT,
#else
			GL_UNSIGNED_BYTE,
#endif
					static_cast<GLvoid *>(m_oImageObject->GetBuffer()));							
#endif
			}				
		}		

#ifdef _DEBUG
		//LOG0("} End LoadImage");
#endif
		return true;
	}


	template <class T, class U>
	inline bool GLMPRRenderer<T, U>::UpdateRender(int iIndex=-1, bool iUpdateDisplay=0)
		{
			int iBegin = (iIndex==-1) ? 0 : iIndex;
			int iEnd = (iIndex==-1) ?  m_mprRenderer.size() : iIndex+1;
			bool res = LoadImage(iBegin, iEnd);
			if (!res) return false;

			if (iUpdateDisplay)			
			{
				Render();
			}
			
			if (!m_externalContext)
			{
				bool res = GLRendererBase<T, U>::UpdateRender(iIndex, iUpdateDisplay);
				if (!res) return false;
			}

			return true;;
		}
#ifdef _PG_GENERATE_SDK_LIBS_
	template class GLMPRRenderer<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 		
#endif


};




//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLMPRRenderer_HPP_
