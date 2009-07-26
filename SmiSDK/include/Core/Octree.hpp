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
// Filename:	Octree.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Octree_HPP_
#define _PG_Octree_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Octree.h"


namespace PGCore
{	
	
	template<class T>
	inline TreeNode<T>::TreeNode(TreeNode* iParent/*=0*/)
	{
		Clear();
		pParent = iParent;		
	}

	template<class T>
	inline TreeNode<T>::~TreeNode(void)
	{
		pParent = 0;
		pChildren.clear();
	}

	template<class T>
	inline bool TreeNode<T>::AddChild(TreeNode<T>* iNode) 
	{
		if (!iNode) return false;
		
		pChildren.push_back(iNode);
		iNode->SetParent(this);

		return (iNode!=0);
	}

	template<class T>
	inline const TreeNode<T>* TreeNode<T>::GetChild(const int& iIndex) const
	{
		if (iIndex>=0 && iIndex<=pChildren.size())
		{
			return pChildren[iIndex];
		}
		return 0;
	}
	
	template<class T>
	inline TreeNode<T>* TreeNode<T>::GetChild(const int& iIndex)
	{
		if (iIndex>=0 && iIndex<=pChildren.size())
		{
			return pChildren[iIndex];
		}
		return 0;
	}

	template<class T>
	inline const unsigned int TreeNode<T>::GetChildrenCount() const
	{
		return pChildren.size();
	}

	template<class T>
	inline bool TreeNode<T>::SetParent(TreeNode<T>* iNode)
	{
		pParent = iNode;
		return (iNode!=NULL);
	}

	template<class T>
	inline const TreeNode<T>* TreeNode<T>::GetParent() const
	{
		return pParent;
	}

	template<class T>
	inline bool TreeNode<T>::SetMeanValue(const T iValue)
	{
		meanValue = iValue;
		return true;
	}

	template<class T>
	inline const T TreeNode<T>::GetMeanValue() const
	{
		return meanValue;
	}

	template<class T>
	inline const bool TreeNode<T>::GetUniformBlockFlag() const
	{
		return uniformBlock;
	}

	template<class T>
	inline void TreeNode<T>::SetUniformBlockFlag(const bool iFlag) 
	{
		uniformBlock = iFlag;
	}

	template<class T>
	inline bool TreeNode<T>::Clear()
	{
		pParent = 0;
		uniformBlock = false;
		meanValue = -1;

		for (int i=0; i<pChildren.size(); i++)
		{
			if (pChildren[i]) pChildren[i]->Clear();
			pChildren[i] = 0;
		}

		pChildren.clear();

		return pChildren.empty();
	}

	
	// Octree
	template<class T>
	inline Octree<T>::Octree(TreeNode<T>* iRootNode/*=0*/)
	{	
		SetRootNode(iRootNode);
	}

	template<class T>
	inline TreeNode<T>* Octree<T>::GetRootNode() const
	{
		return m_rootNode;
	}

	template<class T>
	inline void Octree<T>::SetRootNode(TreeNode<T>* iNode)
	{
		m_rootNode = iNode;
	}


	template<class T>
	inline bool Octree<T>::Clear() 
	{		
		if (m_rootNode)
		{
			m_rootNode->Clear();
			m_rootNode = 0;
		}
		return true;
	}

	template<class T>
	inline Octree<T>::~Octree(void)
	{	
		Clear();
	}
};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Octree_HPP_