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
// Filename:	GLRendererBase.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLRendererBase_HPP_
#define _PGGLRendererBase_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Algs/GLRendererBase.h"

namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;

	template <class T, class U>
	GLRendererBase<T, U>::GLRendererBase()
	{		
		m_hdContext = (HDC)0;
		m_glContext = (HGLRC)0;		
		m_externalContext = false;		
		m_glutInitialized = false;
		m_glutLoopInitialized = false;
		m_skipFactor = 1;
		m_max3DTextureSize = 0;
		m_remaining3DTextureSize = 0;

		switch(sizeof(U))
		{
		case 4:		m_imageFormat = GL_RGBA;
			m_imageType = GL_UNSIGNED_BYTE;
			break;

		case 1:	

		default:	m_imageFormat = GL_LUMINANCE;
			m_imageType = GL_UNSIGNED_BYTE;
			break;						
		}					
	};


	template <class T, class U>
	bool GLRendererBase<T, U>::SetRotationAngles(const float& iRotX, const float& iRotY, const float& iRotZ)
	{
		gRotation = PGMath::Vector3D<float>(iRotX, iRotY, iRotZ);
		return true;
	}


	template <class T, class U>
	GLRendererBase<T, U>::~GLRendererBase(void) 
	{ 
		Clear(); 

		/*
		try 
		{
		if (m_glContext) wglDeleteContext( m_glContext );		
		} catch (...)
		{
		LOG0("Failed to delete glContext in GLRendererBase<T, U>::Clear");
		}*/
	};			

	template <class T, class U>
	bool GLRendererBase<T, U>::UpdateRender(int iIndex/*=-1*/, bool iUpdateDisplay/*=0*/)
	{
		if (!m_glutLoopInitialized)
		{
			m_glutLoopInitialized = PGAlgs::GLRendererBase<T, U>::StartGlutRenderLoop(this);			
		}

		return m_glutLoopInitialized;
	}

	template <class T, class U>
	bool GLRendererBase<T, U>::SetTranslationUnit(const float& iTransX, const float& iTransY, const float& iTransZ)
	{
		gTranslation = PGMath::Vector3D<float>(iTransX, iTransY, iTransZ);
		return true;
	}

	template <class T, class U>
	bool GLRendererBase<T, U>::PrepareRenderer()
	{
		return true;
	}

	template <class T, class U>
	bool GLRendererBase<T, U>::SetContext(HDC iContext)
	{
		if (m_hdContext!=0 && m_hdContext==iContext) return true;

		m_externalContext = initializeOpenGL(iContext);	
		return true;
	}	

	template <class T, class U>
	void GLRendererBase<T, U>::Render()
	{
		if (m_externalContext)
		{
			if((wglMakeCurrent(m_hdContext, m_glContext)) == NULL)
			{
				return;
			}

			this->display();
			SwapBuffers(m_hdContext);
		} 
		else 
		{		
			this->display();
			glutSwapBuffers();			
		}
	}


	template <class T, class U>
	void  GLRendererBase<T, U>::Clear() 
	{ 
		DeviceRendererBase::Clear();

		/*
		try 
		{
		if (m_glContext) wglDeleteContext( m_glContext );		
		} catch (...)
		{
		LOG0("Failed to delete glContext in GLRendererBase<T, U>::Clear");
		}*/

	}


	template <class T, class U>
	void GLRendererBase<T, U>::StaticRender(void) 
	{ 
		if (PgStaticRenderer)  
		{
			(static_cast<GLRendererBase<T, U> * >(PgStaticRenderer))->Render(); 
		}
	};	


	template <class T, class U>
	long GLRendererBase<T, U>::GetAvailableGPUMemory()
	{
		return m_remaining3DTextureSize;
	}


	template <class T, class U>
	long GLRendererBase<T, U>::GetMaximum3DTextureMemory()
	{
		if (m_externalContext)
		{
			if((wglMakeCurrent(m_hdContext, m_glContext)) == NULL)
			{
				LOG0("GLRendererBase::wglMakeCurrent Failed");
			}
		} else if (!m_glutInitialized)
		{
			GLRendererBase::initGLthread();
		}

		glEnable(GL_TEXTURE_3D);
		
		//3D texture stuff	
		int txSize=0, txSizeActual=0;

		GLint isIt;
		glGetIntegerv(GL_TEXTURE_3D, &(isIt));

		if(isIt)
		{
			glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &(txSizeActual));
			
			txSize = min(_PG_MAX_GL_TX_SIZE_, txSizeActual);

			//cout<<"Max 3D texture size: "<<txSizeActual<<endl;
			LOG2("Max 3D texture size: limited: %d (Actual: %d)", txSize, txSizeActual);	
		}
		else 
		{
			cout <<"3D textures not enabled!"<<endl;	
			LOG0("3D textures not enabled!");
			return 0;
		}

		return txSize;
	}


	template <class T, class U>
	bool GLRendererBase<T, U>::InitGlutWindow(int argc, char *argv[], const int iSx=256, const int iSy=256)
	{
		gTranslationIncrement = iSx;
 
		try
		{
			glutInit(&argc, argv);

			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
			glutInitWindowSize(iSx, iSy); 
			glutInitWindowPosition (100, 100);
			glutCreateWindow (argv[0]);
		} catch (...)
		{
			cout<<"Exception caught in glutInit. "<<endl;
			LOG0("Exception caught in glutInit.");	
		}

		return true;
	}

	template <class T, class U>
	bool GLRendererBase<T, U>::StartGlutRenderLoop(void *iRenderer)
	{
		PgStaticRenderer = iRenderer;
		glutDisplayFunc(&PGAlgs::GLRendererBase<T, U>::StaticRender); 		
		glutIdleFunc(&PGAlgs::GLRendererBase<T, U>::StaticRender);
		glutReshapeFunc(&PGAlgs::GLRendererBase<T, U>::ReshapeCB);
		glutKeyboardFunc(&PGAlgs::GLRendererBase<T, U>::KeyPressCB);
		glutMouseFunc(&PGAlgs::GLRendererBase<T, U>::MouseClickCB);
		glutMotionFunc(&PGAlgs::GLRendererBase<T, U>::MouseMotionCB);
		glutMainLoop();
		return true;
	}

	template <class T, class U>
	void GLRendererBase<T, U>::ReshapeCB(int iNewWidth, int iNewHeight)
	{
		return;
	}

	template <class T, class U>
	void GLRendererBase<T, U>::MouseClickCB(int button, int state, int x, int y )
	{
		if (state == GLUT_DOWN)
		{
			{
				gSparseMode=2;
				switch (button)
				{
				case GLUT_LEFT_BUTTON:
					gLeftDown = 1;
					gXDown = x;
					gYDown = y;
					break;
				case GLUT_MIDDLE_BUTTON:
					gMiddleDown = 1;
					break;
				case GLUT_RIGHT_BUTTON:
					gRightDown = 1;
					break;
				}
			}
			gXTemp = x;
		}
		else if (state == GLUT_UP)
		{      
			gSparseMode=1;
			glutPostRedisplay();
			switch (button)
			{
			case GLUT_LEFT_BUTTON:
				gLeftDown = 0; 
				gXUp = x;
				gYUp = y;
				gXTemp = gXUp;
				gYTemp = gYUp;

				break;
			case GLUT_MIDDLE_BUTTON:
				gMiddleDown = 0;
				break;
			case GLUT_RIGHT_BUTTON:
				gRightDown = 0;
				break;
			}
		}

		if (PgStaticRenderer)  
		{
			GLRendererBase<T, U> *rendererPtr = (static_cast<GLRendererBase<T, U> * >(PgStaticRenderer));

			int sparseFactor = (rendererPtr->GetRendererType()==kPgRendererType3D) ? 2*gSparseMode : gSparseMode;
		
			rendererPtr->SetSkipFactor(sparseFactor); 			

			if (state == GLUT_UP)
			{
				if (rendererPtr->GetRendererType() ==kPgRendererTypeMPR)
				{
					rendererPtr->UpdateRender(-1, 1);
				}
			}
		}

		return;
	}

	template <class T, class U>
	void GLRendererBase<T, U>::MouseMotionCB(int x, int y)
	{
		PGMath::Vector3D<float> gRot(0, 0, 0);
		//static float gSlicerPos = 0.0f;
		float gSlicerPosIncr = 0.01f;

		if (gRightDown ==1) 
		{
			//#define _MTF
#ifdef _MTF		
			if (x > gXTemp) 
			{
				if (gLowerBound<255)
					gLowerBound = (gLowerBound+1);
			}
			else //if (y < gYTemp) 	
			{
				if (gLowerBound>0)
					gLowerBound = (gLowerBound-1);
			}
			loadLookupTable();
#else
			if (abs(y - gYTemp) < abs(x - gXTemp)) 
			{
				if (x > gXTemp) 
				{
					gSlicerPos -= gSlicerPosIncr; gSlicerPos = max(-0.5f, gSlicerPos);

					//gTranslationStatic = PGMath::Vector3D<float>(gTranslationStatic.X() + gTranslationIncrement, gTranslationStatic.Y(), 
					//	gTranslationStatic.Z());
				}
				else	
				{
					gSlicerPos += gSlicerPosIncr; gSlicerPos = min(0.5f, gSlicerPos);

					//gTranslationStatic = PGMath::Vector3D<float>(gTranslationStatic.X() - gTranslationIncrement, gTranslationStatic.Y(), 
					//	gTranslationStatic.Z());			
				}
			} else
			{
				if (y > gYTemp) 
				{
					gSlicerPos -= gSlicerPosIncr; gSlicerPos = max(-0.5f, gSlicerPos);
					//gTranslationStatic = PGMath::Vector3D<float>(gTranslationStatic.X(), gTranslationStatic.Y() + gTranslationIncrement, 
					//	gTranslationStatic.Z());
				}
				else	
				{
					gSlicerPos += gSlicerPosIncr; gSlicerPos = min(0.5f, gSlicerPos);

					//gTranslationStatic = PGMath::Vector3D<float>(gTranslationStatic.X(), gTranslationStatic.Y() - gTranslationIncrement, 
					//	gTranslationStatic.Z());			
				}

			}
			gXTemp = x;		
			gYTemp = y;
#endif
			glutPostRedisplay();

		}
		else if (gLeftDown == 1) 
		{
				if (abs(y - gYTemp) > abs(x - gXTemp)) 
				{
					float ratioXtoY = abs(x - gXTemp)/abs(y - gYTemp);
					ratioXtoY = 1.0f;
					if (y > gYTemp)
					{
						gSlicerPos -= gSlicerPosIncr; gSlicerPos = max(-0.5f, gSlicerPos);

						gRot = PGMath::Vector3D<float>(gRotationIncrement, 0, 0);					
						// gRotationStatic = PGMath::Vector3D<float>(gRotationStatic.X(), gRotationStatic.Y(), 
						// gRotationStatic.Z()- 0.5*ratioXtoY*gRotationIncrement);											
					}
					else
					{
						gSlicerPos += gSlicerPosIncr; gSlicerPos = min(0.5f, gSlicerPos);

						gRot = PGMath::Vector3D<float>(-gRotationIncrement, 0, 0);					
						//gRotationStatic = PGMath::Vector3D<float>(gRotationStatic.X(), gRotationStatic.Y(), 
						//	gRotationStatic.Z() + 0.5*ratioXtoY*gRotationIncrement);											
					}
				}
				else {
					if (x > gXTemp)
					{
						gSlicerPos -= gSlicerPosIncr; gSlicerPos = max(-0.5f, gSlicerPos);
						gRot = PGMath::Vector3D<float>(0, -gRotationIncrement, 0);										
					}
					else
					{
						gSlicerPos += gSlicerPosIncr; gSlicerPos = min(0.5f, gSlicerPos);
						gRot = PGMath::Vector3D<float>(0, gRotationIncrement, 0);											
					}
				}

				glutPostRedisplay();
				gXTemp = x;
				gYTemp = y;
		}  	

		gRotationStatic = gRotationStatic + gRot;

		if (PgStaticRenderer)  
		{
			GLRendererBase<T, U> * rendererPtr = (static_cast<GLRendererBase<T, U> * >(PgStaticRenderer));

			//LOG3("New Translation: %3.2f %3.2f %3.2f", gTranslationStatic.X(), gTranslationStatic.Y(), gTranslationStatic.Z());
			//LOG3("New Rotation: %3.2f %3.2f %3.2f", gRotationStatic.X(), gRotationStatic.Y(), gRotationStatic.Z());

			switch(rendererPtr->GetRendererType())
			{
				case kPgRendererTypeMPR :
					{
					//PGMath::DegreesOfFreedomBase<float> dofs;
					//dofs.SetParameter(3, gRotationStatic.X());
					//dofs.SetParameter(4, gRotationStatic.Y());
					//dofs.SetParameter(5, gRotationStatic.Z());
					//PGMath::AffineTransform<float> newTransform(PGMath::Point3D<float>(0, 0, 0), dofs);
					//rendererPtr->SetViewTransform(newTransform);
										
					//rendererPtr->SetTranslationUnit(gTranslationStatic.X(),
					//	gTranslationStatic.Y(), gTranslationStatic.Z()); 					

					rendererPtr->SetSlicerPosition(gSlicerPos); 
					
					rendererPtr->UpdateRender(-1, 1);

					break;
					};

				case kPgRendererType3D :
					{
					//rendererPtr->SetTranslationUnit(gTranslationStatic.X(),
					//	gTranslationStatic.Y(), gTranslationStatic.Z()); 

					
					rendererPtr->SetRotationAngles(gRotationStatic.X(),
						gRotationStatic.Y(), gRotationStatic.Z()); 

					float slPos = (1.0f-(gSlicerPos+0.5f))-0.5f;
					rendererPtr->SetSlicerPosition(slPos); 

					break;
					};

				default: break;

			}
		}

		return;

	}

	template <class T, class U>
	inline bool GLRendererBase<T, U>::SetSlicerPosition(double iPosition)
	{
		return false;
	}

	/* 
	* Method: KeyPressCB
	* 
	* Input Parameters : key value, x, y
	* Output Parameters:
	* 
	* Comments : keyboard interrupt serving callback, quits the module on 'q' from interactve mode
	*						will be refined soon
	*
	*/   
	template <class T, class U>
	void GLRendererBase<T, U>::KeyPressCB(unsigned char key, int x, int y)
	{
		/*
	if (key == 'q')
	exit(0);
	else if (key == 'r' || key == 'R') {
	gTranslationStatic = PGMath::Vector3D<float>(0.0f, 0.0f, 0.0f);
	gRotationStatic = PGMath::Vector3D<float>(0.0f, 0.0f, 0.0f);		

	glutPostRedisplay();
	} 
	else if (key == '>' || key == '.')
	{

	if (gLowerBound<255)
	gLowerBound = (gLowerBound+10);	

	#ifdef _USE_CG
	loadLookupTable();
	#endif
	glutPostRedisplay();
	}
	else if (key == '<' || key == ',')
	{
	if (gLowerBound>0)
	gLowerBound = (gLowerBound-10);
	#ifdef _USE_CG
	loadLookupTable();
	#endif
	glutPostRedisplay();
	}
	else if (key == '[' || key == '{')
	{
	gSuperSampligFactorZ-=1.0f;
	#ifdef _USE_CG
	loadLookupTable();
	#endif
	glutPostRedisplay();
	}
	else if (key == ']' || key == '}')
	{
	gSuperSampligFactorZ+=1.0f;
	#ifdef _USE_CG
	loadLookupTable();
	#endif
	glutPostRedisplay();
		*/
		return;
	}

	template <class T, class U>
	int GLRendererBase<T, U>::choosePixelFormatEx(HDC hdc,int *p_bpp,int *p_depth,int *p_dbl,int *p_acc)
	{ 
		int wbpp; if (p_bpp==NULL) wbpp=-1; else wbpp=*p_bpp;
		int wdepth; if (p_depth==NULL) wdepth=16; else wdepth=*p_depth;
		int wdbl; if (p_dbl==NULL) wdbl=-1; else wdbl=*p_dbl;
		int wacc; if (p_acc==NULL) wacc=1; else wacc=*p_acc;
		PIXELFORMATDESCRIPTOR pfd; ZeroMemory(&pfd,sizeof(pfd)); pfd.nSize=sizeof(pfd); pfd.nVersion=1;
		int num=DescribePixelFormat(hdc,1,sizeof(pfd),&pfd);
		if (num==0) return 0;
		unsigned int maxqual=0; int maxindex=0;
		int max_bpp, max_depth, max_dbl, max_acc;
		for (int i=1; i<=num; i++)
		{ ZeroMemory(&pfd,sizeof(pfd)); pfd.nSize=sizeof(pfd); pfd.nVersion=1;
		DescribePixelFormat(hdc,i,sizeof(pfd),&pfd);
		int bpp=pfd.cColorBits;
		int depth=pfd.cDepthBits;
		bool pal=(pfd.iPixelType==PFD_TYPE_COLORINDEX);
		bool mcd=((pfd.dwFlags & PFD_GENERIC_FORMAT) && (pfd.dwFlags & PFD_GENERIC_ACCELERATED));
		bool soft=((pfd.dwFlags & PFD_GENERIC_FORMAT) && !(pfd.dwFlags & PFD_GENERIC_ACCELERATED));
		bool icd=(!(pfd.dwFlags & PFD_GENERIC_FORMAT) && !(pfd.dwFlags & PFD_GENERIC_ACCELERATED));
		bool opengl=(pfd.dwFlags & PFD_SUPPORT_OPENGL);
		bool window=(pfd.dwFlags & PFD_DRAW_TO_WINDOW);
		bool bitmap=(pfd.dwFlags & PFD_DRAW_TO_BITMAP);
		bool dbuff=(pfd.dwFlags & PFD_DOUBLEBUFFER);
		//
		unsigned int q=0;
		if (opengl && window) q=q+0x8000;
		if (wdepth==-1 || (wdepth>0 && depth>0)) q=q+0x4000;
		if (wdbl==-1 || (wdbl==0 && !dbuff) || (wdbl==1 && dbuff)) q=q+0x2000;
		if (wacc==-1 || (wacc==0 && soft) || (wacc==1 && (mcd || icd))) q=q+0x1000;
		if (mcd || icd) q=q+0x0040; if (icd) q=q+0x0002;
		if (wbpp==-1 || (wbpp==bpp)) q=q+0x0800;
		if (bpp>=16) q=q+0x0020; if (bpp==16) q=q+0x0008;
		if (wdepth==-1 || (wdepth==depth)) q=q+0x0400;
		if (depth>=16) q=q+0x0010; if (depth==16) q=q+0x0004;
		if (!pal) q=q+0x0080;
		if (bitmap) q=q+0x0001;
		if (q>maxqual) {maxqual=q; maxindex=i;max_bpp=bpp; max_depth=depth; max_dbl=dbuff?1:0; max_acc=soft?0:1;}
		}
		if (maxindex==0) return maxindex;
		if (p_bpp!=NULL) *p_bpp=max_bpp;
		if (p_depth!=NULL) *p_depth=max_depth;
		if (p_dbl!=NULL) *p_dbl=max_dbl;
		if (p_acc!=NULL) *p_acc=max_acc;
		return maxindex;
	}


	template <class T, class U>
	int GLRendererBase<T, U>::initGLthread(const int iVolumeIndex=0)
	{
		int argc = iVolumeIndex;
		char *argv[] = {"SMISDK GLRenderer"};
		int sx = max(256, m_columns);
		int sy = max(256, m_rows);

		m_glutInitialized = false;
		if (!m_externalContext)
		{
			m_glutInitialized = PGAlgs::GLRendererBase<T, U>::InitGlutWindow(argc, argv, sx, sy);
		}

		return m_glutInitialized;
	}


	template <class T, class U>
	bool GLRendererBase<T, U>::initializeOpenGL(HDC hdc)
	{
#ifdef _USE_GLEW
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			LOG1("WARNING: initializeOpenGL: %s", glewGetErrorString(err));	
			//return false;
		}
		LOG1("initializeOpenGL: Status: Using GLEW %s", glewGetString(GLEW_VERSION));

#if (_PG_GLSL_VR_)
		if (glewIsSupported("GL_VERSION_2_0"))
		{
			LOG0("Ready for OpenGL 2.0");
		}
		else 
		{
			LOG0("WARNING: OpenGL 2.0 not supported");
			//return false;
		}
#endif

#endif

		if (m_glContext) wglDeleteContext( m_glContext );

		m_hdContext = hdc;

		PIXELFORMATDESCRIPTOR pfd = 
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0,0,0,0,0,0,
			8,0,
			0,0,0,0,0,
			24,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,
		};

		GLint  iPixelFormat; 

		// get the device context's best, available pixel format match 
		int bpp=-1; // don't care. (or a positive integer)
		int depth=-1; // don't care. (or a positive integer)
		int dbl=1; // we want double-buffering. (or -1 for 'don't care', or 0 for 'none')
		int acc=1; // we want acceleration. (or -1 or 0)
		//GLint iPixelFormat=choosePixelFormatEx(hdc,&bpp,&depth,&dbl,&acc);
		if((iPixelFormat = choosePixelFormatEx(hdc,&bpp,&depth,&dbl,&acc)) == 0)
		{
			return 0;
		}


		// make that match the device context's current pixel format 
		if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
		{

			return 0;
		}

		if((m_glContext = wglCreateContext(m_hdContext)) == NULL)
		{
			DWORD le = GetLastError();		
			return 0;
		}

		return true;
	} 



	template <class T, class U>
	int GLRendererBase<T, U>::logLastGlError()
	{
		GLenum retVal = glGetError();
		int rVal = 0;

		switch (retVal)
		{
		case GL_NO_ERROR:
			LOG0("GLRendererBase::logLastGlError:No error has been recorded.");// The value of this symbolic constant is guaranteed to be zero.
			break;

		case GL_INVALID_ENUM:
			LOG0("GLRendererBase::logLastGlError:An unacceptable value is specified for an enumerated argument.");// The offending command is ignored, having no side effect other than to set the error flag.
			rVal = -1;
			break;

		case GL_INVALID_VALUE:
			LOG0("GLRendererBase::logLastGlError:A numeric argument is out of range.");// The offending command is ignored, having no side effect other than to set the error flag.
			rVal = -1;
			break;

		case GL_INVALID_OPERATION:
			LOG0("GLRendererBase::logLastGlError:The specified operation is not allowed in the current state.");// The offending command is ignored, having no side effect other than to set the error flag.
			rVal = -1;
			break;

		case GL_STACK_OVERFLOW:
			LOG0("GLRendererBase::logLastGlError:This command would cause a stack overflow.");// The offending command is ignored, having no side effect other than to set the error flag.
			rVal = -1;
			break;

		case GL_STACK_UNDERFLOW:
			LOG0("GLRendererBase::logLastGlError:This command would cause a stack underflow.");// The offending command is ignored, having no side effect other than to set the error flag.
			rVal = -1;
			break;

		case GL_OUT_OF_MEMORY:
			LOG0("GLRendererBase::logLastGlError:Out of memory");
			rVal = -1;
			break;

		default: 
			LOG0("logLastGlError:Cant interpret error value.");
			rVal = -1;
			break;
		}
		return rVal;
	};


#ifdef _PG_GENERATE_SDK_LIBS_
	template class GLRendererBase<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 	
#endif


};




//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLRendererBase_HPP_
