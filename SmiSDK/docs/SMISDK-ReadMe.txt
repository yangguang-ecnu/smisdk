// *********************************************************************************************************
// Document		:	Readme file for SMISDK	
// Date created		:	August 09, 2008	
// Author		:	Prashant Chopra
//				http://groups.google.com/group/medicalimagingscience/web/smiviewer-download-page
// Editing notes	:
// 	August 09, 2008	:	First version.
//	Dec    21, 2008 :	Second version.
// *********************************************************************************************************

I. DISCLAIMER

	SMISDK (Science.Medical.Imaging Software Development Kit) is meant for research and 
academic purpose only. It should not be used clinically and no diagnostic decisions should 
be made based on any results from this software. If you have any questions about this disclaimer
please contact the developer(s) at pixel.to.life@gmail.com. 



II. INTRODUCTION

	SMISDK is a free non-commercial software framework thats helps build applications for 
DICOM image analysis. It originated as an effort to develop a free DICOM image analyzer by the
moderators of group Science.Medical.Imaging [Group's website is
http://groups.google.com/medicalimagingscience].

	The idea is to develop a simple opensource framework that is freely available to 
academicians and researchers, which allows them to add their own image analysis algorithms in
no time. 

	For now, its compatibility has been tested only with Microsoft Windows(R) Vista(TM) 2007 and 
Microsoft Windows(R) XP 2002 SP2. More releases on other platforms can be expected in future.
 
Needed resources:


[a] A personal computer with a memory of at least 500MB (recommended: 1GB+)
[b] Microsoft Vista or XP edition
[c] Microsoft .Net Framework 2.0 and above
[d] For volume rendering, nVidia GeForce series or above graphics card



III. WHERE TO GET IT

	Please write to the developer(s) at pixel.to.life@gmail.com specifying the intended usage and the
targetted audience. A copy of the SDK will be made available free of cost for evaluation.





IV. HOW TO USE IT

IV.I. Installation
	
[a] By the time you read this text, you would already have unzipped the installer package, and executed it.
A link to this file is created as a shortcut on the current user's desktop. During installation, you would
have chosen a base path to install the package. 

** It is highly recommended that you keep the default installation path, which is 
"(System Drive)/Program Files/Science.Medical.Imaging/SMISDK/". The reason is that all the examples in the 
SDK are provided with this path as the SDK path.
	


IV.II. Setup

	The default path of installation will be in (Program Files) folder. Example,

C:/Program Files/Science.Medical.Imaging/SMISDK/include 	: All header files
C:/Program Files/Science.Medical.Imaging/SMISDK/include/Core	: All Core headers
C:/Program Files/Science.Medical.Imaging/SMISDK/include/Math	: All Math headers
C:/Program Files/Science.Medical.Imaging/SMISDK/include/IO	: All IO headers
C:/Program Files/Science.Medical.Imaging/SMISDK/include/Algs	: All Algorithms headers
C:/Program Files/Science.Medical.Imaging/SMISDK/include/dcmtk	: All dcmtk related headers
C:/Program Files/Science.Medical.Imaging/SMISDK/include/GL	: All GL headers
C:/Program Files/Science.Medical.Imaging/SMISDK/lib		: All SDK libs
C:/Program Files/Science.Medical.Imaging/SMISDK/docs		: All SDK documents 
C:/Program Files/Science.Medical.Imaging/SMISDK/Examples	: All SDK examples


** Note that all compatible OpenGL dlls are provided in the lib folder, just in case your system missed them.
To use them, just copy them in the path where your system searches for binaries, or copy them locally to your
example binary's path. Those dlls are:

C:/Program Files/Science.Medical.Imaging/SMISDK/lib/GLU32.dll
C:/Program Files/Science.Medical.Imaging/SMISDK/lib/glut32.dll
C:/Program Files/Science.Medical.Imaging/SMISDK/lib/OpenGL32.dll



IV.III. Examples

	All SDK examples can be found here:

C:/Program Files/Science.Medical.Imaging/SMISDK/Examples	: All SDK examples

** Note that these example projects need write access to their local folder to be able to compile and build.
You will thus have to copy them to another location before building them. A good place would be "C:/SMISDK/Examples".


** The example projects have been created with Visual Studio 8.0 Professional Edition 2005. If you use a different
visual studio version, you will have to convert these projects. We will provide a guide to creating your own projects
from scratch in the next releases.


** They have been setup to look for the SDK in the default path as explained in section "IV.I" above.
So if you chose a different path for installation, make sure to update the example project's path
settings for include and lib folders.

Once you take care of the above, you are ready to compile your example projects. Most of them are self explanatory.
If you face any problems, hoewver, please send an email to "pixel.to.life@gmail.com" with the subject line "SMISDK-Examples".


IV.IV. Data

	All SDK examples are set up to use the default test data. To get this data, please email pixel.to.life@gmail.com.
You can also visit "http://groups.google.com/group/medicalimagingscience/web/smiviewer-download-page" for more information 
on this topic.



V. KNOWN LIMITATIONS

Here are known limitations of this system as of the date of this release:

1. It works only on Microsoft Windows (R) XP(TM) 2000+ or Vista(TM) Home/Professional edition 2007+.
You may need to have the Microsoft .Net framework installed on the system.

2. To be able to use the GLRenderer examples, you will need to have a graphics card that support texture memory of at least 128MB.
A good choice is nVidia GeForce Go series for notebooks. If you see any issues, please contact "pixel.to.life@gmail.com" with 
the subject line "SMISDK-GL".





VI. BUG REPORTS AND/OR FEATURE REQUESTS

1. Bug reports: Please send the report to pixel.to.life@gmail.com with subject line: [SMISDK-Bug].


2. Feature requests: Please send the feature requests to pixel.to.life@gmail.com with subject line: [SMISDK-Feature].



Please understand that the developer(s) of SMISDK are not compensated for this work, and work on it in their free time.
Please be patient while waiting to see your requests acknowledged!


Thank you for supporting this free effort in public and research interest.



[CREDITS]

[1] All components of this software, SMISDK, except the DICOM file reading/writing capabilities (please see [2] below), have been developed independently 

by Prashant Chopra (referred hereafter as 'the author', contact: pixel.to.life@gmail.com). 

Unless specified otherwise, 'the author' owns all rights to all the code (except the dcmtk library, please see [2]). 

'The author' grants permission to freely distribute this SDK, SMISDK, for the sole purposes of academic research, 

This software has not been tested in a clinical environment, and should NOT be relied upon for diagnosis purposes. 

If you intend to use this software commercially, please contact 'the author' right away.

'The author' is eternally thankful to his wife, Radhika Varma, who sacrificed many many weekend hours to help him make this free application a reality.

Last but not the least, 'the author' is highly indebted to Dr. Joerg Meyer, University of California-Irvine, for keeping the flame of innovations alive. 

'The author' is also grateful to Dr. Meyer for volunteering to evaluate SMISDK for research.


[2] The DICOM data parsing capabilities of SMIViewer are built upon dcmtk library, available from the world wide web at
http://dicom.offis.de/dcmtk. 

This library has been compiled with SMIViewer's code to build this application.

The following copyright notice is included from 
ftp://dicom.offis.de/pub/dicom/offis/software/dcmtk/dcmtk354/COPYRIGHT 
to comply with free distribution policy:

/*
 *  Copyright (C) 1994-2005, OFFIS
 *
 *  This software and supporting documentation were developed by
 *
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *
 *  Copyright of the software  and  supporting  documentation  is,  unless
 *  otherwise stated, owned by OFFIS, and free access is hereby granted as
 *  a license to  use  this  software,  copy  this  software  and  prepare
 *  derivative works based upon this software.  However, any  distribution
 *  of this software source code or supporting documentation or derivative
 *  works  (source code and  supporting documentation)  must  include  the
 *  three paragraphs of this copyright notice.
 *
 */


APPENDIX A. DEF file format. SMISDK supports the DEF
file format. This format is used at UC Irvine Imaging laboratory headed by Dr.
Joerg Meyer. Here are the specifications:

==========================
directory path, or "." for current working directory
base filename or "NONE"
file extension or "NONE"
number of digits in filename
"first_number last_number" or "*" for all files with basename and
extension in directory
resolution of data: "w h" (w, h should be identical to PPM/PGM header)
bytes per pixel (1: 8 bits, 3: RGB)
header length or "SKIP" (total bytes - resolution * bytes_per_pixel)
depth scale (scaling factor along slicing direction)
==========================

Example:

C:\\Data\\SamplePGM
Image_
pgm
3
23 179
256 256
1
SKIP
1.25


APPENDIX B. MHD file format. SMISDK also supports the MHD
file format to allow reading raw image data. Example:

--- START TEST.MHD ---
ObjectType = Image
NDims = 3
BinaryData = True
BinaryDataByteOrderMSB = False
TransformMatrix = 1 0 0 0 1 0 0 0 1
Offset = 0 0 0
CenterOfRotation = 0 0 0
ElementSpacing = 0.5 0.5 0.5
DimSize = 256 256 256
AnatomicalOrientation = ???
ElementType = MET_UCHAR
ElementDataFile = test.raw
--- END TEST.MHD ---

********************************* End of file **************************************************************