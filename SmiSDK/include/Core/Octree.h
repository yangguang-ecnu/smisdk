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
#ifndef _PG_Octree_H_
#define _PG_Octree_H_
//////////////////////////////////////////////////////////////////////////

#include "BaseDataObject.h"

namespace PGCore
{
	template<class T>
	class TreeNode
	{
		public:
				
			TreeNode(TreeNode* iParent=0);	

			virtual ~TreeNode(void);

			bool AddChild(TreeNode<T>* iNode);
			const TreeNode<T>* GetChild(const int& iIndex) const;
			TreeNode<T>* GetChild(const int& iIndex);

			bool SetParent(TreeNode<T>* iNode);
			const TreeNode<T>* GetParent() const;

			bool SetMeanValue(const T iValue);
			const T GetMeanValue() const;

			void SetUniformBlockFlag(const bool iFlag);
			const bool GetUniformBlockFlag() const;

			const unsigned int GetChildrenCount() const;

			bool Clear();

		private:

			// data information
			bool						uniformBlock;
			T							meanValue;
						
			// links
			TreeNode<T>*				pParent;
			std::vector<TreeNode<T>* >	pChildren;

			
			
	};


	template<class T>
	class Octree : public BaseDataObject
	{

	public:

		Octree(TreeNode<T>* iRootNode=0);	

		virtual ~Octree(void);

		void SetRootNode(TreeNode<T>* iNode);

		TreeNode<T>* GetRootNode() const;
		
		bool Clear();

		
	protected: 
				

	private:
		TreeNode<T>		*m_rootNode;
		
	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Octree.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Octree_H_

