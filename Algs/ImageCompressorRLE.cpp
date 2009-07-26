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
// Filename:	ImageCompressorRLE.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageCompressorRLE_CPP_
#define _PGImageCompressorRLE_CPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageCompressorRLE.h"

namespace PGAlgs
{

	
template <class T, class U>
ImageCompressorRLE<T, U>::ImageCompressorRLE(void)
{ 
	m_range=0.0f;
};

template <class T, class U>
void ImageCompressorRLE<T, U>::SetQuantizationRange(const float& iRange) 
{ 
	m_range = iRange; 
};

template <class T, class U>
const float& ImageCompressorRLE<T, U>::GetQuantizationRange() const 
{ 
	return m_range; 
};

	
template <class T, class U>
ImageCompressorRLE<T, U>::~ImageCompressorRLE(void)
{ 	
}


template <class T, class U>
bool ImageCompressorRLE<T, U>::Execute() 
{
	// swap endians here
	//if (sizeof(T) != sizeof(U) || sizeof(T)!=sizeof(short)) {
	//	LOG2("ImageCompressorRLE: Error swapping endians. Types different sizes: %d - %d", sizeof(T), sizeof(U));
	//	return false;
	//}

	if (!m_iDataObject) 
	{
		LOG0("ImageCompressorRLE: Error: no input");
		return false;
	}
	
	// also check base image types here
	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) 
	{
		LOG0("ImageCompressorRLE: Error: failure to fetch input image");
		return false;
	}

	return m_Forward ? compress(inImage) : decompress(inImage);	
}


// private
template <class T, class U>
bool ImageCompressorRLE<T, U>::compress(PGCore::Image<T> *inImage)
{
	LOG0("ImageCompressorRLE<T, U>::compress: Not implemented!");
	return false;
}

template <>
bool ImageCompressorRLE<PGMath::Point3D<unsigned char>, unsigned char>::compress(PGCore::Image<PGMath::Point3D<unsigned char>> *inImage)
{
	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) 
	{
		LOG0("ImageCompressorRLE: Error: invalid image size");
		return false;
	}

	PGCore::PixelBase<PGMath::Point3D<unsigned char>> px;
	
	PGMath::Point3D<unsigned char> iMin = px.GetMaxValue(), iMax = px.GetMinValue();
	

	LOG0("{ ImageCompressorRLE: estimate range");
	bool rv = inImage->GetDataRange(&iMin, &iMax);
	if (!rv || iMax.Length()<=0)
	{
		LOG0("ImageCompressorRLE: Error: failed to estimate range");
		return false;
	}
	LOG0("} ImageCompressorRLE: estimate range");

	float invMaxX = 1.0f/float(iMax.X());


	//PGMath::Point3D<unsigned char> range = PGMath::Point3D<unsigned char>((iMax-iMin)*m_range);

	double m_rangeSq = m_range*m_range;
	//PGMath::Point3D<float> range = PGMath::Point3D<float>(m_range);
	//float rangeLen(range.Length());

	PGMath::Point3D<unsigned char>* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageCompressorRLE: Error: invalid image");
		return false;
	}
 
	std::vector<unsigned char> rleBuf;
	long totSize = iRows*iColumns;

	rleBuf.reserve(totSize*3*sizeof(unsigned char) + 2*sizeof(long));	
	
	// first store image dimensions
	unsigned char *rBuf = (unsigned char *)(&iRows);	
	for (int k=0; k<sizeof(iRows); k++)
	{
		rleBuf.push_back(rBuf[k]);
	}

	rBuf = (unsigned char *)&iColumns;
	for (int k=0; k<sizeof(iColumns); k++)
	{
		rleBuf.push_back(rBuf[k]);
	}

	LOG0("{ ImageCompressorRLE: core loop");

#define _PG_PROCESS_4ROWS_TGETHER_ 1
#if (_PG_PROCESS_4ROWS_TGETHER_)
	long freq[4]={ 0, 0, 0, 0 };
	PGMath::Point3D<unsigned char> curValue[4] = { px.GetMinValue(), px.GetMinValue(), px.GetMinValue(), px.GetMinValue() };
	PGMath::Point3D<unsigned char> nextValue[4] = { px.GetMaxValue(), px.GetMaxValue(), px.GetMaxValue(), px.GetMaxValue() };
#else
	long freq=0;
	PGMath::Point3D<unsigned char> curValue = px.GetMinValue();//[4] = { px.GetMinValue(), px.GetMinValue(), px.GetMinValue(), px.GetMinValue() };
	PGMath::Point3D<unsigned char> nextValue = px.GetMaxValue();//[4] = { px.GetMaxValue(), px.GetMaxValue(), px.GetMaxValue(), px.GetMaxValue() };
#endif

	int rowSize_4 = iColumns*4;

	int num_4_Rows = iRows/4;
	int remainderRows = iRows%4;// - (4*num_4_Rows);
	long i = 0, j=0, rowIndex_4=0, colIndex=0;

#if (_PG_PROCESS_4ROWS_TGETHER_)
	while (i<num_4_Rows)
	{
		rowIndex_4 = i*rowSize_4;

		j = 0;	
		std::vector<unsigned char> rleBufTemp[4];

		for (int ll=0; ll<4; ll++)
		{
			rleBufTemp[ll].reserve(iColumns);
			curValue[ll] = px.GetMinValue();
			nextValue[ll] = px.GetMaxValue();
			freq[ll] = 0;
		}

		while (j<iColumns)
		{
			// get next value
			colIndex = rowIndex_4 + j;				

			for (int k=0; k<=3; k++, colIndex += iColumns)
			{
				nextValue[k] = inBuf[colIndex];

				PGMath::Point3D<unsigned char> mn = PGCore::Functional<PGMath::Point3D<unsigned char>>::Min(nextValue[k], curValue[k]);
				PGMath::Point3D<unsigned char> mx = PGCore::Functional<PGMath::Point3D<unsigned char>>::Max(nextValue[k], curValue[k]);

				// assert(iMax.X()>0);		

				PGMath::Point3D<float> mnd = (float(mn.X())*invMaxX, float(mn.Y())*invMaxX, float(mn.Z())*invMaxX);
				PGMath::Point3D<float> mxd = (float(mx.X())*invMaxX, float(mx.Y())*invMaxX, float(mx.Z())*invMaxX);

				PGMath::Point3D<float> mxdiff((mxd-mnd));

				//if (mxdiff.LengthSquared() > m_rangeSq || colIndex==(totSize-1) || (nextValue.LengthSquared()==0 && curValue.LengthSquared()))
				if (mxdiff.LengthSquared() > m_rangeSq /*|| j==(iColumns-1)*/ || (nextValue[k].LengthSquared()==0 && curValue[k].LengthSquared()))
				{
					if (freq[k])
					{
						rleBufTemp[k].push_back(curValue[k].X());
						rleBufTemp[k].push_back(curValue[k].Y());
						rleBufTemp[k].push_back(curValue[k].Z());

						unsigned char* rBuf = (unsigned char *)&(freq[k]);
						for (int kk=0; kk<sizeof(freq[k]); kk++)
						{
							// rleBuf.push_back(rBuf[kk]);
							rleBufTemp[k].push_back(rBuf[kk]);
						}
					}

					curValue[k] = nextValue[k];
					freq[k]=1;		
				} else if (j<(iColumns))//(mxdiff<=range)
				{
					freq[k]++;
				}

			}

			j++;
		}
		
		for (int ll=0; ll<4; ll++)
		{
			if (j==(iColumns))
			{
				if (freq[ll])
				{
					rleBufTemp[ll].push_back(curValue[ll].X());
					rleBufTemp[ll].push_back(curValue[ll].Y());
					rleBufTemp[ll].push_back(curValue[ll].Z());

					unsigned char* rBuf = (unsigned char *)&(freq[ll]);
					for (int kk=0; kk<sizeof(freq[ll]); kk++)
					{
						// rleBuf.push_back(rBuf[kk]);
						rleBufTemp[ll].push_back(rBuf[kk]);
					}
				}				
			}
			rleBuf.insert(rleBuf.end(), rleBufTemp[ll].begin(), rleBufTemp[ll].end());
		}
		
		i++;
	}


	i = num_4_Rows*rowSize_4;
	int k = 0;
	curValue[0] = px.GetMinValue();
	nextValue[0] = px.GetMaxValue();
	freq[0] = 0;
	
	while (i<totSize)
	{
		// get next value
		nextValue[k] = inBuf[i];
		
		PGMath::Point3D<unsigned char> mn = PGCore::Functional<PGMath::Point3D<unsigned char>>::Min(nextValue[k], curValue[k]);
		PGMath::Point3D<unsigned char> mx = PGCore::Functional<PGMath::Point3D<unsigned char>>::Max(nextValue[k], curValue[k]);
		 
		// assert(iMax.X()>0);		

		PGMath::Point3D<float> mnd = (float(mn.X())*invMaxX, float(mn.Y())*invMaxX, float(mn.Z())*invMaxX);
		PGMath::Point3D<float> mxd = (float(mx.X())*invMaxX, float(mx.Y())*invMaxX, float(mx.Z())*invMaxX);
		

		PGMath::Point3D<float> mxdiff((mxd-mnd));
		 
		if (mxdiff.LengthSquared() > m_rangeSq || i==(totSize-1) || (nextValue[k].LengthSquared()==0 && curValue[k].LengthSquared()))
		{
			if (freq[k])
			{
				rleBuf.push_back(curValue[k].X());
				rleBuf.push_back(curValue[k].Y());
				rleBuf.push_back(curValue[k].Z());

				unsigned char* rBuf = (unsigned char *)&(freq[k]);
				for (int kk=0; kk<sizeof(freq[kk]); kk++)
				{
					rleBuf.push_back(rBuf[kk]);
				}
			}

			curValue[k] = nextValue[k];
			freq[k]=1;		
		} else if (i<(totSize-1))//(mxdiff<=range)
		{
			freq[k]++;
		}		

		i++;
	}
#else
	
	while (i<totSize)
	{
		// get next value
		nextValue = *inBuf++;
		
		PGMath::Point3D<unsigned char> mn = PGCore::Functional<PGMath::Point3D<unsigned char>>::Min(nextValue, curValue);
		PGMath::Point3D<unsigned char> mx = PGCore::Functional<PGMath::Point3D<unsigned char>>::Max(nextValue, curValue);
		 
		assert(iMax.X()>0);		
		PGMath::Point3D<float> mnd = (float(mn.X())*invMaxX, float(mn.Y())*invMaxX, float(mn.Z())*invMaxX);
		PGMath::Point3D<float> mxd = (float(mx.X())*invMaxX, float(mx.Y())*invMaxX, float(mx.Z())*invMaxX);
		

		PGMath::Point3D<float> mxdiff((mxd-mnd));
		
		if (mxdiff.LengthSquared() > m_rangeSq || i==(totSize-1) || (nextValue.LengthSquared()==0 && curValue.LengthSquared()))
		{
			if (freq)
			{
				rleBuf.push_back(curValue.X());
				rleBuf.push_back(curValue.Y());
				rleBuf.push_back(curValue.Z());

				unsigned char* rBuf = (unsigned char *)&freq;
				for (int k=0; k<sizeof(freq); k++)
				{
					rleBuf.push_back(rBuf[k]);
				}
			}

			curValue = nextValue;
			freq=1;		
		} else if (i<(totSize-1))//(mxdiff<=range)
		{
			freq++;
		}

		i++;
	}
#endif

	LOG0("} ImageCompressorRLE: core loop");

	long oR = 1;
	long oC = rleBuf.size();

	if (m_oImageObject && m_oImageObject->GetDataSize()!=oR*oC*sizeof(unsigned char))
	{
		if (m_oImageObject) 
		{
			delete m_oImageObject;
			m_oImageObject = 0;
		};	
	}

	if (m_oImageObject == NULL)	
	{
		m_oImageObject = new PGCore::Image<unsigned char>(oR, oC);
	}

	unsigned char* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageCompressorRLE: Error: invalid out image");
		return false;
	}

	memcpy(outBuf, &(rleBuf[0]), oC);

	rleBuf.clear();

	//PGAlgs::DumpImageAsPGM(*m_oImageObject, "C:\\Tmp\\sliceTest\\Image_InternalDBL.pgm");
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}


template <>
bool ImageCompressorRLE<unsigned char, PGMath::Point3D<unsigned char>>::decompress(PGCore::Image<unsigned char> *inImage)
{
	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=2) // need at least dimensions 
	{
		LOG0("ImageCompressorRLE: Error: invalid image size");
		return false;
	}

	// first read in dimensions
	long oR = 0;
	long oC = 0;

	const unsigned char* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageCompressorRLE: Error: invalid image");
		return false;
	}

	unsigned char* rBuf = (unsigned char *)&oR, *cBuf = (unsigned char *)&oC;
	for (int k=0; k<sizeof(oR); k++)
	{
		rBuf[k] = inBuf[k];
		cBuf[k] = inBuf[k+sizeof(oR)];
	}

	if (m_oImageObject && m_oImageObject->GetDataSize()!=oR*oC*sizeof(PGMath::Point3D<unsigned char>))
	{
		if (m_oImageObject) 
		{
			delete m_oImageObject;
			m_oImageObject = 0;
		};	
	}

	if (m_oImageObject == NULL)	m_oImageObject = new PGCore::Image<PGMath::Point3D<unsigned char>>(oR, oC);
	PGMath::Point3D<unsigned char>* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageCompressorRLE: Error: invalid out image");
		return false;
	}
	

	PGCore::PixelBase<PGMath::Point3D<unsigned char>> px;

	long i=0, freq=0, totSize=iRows*iColumns-2*sizeof(long), l=0;
	PGMath::Point3D<unsigned char> curValue = px.GetMinValue(), nextValue = px.GetMaxValue();

	unsigned char* iBuf = (unsigned char *)(inBuf+2*sizeof(long)); // skip dimensions
	while (i<totSize && l<oR*oC)
	{
		// get next value
		nextValue = PGMath::Point3D<unsigned char>(*iBuf, *(iBuf+1), *(iBuf+2), 255); // actual value
		iBuf+=3;
		i+=3; 
		
		// now the freq
		freq = 0;
		unsigned char* fBuf = (unsigned char *)&freq;
		for (int k=0; k<sizeof(freq); k++)
		{
			fBuf[k] = *iBuf++;			
		}
		i+=sizeof(freq);
		
		long j=0;	
		while (j<freq)
		{
			if (l>=oR*oC) break;

			*outBuf++ = nextValue;
			j++; l++;		
		}			
	} 

	if (l!=oR*oC)
	{
		LOG0("ImageCompressorRLE: Error: not all pixels written!");
	}

	//PGAlgs::DumpImageAsPGM(*m_oImageObject, "C:\\Tmp\\sliceTest\\Image_InternalDBL.pgm");
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}


template <class T, class U>
bool ImageCompressorRLE<T, U>::decompress(PGCore::Image<T> *inImage)
{
	LOG0("ImageCompressorRLE<T, U>::decompress: Not implemented!");
	return false;
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageCompressorRLE<short, short>; 
	template class ImageCompressorRLE<unsigned char, unsigned char>;
	template class ImageCompressorRLE<PGMath::Point3D<unsigned char>, PGMath::Point3D<unsigned char>>;	
	template class ImageCompressorRLE<PGMath::Point3D<unsigned char>, unsigned char>;	
	template class ImageCompressorRLE<unsigned char, PGMath::Point3D<unsigned char>>;	
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageCompressorRLE_CPP_
