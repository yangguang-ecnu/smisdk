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
// Filename:	DrawHelper.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
//////////////////////////////////////////////////////////////////////////

#include "Core/DrawHelper.h"

namespace PGCore
{
	namespace DrawHelper
	{
		// works only on square images for now
		template <class T>
		static bool DrawCircleOverlaid(int iXc, int iYc, int iRadius, PGCore::Image<T> & ioImage, const float iValue/*=-1*//*, bool iClean=false*/)
		{
			T* ioBuf = ioImage.GetBuffer();
			if (!ioBuf) return false;

			long iRows=0, iCols=0;

			bool res = ioImage.GetDimensions(iRows, iCols);
			if (!res || iCols!=iRows) return false;

			if (iRadius*2 > iCols)
			{
				return false;
			}
			
			float maxVal = iValue;

			if (iValue<0.0f || maxVal>1.0f)
			{
				maxVal=1;
			}

			if (iCols==3 && iRadius==1)
			{
				ioBuf[1] *= maxVal;
				ioBuf[3] *= maxVal; ioBuf[4] *= maxVal; ioBuf[5] *= maxVal;
				ioBuf[7] *= maxVal;

				return true;

			} else
				if (iCols==5 && iRadius==2)
				{
					ioBuf[2] *= maxVal;
					ioBuf[6] *= maxVal;	ioBuf[7] *= maxVal;	 ioBuf[8] *= maxVal;
					ioBuf[10] *= maxVal;	ioBuf[11] *= maxVal;	ioBuf[12] *= maxVal;   ioBuf[13] *= maxVal;   ioBuf[14] *= maxVal;
					ioBuf[16] *= maxVal;	ioBuf[17] *= maxVal;	 ioBuf[18] *= maxVal;
					ioBuf[22] *= maxVal;			
					return true;
				}


				int x=0, y=iRadius;				
				
				long index = iCols*iYc + iXc, maxIndex = iCols*iRows - 1;

				//if (index<maxIndex) ioBuf[index] = maxVal;

				double p=1-iRadius;

				while(x<=y)
				{
					if(p<0)
					{
						x=x+1;
						p=p+2*x+1;
					}
					else
					{
						x=x+1;
						y=y-1;
						p=p+2*(x-y)+1;
					}

					index = iCols*(x+iXc) + (y+iYc);
					if (index<maxIndex)	ioBuf[index] *= maxVal;			

					index = iCols*(-x+iXc) + (y+iYc);
					if (index<maxIndex)	ioBuf[index] *= maxVal;			

					index = iCols*(x+iXc) + (-y+iYc);
					if (index<maxIndex)	ioBuf[index] *= maxVal;			

					index = iCols*(-x+iXc) + (-y+iYc);
					if (index<maxIndex)	ioBuf[index] *= maxVal;		

					// other halves
					index = iCols*(y+iYc) + (x+iXc);
					if (index<maxIndex)	ioBuf[index] *= maxVal;			

					index = iCols*(-y+iYc) + (x+iXc);
					if (index<maxIndex)	ioBuf[index] *= maxVal;			

					index = iCols*(y+iYc) + (-x+iXc);
					if (index<maxIndex)	ioBuf[index] *= maxVal;			

					index = iCols*(-y+iYc) + (-x+iXc);
					if (index<maxIndex)	ioBuf[index] *= maxVal;		
				}
				return true;
		}

		template <class T>
		static bool DrawBox(int iXLeftTop, int iYLeftTop, int iXRightBottom, int iYRightBottom, PGCore::Image<T> & ioImage, const T iValue/*=-1*//*, bool iClean=false*/)
		{
			T* ioBuf = ioImage.GetBuffer();
			if (!ioBuf) return false;

			long iRows=0, iCols=0;

			bool res = ioImage.GetDimensions(iRows, iCols);
			if (!res || iCols!=iRows) return false;

			if (iXLeftTop<0 || iXLeftTop>iCols-1 || iYLeftTop<0 || iYLeftTop>iRows-1)
			{
				return false;
			}

			if (iXRightBottom<0 || iXRightBottom>iCols-1 || iYRightBottom<0 || iYRightBottom>iRows-1)
			{
				return false;
			}
						
			
			T maxVal = iValue;

			if (iValue==-1)
			{
				PixelBase<T> tPixel;
				T minVali=tPixel.GetMaxValue(), maxVali=tPixel.GetMinValue();
				ioImage.GetDataRange(&minVali, &maxVali);
				maxVal=maxVali;
			}

			// top line
			// bottom line
			for (int x=iXLeftTop; x<=iXRightBottom; x++)
			{
				ioImage.SetValue(x, iYLeftTop, maxVal);
				ioImage.SetValue(x, iYRightBottom, maxVal);
			}

			// left line
			for (int y=iYLeftTop; y<=iYRightBottom; y++)
			{
				ioImage.SetValue(iXLeftTop, y, maxVal);
				ioImage.SetValue(iXRightBottom, y, maxVal);
			}

			return true;			
		}
	

		template <class T>
		static bool DrawCircle(int iXc, int iYc, int iRadius, PGCore::Image<T> & ioImage, const T iValue/*=-1*//*, bool iClean=false*/)
		{
			T* ioBuf = ioImage.GetBuffer();
			if (!ioBuf) return false;

			long iRows=0, iCols=0;

			bool res = ioImage.GetDimensions(iRows, iCols);
			if (!res || iCols!=iRows) return false;

			if (iRadius*2 > iCols)
			{
				return false;
			}

			
			
			/*if (iClean) 
			{
				PixelBase<T> tPixel;
				ioImage.Reset(tPixel.GetMinValue());
				maxVal=1;
			}*/


			//memset(ioBuf, 0, iRows*iCols*sizeof(char));
			T maxVal = iValue;//, minVal=0;//tPixel.GetMaxValue();

			if (iValue==-1)
			{
				PixelBase<T> tPixel;
				T minVali=tPixel.GetMaxValue(), maxVali=tPixel.GetMinValue();
				ioImage.GetDataRange(&minVali, &maxVali);
				maxVal=maxVali;
			}

			if (iCols==3 && iRadius==1)
			{
				ioBuf[1] = maxVal;
				ioBuf[3] = maxVal; ioBuf[4] = maxVal; ioBuf[5] = maxVal;
				ioBuf[7] = maxVal;

				return true;

			} else
				if (iCols==5 && iRadius==2)
				{
					ioBuf[2] = maxVal;
					ioBuf[6] = maxVal;	ioBuf[7] = maxVal;	 ioBuf[8] = maxVal;
					ioBuf[10] = maxVal;	ioBuf[11] = maxVal;	ioBuf[12] = maxVal;   ioBuf[13] = maxVal;   ioBuf[14] = maxVal;
					ioBuf[16] = maxVal;	ioBuf[17] = maxVal;	 ioBuf[18] = maxVal;
					ioBuf[22] = maxVal;			
					return true;
				}


				int x=0, y=iRadius;				
				
				long index = iCols*iYc + iXc, maxIndex = iCols*iRows - 1;

				//if (index<maxIndex) ioBuf[index] = maxVal;

				double p=1-iRadius;

				while(x<=y)
				{
					if(p<0)
					{
						x=x+1;
						p=p+2*x+1;
					}
					else
					{
						x=x+1;
						y=y-1;
						p=p+2*(x-y)+1;
					}

					index = iCols*(x+iXc) + (y+iYc);
					if (index<maxIndex)	ioBuf[index] = maxVal;			

					index = iCols*(-x+iXc) + (y+iYc);
					if (index<maxIndex)	ioBuf[index] = maxVal;			

					index = iCols*(x+iXc) + (-y+iYc);
					if (index<maxIndex)	ioBuf[index] = maxVal;			

					index = iCols*(-x+iXc) + (-y+iYc);
					if (index<maxIndex)	ioBuf[index] = maxVal;		

					// other halves
					index = iCols*(y+iYc) + (x+iXc);
					if (index<maxIndex)	ioBuf[index] = maxVal;			

					index = iCols*(-y+iYc) + (x+iXc);
					if (index<maxIndex)	ioBuf[index] = maxVal;			

					index = iCols*(y+iYc) + (-x+iXc);
					if (index<maxIndex)	ioBuf[index] = maxVal;			

					index = iCols*(-y+iYc) + (-x+iXc);
					if (index<maxIndex)	ioBuf[index] = maxVal;		
				}
				return true;
		}
	};

#ifdef _PG_GENERATE_SDK_LIBS_
	//PGCore::DrawHelper::DrawBox(0, 0, 0, 0, PGCore::Image<short>(), short(255));
	//template class IODicom3D<short>; 
	//template class IODicom3D<unsigned char>; 
#endif

};
//////////////////////////////////////////////////////////////////////////
// EOF
