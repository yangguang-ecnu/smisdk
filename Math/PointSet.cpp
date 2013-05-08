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
// Filename:	PointSet.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM

#include "Math/PointSet.h"

/************************************************************************
BODY OF THE MEMBER ROUTINES
************************************************************************/
// Default constructor


namespace PGMath
{


	/************************************************************************
	BODY OF THE MEMBER ROUTINES
	************************************************************************/
	// Default constructor
	PointSet::PointSet()
	{
	}

	
	// Constructor with assignment of each field
	PointSet::PointSet(const std::vector<Point3D<float> >& iPointSet)
	{
		m_pointSet = iPointSet;		
	}

	// Routine that computes the x- and y- sample means (the coordinates of the centeroid)
	const Point3D<float>& PointSet::ComputeMeans(void) const
	{
		if (m_pointSet.empty()) return Point3D<float>(0, 0, 0);

		float meanX=0., meanY=0., meanZ=0.;

		for (int i=0; i<m_pointSet.size(); i++)
		{
			meanX += m_pointSet[i].X();
			meanY += m_pointSet[i].Y();
			meanZ += m_pointSet[i].Z();
		}

		Point3D<float> mean(meanX, meanY, meanZ);
		
		mean/=m_pointSet.size();

		return mean;
	}

	const int PointSet::Size() const
	{
		return m_pointSet.size();
	}
	
	const Point3D<float>& PointSet::Element(const int& iIndex) const
	{
		if (iIndex<0 || iIndex>(Size()-1)) return Point3D<float>(0, 0, 0);

		return m_pointSet[iIndex];
	}
	

	// Routine that centers the PointSet set (shifts the coordinates to the centeroid)
/*
	bool  PointSet::SubtractMean(void)
	{
		float sX=0.,sY=0.;  
		int i;

		for (i=0; i<n; i++)
		{
			sX += X[i];
			sY += Y[i];
		}
		sX /= n;
		sY /= n;

		for (i=0; i<n; i++)
		{
			X[i] -= sX;
			Y[i] -= sY;
		}
		meanX = 0.;
		meanY = 0.;
	}

	// Routine that scales the coordinates (makes them of order one)

	void PointSet::scale(void)
	{
		float sXX=0.,sYY=0.,scaling;  
		int i;

		for (i=0; i<n; i++)
		{
			sXX += X[i]*X[i];
			sYY += Y[i]*Y[i];
		}
		scaling = sqrt((sXX+sYY)/n/Two);

		for (i=0; i<n; i++)
		{
			X[i] /= scaling;
			Y[i] /= scaling;
		}
	}

	// Printing routine

	void PointSet::print(void)
	{
		cout << endl << "The PointSet set has " << n << " points with coordinates :"<< endl;

		for (int i=0; i<n-1; i++) cout << "(" << X[i] << ","<< Y[i] << "), ";

		cout << "(" << X[n-1] << ","<< Y[n-1] << ")\n";
	}
*/
	// Destructor
	PointSet::~PointSet()
	{
		// TO IMPLEMENT 
	}

};