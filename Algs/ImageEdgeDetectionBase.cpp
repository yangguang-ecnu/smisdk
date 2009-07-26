// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	ImageEdgeDetectionBase.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

/////////////////////////////////////////////////////////////////////////


#include "Algs/ImageEdgeDetectionBase.h"
#include "Core/GaussianKernel.h"
#include "Core/DiffOfGaussianDerivativeKernel.h"
#include "Core/DiffOfGaussianKernel.h"

namespace PGAlgs
{

	template <class T, class U>
	ImageEdgeDetectionBase<T, U>::ImageEdgeDetectionBase(double iSigma/*=1.0f*/)
	{ 
		m_oImageObject = 0; m_sigma = iSigma;		
		m_loTh = 0.1f;
		m_hiTh = 0.9f;
	};

	template <class T, class U>
	ImageEdgeDetectionBase<T, U>::~ImageEdgeDetectionBase(void)
	{ 
		if (m_oImageObject) 
		{
			delete m_oImageObject;
			m_oImageObject = 0;
		}
	};


	template <class T, class U>
	bool ImageEdgeDetectionBase<T, U>::Execute() {

		if (!m_iDataObject) {
			LOG0("ImageEdgeDetectionBase: Error: no input");
			return false;
		}

		// also check base image types here

		// get first image
		PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
		if (!inImage) {
			LOG0("ImageEdgeDetectionBase: Error: failure to fetch input image");
			return false;
		}

		long iRows=0, iColumns=0;
		inImage->GetDimensions(iRows, iColumns);
		if (iRows*iColumns <=0) {
			LOG0("ImageEdgeDetectionBase: Error: invalid image size");
			return false;
		}

		const T* inBuf = inImage->GetBuffer();
		if (!inBuf) 
		{
			LOG0("ImageEdgeDetectionBase: Error: invalid image");
			return false;
		}

		if (m_oImageObject == NULL)
			m_oImageObject = new PGCore::Image<U>(iRows, iColumns);
		U* outBuf = m_oImageObject->GetBuffer();
		if (!outBuf) 
		{
			LOG0("ImageEdgeDetectionBase: Error: invalid out image");
			return false;
		}

		bool rv = detectEdges(m_sigma, m_loTh, m_hiTh, inImage, m_oImageObject);	

		m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

		return rv;
	}

	template <class T, class U>
	void ImageEdgeDetectionBase<T, U>::SetHysterisisThresholds(const float& iLowTh, const float& iHighTh)
	{
		m_loTh = iLowTh;
		m_hiTh = iHighTh;
	}

	template <class T, class U>
	bool ImageEdgeDetectionBase<T, U>::detectEdges(float iSigma, /* Standard deviation of the gaussian kernel. */
		float iTlow,  /* Fraction of the high threshold in hysteresis. */
		float iThigh,
		PGCore::Image<T> *iInImage,
		PGCore::Image<U> *oEdgeImage //must be allocated outside
		)
	{
		if (!iInImage || !oEdgeImage)
			return false;

		//iOutImage = iInImage;
		//char composedfname[128];  /* Name of the output "direction" image */
		T *image = (T *)iInImage->GetBuffer();     /* The input image */
		U *edge  = (U *)oEdgeImage->GetBuffer();      /* The output edge image */
		//int rows , cols;           /* The dimensions of the image. */
		//float sigma,              /* Standard deviation of the gaussian kernel. */
		//	tlow,               /* Fraction of the high threshold in hysteresis. */
		//	thigh;              
		/* High hysteresis threshold control. The actual
								threshold is the (100 * thigh) percentage point
								in the histogram of the magnitude of the
								gradient image that passes non-maximal
								suppression. */

		/****************************************************************************
		* Get the command line arguments.
		****************************************************************************/

		//infilename = argv[1];
		//sigma = iSigma;//atof(argv[2]);
		//tlow = iTlow;//atof(argv[3]);
		//thigh = iThigh;//atof(argv[4]);

		long rows, cols;
		iInImage->GetDimensions(rows, cols);
		//rows = (int)lrows;
		//cols = (int)lcols;

		/****************************************************************************
		* Perform the edge detection. All of the work takes place here.
		****************************************************************************/



		//void canny(unsigned char *image, int rows, int cols, float sigma,
		//       float tlow, float thigh, unsigned char **edge, char *fname)
		//{
		//FILE *fpdir=NULL;          /* File to write the gradient image to.     */
		//U *nms;        /* Points that are local maximal magnitude. */
		T *smoothedim;     /* The image after gaussian smoothing.      */
		T *delta_x,        /* The first devivative image, x-direction. */
			*delta_y,        /* The first derivative image, y-direction. */
			*magnitude;      /* The magnitude of the gadient image.      */
		//int r, c, pos;
		//float *dir_radians=NULL;   /* Gradient direction image.                */
#ifdef _DEBUG
		PGAlgs::DumpImageAsPGM(*iInImage, std::string("C:\\Tmp\\Dump\\OriginalImage.pgm"));
#endif

		/****************************************************************************
		* Perform gaussian smoothing on the image using the input standard
		* deviation.
		****************************************************************************/
		//if(0) printf("Smoothing the image using a gaussian kernel.\n");
		//gaussian_smooth(image, rows, cols, sigma, &smoothedim);
		PGCore::Image<T> smoothedImage(rows, cols);
		PGCore::GaussianKernel<T, T> gausskernel(iSigma, 3);
		gausskernel.Convolve(*iInImage, smoothedImage);
		smoothedim = (T *)smoothedImage.GetBuffer();

#ifdef _DEBUG
		PGAlgs::DumpImageAsPGM(smoothedImage, std::string("C:\\Tmp\\Dump\\SmoothImage.pgm"));
#endif

		/****************************************************************************
		* Compute the first derivative in the x and y directions.
		****************************************************************************/
		//if(0) printf("Computing the X and Y first derivatives.\n");
		//derrivative_x_y(smoothedim, rows, cols, &delta_x, &delta_y);
		PGCore::DerivativeOfGaussianKernel<T, T> derivOfGaussian(iSigma);		
		PGCore::DiffOfGaussianKernel<T, T> dogFilter(iSigma+0.1f, iSigma);
		PGCore::Image<T> oImageDerivG(rows, cols), oImageDerivGX(rows, cols), oImageDerivGY(rows, cols);
		// X gradient
		if (1)
		{
			//gausskernel.Convolve1D_Y(smoothedImage, oImageDerivG);
			//derivOfGaussian.Convolve1D_X(oImageDerivG, oImageDerivGX);
			dogFilter.Convolve1D_X(smoothedImage, oImageDerivGX);
			delta_x = (T *)oImageDerivGX.GetBuffer();
#ifdef _DEBUG
			PGAlgs::DumpImageAsPGM(oImageDerivGX, std::string("C:\\Tmp\\Dump\\GradientX.pgm"));
#endif
		}
		// Y gradient
		if (1)
		{
			//gausskernel.Convolve1D_X(smoothedImage, oImageDerivG);
			//derivOfGaussian.Convolve1D_Y(oImageDerivG, oImageDerivGY);
			dogFilter.Convolve1D_Y(smoothedImage, oImageDerivGY);
			delta_y = (T *)oImageDerivGY.GetBuffer();
#ifdef _DEBUG
			PGAlgs::DumpImageAsPGM(oImageDerivGY, std::string("C:\\Tmp\\Dump\\GradientY.pgm"));
#endif
		}


		/****************************************************************************
		* Compute the magnitude of the gradient.
		****************************************************************************/
		//if(0) printf("Computing the magnitude of the gradient.\n");
		//magnitude_x_y(delta_x, delta_y, rows, cols, &magnitude);
		PGCore::Image<T> gradientMagntudeImage(rows, cols);
		magnitude = (T *)gradientMagntudeImage.GetBuffer();
		{				
			long imgIter = 0;				
			while (imgIter< rows*cols) 
			{
				double inValX = *(delta_x+imgIter);
				double inValY = *(delta_y+imgIter);
				//double inVal = fabs(inValX) + fabs(inValY);//inValX>inValY ? inValX : inValY;
				double inVal = sqrt((inValX)*(inValX) + (inValY)*(inValY));//inValX>inValY ? inValX : inValY;

				T outVal = (T)((inVal + 0.5f));
				*(magnitude+imgIter) = outVal;			
				imgIter++;

			}		
		} 	

#ifdef _DEBUG
		PGAlgs::DumpImageAsPGM(gradientMagntudeImage, std::string("C:\\Tmp\\Dump\\GradientMagnitude.pgm"));
#endif

		/****************************************************************************
		* Perform non-maximal suppression.
		****************************************************************************/

		PGCore::Image<U> nonMaxSuppressedImage(rows, cols);
		U* nms = (U *)nonMaxSuppressedImage.GetBuffer();
		if(!nms)
		{
			return false;
		}

		bool rv = nonMaximumSuppression(magnitude, delta_x, delta_y, rows, cols, nms);
		if (!rv) 
			return false;
#ifdef _DEBUG
		PGAlgs::DumpImageAsPGM(nonMaxSuppressedImage, std::string("C:\\Tmp\\Dump\\NonMaxSuppressed.pgm"));
#endif

		/****************************************************************************
		* Use hysteresis to mark the edge pixels.
		****************************************************************************/
		//if(0) printf("Doing hysteresis thresholding.\n");
		/*
		if((*edge=(unsigned char *)calloc(rows*cols,sizeof(unsigned char))) ==NULL){
		fprintf(stderr, "Error allocating the edge image.\n");
		exit(1);
		}*/

		//error in here somewhere

		rv = applyHysteresis(magnitude, nms, rows, cols, iTlow, iThigh, edge);
		if (!rv)
		{
			return false;
		}
#ifdef _DEBUG
		PGAlgs::DumpImageAsPGM(*oEdgeImage, std::string("C:\\Tmp\\Dump\\Edges.pgm"));
#endif
		/****************************************************************************
		* Free all of the memory that we allocated except for the edge image that
		* is still being used to store out result.
		****************************************************************************/
		//free(smoothedim);
		//free(delta_x);
		//free(delta_y);
		//free(magnitude);
		//free(nms);

		return rv;
	}




	/*******************************************************************************
	* PROCEDURE: nonMaximumSuppression
	* PURPOSE: This routine applies non-maximal suppression to the magnitude of
	* the gradient image.
	* NAME: Mike Heath
	* DATE: 2/15/96
	*******************************************************************************/
	template <class T, class U>
	bool ImageEdgeDetectionBase<T, U>::nonMaximumSuppression(
		T *mag, T *gradx, T *grady, int nrows, int ncols,
		U *result) 
	{
		if (!mag || !gradx || !grady || !result)
			return false;

		int rowcount, colcount,count;
		T *magrowptr,*magptr;
		T *gxrowptr,*gxptr;
		T *gyrowptr,*gyptr,z1,z2;
		T m00,gx,gy;
		float mag1,mag2,xperp,yperp;
		U *resultrowptr, *resultptr;


		/****************************************************************************
		* Zero the edges of the result image.
		****************************************************************************/
		for(count=0,resultrowptr=result,resultptr=result+ncols*(nrows-1); 
			count<ncols; resultptr++,resultrowptr++,count++){
				*resultrowptr = *resultptr = (U) 0;
		}

		for(count=0,resultptr=result,resultrowptr=result+ncols-1;
			count<nrows; count++,resultptr+=ncols,resultrowptr+=ncols){
				*resultptr = *resultrowptr = (U) 0;
		}

		/****************************************************************************
		* Suppress non-maximum points.
		****************************************************************************/
		for(rowcount=1,magrowptr=mag+ncols+1,gxrowptr=gradx+ncols+1,
			gyrowptr=grady+ncols+1,resultrowptr=result+ncols+1;
			rowcount<nrows-2; 
		rowcount++,magrowptr+=ncols,gyrowptr+=ncols,gxrowptr+=ncols,
			resultrowptr+=ncols){   
				for(colcount=1,magptr=magrowptr,gxptr=gxrowptr,gyptr=gyrowptr,
					resultptr=resultrowptr;colcount<ncols-2; 
					colcount++,magptr++,gxptr++,gyptr++,resultptr++){   
						m00 = *magptr;
						if(m00 == 0){
							gx = -1;
							gy = -1;
							xperp = 0;
							yperp = 0;
							*resultptr = (U) kPgNOEDGE;
						}
						else{
							gx = *gxptr;
							gy = *gyptr;
							xperp = -(gx)/((float)m00);
							yperp = (gy)/((float)m00);
						}

						if(gx >= 0){
							if(gy >= 0){
								if (gx >= gy)
								{  
									/* 111 */
									/* Left point */
									z1 = *(magptr - 1);
									z2 = *(magptr - ncols - 1);

									mag1 = (m00 - z1)*xperp + (z2 - z1)*yperp;

									/* Right point */
									z1 = *(magptr + 1);
									z2 = *(magptr + ncols + 1);

									mag2 = (m00 - z1)*xperp + (z2 - z1)*yperp;
								}
								else
								{    
									/* 110 */
									/* Left point */
									z1 = *(magptr - ncols);
									z2 = *(magptr - ncols - 1);

									mag1 = (z1 - z2)*xperp + (z1 - m00)*yperp;

									/* Right point */
									z1 = *(magptr + ncols);
									z2 = *(magptr + ncols + 1);

									mag2 = (z1 - z2)*xperp + (z1 - m00)*yperp; 
								}
							}
							else
							{
								if (gx >= -gy)
								{
									/* 101 */
									/* Left point */
									z1 = *(magptr - 1);
									z2 = *(magptr + ncols - 1);

									mag1 = (m00 - z1)*xperp + (z1 - z2)*yperp;

									/* Right point */
									z1 = *(magptr + 1);
									z2 = *(magptr - ncols + 1);

									mag2 = (m00 - z1)*xperp + (z1 - z2)*yperp;
								}
								else
								{    
									/* 100 */
									/* Left point */
									z1 = *(magptr + ncols);
									z2 = *(magptr + ncols - 1);

									mag1 = (z1 - z2)*xperp + (m00 - z1)*yperp;

									/* Right point */
									z1 = *(magptr - ncols);
									z2 = *(magptr - ncols + 1);

									mag2 = (z1 - z2)*xperp  + (m00 - z1)*yperp; 
								}
							}
						}
						else
						{
							if ((gy = *gyptr) >= 0)
							{
								if (-gx >= gy)
								{          
									/* 011 */
									/* Left point */
									z1 = *(magptr + 1);
									z2 = *(magptr - ncols + 1);

									mag1 = (z1 - m00)*xperp + (z2 - z1)*yperp;

									/* Right point */
									z1 = *(magptr - 1);
									z2 = *(magptr + ncols - 1);

									mag2 = (z1 - m00)*xperp + (z2 - z1)*yperp;
								}
								else
								{
									/* 010 */
									/* Left point */
									z1 = *(magptr - ncols);
									z2 = *(magptr - ncols + 1);

									mag1 = (z2 - z1)*xperp + (z1 - m00)*yperp;

									/* Right point */
									z1 = *(magptr + ncols);
									z2 = *(magptr + ncols - 1);

									mag2 = (z2 - z1)*xperp + (z1 - m00)*yperp;
								}
							}
							else
							{
								if (-gx > -gy)
								{
									/* 001 */
									/* Left point */
									z1 = *(magptr + 1);
									z2 = *(magptr + ncols + 1);

									mag1 = (z1 - m00)*xperp + (z1 - z2)*yperp;

									/* Right point */
									z1 = *(magptr - 1);
									z2 = *(magptr - ncols - 1);

									mag2 = (z1 - m00)*xperp + (z1 - z2)*yperp;
								}
								else
								{
									/* 000 */
									/* Left point */
									z1 = *(magptr + ncols);
									z2 = *(magptr + ncols + 1);

									mag1 = (z2 - z1)*xperp + (m00 - z1)*yperp;

									/* Right point */
									z1 = *(magptr - ncols);
									z2 = *(magptr - ncols - 1);

									mag2 = (z2 - z1)*xperp + (m00 - z1)*yperp;
								}
							}
						} 

						/* Now determine if the current point is a maximum point */

						if ((mag1 > 0.0) || (mag2 > 0.0))
						{
							*resultptr = (U) kPgNOEDGE;
						}
						else
						{    
							//if (mag2 == 0.0)
							//	*resultptr = (U) kPgNOEDGE;
							//else
								*resultptr = (U) kPgPOSSIBLE_EDGE;
						}
				} 
		}
		return true;
	}



	/*******************************************************************************
	* PROCEDURE: applyHysteresis
	* PURPOSE: This routine finds edges that are above some high threshhold or
	* are connected to a high pixel by a path of pixels greater than a low
	* threshold.
	* NAME:
	* DATE: 
	*******************************************************************************/
	template <class T, class U>
	bool ImageEdgeDetectionBase<T, U>::applyHysteresis(T *mag, T *nms, int rows, int cols,
		float tlow, float thigh, U*edge)
	{
		int r, c, pos, numedges, /*lowcount,*/ highcount, lowthreshold, highthreshold,
			//i, 
			hist[32768];//, rr, cc;
		T maximum_mag;//, sumpix;

		/****************************************************************************
		* Initialize the edge map to possible edges everywhere the non-maximal
		* suppression suggested there could be an edge except for the border. At
		* the border we say there can not be an edge because it makes the
		* followEdges algorithm more efficient to not worry about tracking an
		* edge off the side of the image.
		****************************************************************************/
		for(r=0,pos=0;r<rows;r++){
			for(c=0;c<cols;c++,pos++){
				if(nms[pos] == kPgPOSSIBLE_EDGE) edge[pos] = kPgPOSSIBLE_EDGE;
				else edge[pos] = kPgNOEDGE;
			}
		}

		for(r=0,pos=0;r<rows;r++,pos+=cols){
			edge[pos] = kPgNOEDGE;
			edge[pos+cols-1] = kPgNOEDGE;
		}
		pos = (rows-1) * cols;
		for(c=0;c<cols;c++,pos++){
			edge[c] = kPgNOEDGE;
			edge[pos] = kPgNOEDGE;
		}

		/****************************************************************************
		* Compute the histogram of the magnitude image. Then use the histogram to
		* compute hysteresis thresholds.
		****************************************************************************/
		for(r=0;r<32768;r++) hist[r] = 0;
		for(r=0,pos=0;r<rows;r++){
			for(c=0;c<cols;c++,pos++){
				int magi = (int)(0.5f + mag[pos]);
				magi = min(max(magi, 0), 32767);
				if(edge[pos] == kPgPOSSIBLE_EDGE) hist[magi]++;
			}
		}

		/****************************************************************************
		* Compute the number of pixels that passed the nonmaximal suppression.
		****************************************************************************/
		for(r=1,numedges=0;r<32768;r++){
			if(hist[r] != 0) maximum_mag = r;
			numedges += hist[r];
		}

		highcount = (int)(numedges * thigh + 0.5);

		/****************************************************************************
		* Compute the high threshold value as the (100 * thigh) percentage point
		* in the magnitude of the gradient histogram of all the pixels that passes
		* non-maximal suppression. Then calculate the low threshold as a fraction
		* of the computed high threshold value. John Canny said in his paper
		* "A Computational Approach to Edge Detection" that "The ratio of the
		* high to low threshold in the implementation is in the range two or three
		* to one." That means that in terms of this implementation, we should
		* choose tlow ~= 0.5 or 0.33333.
		****************************************************************************/
		r = 1;
		numedges = hist[1];
		while((r<(maximum_mag-1)) && (numedges < highcount)){
			r++;
			numedges += hist[r];
		}
		highthreshold = r;
		lowthreshold = (int)(highthreshold * tlow + 0.5);

		if(0){
			printf("The input low and high fractions of %f and %f computed to\n",
				tlow, thigh);
			printf("magnitude of the gradient threshold values of: %d %d\n",
				lowthreshold, highthreshold);
		}

		/****************************************************************************
		* This loop looks for pixels above the highthreshold to locate edges and
		* then calls followEdges to continue the edge.
		****************************************************************************/
		for(r=0,pos=0;r<rows;r++){
			for(c=0;c<cols;c++,pos++){
				if((edge[pos] == kPgPOSSIBLE_EDGE) && (mag[pos] >= highthreshold)){
					edge[pos] = kPgEDGE;
					followEdges((edge+pos), (mag+pos), lowthreshold, cols);
				}
			}
		}

		/****************************************************************************
		* Set all the remaining possible edges to non-edges.
		****************************************************************************/
		for(r=0,pos=0;r<rows;r++){
			for(c=0;c<cols;c++,pos++) if(edge[pos] != kPgEDGE) edge[pos] = kPgNOEDGE;
		}

		return true;
	}


	/*******************************************************************************
	* PROCEDURE: followEdges
	* PURPOSE: This procedure edges is a recursive routine that traces edgs along
	* all paths whose magnitude values remain above some specifyable lower
	* threshhold.
	* NAME: 
	* DATE: 
	*******************************************************************************/
	template <class T, class U>
	bool ImageEdgeDetectionBase<T, U>::followEdges(T* edgemapptr, T *edgemagptr, short lowval,
		int cols)
	{
		if (!edgemapptr || !edgemagptr)
			return false;

		T *tempmagptr;
		T *tempmapptr;
		int i;
		//float thethresh;
		int x[8] = {1,1,0,-1,-1,-1,0,1},
			y[8] = {0,1,1,1,0,-1,-1,-1};

		for(i=0;i<8;i++){
			tempmapptr = edgemapptr + y[i]*cols + x[i];
			tempmagptr = edgemagptr + y[i]*cols + x[i];

			if((*tempmapptr == kPgPOSSIBLE_EDGE) && (*tempmagptr > lowval)){
				*tempmapptr = (T) kPgEDGE;
				followEdges(tempmapptr,tempmagptr, lowval, cols);
			}
		}

		return true;
	}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageEdgeDetectionBase<short, short>; 
	template class ImageEdgeDetectionBase<unsigned char, unsigned char>; 	
#endif



};


//////////////////////////////////////////////////////////////////////////
// EOF

