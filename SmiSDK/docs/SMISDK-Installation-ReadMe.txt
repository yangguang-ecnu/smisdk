// *********************************************************************************************************
//
// Document			:	Readme file for SMISDK	installer
// Date created			:	August 09, 2008	
// Author			:	Prashant Chopra
//					pixel.to.life@gmail.com
//
// Modifications		
//
// 	August	09, 2008	:	First version.
//	Dec    	21, 2008 	:	Second version. 
//					Fixed unhandled exception issue in debug mode when running any of the OpenGL 
//					examples.
//	Jul    	26, 2009	:	Third version. SMISDK open source now.	
//
// *********************************************************************************************************

This file provides a help resource accompanying the SMISDK setup program.

To install SMISDK, follow the following steps:
 

[1] Obtain an installer for SMISDK by one of the following two steps:

	- Obtain an installer from the download page 
		at http://groups.google.com/group/medicalimagingscience/web/smiviewer-download-page.
	  	Unzip the package using the secure password. 
	  	After unzipping, you may see many files in the folder, including one named SMISDKSetup.msi.
	  	Double click on this file to run the installer.
	
	- Build the installer yourself (Requires Microsoft Visual Studio 2005 and above): 
		-- Obtain the source code from http://code.google.com/p/smisdk/source/checkout.
		-- Open the main solution : SmiSDK/SmiSDK.sln.
		-- Rebuild all.
		-- Upon a successful build, right click on the Setup project in the solution explorer
	   	   and select 'install'. 
		-- Remember to back up the installer you just built to uninstall the program if needed in future.


[2] The installer will ask you for an installation path. It is highly recommended that you keep the default
	path (system drive)/Program Files/. This will make it easy for you to compile the examples.


[3] After installation is complete, you will see a shortcut on your desktop to another Readme file (SMISDK-ReadMe.txt).
	Refer to that file for further instructions.


We hope you enjoy another free resource from Science.Medical.Imaging group. Please consider helping with your share if 
you like what you see.


********************************* End of file **************************************************************