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
// Filename:	Circle.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM

#include "Core/Circle.h"

/************************************************************************
			BODY OF THE MEMBER ROUTINES
************************************************************************/
// Default constructor


namespace PGCore
{

Circle::Circle()
{
	m_a=0.; m_b=0.; m_r=1.; m_s=0.; i=0; j=0;
}

// Constructor with assignment of the circle parameters only

Circle::Circle(float aa, float bb, float rr)
{
	m_a=aa; m_b=bb; m_r=rr;
}

};