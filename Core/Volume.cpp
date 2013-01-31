// -*- C++ -*-
// © [2006] Prashant Chopra [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
// INFORMATION OF PRASHANT CHOPRA. ANY DUPLICATION, MODIFICATION,
// DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART,
// IS STRICTLY PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN
// PERMISSION OF PRASHANT CHOPRA (PRASHdotCHOPRAatGMAILdotCOM).
//
// Filename:	Volume.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:38:35 AM
//
//////////////////////////////////////////////////////////////////////////

#include "Core/Volume.h"
#include "Core/DilationKernel.h"
#include "Core/ErosionKernel.h"

namespace PGCore 
{

template <class T>
Volume<T>::Volume()
{
	Clear();			
	m_type = kPgDataObjectTypeVolume;	
}

template <class T>
Volume<T>::Volume(const PGMath::Point3D<float>& iOrigin)
{
	Clear();			
	m_type = kPgDataObjectTypeVolume;	
	SetOrigin(iOrigin);
}

template <class T>
void Volume<T>::SetOrigin(const PGMath::Point3D<float>& iOrigin) 
{
	m_origin = iOrigin;
}

template <class T>
const PGMath::Point3D<float>& Volume<T>::GetOrigin() const
{
	return m_origin;
}

template <class T>
int Volume<T>::GetSize() const 
	{
		return m_images.size();			
	};

	template <class T>
void Volume<T>::GetDimensions(PGMath::Point3D<long> &oDimensions) const 
	{
		long xSize = 0, ySize=0, zSize= GetSize();
		if (zSize>0) 
		{			
			m_images[0].GetDimensions(ySize, xSize); 
		}		
		oDimensions = PGMath::Point3D<long>(xSize, ySize, zSize);
		return;
	};


template <class T>
const std::vector<Image <T > > & Volume<T>::GetImages() const 
	{
		return m_images;
	}

template <class T>
bool Volume<T>::AddImage(const Image<T>& iImageObject) {
	Image<T> newImage(iImageObject);
	
	m_images.push_back(newImage);	
	int zSize = m_images.size();
	if (zSize==1)
	{
		long iR=0, iC=0;
		newImage.GetDimensions(iR, iC);
		m_rows = iR; m_columns = iC;	
	}
	return (zSize>0);
}


template <class T>
bool Volume<T>::InvertBitVolume(const int iImageIndex/*=0*/)
{
	PGCore::BitVolume& maskVol = GetBitVolume(iImageIndex);
	return maskVol.Invert();
}


template <class T>
bool Volume<T>::DilateBitVolume(const int iImageIndex/*=0*/, const int iVoxels/*=1*/)
{
	if (iVoxels)
	{
		GetLogger()->Log("Start Dilation...%d voxels", iVoxels);
		PGCore::DilationKernel<char, char> dilationKernel(iVoxels);
		PGCore::BitVolume& maskVol = GetBitVolume(iImageIndex);
		PGMath::Point3D<long> mDims;
		maskVol.GetDimensions(mDims);

		PGCore::BitImage sbImage(mDims.Y(), mDims.X());
		const std::vector<BitImage > & maskImages = maskVol.GetImages();
		for (int i=0; i<mDims.Z(); i++)
		{
			sbImage.Clear();

			PGCore::BitImage bImage = maskImages[i];
			dilationKernel.Convolve(bImage, sbImage);	

			// set it back
			maskVol.SetImage(i, sbImage);
		}
		GetLogger()->Log("End Dilation.");
	}

	return true;
}


template <class T>
bool Volume<T>::ErodeBitVolume(const int iImageIndex/*=0*/, const int iVoxels/*=1*/)
{
	if (iVoxels)
	{
		GetLogger()->Log("Start Erosion...%d voxels", iVoxels);
		PGCore::ErosionKernel<char, char> erosionKernel(iVoxels);
		PGCore::BitVolume& maskVol = GetBitVolume(iImageIndex);
		PGMath::Point3D<long> mDims;
		maskVol.GetDimensions(mDims);

		PGCore::BitImage sbImage(mDims.Y(), mDims.X());
		const std::vector<BitImage > & maskImages = maskVol.GetImages();
		for (int i=0; i<mDims.Z(); i++)
		{
			sbImage.Clear();

			PGCore::BitImage bImage = maskImages[i];
			erosionKernel.Convolve(bImage, sbImage);	

			// set it back
			maskVol.SetImage(i, sbImage);
		}
		GetLogger()->Log("End Erosion.");
	}

	return true;

}


template <class T>
bool Volume<T>::ResetBitVolume(const int iImageIndex/*=0*/)
{
	if (iImageIndex<0 || iImageIndex>1) return false;

	return m_maskVolume[iImageIndex].Clear();
}

template <class T>
bool Volume<T>::ResetPointCloud(const int iImageIndex/*=0*/)
{
	if (iImageIndex<0 || iImageIndex>1) return false;

	m_pointCloud[iImageIndex].clear();
	return true;
}

template <class T>
bool Volume<T>::InitializeBitVolume(const int iImageIndex/*=0*/)
{
	if (iImageIndex<0 || iImageIndex>1) return false;

	// make sure the bitVolume is initialised
	PGMath::Point3D<long> bDims;
	m_maskVolume[iImageIndex].GetDimensions(bDims);

	if (bDims.X()!=m_columns || bDims.Y()!=m_rows || bDims.Z()!=m_images.size())
	{
		// reset
		GetDimensions(bDims);
		m_maskVolume[iImageIndex].SetDimensions(bDims);		
	}

	return true;
}


template <class T>
BitVolume& Volume<T>::GetBitVolume(const int iImageIndex/*=0*/)
{
	if (iImageIndex<0 || iImageIndex>1) return m_maskVolume[0];

	return m_maskVolume[iImageIndex];
}

template <class T>
bool Volume<T>::FinalizeMask() // OR temp mask with main mask
{
	m_maskVolume[0] |= m_maskVolume[1];
	return true;
}

template <class T>
bool Volume<T>::CreateOctree()
{
	bool res = false;
	/*
	bool res = clearOctreeBlocks();
	if (!res) return false;

	m_Octree.Clear();
	m_LowestBlockSize = 0;

	m_Octree.SetRootNode(this);

	res = CreateOctreeForBlock();
	if (!res)
	{
		clearOctreeBlocks();
	}
	*/
	return res;
}

template <class T>
bool Volume<T>::clearOctreeBlocks()
{
	/*
	// first get root node
	// then children

	int cCount = GetChildrenCount();
	for (int i=0; i<cCount; i++)
	{
		PGCore::TreeNode<T>* childNode = GetChild(i);
		if (!childNode)	{ return false; }

		PGCore::Volume<T>* volumeNode = static_cast<PGCore::Volume<T>*>(childNode);
		if (!volumeNode)	{ return false; }
		
		delete volumeNode;
		volumeNode = 0;
	}
	*/

	return true;	
}

// How to use this Octree??
// For ray casting: skip blocks
// For MPR        : really dont need this??
// For VR         : when loading volume, set a coarse volume as input
//					when zoomed in, find out which blocks are in view, and upload only those blocks
//					to memory for finer review

template <class T>
bool Volume<T>::CreateOctreeForBlock()
{
	/*
	// For the incoming cube,
	// create 8 cubes with their respective origins
	// ( create new Volume<T> structures for each cube, complete with their origin/dimension. )
	// for each cube, 
	//		see if the data is uniform:	if it is, mark the uniform value
	//		else, queue this block for further subdivision

	// perhaps do this for blocks in a set of images first, so they can be freed as soon as the blocks are created
	// finally, m_volumeaccessor should be cleared

	// actually, the octree shd be a property of the volume structure

	T mean=-1, variance=-1;
	bool rv = GetMoments(&mean, &variance);
	if (rv)
	{
		SetMeanValue(mean);
		if (variance<=0.0f)
		{
			SetUniformBlockFlag(true);
			return true;
		}
	}

	PGMath::Point3D<long> vDims, vDimsBy2;

	GetDimensions(vDims);

	PGCore::TreeNode<T>* ioNode = static_cast<PGCore::TreeNode<T>*>(this);

	// divide recursively until cube size reaches _PG_MIN_OCTREE_CUBE_SIZE_
	m_LowestBlockSize = min(min(vDims.X(), vDims.Y()), vDims.Z());

	if (m_LowestBlockSize/2 < 1) return  true; //_PG_MIN_OCTREE_CUBE_SIZE_

	m_LowestBlockSize/=2;	

	//
	

	// create volume structures
	vDimsBy2 = PGMath::Point3D<long>(vDims.X()/2, vDims.Y()/2, vDims.Z()/2);

	std::vector<PGCore::Volume<T> *> children;
	*/
	// ok now create 8 volumes in sequence	
	/* create new blocks of data and add to the octree

	  ^ +Z
	  |	
	  |______________________________	
	  /		5		/		6		/|
	 /-----------------------------/ | ^ +Y
	/_____________/______________ /  |/ 
	|		1	  |		2		 |  7/
	|_____________|______________|  /
	|             |      3       | /
	|______0______|______________|/_______> +X

	*/

	/*
	// Block 00: X (0->X/2), Y (0->Y/2), Z (0->Z/2)
	{
		PGMath::Point3D<float>origin(0, 0, 0);
		PGCore::Volume<T> *newVolume = new PGCore::Volume<T>(origin);
		if (!newVolume) return false;
		children.push_back(newVolume);


		// fill image data here
		for (int z=0; z<vDimsBy2.Z(); z++)
		{
			// get image
			const PGCore::Image<T> &nextImageFull = GetImage(z);
			PGCore::Image<T> nextImage(vDimsBy2.Y(), vDimsBy2.X());

			const T* inBuf = nextImageFull.GetBuffer();
			if (!inBuf) return false;

			T* outBuf = nextImage.GetBuffer();
			if (!outBuf) return false;

			// crop first quater of the image
			for (int row=0; row<vDimsBy2.Y(); row++)
			{
				memcpy(outBuf+row*vDimsBy2.X(), inBuf+row*vDims.X(), vDimsBy2.X());
			}

			newVolume->AddImage(nextImage);
		}

		
	}


	// Block 01: X (0->X/2), Y (0->Y/2), Z (Z/2->Z)
	{
		PGMath::Point3D<float>origin(0, 0, vDimsBy2.Z());
		PGCore::Volume<T> *newVolume = new PGCore::Volume<T>(origin);
		if (!newVolume) return false;
		children.push_back(newVolume);


		// fill image data here
		for (int z=vDimsBy2.Z(); z<vDims.Z(); z++)
		{
			// get image
			const PGCore::Image<T> &nextImageFull = GetImage(z);
			PGCore::Image<T> nextImage(vDimsBy2.Y(), vDimsBy2.X());

			const T* inBuf = nextImageFull.GetBuffer();
			if (!inBuf) return false;

			T* outBuf = nextImage.GetBuffer();
			if (!outBuf) return false;

			// crop first quater of the image
			for (int row=0; row<vDimsBy2.Y(); row++)
			{
				memcpy(outBuf+row*vDimsBy2.X(), inBuf+row*vDims.X(), vDimsBy2.X());
			}

			newVolume->AddImage(nextImage);
		}

		
	}

	// Block 02: X (X/2->X), Y (0->Y/2), Z (Z/2->Z)
	{
		PGMath::Point3D<float>origin(vDimsBy2.X(), 0, vDimsBy2.Z());
		PGCore::Volume<T> *newVolume = new PGCore::Volume<T>(origin);
		if (!newVolume) return false;
		children.push_back(newVolume);


		// fill image data here
		for (int z=vDimsBy2.Z(); z<vDims.Z(); z++)
		{
			// get image
			const PGCore::Image<T> &nextImageFull = GetImage(z);
			PGCore::Image<T> nextImage(vDimsBy2.Y(), vDimsBy2.X());

			const T* inBuf = nextImageFull.GetBuffer();
			if (!inBuf) return false;

			T* outBuf = nextImage.GetBuffer();
			if (!outBuf) return false;

			// crop second quater of the image
			for (int row=0; row<vDimsBy2.Y(); row++)
			{
				memcpy(outBuf+row*vDimsBy2.X(), inBuf+row*vDims.X()+vDimsBy2.X(), vDimsBy2.X());
			}

			newVolume->AddImage(nextImage);
		}

		
	}


	// Block 03: X (X/2->X), Y (0->Y/2), Z (0->Z/2)
	{
		PGMath::Point3D<float>origin(vDimsBy2.X(), 0, 0);
		PGCore::Volume<T> *newVolume = new PGCore::Volume<T>(origin);
		if (!newVolume) return false;
		children.push_back(newVolume);


		// fill image data here
		for (int z=0; z<vDimsBy2.Z(); z++)
		{
			// get image
			const PGCore::Image<T> &nextImageFull = GetImage(z);
			PGCore::Image<T> nextImage(vDimsBy2.Y(), vDimsBy2.X());

			const T* inBuf = nextImageFull.GetBuffer();
			if (!inBuf) return false;

			T* outBuf = nextImage.GetBuffer();
			if (!outBuf) return false;

			// crop second quater of the image
			for (int row=0; row<vDimsBy2.Y(); row++)
			{
				memcpy(outBuf+row*vDimsBy2.X(), inBuf+row*vDims.X()+vDimsBy2.X(), vDimsBy2.X());
			}

			newVolume->AddImage(nextImage);
		}

		
	}



	// Block 04: X (0->X/2), Y (Y/2->Y), Z (0->Z/2)
	{
		PGMath::Point3D<float>origin(0, vDimsBy2.Y(), 0);
		PGCore::Volume<T> *newVolume = new PGCore::Volume<T>(origin);
		if (!newVolume) return false;
		children.push_back(newVolume);


		// fill image data here
		for (int z=0; z<vDimsBy2.Z(); z++)
		{
			// get image
			const PGCore::Image<T> &nextImageFull = GetImage(z);
			PGCore::Image<T> nextImage(vDimsBy2.Y(), vDimsBy2.X());

			const T* inBuf = nextImageFull.GetBuffer();
			if (!inBuf) return false;

			T* outBuf = nextImage.GetBuffer();
			if (!outBuf) return false;

			// crop first quater of the image
			for (int row=0; row<vDimsBy2.Y(); row++)
			{
				memcpy(outBuf+row*vDimsBy2.X(), inBuf+(vDimsBy2.Y()+row)*vDims.X(), vDimsBy2.X());
			}

			newVolume->AddImage(nextImage);
		}

		
	}


	// Block 05: X (0->X/2), Y (Y/2->Y), Z (Z/2->Z)
	{
		PGMath::Point3D<float>origin(0, vDimsBy2.Y(), vDimsBy2.Z());
		PGCore::Volume<T> *newVolume = new PGCore::Volume<T>(origin);
		if (!newVolume) return false;
		children.push_back(newVolume);


		// fill image data here
		for (int z=vDimsBy2.Z(); z<vDims.Z(); z++)
		{
			// get image
			const PGCore::Image<T> &nextImageFull = GetImage(z);
			PGCore::Image<T> nextImage(vDimsBy2.Y(), vDimsBy2.X());

			const T* inBuf = nextImageFull.GetBuffer();
			if (!inBuf) return false;

			T* outBuf = nextImage.GetBuffer();
			if (!outBuf) return false;

			// crop first quater of the image
			for (int row=0; row<vDimsBy2.Y(); row++)
			{
				memcpy(outBuf+row*vDimsBy2.X(), inBuf+(vDimsBy2.Y()+row)*vDims.X(), vDimsBy2.X());
			}

			newVolume->AddImage(nextImage);
		}

		
	}

	// Block 06: X (X/2->X), Y (Y/2->Y), Z (Z/2->Z)
	{
		PGMath::Point3D<float>origin(vDimsBy2.X(), vDimsBy2.Y(), vDimsBy2.Z());
		PGCore::Volume<T> *newVolume = new PGCore::Volume<T>(origin);
		if (!newVolume) return false;
		children.push_back(newVolume);


		// fill image data here
		for (int z=vDimsBy2.Z(); z<vDims.Z(); z++)
		{
			// get image
			const PGCore::Image<T> &nextImageFull = GetImage(z);
			PGCore::Image<T> nextImage(vDimsBy2.Y(), vDimsBy2.X());

			const T* inBuf = nextImageFull.GetBuffer();
			if (!inBuf) return false;

			T* outBuf = nextImage.GetBuffer();
			if (!outBuf) return false;

			// crop second quater of the image
			for (int row=0; row<vDimsBy2.Y(); row++)
			{
				memcpy(outBuf+row*vDimsBy2.X(), inBuf+(vDimsBy2.Y()+row)*vDims.X()+vDimsBy2.X(), vDimsBy2.X());
			}

			newVolume->AddImage(nextImage);
		}

		
	}


	// Block 07: X (X/2->X), Y (Y/2->Y), Z (0->Z/2)
	{
		PGMath::Point3D<float>origin(vDimsBy2.X(), vDimsBy2.Y(), 0);
		PGCore::Volume<T> *newVolume = new PGCore::Volume<T>(origin);
		if (!newVolume) return false;
		children.push_back(newVolume);


		// fill image data here
		for (int z=0; z<vDimsBy2.Z(); z++)
		{
			// get image
			const PGCore::Image<T> &nextImageFull = GetImage(z);
			PGCore::Image<T> nextImage(vDimsBy2.Y(), vDimsBy2.X());

			const T* inBuf = nextImageFull.GetBuffer();
			if (!inBuf) return false;

			T* outBuf = nextImage.GetBuffer();
			if (!outBuf) return false;

			// crop second quater of the image
			for (int row=0; row<vDimsBy2.Y(); row++)
			{
				memcpy(outBuf+row*vDimsBy2.X(), inBuf+(vDimsBy2.Y()+row)*vDims.X()+vDimsBy2.X(), vDimsBy2.X());
			}

			newVolume->AddImage(nextImage);
		}

		
	}
	
	int countRes = 0;
	for (int i=0; i<children.size(); i++)
	{
		if (children[i])
		{
			PGCore::TreeNode<T>* tNode = static_cast<PGCore::TreeNode<T>*>(children[i]);
			if (!tNode) continue;

			if (ioNode) ioNode->AddChild(tNode);

			countRes += children[i]->CreateOctreeForBlock() ? 1 : 0;	
		}
	}

	return (countRes==children.size());
	*/
	return false;
}


template <class T>
const Image<T>& Volume<T>::GetImage(int iImageIndex) const 
{
	if (iImageIndex<m_images.size()) 
	{
		return m_images[iImageIndex];	
	}
	return Image<T>();
}
 
template <class T>
inline T Volume<T>::GetValue(const int iRow, const int iColumn, const int iImageIndex) const
{
	if (iImageIndex>=m_images.size()) 
	{
		return 0;
	}
	return m_images[iImageIndex].GetValue((long)iRow, (long)iColumn);
}

// perhaps this code should be in the renderer class
// where we define a ray from image space into the object space
// then compute which box it intersects (Top Down)
// and stop at a desired level based on interactivity
template <class T>
inline T Volume<T>::GetValueFromOctree(const int iRow, const int iColumn, const int iImageIndex) const
{
	/*
	// compute block 
	// go over the tree and compute which one encompasses this point
	PGCore::TreeNode<T>* rootNode = m_Octree.GetRootNode();
	bool foundBlock = false;
	
	PGMath::Point3D<float> location(iColumn, iRow, iImageIndex);

	while (!foundBlock)
	{
		const int cCount = rootNode->GetChildrenCount();
		if (!cCount) 
		{
			foundBlock = true;
			break;
		};

		for (int i=0; i<cCount; i++)
		{
			PGCore::TreeNode<T>* nextNode = rootNode->GetChild(i);
			if (!nextNode) continue;

			PGCore::Volume<T>* vNode = static_cast<PGCore::Volume<T>*>(nextNode);
			if (!vNode) continue;

			const PGMath::Point3D<float>& org = vNode->GetOrigin();
			PGMath::Point3D<long> dims;
			
			vNode->GetDimensions(dims);

			PGMath::Point3D<float> diffVec = location - org;

			if (diffVec.X()<0 || diffVec.X()>=dims.X()
				|| diffVec.Y()<0 || diffVec.Y()>=dims.Y()
				|| diffVec.Z()<0 || diffVec.Z()>=dims.Z())
			{
				continue;
			}

			// found the super block
			// go further
			rootNode = nextNode; 
			location = diffVec;

			break; // break the for loop
		}
	}

	*/
	return 0;
}

template <class T>
inline bool Volume<T>::SetValue(const int iRow, const int iColumn, const int iImageIndex, const T& iValue)
{
	if (iImageIndex>=m_images.size()) 
	{
		return 0;
	}

	return m_images[iImageIndex].SetValue((long)iRow, (long)iColumn, iValue);
}

template <class T>
inline T Volume<T>::GetValueNoBoundCheck(const int iRow, const int iColumn, const int iImageIndex) const
{
	return m_images[iImageIndex].GetValueNoBoundCheck((long)iRow, (long)iColumn);
}

template <class T>
inline T Volume<T>::GetAvgValue(const int iRow, const int iColumn, const int iImageIndex) const
{
	T neighborValues[8];
	bool res = GetEightNeighborValues(iRow, iColumn, iImageIndex, neighborValues);
	if (!res)
	{
		return GetValue(iRow, iColumn, iImageIndex);
	}

	T totalValue = 0;
	for (int i=0; i<8; i++)
	{
		totalValue+=neighborValues[i];
	}

	return T(totalValue>>3);
}	


template <>
inline float Volume<float>::GetAvgValue(const int iRow, const int iColumn, const int iImageIndex) const
{
	float neighborValues[8];
	bool res = GetEightNeighborValues(iRow, iColumn, iImageIndex, neighborValues);
	if (!res)
	{
		return GetValue(iRow, iColumn, iImageIndex);
	}

	float totalValue = 0;
	for (int i=0; i<8; i++)
	{
		totalValue+=neighborValues[i];
	}

	return float(totalValue/8.0f);
}	

template <>
inline bool Volume<PGMath::Point3D<unsigned short> >::GetGradient(const int iRow, const int iColumn, const int iImageIndex, 
								   PGMath::Vector3D<float>& oGradient) const
{
	return false;
}

template <>
inline bool Volume<PGMath::Point3D<unsigned char> >::GetGradient(const int iRow, const int iColumn, const int iImageIndex, 
								   PGMath::Vector3D<float>& oGradient) const
{
	return false;
}
	

template <class T>
inline bool Volume<T>::GetGradient(const int iRow, const int iColumn, const int iImageIndex, 
								   PGMath::Vector3D<float>& oGradient) const
{
	if (iImageIndex<0 || iImageIndex>m_images.size()-2) 
	{
		return false;
	}

	float gX=0, gY=0, gZ=0;
	int r=iRow, rPlus=iRow+1, c=iColumn, cPlus=iColumn+1, z=iImageIndex, zPlus=iImageIndex+1;
	if (iRow > 0 && iRow < (m_rows-1) && iColumn > 0 && iColumn < (m_columns-1)
		&& iImageIndex>0 && m_images.size()>2)
	{	
		gY = float(GetValueNoBoundCheck(rPlus, c, z) - GetValueNoBoundCheck(r-1, c, z));	
		gX = float(GetValueNoBoundCheck(r, cPlus, z) - GetValueNoBoundCheck(r, c-1, z));	
		gZ = float(GetValueNoBoundCheck(r, c, zPlus) - GetValueNoBoundCheck(r, c, z-1));	
	} 
	else 
	{	
		gY = float(GetValue(rPlus, c, z) - GetValue(r-1, c, z));	
		gX = float(GetValue(r, cPlus, z) - GetValue(r, c-1, z));	
		gZ = float(GetValue(r, c, zPlus) - GetValue(r, c, z-1));	
	}

	oGradient = PGMath::Vector3D<float>(gX, gY, gZ);

	return true;
}



template <class T>
inline bool Volume<T>::GetEightNeighborValues(const int iRow, const int iColumn, const int iImageIndex, 
			T ioEightNeighbors[8]) const
{
	if (iImageIndex<0 || iImageIndex>m_images.size()-2) 
	{
		return false;
	}

	int r=iRow, rPlus=iRow+1, c=iColumn, cPlus=iColumn+1, z=iImageIndex, zPlus=iImageIndex+1;
	if (iRow > -1 && iRow < (m_rows-1) && iColumn > -1 && iColumn < (m_columns-1)
		&& m_images.size()>1)
	{	
		ioEightNeighbors[0] = GetValueNoBoundCheck(r, c, z);		
		ioEightNeighbors[1] = GetValueNoBoundCheck(r, c, zPlus); 
		ioEightNeighbors[2] = GetValueNoBoundCheck(r, cPlus, z);
		ioEightNeighbors[3] = GetValueNoBoundCheck(r, cPlus, zPlus);
		ioEightNeighbors[4] = GetValueNoBoundCheck(rPlus, c, z);	
		ioEightNeighbors[5] = GetValueNoBoundCheck(rPlus, c, zPlus);
		ioEightNeighbors[6] = GetValueNoBoundCheck(rPlus, cPlus, z);
		ioEightNeighbors[7] = GetValueNoBoundCheck(rPlus, cPlus, zPlus);		
	} 
	else 
	{	
		ioEightNeighbors[0] = GetValue(r, c, z);		
		ioEightNeighbors[1] = GetValue(r, c, zPlus);
		ioEightNeighbors[2] = GetValue(r, cPlus, z);
		ioEightNeighbors[3] = GetValue(r, cPlus, zPlus);
		ioEightNeighbors[4] = GetValue(rPlus, c, z);	
		ioEightNeighbors[5] = GetValue(rPlus, c, zPlus);
		ioEightNeighbors[6] = GetValue(rPlus, cPlus, z);
		ioEightNeighbors[7] = GetValue(rPlus, cPlus, zPlus);
	}
	return true;
}


template <class T>
bool Volume<T>::GetMoments(T* oMean, T* oVariance/*=0*/) const
{
	if (!oMean || m_images.empty())
	{
		return false;
	}

	*oMean = 0.0f;
	T mean=0.0f;
	for (int i=0; i< m_images.size(); i++)
	{
		if (!m_images[i].GetMoments(&mean)) 
		{
			*oMean = 0.0f;
			return false;
		}	
		*oMean += mean;
	}
	
	*oMean/=m_images.size();

	if (oVariance)
	{
		PGMath::Point3D<long> oDimensions;
		long xSize=0, ySize=0, zSize=m_images.size();

		GetDimensions(oDimensions);
		
		xSize = oDimensions.X();
		ySize = oDimensions.Y();
		zSize = oDimensions.Z();

		long double pixelDenom=1.0f/(double)(xSize*ySize*zSize);
		T variance=0.0f;

		T nextVal = 0;		

		for (int k=0; k< zSize; k++)
		{
			const T* imgBuffer = m_images[k].GetBuffer();
			if (!imgBuffer) return false;

			for (int j=0; j<ySize; j++)
			{
				for (int i=0; i<xSize; i++)
				{
					nextVal = imgBuffer[j*xSize + i];

					variance += (((nextVal - *oMean)*(nextVal - *oMean))
						* pixelDenom);
					
				}
			}		
		}

		*oVariance = variance;
	}

	return true;
}

template <class T>
bool Volume<T>::Reset(const T& iValue)
{
	for (int i=0; i< m_images.size(); i++)
	{
		if (!m_images[i].Reset(iValue)) 
		{
			return false;
		}
	}
	return true;
}

template <class T>
bool Volume<T>::Normalize(Volume<T> &oVolume) const
{
	T iMean=-1, iVariance=0;

	if (!GetMoments(&iMean, &iVariance))
	{
		return false;
	} 

	if (iVariance<=0.0f)
	{
		return false;
	}

	oVolume.Clear();

	for (int i=0; i< m_images.size(); i++)
	{
		Image<T> oImage;
		if (!m_images[i].Normalize(iMean, iVariance, oImage)) 
		{
			return false;
		}
		oVolume.AddImage(oImage);
	}

	return true;
}



template <class T>
bool Volume<T>::GetDataRange(T *oMin, T* oMax) 
{
	if (!oMin || !oMax) {
		return false;
	}

	// go over the data and get min/max	
	PixelBase<T> tPixel;
	T pMin	=	(T)tPixel.GetMaxValue();
	T pMax	=	(T)tPixel.GetMinValue();

	*oMin	=	pMin;
	*oMax	=	pMax;

	for (int i=0; i<m_images.size(); i++) 
	{		
		PGCore::Image<T>& firstImage = m_images[i];
		firstImage.Update();

		if (firstImage.GetDataRange(&pMin, &pMax)) 
		{		
			*oMin = *oMin > pMin ? pMin : *oMin;
			*oMax = *oMax < pMax ? pMax : *oMax;
		}
	}		

	return true;
}

// pointcloud ops
template <class T>
std::vector<PGMath::Point3D<float> >& Volume<T>::GetPointCloud(const int iIndex/*=0*/) // in CT space
{
	if (iIndex<0 || iIndex>1) return std::vector<PGMath::Point3D<float> >();

	return m_pointCloud[iIndex];
}

template <class T>
bool Volume<T>::Clear() 
{
	bool res = clearOctreeBlocks();
	if (!res) return false;

	m_images.clear();			
	m_pointCloud[0].clear();
	m_pointCloud[1].clear();

	m_rows = 0; m_columns = 0;
	m_origin = PGMath::Point3D<float>(0, 0, 0);
	return (m_images.size()==0);
}



template <class T>
Volume<T>::~Volume(void)
{	
	Clear();		
}


template <class T>
Volume<T> &Volume<T>::operator=(const Volume<T> &iVolumeObject) {	
	Clear();
	// copy all images
	for (int i=0; i<iVolumeObject.GetSize(); i++) 
	{
		AddImage(((Volume<T>&)iVolumeObject).GetImage(i));
	}
	return *this;
}



#ifdef _PG_GENERATE_SDK_LIBS_
	template class Volume<PG_RENDERING_IN_TYPE>; 
	template class Volume<char>;
	template class Volume<float>;
	template class Volume<unsigned short>; 
	template class Volume<unsigned char>; 
	template class Volume<PGMath::Point3D<unsigned short> >;
	template class Volume<PGMath::Point3D<unsigned char> >;
#endif

	
};

//////////////////////////////////////////////////////////////////////////
// EOF

