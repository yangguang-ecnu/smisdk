// *********************************************************************************************************
// Document		:	Reademe file for SMIViewer.exe	
// Date created		:	May 11, 2008	
// Author		:	Prashant Chopra
// Editing notes	:
// 	May 22, 2008	:	Added items to known issues list. Added copyright and credit information. 
// 	May 26, 2008	:	v1.0.0.2: Can now load PGM/PPM/DEF file formats. Minor bug fixes.  
//	June 5, 2008	:	v1.0.0.3: Loading for images with lacking/invalid spacing fixed (thanks Vince Hradil!).
//					  SOPClass UID parsing and display. 
//					  Ability to load all DICOM images from a specified folder.
//					  Added supersampling capability.
//					  Separated modules to an SDK in preparation of an opensource version.	
//					  Many GUI and progress related bug fixes.
//					  Can now load 8-bit secondary captures.
//	June 16, 2008	:	v1.0.0.4: Basic segmentation enabled.
//					  Basic mask operations.
//					  Multi-threaded processing.
//					  Many bug fixes.
//	June 29, 2008	:	v1.0.0.5: Support for large datasets.
//					  Improved VR with cut plane.
//					  Multi-frame data support.
//					  Many bug fixes.
//	July 15, 2008	:	v1.0.0.6: Orientation labels
//					  Translucent mask overlay
//					  Correct value display
//					  Correct orientation for MR
//					  Patient name display
//					  Many bug fixes
// *********************************************************************************************************

0. DISCLAIMER

	SMIViewer is not approved for clinical or commercial usage and should not be relied upon
for any diagnostic decisions. It is provided 'as is' and any risks associated with its misuse or
lack of quality lie completely with the user. The only purpose of SMIViewer is to support and 
enhance research in the area of medical imaging.


I. INTRODUCTION

	SMIViewer is a free non-commercial DICOM image analyzer. It originated as an effort to develop
a free DICOM image analyzer by the moderators of group Science.Medical.Imaging [Group's website is
http://groups.google.com/medicalimagingscience].

	The idea is to develop a simple opensource framework that is freely available to academicians and
researchers, which allows them to add their own image analysis algorithms in no time. The first version
(1.0.0.1) does not make the source code publicly available since the platform itself is under development.
But very soon the code will be made freely available.

	For now, its compatibility has been tested only with Microsoft Windows(R) Vista(TM) 2007 and 
Microsoft Windows(R) XP 2002 SP2. More releases on other platforms can be expected in future.
 
	Needed resources:


[a] A personal computer with a memory of at least 500MB (recommended: 1GB+)
[b] Microsoft Vista or XP edition
[c] Microsoft .Net Framework 2.0 and above
[d] For volume rendering, nVidia GeForce series or above graphics card




II. DISCLAIMER

	SMIViewer is meant for research and academic purpose only. It should not be used clinically 
and no diagnostic decisions should be made based on any results from this software. If you have any questions
about this disclaimer please contact the developer(s) at pixel.to.life@gmail.com. 





III. WHERE TO GET IT

	Please write to the developer(s) at pixel.to.life@gmail.com specifying the intended usage and the
targetted audience. A binary will be made available free of cost for evaluation.






IV. HOW TO USE IT

1. To load DICOM data, click "File -> Load Primary (DICOM)"

2. If the images conform to DICOM 3.0 standard, the volume will be loaded in the three MPR
and one 3D window.

3. The cursor is linked in the three MPR views. As it is moved in any of the three windows, the position
and value information is updated in the overlays.

4. To change the Window/Level for visualizing the images, please use the appropriate slider bar.

5. To increase viewer resolution, use the 'maximize' button on the viewer application. If the viewer windows
look distorted after resize, please minimize to return to the original size.






V. KNOWN LIMITATIONS

Here are known limitations of this system as of the date of this release:

1. It works only on Microsoft Windows (R) XP(TM) 2000+ or Vista(TM) Home/Professional edition 2007+.
You will need to have the Microsoft .Net framework installed on the system.

2. It can load only DICOM complying image sets, and PGM (Portable Grey Map), PPM (Portable Pixel Map), and on 
special request, the DEF format (for UC Irvine). Future versions will support .mhd and .raw image formats.

3. It loads only square image-sets into the volume rendering viewer. MPRs must be available for all valid DICOM datasets.

4. It is limited by available texture memory on the target system for volume rendering. In cases the need surpasses this limit, 
volume rendering display may not be available. In this case, 2D MultiPlanarReformat display may still be used.

5. It is distributed with a fixed per use limit. The current limit is 25 uses for trial version and 2500 uses for a 
research version. This limit will be relaxed once the system becomes more stable.

6. The 'Capture All Views' button captures only raw rendered MPR images. The Volume Rendered image cannot be captured yet either.
This means none of the overlays (cursor/cross hair/annotations) will be captured. If you need to capture extra information, 
please use the operating system's or third party software capabilities.

7. Only one DICOM volume can be loaded at a time. There is an option to load a secondary volume, but it is disabled for now.

8. Gantry tilted data or secondary batch capture data may not load properly.

9. PPM image sets loaded with a .def file have RGB pixels converted to greyscale data before display.

10. Images with multiple samples per pixels are not supported yet.





VI. BUG REPORTS AND/OR FEATURE REQUESTS

1. Bug reports: Please send the report to pixel.to.life@gmail.com with subject line: [SMIViewer-Bug].
Please attach the log (and dump files) from the log path. The log path is displayed in the disclaimer message
box when the application is started.



2. Feature requests: Please send the feature requests to pixel.to.life@gmail.com with subject line: [SMIViewer-Feature].



Please understand that the developer(s) of SMIViewer are not compensated for this work, and work on it in their free time.
Please be patient while waiting to see your requests acknowledged!

Thank you for supporting this free effort in public and research interest.



[CREDITS]

[1] All components of this software, SMIViewer, except the DICOM file reading/writing capabilities (please see [2] below),
have been developed independently by Prashant Chopra (referred hereafter as 'the author', contact: pixel.to.life@gmail.com). 
Unless specified otherwise, 'the author' owns all rights to all the code (except the dcmtk library, please see [2]).
'The author' grants permission to freely distribute this application, SMIViewer, for the sole purposes of academic research,
This software has not been tested in a clinical environment, and should NOT be relied upon for diagnosis purposes.
If you intend to use this software commercially, please contact 'the author' right away.

'The author' is eternally thankful to his wife, Radhika Varma, who sacrificed many many weekend hours to help him make this
free application a reality.

Last but not the least, 'the author' is highly indebted to Dr. Joerg Meyer, University of California-Irvine, for keeping the 
flame of innovations alive. 'The author' is also grateful to Dr. Meyer for volunteering to evaluate SMIViewer for research.


The following copyright notice expresses the rights to all parts of this software (except dcmtk, see [2]):

// 
// © [2007-2017] Prashant Chopra [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States. Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
// INFORMATION OF PRASHANT CHOPRA (EXCEPT THE DCMTK DICOM LIBRARY). 
// PERMISSION IS GRANTED TO FREELY DISTRIBUTE THIS SOFTWARE SOLELY 
// FOR RESEARCH AND/OR ACADEMIC PURPOSES.
// THIS SOFTWARE HAS NOT BEEN EVALUATED CLINICALLY. THEREFORE,
// CLINICAL OR COMMERCIAL USE OF THIS SOFTWARE IN ANY FORM, IN WHOLE, 
// OR IN PART, IS STRICTLY PROHIBITED. IF THERE ARE ANY QUESTIONS 
// REGARDING THE SUGGESTED USAGE OF THIS SOFTWARE, PLEASE CONTACT THE
// PRIMARY AUTHOR, PRASHANT CHOPRA, AT PIXEL.TO.LIFE@GMAIL.COM.
//
// SOFTWARE	:	SMIViewer.exe
// Author	:	Prashant Chopra
// 
//




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


APPENDIX A. DEF file format. Version 1.0.0.2 onwards, SMIViewer supports the DEF
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


APPENDIX B. DEF file format. Version 1.0.0.7 onwards, SMIViewer supports the MHD
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
