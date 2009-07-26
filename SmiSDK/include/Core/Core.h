// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	Core.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 10:49:21 PM
//

#ifndef _PG_Core_H_
#define _PG_Core_H_ "_PG_Core_H_"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

// Disable warnings
// 'type cast' : pointer truncation from 'PGCore::Logger *const ' to 'int'
#pragma warning(disable : 4311) 

// 'fprintf' : '%d' in format string conflicts with argument 1 of type 'PGCore::Logger *const '
#pragma warning(disable : 4313) 

// 'initializing' : conversion from 'size_t' to 'int', possible loss of data
#pragma warning(disable : 4267) 

// 'initializing' : PDB 'vc80.pdb' was not found
#pragma warning(disable : 4099) 

// 'void' function returning a value
#pragma warning(disable : 4098) 

// nonstandard extension used: enum 'PGCore::ePGModality' used in qualified name
#pragma warning(disable : 4482)


#define _PG_GENERATE_SDK_LIBS_ "_PG_GENERATE_SDK_LIBS_" 


#define _PG_BREAK_NATIVE DebugBreak();

#define PG_UCHAR_MAX 255
#define PG_UCHAR_MIN 0
#define PG_CHAR_MAX 127
#define PG_CHAR_MIN -127
#define PG_INT_MAX +32767
#define PG_INT_MIN -32767
#define PG_SHORT_MAX +32767
#define PG_SHORT_MIN -32767
#define PG_USHORT_MAX +65535
#define PG_USHORT_MIN 0
#define PG_LUT_SIZE 4096
#define PG_DEGREE_TO_RADIANS 0.01746031f 
#define PG_RADIANS_TO_DEGREES 57.2727273f
#define PG_PI	3.1428571428571428571428571428571
#define PG_SQRT_TWO 1.4142135623730950488016887242097

//#define _TRACK_MEMORY 0

#define PG_USE_RGBA16 0

		
#define PG_RENDERING_IN_TYPE short
#if (PG_USE_RGBA16)
	#define PG_RENDERING_OUT_TYPE unsigned short
#else
	#define PG_RENDERING_OUT_TYPE unsigned char
#endif

static const char* kPgLoggerFileName = "SMIViewerLog.txt";
static const char* kPgApplicationName = "SMIViewer";
static const char* kPgRegistryPath = "Software\\Science.Medical.Imaging\\SMIViewer";
static const double	kPgEpsilon=0.00000001f;
static const double	kPgCoarseEpsilon=0.001f;
static const long double	kPgFineEpsilon=0.00000000001f;
static const float kPg3DTexBufferFactor = 16.0f;
static const int kPgBytesInKB = 1024;

typedef void (* StatusCallBackFunction) (double, std::string&, void *, void *);
typedef int (__stdcall *StatusCallBackFunctionD)(double);

typedef unsigned int (__stdcall ProcessInThread)(void *iParams);

//template <class T>
typedef float (*ObjectiveFunction) (void * iSourceModule, const std::vector<float>&, StatusCallBackFunction);
typedef std::vector<float> (*ObjectiveGradientFunction) (void * iSourceModule, const std::vector<float>&, const float & iLastValue, StatusCallBackFunction);

/*static PGCore::Logger* GetLogger() 
{
	return &pgLogger;
};*/

#define LOG0(fmt)				{ if (GetLogger()) GetLogger()->Log(fmt); } 
#define LOG1(fmt, x)			{ if (GetLogger()) GetLogger()->Log(fmt, x); } 
#define LOG2(fmt, x, y)			{ if (GetLogger()) GetLogger()->Log(fmt, x, y); } 
#define LOG3(fmt, x, y, z)      { if (GetLogger()) GetLogger()->Log(fmt, x, y, z); } 
#define LOG4(fmt, x, y, z, w)   { if (GetLogger()) GetLogger()->Log(fmt, x, y, z, w); } 

#define T_ROUND( x ) (T)(x+0.5f)	> (T)(x)	?  (T)(x+0.5f) : (T)(x) ;
#define I_ROUND( x ) (int)(x+0.5f)	> (int)(x)  ?  (int)(x+0.5f) : (int)(x); 
#define U_ROUND( x ) (U)(x+0.5f)	> (U)(x)	?  (U)(x+0.5f) : (U)(x) ;
#define MIN( a, b ) { return (a > b) ? b : a; }
#define MAX( a, b ) { return (a > b) ? a : b; }


static bool IN_CIRCLE( double DIAMETER, double ROW, double COL, double INNERRADIUS ) 
{ 
	bool res = 
		(   
		(
		( (double)ROW - (double)DIAMETER*0.5f ) * ( (double)ROW - (double)DIAMETER*0.5f ) +
		( (double)COL - (double)DIAMETER*0.5f ) * ( (double)COL - (double)DIAMETER*0.5f )
		)
		<= ( (double)(INNERRADIUS*INNERRADIUS) )  
		); 

	if (res)
	{
		res = true;
	}
	return res;
};


static bool IN_SQUARE( double DIAMETER, double ROW, double COL, double INNERRADIUS ) 
{ 
	bool res = 
		(   
		(
		( (double)ROW - (double)DIAMETER*0.5f ) * ( (double)ROW - (double)DIAMETER*0.5f ) 
		<= ( (double)(INNERRADIUS*INNERRADIUS) )
		&&
		( (double)COL - (double)DIAMETER*0.5f ) * ( (double)COL - (double)DIAMETER*0.5f )
		<= ( (double)(INNERRADIUS*INNERRADIUS) )
		)

		); 

	if (res)
	{
		res = true;
	}
	return res;
};



static bool IN_RANDOM( double DIAMETER, double ROW, double COL, double INNERRADIUS ) 
{ 
	int r = rand();
	bool res = r%15==0;



	if (res)
	{
		res = true;
	}
	return res;
};

template <class T>
static bool DrawFilledCircle(T* ioImage, int iRows, int iCols, T iRadius)
{
	if (!ioImage)
		return false;

	int i=0, j=0;
	for (j=0; j<iRows; j++)
	{
		for (i=0; i<iCols; i++)
		{
			if ( IN_CIRCLE(iCols, j, i, iRadius) )					
			{
				ioImage[j*iCols + i] = (T)255;
			} else 
			{
				ioImage[j*iCols + i] = (T)0;
			}
		}
	}
	return true;
}

template <class T>
bool IsPowerOfTwo(const T& iNumber, int &oPower)
{
	if (iNumber%2)
		return false;

	bool res = false;
	T testNum=1;
	int testPower=0;

	while (testNum<iNumber)
	{
		testPower++;
		testNum*=2;		
	}

	if (pow((float)2.0f, testPower) == iNumber)
	{
		oPower = testPower;
		res = true;
	}
	
	return res;
}

namespace PGCore
{
	static void ToUpper(std::string& ioSrcStr)
	{
		std::transform(ioSrcStr.begin(), ioSrcStr.end(), ioSrcStr.begin(), toupper);
	};
}

#endif