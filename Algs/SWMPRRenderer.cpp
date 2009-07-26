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
// Filename:	SWMPRRenderer.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGSWMPRRenderer_HPP_
#define _PGSWMPRRenderer_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/SWMPRRenderer.h"


namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;

	static const char *outdataPath = "C:/Data/";

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
    SWMPRRenderer<T, U>::SWMPRRenderer() 
	{		
		m_renderCount = 0;
	};

	template <class T, class U>
    SWMPRRenderer<T, U>::~SWMPRRenderer() 
	{		
		Clear();
	};

	template <class T, class U>
	inline bool SWMPRRenderer<T, U>::PrepareRenderer()
	{
		if (!m_iDataObject || (m_rows*m_columns)<=0) 
		{
			LOG0("SWMPRRenderer: Error: no input");
			return false;
		}		

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) {
			LOG0("SWMPRRenderer:initVolumes Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount();
		if (iMultiVolumeCount < 1)
		{
			LOG0("SWMPRRenderer:initVolumes Error: Needs at least one volume");
			return false;
		}

		Clear();

		int i=0;
		for (i=0; i<iMultiVolumeCount; i++)
		{

			m_blendRatio.push_back(float(1.0f/float(iMultiVolumeCount)));
		}

		for (i=0; i<iMultiVolumeCount; i++)
		{
			// get the input volume

			PGCore::Voxel3D< T > *inVolume = (static_cast<PGCore::Voxel3D < T > *>(inMultiVolume->GetDataObject(i)));
			if (!inVolume) {
				LOG1("SWMPRRenderer: Error: failure to fetch input volume %d", i);
				return false;
			}

			PGMath::Point3D<float> pDimensions;
			pDimensions = inVolume->GetDimensions();
			if (pDimensions.Z()<=0) {
				LOG1("SWMPRRenderer: Error: empty input volume %d", i);
				return false;
			}

			// ok query 		
			if (m_oImageObject==0)
			{
				//int iRows = (int)(pDimensions.Y()), iColumns = (int)(pDimensions.X());
				m_oImageObject = new PGCore::Image<PGMath::Point3D<U>>(m_rows, m_columns);
				m_oImageBuf = m_oImageObject->GetBuffer();				

				long ir = m_rows, ic = m_columns;
				m_tempImageObject.SetDimensions(ir, ic);


#if (_DEBUG_DUMP_IMG)
				//PGCore::PixelBase<U> tPixel;
				//U rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
				//m_checkImage.GetDataRange(&rangeMin, &rangeMax);	
				//DumpImageAsPPM(m_checkImage, "C:/Tmp/CheckImage.ppm");
#endif
			}			

			unsigned int rRows = m_rows, rCols = m_columns;
			PGAlgs::VoxelToMPRRenderer<T, U> *mprRenderer = new PGAlgs::VoxelToMPRRenderer<T, U>;
			if (!mprRenderer) return false;
			mprRenderer->SetInput(static_cast<PGCore::BaseDataObject *>(inVolume));//m_iDataObject);
			mprRenderer->SetImageSize(rRows, rCols);
			mprRenderer->PrepareRenderer();
			m_mprRenderer.push_back(mprRenderer);

		}

		return true;
		//return SetSlicerPositionAndDirection(iPosition, iDirection);
	}


	template <class T, class U>
	bool SWMPRRenderer<T, U>::ImageSizeUpdated()
	{
		if (m_mprRenderer.empty()) return false;

		// reallocate img buffer here
		// ok query 		
		if (m_oImageObject)
		{
			delete m_oImageObject;
			m_oImageObject = 0;
		}


		{

			m_oImageObject = new PGCore::Image<PGMath::Point3D<U>>(m_rows, m_columns);
			m_oImageBuf = m_oImageObject->GetBuffer();				

			long ir = m_rows, ic = m_columns;
			m_tempImageObject.SetDimensions(ir, ic);


#if (_DEBUG_DUMP_IMG)
			//PGCore::PixelBase<U> tPixel;
			//U rangeMin = tPixel.GetMaxValue(), rangeMax = tPixel.GetMinValue();
			//m_checkImage.GetDataRange(&rangeMin, &rangeMax);	
			//DumpImageAsPPM(m_checkImage, "C:/Tmp/CheckImage.ppm");
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
	inline bool SWMPRRenderer<T, U>::Execute()
	{	
		bool rv = PrepareRenderer();
		if (!rv)
			return false;
		return rv;
	}


	template <class T, class U>
	void SWMPRRenderer<T, U>::Render()
	{
		this->display();		
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
	int SWMPRRenderer<T, U>::init(const int iVolumeIndex/*=0*/)// int    argc, char **argv )
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
	void SWMPRRenderer<T, U>::display()
	{		
		return;
	}


	template <class T, class U>
	inline bool SWMPRRenderer<T, U>::UpdateCB(void	*iDataObject)
	{
		if (m_mprRenderer.size()<2) return false;

		bool res = false;
		int successCount=0;
		char dumpImgPath[256] = {0};
		for (int iRendererIndex=1; iRendererIndex<m_mprRenderer.size(); iRendererIndex++)
		{
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
	inline bool SWMPRRenderer<T, U>::SetTransferFunctionLUT(PGCore::TransferFunctionLUT<unsigned char> *iLUT, const int iRendererIndex=0)
	{
		if (iRendererIndex < 0 || iRendererIndex > m_mprRenderer.size()-1)
		{
			return false;
		}

		if (!m_mprRenderer[iRendererIndex]->SetTransferFunctionLUT(iLUT, 0))//iRendererIndex))
			return false;

		return true;//UpdateRender();
	}

	template <class T, class U>
	inline bool SWMPRRenderer<T, U>::SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<unsigned char> *iLUT, const int iRendererIndex=0)
	{
		if (iRendererIndex < 0 || iRendererIndex > m_mprRenderer.size()-1)
		{
			return false;
		}

		if (!m_mprRenderer[iRendererIndex]->SetMaskTransferFunctionLUT(iLUT, 0))//iRendererIndex))
			return false;

		return true;//UpdateRender();
	}


	template <class T, class U>
	inline bool SWMPRRenderer<T, U>::SetSlicerPositionAndDirection(double iPosition, ePgSlicingDirection iDirection)
	{
		for (int iRendererIndex=0; iRendererIndex<m_mprRenderer.size(); iRendererIndex++)
		{
			m_mprRenderer[iRendererIndex]->SetSlicingDirection(iDirection);
		}

		return SetSlicerPosition(iPosition);
	}


	//iCursor3D has xyz in [-0.5...0.5] range
	template <class T, class U>
	inline bool SWMPRRenderer<T, U>::SetCursorPosition(const PGMath::Point3D<float>& iCursor3D)
	{
		if (m_mprRenderer.empty())
			return false;

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
			m_screenY = pCursor3D.Y();
			slicePos =  pCursor3D.Z();
			break;

		default: return false;

		}

		return true;
	}


	template <class T, class U>
	inline bool SWMPRRenderer<T, U>::SetSkipFactor(int iSkipFactor)
	{
		for (int iRendererIndex=0; iRendererIndex<m_mprRenderer.size(); iRendererIndex++)
		{
			m_mprRenderer[iRendererIndex]->SetSkipFactor(iSkipFactor);
		}
		return true;
	}

	template <class T, class U>
	inline int SWMPRRenderer<T, U>::GetSkipFactor() const 
	{
		if (m_mprRenderer.empty()) return 0;
		return m_mprRenderer[0]->GetSkipFactor();
	}

	template <class T, class U>
	inline bool SWMPRRenderer<T, U>::SetSlicerPosition(double iPosition)
	{
		//LOG0("{ VoxelToMPRRenderer: Begin SetSlicerPosition.");
		for (int iRendererIndex=0; iRendererIndex<m_mprRenderer.size(); iRendererIndex++)
		{
			m_mprRenderer[iRendererIndex]->SetSlicerPosition(iPosition/m_zoomFactor);
		}
		//LOG0("} VoxelToMPRRenderer: End SetSlicerPosition.");
		return UpdateRender(-1, 1);//rv;
	}



	template <class T, class U>
	inline void SWMPRRenderer<T, U>::Clear() 
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


		m_blendRatio.clear();

		if (m_oImageObject) 
		{ 
			delete m_oImageObject; 
			m_oImageObject=0; 
		}

	}


	template <class T, class U>
	inline bool SWMPRRenderer<T, U>::UpdateRender(int iIndex=-1, bool iUpdateDisplay=0)
	{
		//LOG0("Begin UpdateRender.");
		int iBegin = (iIndex==-1) ? 0 : iIndex;
		int iEnd = (iIndex==-1) ?  m_mprRenderer.size() : iIndex+1;			

		for (int iRendererIndex=iBegin; iRendererIndex<iEnd; iRendererIndex++)
		{
			//LOG0("Begin render..");
			m_mprRenderer[iRendererIndex]->Render();
			{
				//LOG0("Begin GetOutput..");			

				m_mprRenderer[iRendererIndex]->GetOutput(&m_tempImageObject);	
				//LOG0("End GetOutput..");
				//#undef _DEBUG_DUMP_IMG
#if (_DEBUG_DUMP_IMG)
				if (1)//gDumpedImage%8==0)
				{
					char imgPath[255] = {0};
					_snprintf(imgPath, 254, "C:/Tmp/Dump/RenderedImage_%02d_%03d.ppm", iRendererIndex, gDumpedImage);
					imgPath[254] = '\0';						
					DumpImageAsPPM(m_tempImageObject, imgPath);
					gDumpedImage++;
				}
#endif

				//m_tempImageObject.FlipVertical();
/*
#if (_DEBUG_DUMP_IMG)
				if (1)//gDumpedImage%8==0)
				{
					char imgPath[255] = {0};
					_snprintf(imgPath, 254, "C:/Tmp/Dump/RenderedImageFlipped_%02d_%03d.ppm", iRendererIndex, gDumpedImage);
					imgPath[254] = '\0';						
					DumpImageAsPPM(m_tempImageObject, imgPath);
					gDumpedImage++;
				}
#endif
*/

				//blend with the image
				//*m_oImageObject = m_tempImageObject;
				m_oImageObject->Blend(m_tempImageObject, m_blendRatio[iRendererIndex], *m_oImageObject); 
			}				
		}		

		m_oImageObject->FlipVertical();

		m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));
		////use this way to iupdate display
		//LOG0("Begin iUpdateDisplay..");
		if (iUpdateDisplay)			
		{
			display();				
		}
		//LOG0("End iUpdateDisplay..");

		//LOG0("End UpdateRender.");
		return true;;
	}


#ifdef _PG_GENERATE_SDK_LIBS_
	template class SWMPRRenderer<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 		
#endif


};




//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGSWMPRRenderer_HPP_
