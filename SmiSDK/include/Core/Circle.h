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
// Filename:	Circle.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_CIRCLE_HPP_
#define _PG_CIRCLE_HPP_
//////////////////////////////////////////////////////////////////////////

//
//						 circle.h
//
/************************************************************************
DECLARATION OF THE CLASS CIRCLE
************************************************************************/
// Class for Circle
// A circle has 7 fields: 
//     a, b, r (of type float), the circle parameters
//     s (of type float), the estimate of sigma (standard deviation)
//     g (of type float), the norm of the gradient of the objective function
//     i and j (of type int), the iteration counters (outer and inner, respectively)

#include "ObjectBase.h"
#include "BaseDataObject.h"


namespace PGCore
{

	class Circle : public ObjectBase, public BaseDataObject
	{
	public:
		// constructors
		Circle();
		Circle(float aa, float bb, float rr);

		float& a() { return m_a; } 
		float& b() { return m_b; } 
		float& r() { return m_r; } 
		float& s() { return m_s; } 
		float& g() { return m_g; } 
		
	private:
		// The fields of a Circle
		float m_a, m_b, m_r, m_s, m_g;
		int i, j;
	};

};

#endif