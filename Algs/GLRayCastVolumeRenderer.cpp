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
// Filename:	GLRayCastVolumeRenderer.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLRayCastVolumeRenderer_HPP_
#define _PGGLRayCastVolumeRenderer_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/GLRayCastVolumeRenderer.h"


namespace PGAlgs
{	
	
	template <class T, class U>
	GLRayCastVolumeRenderer<T, U>::GLRayCastVolumeRenderer() 
	{
		m_textureFormat=GL_LUMINANCE;
		m_rawFormat=GL_LUMINANCE;
		m_numChannels=2;

		m_textureFormat=GL_LUMINANCE;
		m_rawFormat=GL_LUMINANCE;

		switch(m_numChannels)
		{
		case 1: m_textureFormat=GL_LUMINANCE;
			m_rawFormat=GL_LUMINANCE;
			break;

		case 2: m_textureFormat=GL_LUMINANCE8_ALPHA8;
			m_rawFormat=GL_LUMINANCE_ALPHA;
			break;

		case 4: 
			//m_textureFormat=GL_RGBA;
			//m_textureFormat=GL_RGBA32UI_EXT;
#if (PG_USE_RGBA16)
			m_textureFormat=GL_RGBA16;
#else
			m_textureFormat=GL_RGBA;
#endif

#ifdef _HIGH_BW
			m_rawFormat=GL_BGRA;
#else
			m_rawFormat=GL_RGBA;
#endif
			break;

		default:  m_textureFormat=GL_LUMINANCE8_ALPHA8;
			m_rawFormat=GL_LUMINANCE_ALPHA;
			break;
		}

		gDrSparse = 1.0f; gDrFull = 1.0f;  
		gDz = 1.0f;//, gDzFull = 1.0f, gDzSparse = 8.0f; 	

		gLUTName=0; 
		
		//gTextureName.clear();
		//gTexScale.clear();
		//gXTexTrans.clear(); gYTexTrans.clear(); gZTexTrans.clear();
		//gXTexRatio.clear(); gYTexRatio.clear(); gZTexRatio.clear();		
	};

	template <class T, class U>
	GLRayCastVolumeRenderer<T, U>::~GLRayCastVolumeRenderer(void) 
	{ 
#ifdef _USE_CG
		disableFragmentShader();
#endif
		m_voxelChunk[0].Clear();
		m_voxelChunk[1].Clear();
	};


	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::SetInput(PGCore::BaseDataObject *iDataObject)
	{
		bool rv = GLVolumeRenderer::SetInput(iDataObject);
		if (!rv) return false;

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) 
		{
			LOG0("GLRayCastVolumeRenderer:SetInput Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount(kPgDataObjectTypeIsotropicVoxel3D);
		if (iMultiVolumeCount < 1)
		{
			LOG0("GLRayCastVolumeRenderer:SetInput Error: Needs at least one volume");
			return false;
		}

		if (gTextureName.size() < iMultiVolumeCount)
		{
			gTextureName.resize(iMultiVolumeCount);
			gXTexTrans.resize(iMultiVolumeCount); 
			gYTexTrans.resize(iMultiVolumeCount); 
			gZTexTrans.resize(iMultiVolumeCount);
			gXTexRatio.resize(iMultiVolumeCount); 
			gYTexRatio.resize(iMultiVolumeCount);	
			gZTexRatio.resize(iMultiVolumeCount);
			gTexScale.resize(iMultiVolumeCount);
		}

		return true;		
	};
	
	
 
	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::PrepareRenderer()
	{
		if (m_externalContext)
		{
			if (m_hdContext == (HDC)0 || m_glContext == (HGLRC)0)
			{
				LOG0("GLRayCastVolumeRenderer: Error: no context");
				return false;
			}
		}

		if (!m_iDataObject || (m_rows*m_columns)<=0) 
		{
			LOG0("GLRayCastVolumeRenderer: Error: no input");
			return false;
		}		

		PGCore::MultiDataBase<T> *inMultiVolume = (static_cast<PGCore::MultiDataBase<T> *>(m_iDataObject));
		if (!inMultiVolume) {
			LOG0("GLRayCastVolumeRenderer:initVolumes Error: failure to fetch input multivolume");
			return false;
		}

		int iMultiVolumeCount = inMultiVolume->GetDataCount(kPgDataObjectTypeIsotropicVoxel3D);
		if (iMultiVolumeCount < 1)
		{
			LOG0("GLRayCastVolumeRenderer:initVolumes Error: Needs at least one volume");
			return false;
		}	

		int iMultiDataCount   = inMultiVolume->GetDataCount();
		for (int i=0; i<iMultiDataCount; i++)
		{
			if (inMultiVolume->GetDataObject(i)->Type() != kPgDataObjectTypeIsotropicVoxel3D) continue;

			// get the input volume
			PGCore::Voxel3D< T > *inVolume = (static_cast<PGCore::Voxel3D < T > *>(inMultiVolume->GetDataObject(i)));
			if (!inVolume) 
			{
				LOG1("GLRayCastVolumeRenderer: Error: failure to fetch input volume %d", i);
				return false;
			}

			m_voxel3DList[i] = inVolume;

			PGMath::Point3D<float> pDimensions;
			pDimensions = inVolume->GetDimensions();
			if (pDimensions.Z()<=0) 
			{
				LOG1("GLRayCastVolumeRenderer: Error: empty input volume %d", i);
				return false;
			}

			PGCore::Volume< T > *volumeAccessor = inVolume->GetVolumeAccessor();

			if (!volumeAccessor) 
			{
				LOG0("GLRayCastVolumeRenderer: Error: failure to fetch input volume accessor");
				return false;
			}

			m_volumeAccessor[i] = volumeAccessor;
			m_volumeAccessor[i]->GetDimensions(m_voxelDims[i]);	

			gVolumeScope[i] = m_voxelDims[i];

			clampToNextHigherNumber(m_voxelDims[i]);

			LOG3("GLRayCastVolumeRenderer: Dimensions (padded): %d / %d / %d", m_voxelDims[i][0], m_voxelDims[i][1], m_voxelDims[i][2]);

			int maxVoxelDim = max(m_voxelDims[i].X(), m_voxelDims[i].Y());
			maxVoxelDim = max(maxVoxelDim, m_voxelDims[i].Z());

			if (!initGLthread())
			{
				LOG0("GLRayCastVolumeRenderer: Error: failed to initialize GL machine.");
				return false;
			}						

			long totalTxSize =  _PG_MAX_GL_TX_XY_SIZE_*_PG_MAX_GL_TX_XY_SIZE_*m_max3DTextureSize;

			LOG2("GLRayCastVolumeRenderer: Maximum dimension: %d / %d", maxVoxelDim, m_max3DTextureSize);
			long totalVxSize =  m_voxelDims[i][0] * m_voxelDims[i][1] * m_voxelDims[i][2];
			
			PGCore::MetaData< T > iMetaData = inVolume->GetMetaData();
			gTexScale[i] = iMetaData.GetSpacing();	
			float txScaleZ = gTexScale[i].Z();
			
			if (totalVxSize > totalTxSize)
			{	
				LOG3("WARNING: GLRayCastVolumeRenderer: Too big for total texture: %d / %d. Will reduce Z resolution by %d.", totalVxSize, totalTxSize, m_voxelDimsSkipZ[i]);

				m_voxelDimsSkipZ[i]=0;						
				int m_volDimZBck = m_voxelDims[i][2];
				float txScaleZBck = txScaleZ;

				while (totalVxSize > totalTxSize)
				{
					m_voxelDimsSkipZ[i]+=2;
					m_voxelDims[i][2] = m_volDimZBck/m_voxelDimsSkipZ[i];
					txScaleZ = txScaleZBck*float(m_voxelDimsSkipZ[i]);					
					totalVxSize = m_voxelDims[i][0] * m_voxelDims[i][1] * m_voxelDims[i][2];
				}				

				gVolumeScope[i] = PGMath::Point3D<long>(gVolumeScope[i].X(), gVolumeScope[i].Y(), gVolumeScope[i].Z()/m_voxelDimsSkipZ[i]); 
				gTexScale[i] = PGMath::Vector3D<float>(gTexScale[i].X(), gTexScale[i].Y(), txScaleZ);

				
				//m_ready = false;
				//return false;
			} else
			{
				m_voxelDimsSkipZ[i] = 1;
			}

			m_ready = true;			

			LOG2("GLRayCastVolumeRenderer: Needed texture size : %d, remaining: %d", totalVxSize, m_remaining3DTextureSize);			
			if (!m_voxelChunk[i].Alloc(totalVxSize*m_numChannels*
#if (PG_USE_RGBA16)
				sizeof(short)
#else
				sizeof(char)
#endif
				))
			{
				LOG0("GLRayCastVolumeRenderer: Error: failure to allocate voxel memory");
				return false;
			}		

			m_remaining3DTextureSize-=totalVxSize; 

			std::vector<float> spacingVec, sortedSpacingVec;
			spacingVec.push_back(gTexScale[i].X() * m_voxelDims[i][0]);//gVolumeScope[0]);
			spacingVec.push_back(gTexScale[i].Y() * m_voxelDims[i][1]);//gVolumeScope[1]);
			spacingVec.push_back(gTexScale[i].Z() * m_voxelDims[i][2]);//gVolumeScope[2]);
			sortedSpacingVec = spacingVec;
			std::sort(sortedSpacingVec.begin(), sortedSpacingVec.end());	
			gTexScale[i] = PGMath::Vector3D<float>(sortedSpacingVec[2]/spacingVec[0], sortedSpacingVec[2]/spacingVec[1], 
				sortedSpacingVec[2]/spacingVec[2]);		

			LOG4("GLRayCastVolumeRenderer: Relative scaling[%d] : %3.2f %3.2f %3.2f", i, gTexScale[i].X(), gTexScale[i].Y(), gTexScale[i].Z());
			
			gXTexRatio[i] = 1.0f; gYTexRatio[i] = 1.0f; gZTexRatio[i] = 1.0f;		
			gXTexTrans[i] = 0.5f; gYTexTrans[i] = 0.5f; gZTexTrans[i] = 0.5f; 		

			if(!init(i)) 
			{
				LOG1("GLRayCastVolumeRenderer: Error: failure to init for volume %d", i);
				return false;    
			}			

			if (!m_LuT[i]) continue;

			

			glEnable(GL_TEXTURE_3D);
			glEnable(GL_TEXTURE_2D);	
			
			glBindTexture(GL_TEXTURE_3D, gTextureName[i]);

#ifdef _USE_CG
						
			if (!loadVolume(i) || !loadLookupTable())
			{				
				LOG0("GLRayCastVolumeRenderer: Error: failure to loadVolume por lookup table");
				return false; 
			}

		
			enableFragmentShader();		
			
			cgGLLoadProgram(gCgInfo.fragmentProgram);
			cgGLEnableProfile(gCgInfo.fragmentProfile);
			cgGLBindProgram(gCgInfo.fragmentProgram);

			cgGLSetTextureParameter (gCgInfo.cgTex3d, gTextureName[i]);
			cgGLSetTextureParameter (gCgInfo.cgTexColormap, gLUTName);					
			
			cgGLEnableTextureParameter (gCgInfo.cgTex3d);	
			cgGLEnableTextureParameter (gCgInfo.cgTexColormap);
#endif

			
			
		}

		return GLRendererBase::PrepareRenderer();		
	}

	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::Execute() 
	{		
		return false;
	}

	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::reloadLUT()
	{
		LOG0("{ GLRayCastVolumeRenderer::reloadLUT"); 

		int rv2d = initLUTTex();
		if (!rv2d)
		{
			LOG0("} ERROR: GLRayCastVolumeRenderer::reloadLUT"); 
			return false;
		}
		
		bool rv = false;
#ifdef _USE_CG
			
			rv = loadLookupTable();
			if (!rv)
			{				
				LOG0("GLRayCastVolumeRenderer: Error: failure to loadVolume por lookup table");
				return false; 
			}
		
			enableFragmentShader();		
			
			cgGLLoadProgram(gCgInfo.fragmentProgram);
			cgGLEnableProfile(gCgInfo.fragmentProfile);
			cgGLBindProgram(gCgInfo.fragmentProgram);

			cgGLDisableTextureParameter (gCgInfo.cgTex3d);	
			cgGLDisableTextureParameter (gCgInfo.cgTexColormap);

			cgGLSetTextureParameter (gCgInfo.cgTex3d, gTextureName[0]);
			cgGLSetTextureParameter (gCgInfo.cgTexColormap, gLUTName);					
			
			cgGLEnableTextureParameter (gCgInfo.cgTex3d);	
			cgGLEnableTextureParameter (gCgInfo.cgTexColormap);
#endif

		LOG0("} GLRayCastVolumeRenderer::reloadLUT"); 

		return rv;
	}

	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::loadLookupTable()
	{
		LOG2("{ GLRayCastVolumeRenderer::loadLookupTable: %d, %d", gLowerBound, gUpperBound);

		if (!m_ready) 
		{
			LOG0("Error }  GLRayCastVolumeRenderer::loadLookupTable");
			return false;
		}

		float wLow = float(gLowerBound)/255.0f;
		float wHi  = float(gUpperBound)/255.0f;

		m_LuT[0]->SetWindow(wLow, wHi);

		Point3D<U>* m_LuTBuf = m_LuT[0]->GetBuffer();
		if (!m_LuTBuf)
		{
			LOG0("GLRayCastVolumeRenderer:loadLookupTable Error: failure to fetch LUT buffer");		
			return false;			
		}


		//createGauss1DFilter (tv->focusWindowSize);  
		{
			int j=0, i=0;    
			unsigned char colormap[256][256][4],
				subcolormap[256][4]; 
			unsigned char tval=0;
			if (gSuperSampligFactorZ<0.0001f) gSuperSampligFactorZ = 1.0f;
			float tfvalmaxMain = log(256.0f-(float)gLowerBound), tfvalmax = log((float)(gLowerBound-gLowerEndNoiseBound));
			float tmainMultiplier = 128.0f/gSuperSampligFactorZ, tMultiplier = 2.0f/gSuperSampligFactorZ;
			{
				for (j = 0; j < 256; j++) 
				{					
					for (int k=0; k<4; k++)
					{
						subcolormap[j][k] = m_LuTBuf[j][k];								
					}										
					
					if (j>=gLowerBound && j<=gUpperBound)
					{
						subcolormap[j][3] = subcolormap[j][3]<<1;
					} else
					{
						subcolormap[j][3] = subcolormap[j][3]>>1;
					}

					/*
					tval = 0;
					if (j>gLowerBound && j<255)
					{
						float tfval = (log((float)(j-gLowerBound+1))/tfvalmaxMain);					
						tval = (unsigned char)((unsigned int)(tfval*tmainMultiplier));					
						subcolormap[j][0] = (unsigned char)((unsigned int)((tfval*253.0f)));;
						subcolormap[j][1] = (unsigned char)((unsigned int)((tfval*200.0f)));;
						subcolormap[j][2] = (unsigned char)((unsigned int)((tfval*150.0f)));;

					} else if (j<gLowerBound && j>gLowerEndNoiseBound)
					{
						float tfval = tMultiplier*(log((float)(j-gLowerEndNoiseBound+1))/tfvalmax);	
						tval = (unsigned char)((unsigned int)(tfval));			

						tfval = 75.0f + 180.0f*((float)(j-gLowerEndNoiseBound+1)/(float)(gLowerBound-gLowerEndNoiseBound+1));	
						subcolormap[j][0] = (unsigned char)((unsigned int)(tfval));			//(unsigned char)((unsigned int)((tfval*255.0f)));;
						subcolormap[j][1] = (unsigned char)((unsigned int)(tfval));			//(unsigned char)((unsigned int)((tfval*255.0f)));;
						subcolormap[j][2] = (unsigned char)((unsigned int)(tfval));			;//(unsigned char)((unsigned int)((tfval*255.0f)));;
					}						

					subcolormap[j][3] = tval;
					
						
					if (j>=gLowerBound && j<=gUpperBound)
					{
						for (int k=0; k<4; k++)
						{
							subcolormap[j][k] = m_LuTBuf[j][k];								
						}										
						subcolormap[j][3] = subcolormap[j][3]>>2;
					} else
					{
						for (int k=0; k<4; k++)
						{
							subcolormap[j][k] = 32;
						}							
						subcolormap[j][3] = subcolormap[j][3]>>4;
					}
					*/

				}
			}       

			for (i=0; i<256; i++)
			{
				memcpy(&(colormap[i][0][0]), &(subcolormap[0][0]), 256*4*sizeof(unsigned char));
			}

			glEnable(GL_TEXTURE_2D);
		    glBindTexture(GL_TEXTURE_2D, gLUTName);

			glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8,
				256, 256, 0, 
#ifdef _HIGH_BW
				GL_BGRA
#else
				GL_RGBA, 
#endif
				GL_UNSIGNED_BYTE,
				&(colormap[0][0][0]));

		}

		logLastGlError();
		//CheckGLErrors ();

		LOG0("} GLRayCastVolumeRenderer::loadLookupTable");

		return true;
	}


	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::loadVolume(const int iVolumeIndex/*=-1*/)
	{
		if (iVolumeIndex!=-1 && iVolumeIndex>m_volumeAccessor.size()-1)
			return false;

		int highBW = 0;
#ifdef _HIGH_BW
		highBW=1;
#endif

		// this is where to fuse the volumes ? 
		int iBeginIndex = iVolumeIndex==-1 ? 0 : iVolumeIndex;
		int iEndIndex = iVolumeIndex==-1 ? (m_volumeAccessor.size()-1) : iVolumeIndex;

		//int vIndex = iVolumeIndex==-1 ? m_volumeAccessor.size()-1 : iVolumeIndex;

		bool rv = false;		
		for (int vIndex=iBeginIndex; vIndex<=iEndIndex; vIndex++)
		{
			//LOG2("{ GLRayCastVolumeRenderer<T, U>::loadVolume(%d): HighBW mode: %d", vIndex, highBW);
			bool maskInit = m_volumeAccessor[vIndex]->GetBitVolume().IsInitialized();
			bool rv0 = maskInit ?  loadVolumeWithMask(vIndex) : loadVolumeWithoutMask(vIndex);    
			rv = rv0||rv;
			//LOG3("} GLRayCastVolumeRenderer<T, U>::loadVolume(%d): HighBW mode: %d, Success: %d", vIndex, highBW, rv0 ? 1 : 0);
		}
		
		return rv;
	}


	// use pixel buffer objects here
	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::loadVolumeWithMask(const int iVolumeIndex/*=0*/)
	{
		if (!m_ready) return false;

		if (iVolumeIndex > m_volumeAccessor.size()-1) return false; 

		if (iVolumeIndex > m_voxel3DList.size()-1) return false; 
		
#if (_TEST_SCENE)
		return true;
#endif

		//cout<<"Starting to initialize the texture..."<<endl;
		LOG0("{ GLRayCastVolumeRenderer::loadVolumeWithMask");

		// this is where you pass in the volume

		//long iSize = m_max3DTextureSize * m_max3DTextureSize * m_max3DTextureSize;//m_voxelDims.X() * m_voxelDims.Y() * m_voxelDims.Z() * sizeof(T);
		//unsigned char *voxelArray = (unsigned char *)calloc(iSize, m_numChannels*sizeof(char));
		//ObjectBase voxelChunk;
		//LOG0("{ GLRayCastVolumeRenderer::begin init");
#if (PG_USE_RGBA16)
		unsigned short *voxelArray = (unsigned short *)(m_voxelChunk[iVolumeIndex].GetBuffer());//voxelChunk.Alloc(iSize*m_numChannels*sizeof(char));
		unsigned int alphaOffsetVisible=0, alphaOffsetHidden=6;
#else
		unsigned char *voxelArray = (unsigned char *)(m_voxelChunk[iVolumeIndex].GetBuffer());//voxelChunk.Alloc(iSize*m_numChannels*sizeof(char));
		unsigned int alphaOffsetVisible=0, alphaOffsetHidden=6;
#endif
		if (!voxelArray)
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch voxelArray");
			return 0;
		}

		int superSamplingPow = int(log(gSuperSampligFactorZ)/log(2.0f) + 0.5f);
		alphaOffsetVisible+= superSamplingPow;
		alphaOffsetVisible = min(4, alphaOffsetVisible);
		alphaOffsetVisible = max(7, alphaOffsetVisible);
		LOG2("GLRayCastVolumeRenderer: loadVolumeWithMask: SuperSamplingFac: %3.2f, superSamplingPow: %d", gSuperSampligFactorZ, superSamplingPow);


		if (!m_volumeAccessor[iVolumeIndex]) 
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch input volume accessor");
			return 0;
		}

		bool usedDefaultLUT = false;
		PGCore::TransferFunctionLUT<U> defaultLuT;
		defaultLuT.SetType((PGCore::ePgLUTType)0);
		if (!m_LuT[iVolumeIndex])
		{
			LOG0("GLRayCastVolumeRenderer: Warning: failure to fetch LUT. Will use default...");
			m_LuT[iVolumeIndex] = &defaultLuT;
			usedDefaultLUT = true;
			//return 0;			
		}

		bool usedDefaultMaskLUT = false;
		PGCore::TransferFunctionLUT<U> defaultMaskLuT;
		defaultMaskLuT.SetType((PGCore::ePgLUTType)2);
		if (!m_MaskLUT[iVolumeIndex])
		{
			LOG0("GLRayCastVolumeRenderer: Warning: failure to fetch mask LUT. Will use default...");
			m_MaskLUT[iVolumeIndex] = &defaultMaskLuT;
			usedDefaultMaskLUT = true;
			//return 0;			
		}

		//LOG0("} GLRayCastVolumeRenderer::end init");

		//LOG0("{ GLRayCastVolumeRenderer::begin apply LUT");

		// copy LUT to local 
		Point3D<U>* m_LuTBuf = m_LuT[iVolumeIndex]->GetBuffer();
		if (!m_LuTBuf)
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch LUT buffer");
			if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;
			if (usedDefaultMaskLUT) m_LuTBuf[iVolumeIndex] = 0;

			return 0;			
		}

		Point3D<U>* m_MaskLUTBuf = m_MaskLUT[iVolumeIndex]->GetBuffer();
		if (!m_MaskLUTBuf)
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch Mask LUT buffer");
			if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;
			if (usedDefaultMaskLUT) m_LuTBuf[iVolumeIndex] = 0;
			return 0;			
		}		

		Point3D<U> nextLUTValue;
		int lutDim = m_LuT[iVolumeIndex]->GetDimension();

		long pOffset=0, sizeXY = m_voxelDims[iVolumeIndex][0]*m_voxelDims[iVolumeIndex][1];
		PGMath::Point3D<long> iDims;
		m_volumeAccessor[iVolumeIndex]->GetDimensions(iDims);
#if (PG_USE_RGBA16)
		PGCore::Image<short>		
#else
		PGCore::Image<char> 
#endif
		tImage(m_numChannels*m_voxelDims[iVolumeIndex][1], m_voxelDims[iVolumeIndex][0]);

		float monNoiseTh = 0.5f;
		// get modality
		PGCore::ePGModality modality = m_voxel3DList[iVolumeIndex]->GetMetaData().GetModality();
		switch (modality)
		{
			case kPGModalityCT	:	monNoiseTh = kPgMinThNoiseInMask[0]; break;
			case kPGModalityMR	:	monNoiseTh = kPgMinThNoiseInMask[1]; break;
			case kPGModalityPT	:	monNoiseTh = kPgMinThNoiseInMask[2]; break;
			default: break;
		}

		T rangeMin, rangeMax;
		m_volumeAccessor[iVolumeIndex]->GetDataRange(&rangeMin, &rangeMax);	
		T range = (rangeMax-rangeMin);

		T minThForNoise = rangeMin + T(float(range*monNoiseTh) + 0.5f);

		float alphaFactor = (m_blendRatio[iVolumeIndex]);

		if (range<=0) 
		{
			LOG0("GLRayCastVolumeRenderer: Error: zero range");
			if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;
			if (usedDefaultMaskLUT) m_LuTBuf[iVolumeIndex] = 0;
			return false;
		}
		float rangeInv = ((float)lutDim)/((float)range);

		int gSliceSize = m_voxelDims[iVolumeIndex][0]*m_voxelDims[iVolumeIndex][1]*m_numChannels
#if (PG_USE_RGBA16)
			*2
#endif
			;
		int iSliceSize = m_numChannels*sizeXY;

		// top empty slice
		{
			memset(voxelArray, 0,  gSliceSize);	
			pOffset+=iSliceSize; 
		}


		register int zSkip = (m_skipFactor==1) ? 1 : 1;

		int totalZ = m_voxelDims[iVolumeIndex][2];

		//for (int z=1; z<m_voxelDims.Z()-1; z++) {
		//for (int z=1; z<(m_voxelDims[iVolumeIndex][2]-1); z+=m_voxelDimsSkipZ[iVolumeIndex], pOffset+=iSliceSize) 
		for (int z=1; 
			z<(m_voxelDims[iVolumeIndex][2]-1); 
			z++, pOffset+=zSkip*iSliceSize) 
		{
#if (PG_USE_RGBA16)
			short* oBuffer = (short *)tImage.GetBuffer();//m_volumeAccessor->GetImage( z ).GetBuffer();
#else
			char* oBuffer = (char *)tImage.GetBuffer();//m_volumeAccessor->GetImage( z ).GetBuffer();
#endif
			if (!oBuffer) 
			{
				continue;
			}

			int zOrg = z*(m_voxelDimsSkipZ[iVolumeIndex]*zSkip);
			if (zOrg>0 && zOrg<iDims.Z()) 			
			{
				// fetch image and add to the voxelarray		

				T* iBuffer = m_volumeAccessor[iVolumeIndex]->GetImage( zOrg ).GetBuffer();
				if (!iBuffer) 
				{
					continue;
				}

				//		memset(oBuffer, 
				//			1, //((z%4)==0 ? 1 : 0), 
				//			 m_voxelDims.X() * m_voxelDims.Y() * sizeof(T));
				if (1)//threshold)// && z%2==0) 
				{	
					//#define _DUMMY_DATA
#ifdef _DUMMY_DATA
					threshold = 255;
					rangeMax = 255;
					rangeMin = 0;
#endif

					int yOffset = 0, yBufOffset=0;//y*m_max3DTextureSize;
#if (PG_USE_RGBA16)
					unsigned short* pValue = (unsigned short* )(oBuffer);
#else
					unsigned char* pValue = (unsigned char* )(oBuffer);
#endif
					for (int y=0; y<m_voxelDims[iVolumeIndex][1]; y++, yOffset+=m_max3DTextureSize, yBufOffset+=m_voxelDims[iVolumeIndex][0]) 
					{

						for (int x=0; x<m_voxelDims[iVolumeIndex][0]; x++, pValue+=m_numChannels) 
						{

#ifdef _DUMMY_DATA
							T val = (y%2==0 && x%2==0) ? 255 : 0;//iBuffer[y*m_voxelDims.X() + x];
#else
							T val = iBuffer[yBufOffset + x];
#endif
							//unsigned char* pValue = (unsigned char* )(oBuffer);
							//pValue+= m_numChannels*(yOffset + x);
#if (PG_USE_RGBA16)
							unsigned short* pAlpha = pValue + m_numChannels-1;
#else
							unsigned char* pAlpha = pValue + m_numChannels-1;
#endif
							unsigned int oValue = (unsigned int)(rangeInv*( (float)(val-rangeMin) ));

							//unsigned char cValue = m_LuTBuf[oValue].X();

							//*pValue = (cValue);//< m_low || oValue>m_high) ? 0 : ((float)(oValue - m_low))*(transRangeInv);							


							//use the mask LUT here



							float alphaVal = 0;
							int j=0;

							Point3D<U>* xferLuT = m_LuTBuf;


							{			
								int inMaskVal	=	m_volumeAccessor[iVolumeIndex]->GetBitVolume().GetValue(y, x, zOrg);
								if (inMaskVal)
								{
									xferLuT = m_LuTBuf;											
									for (j=0; j<m_numChannels-1; j++)
									{
										*(pValue+j) = xferLuT[oValue][j];								
									}
									alphaVal = (xferLuT[oValue][j])<<alphaOffsetVisible;
									alphaVal = alphaVal*alphaFactor;
								}
								else
								{
									xferLuT = m_MaskLUTBuf;			
									int lutIndex = oValue>minThForNoise ? oValue :  0;
									for (j=0; j<m_numChannels-1; j++)
									{
										*(pValue+j) = xferLuT[lutIndex][j];								
									}									
									alphaVal = xferLuT[lutIndex][j]>>alphaOffsetHidden;									
								}
							} 




							//alphaVal /= (m_numChannels-1);


							//alphaVal = alphaVal > 255 ? 255 : alphaVal;
							//alphaVal = alphaVal < 0 ? 0 : alphaVal;


							//if (m_numChannels>1)
							{

								//#define _USE_GRADIENT 					
#ifdef _USE_GRADIENT
								PGMath::Vector3D<float> gradVec = gradient(x, y, zOrg);
								if ( gradVec.Length()> 0.1f && (oValue >  130))
								{
									float gradVal = gradVec.Length()-0.1f;
									//*pAlpha = (unsigned char)floor(gradVal*255.0f);
									*pValue = (unsigned char)floor(gradVal*255.0f);
								}
#else							
								
								//if (m_numChannels>1)
								{
									
									*pAlpha = 
#if (PG_USE_RGBA16)
										(unsigned short)
#else
										(unsigned char)
#endif	
										(alphaVal); // m_LuTBuf[oValue][j]

									//minTransValue = minTransValue > oValue ? oValue : minTransValue;
									//maxTransValue = maxTransValue < oValue ? oValue : maxTransValue;
								}
								

#endif
							}

						}
					}
				} else 
				{
					memset(oBuffer, 0, gSliceSize);
				}			
			} else 
			{
				memset(oBuffer, 0, gSliceSize);
			}
			//pOffset = z*sizeXY;
			memcpy(voxelArray+pOffset, oBuffer,  gSliceSize);		

			int progressValue=(int)(70.0f*((float)(z)/(float)totalZ) + 0.5f);				
			UpdateProgress(progressValue);
		}

		// last empty slice
		{
			//memset(voxelArray+(m_voxelDims[iVolumeIndex][2]-1)*iSliceSize, 0,  gSliceSize);	
		}

		//	glTexImage3D  ( target , level , internalformat , 
		//		width , height , depth , border , format , type , pixels )

		//LOG0("} GLRayCastVolumeRenderer::end apply LUT");


		//LOG0("{ GLRayCastVolumeRenderer::begin text init");
		if (m_externalContext)
		{
			if((wglMakeCurrent(m_hdContext, m_glContext)) == NULL)
			{
				////MessageBox::Show("wglMakeCurrent Failed");
				LOG0("GLRayCastVolumeRenderer::wglMakeCurrent Failed");
				//return false;
			}
		}

		/*
		//These calls would not require unnecessary
		swizzling
		glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8 ,
		img_width, img_height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
		img_data);
		glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA ,
		img_width, img_height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
		img_data);
		glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0,
		GL_FLOAT_RGBA16_NV , img_width, img_height, 0, GL_RGBA,
		GL_HALF_FLOAT_NV, img_data);
		glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_FLOAT_RGBA_NV,
		img_width, img_height, 0, GL_RGBA, GL_FLOAT_NV, img_data);
		*/

		glEnable(GL_TEXTURE_3D);
		glBindTexture(GL_TEXTURE_3D, gTextureName[iVolumeIndex]);
		glTexImage3D(GL_TEXTURE_3D, 
			0, 
			m_textureFormat,
			m_voxelDims[iVolumeIndex][0], m_voxelDims[iVolumeIndex][1], m_voxelDims[iVolumeIndex][2], 
			0, 
			m_rawFormat, 
#if (PG_USE_RGBA16)
			GL_UNSIGNED_SHORT,
#else
			GL_UNSIGNED_BYTE,
#endif
			(GLvoid *)(voxelArray));  				

		int rVal = logLastGlError();
		if (rVal<0)
		{
			if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;
			if (usedDefaultMaskLUT) m_MaskLUT[iVolumeIndex] = 0;
			return 0;
		}

		//LOG0("} GLRayCastVolumeRenderer::end text init");

		//cout<<"Texture initialized..."<<endl;	
		//LOG2("} GLRayCastVolumeRenderer::loadVolume: [%d -> %d]", minTransValue, maxTransValue);

		if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;
		if (usedDefaultMaskLUT) m_MaskLUT[iVolumeIndex] = 0;

		UpdateProgress(100);

		LOG0("} GLRayCastVolumeRenderer::loadVolumeWithMask");
		return 1;
	}


	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::loadVolumeWithoutMask(const int iVolumeIndex/*=0*/)
	{	

		if (!m_ready) return false;

		if (iVolumeIndex > m_volumeAccessor.size()-1) return false; 

#if (_TEST_SCENE)
		return true;
#endif

		//cout<<"Starting to initialize the texture..."<<endl;
		LOG0("{ GLRayCastVolumeRenderer::loadVolumeWithoutMask");

		// this is where you pass in the volume

		//long iSize = m_max3DTextureSize * m_max3DTextureSize * m_max3DTextureSize;//m_voxelDims.X() * m_voxelDims.Y() * m_voxelDims.Z() * sizeof(T);
		//unsigned char *voxelArray = (unsigned char *)calloc(iSize, m_numChannels*sizeof(char));
		//ObjectBase voxelChunk;
		//LOG0("{ GLRayCastVolumeRenderer::begin init");
#if (PG_USE_RGBA16)
		unsigned short *voxelArray = (unsigned short *)(m_voxelChunk[iVolumeIndex].GetBuffer());//voxelChunk.Alloc(iSize*m_numChannels*sizeof(char));
		unsigned int alphaOffsetVisible=2;
#else
		unsigned char *voxelArray = (unsigned char *)(m_voxelChunk[iVolumeIndex].GetBuffer());//voxelChunk.Alloc(iSize*m_numChannels*sizeof(char));
		unsigned int alphaOffsetVisible=0;
#endif
		if (!voxelArray)
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch voxelArray");
			return 0;
		}

		int superSamplingPow = int(log(gSuperSampligFactorZ)/log(2.0f) + 0.5f);
		alphaOffsetVisible+= superSamplingPow;		
		alphaOffsetVisible = min(4, alphaOffsetVisible);
		alphaOffsetVisible = max(7, alphaOffsetVisible);
		LOG2("GLRayCastVolumeRenderer: loadVolumeWithoutMask: SuperSamplingFac: %3.2f, superSamplingPow: %d", gSuperSampligFactorZ, superSamplingPow);

		if (!m_volumeAccessor[iVolumeIndex]) 
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch input volume accessor");
			return 0;
		}

		bool usedDefaultLUT = false;
		PGCore::TransferFunctionLUT<U> defaultLuT;
		defaultLuT.SetType((PGCore::ePgLUTType)kPgLUTTypeGrayScale);
		if (!m_LuT[iVolumeIndex])
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch LUT");
			m_LuT[iVolumeIndex] = &defaultLuT;
			//usedDefaultLUT = true;
			//return 0;			
		}

		//LOG0("} GLRayCastVolumeRenderer::end init");

		//LOG0("{ GLRayCastVolumeRenderer::begin apply LUT");

		// copy LUT to local 
		Point3D<U>* m_LuTBuf = m_LuT[iVolumeIndex]->GetBuffer();
		if (!m_LuTBuf)
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch LUT buffer");
			if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;			
			return 0;			
		}

		/*
		Point3D<U>* m_MaskLUTBuf = m_MaskLUT[iVolumeIndex]->GetBuffer();
		if (!m_MaskLUTBuf)
		{
			LOG0("GLRayCastVolumeRenderer: Error: failure to fetch Mask LUT buffer");
			return 0;			
		}		
		*/

		Point3D<U> nextLUTValue;
		int lutDim = m_LuT[iVolumeIndex]->GetDimension();
		//std::vector<unsigned char> oneDim_LuT;//(lutDim);, 255);

		/*
		int minTransValue=-1, maxTransValue=-1;
		for (int i=0; i<lutDim; i++)
		{
		nextLUTValue = m_LuTBuf[i];
		//oneDim_LuT.push_back(nextLUTValue.X());

		minTransValue = (minTransValue==-1 && nextLUTValue.X()>0) ? i : minTransValue;

		maxTransValue = (minTransValue!=-1 && maxTransValue==-1 && nextLUTValue.X()==0) ? i : maxTransValue;

		}

		maxTransValue = maxTransValue==-1 ? (lutDim-1) : maxTransValue;
		*/

		long pOffset=0, sizeXY = m_voxelDims[iVolumeIndex][0]*m_voxelDims[iVolumeIndex][1], sizeXYZ = sizeXY*m_voxelDims[iVolumeIndex][2];		
		
		PGMath::Point3D<long> iDims;
		m_volumeAccessor[iVolumeIndex]->GetDimensions(iDims);
#if (PG_USE_RGBA16)
		PGCore::Image<short>
#else
		PGCore::Image<char>
#endif
		tImage(m_numChannels*m_voxelDims[iVolumeIndex][1], m_voxelDims[iVolumeIndex][0]);


		T rangeMin, rangeMax;
		m_volumeAccessor[iVolumeIndex]->GetDataRange(&rangeMin, &rangeMax);	
		T range = (rangeMax-rangeMin);

		float alphaFactor = (m_blendRatio[iVolumeIndex]);

		

		if (range<=0) 
		{
			LOG0("GLRayCastVolumeRenderer: Error: zero range");
			if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;			
			return false;
		}
		float rangeInv = ((float)lutDim)/((float)range);
		//float transRangeInv = (255.0f)/((float)(m_high - m_low));

		int gSliceSize = m_voxelDims[iVolumeIndex][0]*m_voxelDims[iVolumeIndex][1]*m_numChannels
#if (PG_USE_RGBA16)
			*2
#endif
			;
		int iSliceSize = m_numChannels*sizeXY;

		// top empty slice
		{
			memset(voxelArray, 0,  gSliceSize);	
			pOffset+=iSliceSize; 
		}


		register int zSkip = (m_skipFactor==1) ? 1 : 1;
		//register int alphaOff = (m_skipFactor==1) ? 1 : 1;

		int totalZ = m_voxelDims[iVolumeIndex][2];

		//for (int z=1; z<m_voxelDims.Z()-1; z++) {
		for (int z=1; 
			z<(m_voxelDims[iVolumeIndex][2]-1); 
			z++, pOffset+=zSkip*iSliceSize) 
		{
#if (PG_USE_RGBA16)
			short* oBuffer = (short *)tImage.GetBuffer();//m_volumeAccessor->GetImage( z ).GetBuffer();
#else
			char* oBuffer = (char *)tImage.GetBuffer();//m_volumeAccessor->GetImage( z ).GetBuffer();
#endif
			if (!oBuffer) 
			{
				continue;
			}
			int maxVal=-1;
		    float maxAlpha=-1.0f;

			int zOrg = z*(m_voxelDimsSkipZ[iVolumeIndex]*zSkip);
			if (zOrg>0 && zOrg<iDims.Z()) 
			{
				// fetch image and add to the voxelarray		

				T* iBuffer = m_volumeAccessor[iVolumeIndex]->GetImage( zOrg ).GetBuffer();
				if (!iBuffer) 
				{
					continue;
				}

				//		memset(oBuffer, 
				//			1, //((zOrg%4)==0 ? 1 : 0), 
				//			 m_voxelDims.X() * m_voxelDims.Y() * sizeof(T));
				if (1)//threshold)// && zOrg%2==0) 
				{	
					//#define _DUMMY_DATA
#ifdef _DUMMY_DATA
					threshold = 255;
					rangeMax = 255;
					rangeMin = 0;
#endif

					int yOffset = 0, yBufOffset=0;//y*m_max3DTextureSize;

#if (PG_USE_RGBA16)
					unsigned short* pValue = (unsigned short* )(oBuffer);
#else
					unsigned char* pValue = (unsigned char* )(oBuffer);
#endif
					
					for (int y=0; y<m_voxelDims[iVolumeIndex][1]; y++, yOffset+=m_max3DTextureSize, yBufOffset+=m_voxelDims[iVolumeIndex][0]) 
					{

						for (int x=0; x<m_voxelDims[iVolumeIndex][0]; x++, pValue+=m_numChannels) 
						{

#ifdef _DUMMY_DATA
							T val = (y%2==0 && x%2==0) ? 255 : 0;//iBuffer[y*m_voxelDims.X() + x];
#else
							T val = iBuffer[yBufOffset + x];
#endif
							//unsigned char* pValue = (unsigned char* )(oBuffer);
							//pValue+= m_numChannels*(yOffset + x);							

#if (PG_USE_RGBA16)
							unsigned short* pAlpha = pValue + m_numChannels-1;
#else
							unsigned char* pAlpha = pValue + m_numChannels-1;
#endif
							unsigned int oValue = (unsigned int)(rangeInv*( (float)(val-rangeMin) ));

							//unsigned char cValue = m_LuTBuf[oValue].X();

							//*pValue = (cValue);//< m_low || oValue>m_high) ? 0 : ((float)(oValue - m_low))*(transRangeInv);							


							//use the mask LUT here



							float alphaVal = 0;
							int j=0;

#ifndef _USE_CG
							Point3D<U>* xferLuT = m_LuTBuf;
							{
								xferLuT = m_LuTBuf;		
								for (j=0; j<m_numChannels-1; j++)
								{
									*(pValue+j) = xferLuT[oValue][j];								

								}
								alphaVal = xferLuT[oValue][j]<<alphaOffsetVisible;
								//alphaVal = alphaVal> >1;
							}
#else
							for (j=0; j<m_numChannels-1; j++)
							{
								*(pValue+j) = oValue;								
							}
							alphaVal = oValue;
#endif

							//alphaVal /= (m_numChannels-1);


							//alphaVal = alphaVal > 255 ? 255 : alphaVal;
							//alphaVal = alphaVal < 0 ? 0 : alphaVal;


							//if (m_numChannels>1)
							{

								//#define _USE_GRADIENT 					
#ifdef _USE_GRADIENT
								PGMath::Vector3D<float> gradVec = gradient(x, y, zOrg);
								if ( gradVec.Length()> 0.1f && (oValue >  130))
								{
									float gradVal = gradVec.Length()-0.1f;
									//*pAlpha = (unsigned char)floor(gradVal*255.0f);
									*pValue = (unsigned char)floor(gradVal*255.0f);
								}
#else							
								//maxVal=max(maxVal, xferLuT[oValue][0]+xferLuT[oValue][1]+xferLuT[oValue][2]);
								//maxAlpha=max(maxAlpha, alphaVal*alphaFactor);

								//if (m_numChannels>1)
								{
									 

									*pAlpha = 
#if (PG_USE_RGBA16)
										(unsigned short)
#else
										(unsigned char)
#endif

#ifndef _USE_CG
										(alphaVal*alphaFactor); // m_LuTBuf[oValue][j]
#else
										oValue;
#endif

									//minTransValue = minTransValue > oValue ? oValue : minTransValue;
									//maxTransValue = maxTransValue < oValue ? oValue : maxTransValue;
								}
#endif
							}

						}
					}
				} else 
				{
					memset(oBuffer, 0, gSliceSize);
				}			
			} else 
			{
				memset(oBuffer, 0, gSliceSize);
			}
			//pOffset = z*sizeXY;

			//if (pOffset+gSliceSize<sizeXYZ) 
			{
				memcpy(voxelArray+pOffset, oBuffer,  gSliceSize);		
				//LOG4("\tTex[Z:%d, Offset: %d]: Max Val: %d, maxAlpha: %3.2f!\n", z, pOffset, maxVal, maxAlpha); 
			} 

			int progressValue=(int)(70.0f*((float)(z)/(float)totalZ) + 0.5f);				
			UpdateProgress(progressValue);
		}

		// last empty slice
		{
			//memset(voxelArray+(m_voxelDims[iVolumeIndex][2]-1)*iSliceSize, 0,  gSliceSize);	
		}

		LOG2("GLRayCastVolumeRenderer:: pOffset+gSliceSize: %d, sizeXYZ: %d\n", pOffset+gSliceSize, sizeXYZ*m_numChannels); 


		//	glTexImage3D  ( target , level , internalformat , 
		//		width , height , depth , border , format , type , pixels )

		//LOG0("} GLRayCastVolumeRenderer::end apply LUT");


		//LOG0("{ GLRayCastVolumeRenderer::begin text init");
		if (m_externalContext)
		{
			if((wglMakeCurrent(m_hdContext, m_glContext)) == NULL)
			{
				////MessageBox::Show("wglMakeCurrent Failed");
				LOG0("GLRayCastVolumeRenderer::wglMakeCurrent Failed");
				//return false;
			}
		}


		glEnable(GL_TEXTURE_3D);
		glBindTexture(GL_TEXTURE_3D, gTextureName[iVolumeIndex]);
		glTexImage3D(GL_TEXTURE_3D, 
			0, 
			m_textureFormat,
			m_voxelDims[iVolumeIndex][0], m_voxelDims[iVolumeIndex][1], m_voxelDims[iVolumeIndex][2]/(zSkip), 
			0, 
			m_rawFormat, 
#if (PG_USE_RGBA16)
			GL_UNSIGNED_SHORT,
#else
			GL_UNSIGNED_BYTE,
#endif
			(GLvoid *)(voxelArray));  				

		int rVal = logLastGlError();
		if (rVal<0)
		{
			if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;			
			return 0;
		}		
		//LOG0("} GLRayCastVolumeRenderer::end text init");		

		if (usedDefaultLUT) m_LuT[iVolumeIndex] = 0;			

		UpdateProgress(100);

		//cout<<"Texture initialized..."<<endl;	
		//LOG2("} GLRayCastVolumeRenderer::loadVolume: [%d -> %d]", minTransValue, maxTransValue);
		LOG4("} GLRayCastVolumeRenderer::loadVolumeWithoutMask: Uploaded Vol (%d) with Dims [%d, %d, %d]", voxelArray, m_voxelDims[iVolumeIndex][0], m_voxelDims[iVolumeIndex][1], m_voxelDims[iVolumeIndex][2]/(zSkip));
		return 1;
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
	int GLRayCastVolumeRenderer<T, U>::init3DTex(const int iVolumeIndex=0)
	{
		if (iVolumeIndex>gTextureName.size()-1) return 0;

		getGLViewParams(gMinVec, gMaxVec, gCameraCenter, gCameraEyeZ, gCameraNear, gCameraFar, iVolumeIndex);

		// init params
		// m_voxelDims : power of 2, texture dimemsions
		// gVolumeScope: actual volume dimensions 
		{
			gZTexTrans[iVolumeIndex] = 0.5f*(1.0 - gVolumeScope[iVolumeIndex].Z()/((float)m_voxelDims[iVolumeIndex][2]));
			gYTexTrans[iVolumeIndex] = 0.5f*(1.0 - gVolumeScope[iVolumeIndex].Y()/((float)m_voxelDims[iVolumeIndex][1]));
			gXTexTrans[iVolumeIndex] = 0.5f*(1.0 - gVolumeScope[iVolumeIndex].X()/((float)m_voxelDims[iVolumeIndex][0]));
			//gDrSparse = 1.0f/gVolumeScope.Z(), gDrFull = 1.0/gVolumeScope.Z();
			gDrSparse = 1.0f/m_voxelDims[iVolumeIndex][2], gDrFull = 1.0/m_voxelDims[iVolumeIndex][2];
			gDz = 0.25f/((float)m_voxelDims[iVolumeIndex][2]/*m_max3DTextureSize*/);  
			gDzFull = gDz/(gSuperSampligFactorZ); 	
			gDzSparse = 2.0f*gDzFull; 	
			gDrSparse = gDzSparse;
			gDrFull = gDzFull;
		}

		// delete old one
		//int gSize = gTextureName.size();
		GLuint oldTex = gTextureName[iVolumeIndex];
		glDeleteTextures(1, &(oldTex)); 
		GLuint newTex=0;
		//gTextureName.resize(gSize);
		//for (int i=0; i<gSize; i++)
		{
			glMatrixMode(GL_TEXTURE);
			glEnable(GL_TEXTURE_3D);

			glGenTextures(1, &(newTex));
			gTextureName[iVolumeIndex] = newTex;

			int rVal = logLastGlError();
			if (rVal<0)
			{
				return 0;
			}

			LOG2("<%d>: 3D Texture: %d", this, gTextureName[iVolumeIndex]);

			GLfloat bColor[4] = { 0, 0, 0, 0 };

			glBindTexture(GL_TEXTURE_3D, gTextureName[iVolumeIndex]);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_REPLACE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, bColor);
		}		

		return 1;	
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
	int GLRayCastVolumeRenderer<T, U>::initLUTTex(const int iVolumeIndex=0)
	{
		if (iVolumeIndex>gTextureName.size()-1) return 0;
		
		// delete old one
		glDeleteTextures(1, &gLUTName); 
		{
			glGenTextures (1, &gLUTName);

			int rVal = logLastGlError();
			if (rVal<0)
			{
				return 0;
			}

			LOG2("<%d>: LUT Texture: %d", this, gLUTName);

			glBindTexture (GL_TEXTURE_2D, gLUTName);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}   

		return 1;	
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
	int GLRayCastVolumeRenderer<T, U>::init(const int iVolumeIndex/*=0*/)
	{
		int rv3d = init3DTex(iVolumeIndex);
		int rv2d = initLUTTex(iVolumeIndex);

		return (rv3d && rv2d);		
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
	void GLRayCastVolumeRenderer<T, U>::display()
	{
		if (!m_ready) return;

		/*
		if (gFrameCount==0)  
		{
		gTimer.Reset();
		}
		gFrameCount = (gFrameCount+1)%kPgFramesToMeasure;
		*/


#if (_TEST_SCENE)
		glClearColor( 1.0, 0.0, 0.0, 1.0 );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//***************************
		// DRAWING CODE
		//***************************

		//cout << "Rot: (" << gRotation.X() << "," << gRotation.Y() <<", "<< gRotation.Z() << endl;

		//glTranslatef(0.0f,0.0f,-5.0f);
		//glRotatef(0,0.0f,1.0f,0.0f);

		//glRotatef(gRotation.X(), 1, 0, 0);
		glRotatef(gRotation.Y(), 0, 1, 0);
		//glRotatef(gRotation.Z(), 0, 0, 1);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,-1.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,0.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glEnd();

		/*	glBegin(GL_QUADS);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-1.1f,-1.1,0.0f);
		glVertex3f(1.1f,-1.1f,0.0f);
		glVertex3f(1.1f,1.1f,0.0f);
		glVertex3f(-1.1f,1.1f,0.0f);
		glEnd();*/	
#else

		glClearColor( 0.51, 0.55, 0.60, 1.0 );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION); 
		glLoadIdentity(); 
		//gluPerspective(100.0, 1.0, 3, 10.0); 
		glFrustum(-0.5/m_zoomFactor, 0.5/m_zoomFactor, -0.5/m_zoomFactor, 0.5/m_zoomFactor, 4, 10);
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 
		gluLookAt(0.0, 0.0, 7, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

		//glMatrixMode(GL_MODELVIEW);

		//glLoadIdentity();
		//glFrustum(-3*m_zoomFactor, 3*m_zoomFactor, -3*zoomFm_zoomFactoractor, 3*m_zoomFactor, 10, 100);

		//gluLookAt (0.0, 0.0, 10 + m_zoomFactor, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		//	gluLookAt(gCameraCenter.X(), gCameraCenter.Y(), 
		//		//gCameraEyeZ, 
		//		2,
		//		gCameraCenter.X(), gCameraCenter.Y(), 
		//		0.0f,
		//		//-gVolumeScope.Z(), 
		//		0, 1, 0);	
		//	//glPushMatrix();


		//glRotatef(gRotation.X(), gRotation.Y(), gRotation.Z());
		glTranslatef(gTranslation[0].X(), gTranslation[0].Y(), gTranslation[0].Z());

		////glTranslatef(-0.5f, -0.5f, -0.5f);
		//glRotatef(gRotation.X(), 1, 0, 0);
		//glRotatef(gRotation.Y(), 0, 1, 0);
		//	glRotatef(gRotation.Z(), 0, 0, 1);
		//glTranslatef(0.5f, 0.5f, 0.5f);

		renderVolume();

		renderAxisPlanes();

		//glPopMatrix();
#endif

		//glutSwapBuffers();	

		/*
		if (gFrameCount==kPgFramesToMeasure-1)  
		{
		float elapsedTime = gTimer.ElapsedSeconds();
		LOG1("GLRayCastVolumeRenderer: FPS: %3.6f", ((float)(kPgFramesToMeasure)/elapsedTime));		
		//printf("GLRayCastVolumeRenderer: FPS: %3.3lf!", (1000.0f/elapsedTime));		
		}*/

	}

	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::UpdateRender(int iIndex=-1, bool iUpdateDisplay=0)
	{
		if (!m_ready) return false;
	
		if (GetAsyncEnabled())
		{
			return updateRenderAsync();
		}

		return updateRenderSync();			
	};	

	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::updateRenderSync(int iIndex=-1, bool iUpdateDisplay=0)
	{
		bool res = loadVolume(iIndex); 
		if (!res) return false;

		Render();

		if (!m_externalContext)
		{
			bool res = GLRendererBase<T, U>::UpdateRender(iIndex, iUpdateDisplay);
			if (!res) return false;
		}

		return true;
	}

	template <class T, class U>
	bool GLRayCastVolumeRenderer<T, U>::updateRenderAsync(int iIndex=-1, bool iUpdateDisplay=0)
	{
		LOG0("{ updateRenderAsync");

		if (m_voxel3DList.empty()) return false;

		SetMaxThreadCount(1);

		bool rv = false;
		char szPath[MAX_PATH]; 
		DWORD dwBufferSize = MAX_PATH;
		GetTempPath( dwBufferSize, szPath );	
		std::string pStr = std::string(szPath) + kPgApplicationName + "\\";

		// fill params and send to thread
		std::string fName = pStr + std::string("UpdateVR") + std::string(".txt");
		
		m_threadLogger.SetLogFile(fName.c_str());

		// data
		m_updateVRParams.m_pIVolume = m_voxel3DList[0];
		m_updateVRParams.m_MaskLUT = m_MaskLUT[0];
		m_updateVRParams.m_LuT = m_LuT[0];
		m_updateVRParams.m_blendRatio = m_blendRatio[0];
		m_updateVRParams.m_hdContext = m_hdContext;
		m_updateVRParams.m_rendererIndex = iIndex;
		m_updateVRParams.m_updateDisplay = iUpdateDisplay;
		m_updateVRParams.m_rows = m_rows;
		m_updateVRParams.m_columns = m_columns;



		// base params settings
		m_updateVRParams.statusCB = GetProgressCB();
		m_updateVRParams.ioLogger = &(m_threadLogger);			
		m_updateVRParams.iParentThreadPtr = GetAsyncEnabled() ? (void *)this : 0;
		
		// set params
		if (!CreateNewThread((ThreadedProcess *)this, (void *) (&(m_updateVRParams))))
		{
			LOG1("ERROR: Failed to spawn new thread %d.", GetAliveThreadCount()); 
			return false;
		}	

		rv = WaitToFinishIfNeeded();	

		LOG1("} updateRenderAsync : %d threads spawned", GetAliveThreadCount());

		return rv;	
	}

	template <class T, class U>
	void GLRayCastVolumeRenderer<T, U>::StartExecution(void *iParam)
	{
		if (!iParam) return;

		//using namespace PGIO;
		PGAlgs::ThreadedUpdateVRParams<T> * iParams = static_cast<PGAlgs::ThreadedUpdateVRParams<T> *>(iParam);

		iParams->ioLogger->Log("{ VR update begin");

		if (iParams->m_asyncEnabled)
		{
			iParams->ioLogger->Log("} StartExecution: Error: async flag enabled");
			if (iParams->iParentThreadPtr)
			{
				GLRayCastVolumeRenderer<T, U>* iParentThreadPtr = (GLRayCastVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: async flag enabled");
			}
			return;
		}

		GLRayCastVolumeRenderer<T, U> VREngine;
		bool rv = VREngine.SetContext(iParams->m_hdContext);
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to set context");
			if (iParams->iParentThreadPtr)
			{
				GLRayCastVolumeRenderer<T, U>* iParentThreadPtr = (GLRayCastVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to set context");
			}
			return;
		}

		// set data
		PGCore::MultiDataBase<T>* multiVolume = new PGCore::MultiDataBase<T>();
		if (!multiVolume)
		{
			iParams->ioLogger->Log("} StartExecution: Error: no data");
			if (iParams->iParentThreadPtr)
			{
				GLRayCastVolumeRenderer<T, U>* iParentThreadPtr = (GLRayCastVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: no data");
			}
			return;
		}

		multiVolume->AddDataObject(static_cast<PGCore::BaseDataObject *> (iParams->m_pIVolume));
		rv = VREngine.SetInput(static_cast<PGCore::BaseDataObject *> (multiVolume));
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to set input");
			if (iParams->iParentThreadPtr)
			{
				GLRayCastVolumeRenderer<T, U>* iParentThreadPtr = (GLRayCastVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to set input");
			}
			delete multiVolume;
			return;
		}


		//  mask will be implicit to the volume
		VREngine.SetProgressCB(iParams->statusCB);
		VREngine.SetImageSize(iParams->m_rows, iParams->m_columns);
		
		
		rv = VREngine.PrepareRenderer();
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to PrepareRenderer");
			if (iParams->iParentThreadPtr)
			{
				GLRayCastVolumeRenderer<T, U>* iParentThreadPtr = (GLRayCastVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to PrepareRenderer");
			}
			delete multiVolume;
			return;
		}		
		
		VREngine.SetMaskTransferFunctionLUT(iParams->m_MaskLUT);
		VREngine.SetTransferFunctionLUT(iParams->m_LuT);
		VREngine.SetBlendWeight(iParams->m_blendRatio, 0);
		//VREngine.SetBlendWeight(1.0f-iParams->m_blendRatio, 1);

		rv = VREngine.UpdateRender(iParams->m_rendererIndex, iParams->m_updateDisplay);
		if (!rv)
		{
			iParams->ioLogger->Log("} StartExecution: Error: failed to UpdateRender");
			if (iParams->iParentThreadPtr)
			{
				GLRayCastVolumeRenderer<T, U>* iParentThreadPtr = (GLRayCastVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
				iParentThreadPtr->NotifyError("Error: failed to UpdateRender");
			}
			delete multiVolume;
			return;
		}


		if (iParams->iParentThreadPtr)
		{
			GLRayCastVolumeRenderer<T, U>* iParentThreadPtr = (GLRayCastVolumeRenderer<T, U> *)(iParams->iParentThreadPtr);
			iParentThreadPtr->NotifyFinished();
		}
	
		iParams->ioLogger->Log("} GLRayCastVolumeRenderer end");

		return;
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
	int GLRayCastVolumeRenderer<T, U>::initGLthread()
	{
		//3D texture stuff	
		long max3DTextureSize = GLRendererBase::GetMaximum3DTextureMemory();

		if (m_max3DTextureSize==0)
		{
			m_max3DTextureSize = max3DTextureSize;
			if (m_max3DTextureSize<=0) return 0;

			// in KB
			m_remaining3DTextureSize = m_max3DTextureSize*m_max3DTextureSize*m_max3DTextureSize;///(kPg3DTexBufferFactor*kPgBytesInKB);
		}

		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_TRUE);
			glShadeModel(GL_SMOOTH);
		}

		return 1;
	}


	template <class T, class U>
	void GLRayCastVolumeRenderer<T, U>::renderSingleVolume(const int iVolumeIndex/*=0*/)
	{
		if (iVolumeIndex>m_volumeAccessor.size()-1)
			return;

		int iVIndex = iVolumeIndex;
		
		register float r=0.0f, zz=gZMin;
		register float zSparse = (m_skipFactor==1) ? gDzFull : 2.0*m_skipFactor*gDzSparse;	

		gDrSparse = zSparse/2;	

		glEnable(GL_TEXTURE_3D);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_3D, gTextureName[iVIndex]);
		glBindTexture(GL_TEXTURE_2D, gLUTName);

#ifdef _USE_CG
		//cgGLLoadProgram(gCgInfo.fragmentProgram);
		cgGLEnableProfile(gCgInfo.fragmentProfile);
		cgGLBindProgram(gCgInfo.fragmentProgram);

		cgGLSetTextureParameter (gCgInfo.cgTex3d, gTextureName[iVolumeIndex]);
		cgGLSetTextureParameter (gCgInfo.cgTexColormap, gLUTName);
		cgGLEnableTextureParameter (gCgInfo.cgTex3d);	
		cgGLEnableTextureParameter (gCgInfo.cgTexColormap);				
#endif

		float zFac = 0.5;
		glMatrixMode(GL_TEXTURE);

		// img to dicom space
		glLoadIdentity();		

		glTranslatef(-gXTexTrans[iVIndex], -gYTexTrans[iVIndex], -(gZTexTrans[iVIndex]));

		glTranslatef(zFac, zFac, zFac);	

		glScalef(gTexScale[iVIndex].X(), gTexScale[iVIndex].Y(), gTexScale[iVIndex].Z());//0.75f); 7

		glRotatef(90.0, 1, 0, 0);		// this angle is determined based on slice orientation
		glRotatef(gRotation[iVolumeIndex].X(), 1, 0, 0);
		glRotatef(gRotation[iVolumeIndex].Y(), 0, 1, 0);
		glRotatef(gRotation[iVolumeIndex].Z(), 0, 0, 1);					 

		glTranslatef(-zFac, -zFac, -zFac);		

		float blendValue = m_blendRatio[iVIndex];
		float zFactor = 1.0f/float(m_zoomFactor);
		float initialZ = gZMin;
		float finalZ   = (2.0f*m_coronalCutPlanePosition);
		float skipZ	   = zSparse*zFactor;

		blendValue = iVolumeIndex ? min(1, 4*blendValue) : blendValue;
		initialZ = iVolumeIndex ? (finalZ-skipZ) : initialZ;
		r = iVolumeIndex ? (0.5f+m_coronalCutPlanePosition) : r;

		float alphaVal = (m_skipFactor==1) ? 0.5f*blendValue : blendValue; 

		//r = (m_coronalCutPlanePosition+0.5f);

		finalZ = max(gZMin, finalZ);
		finalZ = min(gZMax, finalZ);		
		{ 			 
				//glBindTexture(GL_TEXTURE_3D, gTextureName[iVolumeIndex]);
				for (zz=initialZ; zz < finalZ; zz+=skipZ) 
				{
					r = r<0.0f ? 0.0f : r;
					r = r>1.0f ? 1.0f : r;

					gZTexRatio[iVIndex] = r;//(r-zDiff)>0.0f ? 0.5+(r-zDiff) : 0.0f;
					//gZTexRatio[iVIndex] = max(0.0f, min(1.0f, r));//>1.0f ? 1.0f : gZTexRatio[iVIndex];
					glColor4f(1, 1, 1, alphaVal); // m_skipFactor

					glBegin(GL_QUADS);
					glTexCoord3f(0.0, gYTexRatio[iVIndex], gZTexRatio[iVIndex]); 
					glVertex3f( gXMin, gYMin, zz); 

					glTexCoord3f(gXTexRatio[iVIndex], gYTexRatio[iVIndex], gZTexRatio[iVIndex]); 
					glVertex3f( gXMax, gYMin, zz); 

					glTexCoord3f(gXTexRatio[iVIndex], 0.0, gZTexRatio[iVIndex]); 
					glVertex3f( gXMax, gYMax, zz); 

					glTexCoord3f(0.0, 0.0, gZTexRatio[iVIndex]); 
					glVertex3f( gXMin, gYMax, zz); 
					glEnd();


					r += gDrSparse;				
				}			

		} 

		glMatrixMode(GL_MODELVIEW);
	}

	template <class T, class U>
	void GLRayCastVolumeRenderer<T, U>::renderVolume()
	{
		int i=0;
		for (i=0; i<m_volumeAccessor.size(); i++)
		{
			renderSingleVolume(i);
		}		
	}


#ifdef _USE_CG
	template <class T, class U>
	int GLRayCastVolumeRenderer<T, U>::enableFragmentShader ()
	{
		const char *buffer = "C:\\SW\\SMISDK\\Algs\\fragment.cg";
		//    cgSetErrorCallback (cgErrorCallback);
		// use ARB_fragment_profile if supported
		if (cgGLIsProfileSupported (CG_PROFILE_FP30)) {
			gCgInfo.fragmentProfile = CG_PROFILE_FP30;
			printf ("\nCG profile: CG_PROFILE_FP30\n");
		} else if (cgGLIsProfileSupported (CG_PROFILE_ARBFP1)) {
			gCgInfo.fragmentProfile = CG_PROFILE_ARBFP1;
			printf ("\nCG profile: CG_PROFILE_ARBFP1\n");
		} else if (cgGLIsProfileSupported (CG_PROFILE_FP20)) {
			gCgInfo.fragmentProfile = CG_PROFILE_FP20;
			printf ("\nCG profile: CG_PROFILE_FP20\n");
		} else {
			printf ("Fragment programming extensions (GL_NV_register_combiners2, "
				"GL_NV_fragment_program, or GL_ARB_fragment_program) not "
				"supported, exiting...\n");
			//        ckfree ((void *) buffer);
			return 0;//TCL_ERROR;
		}

		// first load the basic fragment program
		//sprintf (buffer, "%s/vptogl1.0/scripts/fragment.cg", getenv ("VP_LIB"));
		//sprintf (buffer, "%s/vptogl1.0/scripts/fragment.cg", getenv ("VP_LIB"));
		gCgInfo.shaderContext = cgCreateContext ();
		if (gCgInfo.shaderContext == NULL)
		{
			printf("Failed To Create Cg Context");
			return 0;								// We Cannot Continue
		}

		gCgInfo.fragmentProgram = cgCreateProgramFromFile (gCgInfo.shaderContext,
			CG_SOURCE, buffer, gCgInfo.fragmentProfile, NULL, NULL);

		if (gCgInfo.fragmentProgram == NULL)
		{
			// We Need To Determine What Went Wrong
			CGerror Error = cgGetError();

			// Show A Message Box Explaining What Went Wrong
			printf(cgGetErrorString(Error));
			getchar();
			return 0;								// We Cannot Continue
		}


		cgGLLoadProgram (gCgInfo.fragmentProgram);

		gCgInfo.cgTex3d = cgGetNamedParameter (gCgInfo.fragmentProgram, "vol");
		if (!gCgInfo.cgTex3d) {
			printf ("Unable to retrieve \"vol\" parameter...\n");
		}

		gCgInfo.cgTexColormap =
			cgGetNamedParameter (gCgInfo.fragmentProgram, "cmap");
		if (!gCgInfo.cgTexColormap) {
			printf ("Unable to retrieve \"cmap\" parameter...\n");
		}

		if (!gCgInfo.cgTex3d || !gCgInfo.cgTexColormap) {
			printf ("Unable to retrieve program parameters...\n");
			//  ckfree ((void *) buffer);
			return 0;
		}

		//  ckfree ((void *) buffer);
		return 1;
	}


	template <class T, class U>
	void GLRayCastVolumeRenderer<T, U>::disableFragmentShader ()
	{

		if (gCgInfo.fragmentProgram)
			cgDestroyProgram (gCgInfo.fragmentProgram);
		if (gCgInfo.fragmentSlicerProgram)
			cgDestroyProgram (gCgInfo.fragmentSlicerProgram);
		if (gCgInfo.shaderContext)
			cgDestroyContext (gCgInfo.shaderContext);

		printf("\nFragment shader disabled..\n\n");
	}
#endif

#ifdef _PG_GENERATE_SDK_LIBS_
	template class GLRayCastVolumeRenderer<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 		
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLRayCastVolumeRenderer_HPP_
