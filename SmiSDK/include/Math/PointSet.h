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
// Filename:	PointSet.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM


#ifndef _PGPointSET_H_
#define _PGPointSET_H_
//////////////////////////////////////////////////////////////////////////
#include "Point3D.h"

namespace PGMath
{

class PointSet
{
public:
	/************************************************************************
	BODY OF THE MEMBER ROUTINES
	************************************************************************/
	// Default constructor
	PointSet();
	
	// Constructor with assignment of each field
	PointSet(const std::vector<Point3D<float> >& iPointSet);
	
	// Routine that computes the x- and y- sample means (the coordinates of the centeroid)
	const Point3D<float>& ComputeMeans(void) const;
	
	const int Size() const;
	
	const Point3D<float>& Element(const int& iIndex) const;
	
	// Destructor
	~PointSet();

private:
	std::vector<Point3D<float> > m_pointSet;

};

}

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "PointSet.hpp"
#endif

#endif