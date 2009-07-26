<?xml version="1.0" encoding = "Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="8.00"
	Name="dcmdata"
	ProjectGUID="{7168E5E2-75DA-4E7C-8262-32A86D672B6C}"
	SccProjectName=""
	SccLocalPath=""
	Keyword="Win32Proj">
	<Platforms>
		<Platform
			Name="Win32"/>
	</Platforms>
	<Configurations>
		<Configuration
			Name="Debug|Win32"
			OutputDirectory="Debug"
			IntermediateDirectory="dcmdata.dir\Debug"
			ConfigurationType="4"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions="        -DCMAKE_INTDIR=\&quot;Debug\&quot;"
				AdditionalIncludeDirectories="..\..\config\include;..\include;..\..\ofstd\include;"
				CompileAs="2"
				DebugInformationFormat="1"
				EnableFunctionLevelLinking="TRUE"
				ExceptionHandling="FALSE"
				Optimization="0"
				RuntimeLibrary="1"
				SuppressStartupBanner="TRUE"
				WarningLevel="3"
				PreprocessorDefinitions=",_REENTRANT,"
				AssemblerListingLocation="Debug"
				ObjectFile="$(IntDir)\"
/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				AdditionalIncludeDirectories="..\..\config\include;..\include;..\..\ofstd\include;"
				PreprocessorDefinitions=",_REENTRANT," />
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions=",_REENTRANT,"
				MkTypLibCompatible="FALSE"
				TargetEnvironment="1"
				GenerateStublessProxies="TRUE"
				TypeLibraryName="$(InputName).tlb"
				OutputDirectory="$(IntDir)"
				HeaderFileName="$(InputName).h"
				DLLDataFileName=""
				InterfaceIdentifierFileName="$(InputName)_i.c"
				ProxyFileName="$(InputName)_p.c"/>
			<Tool
				Name="VCManifestTool"
				UseFAT32Workaround="true"
			/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
			<Tool
				Name="VCPostBuildEventTool"/>
			<Tool
				Name="VCLibrarianTool"
				OutputFile="$(OutDir)\dcmdata.lib"/>
		</Configuration>
		<Configuration
			Name="Release|Win32"
			OutputDirectory="Release"
			IntermediateDirectory="dcmdata.dir\Release"
			ConfigurationType="4"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions="       -DCMAKE_INTDIR=\&quot;Release\&quot;"
				AdditionalIncludeDirectories="..\..\config\include;..\include;..\..\ofstd\include;"
				CompileAs="2"
				EnableFunctionLevelLinking="TRUE"
				ExceptionHandling="FALSE"
				Optimization="2"
				RuntimeLibrary="0"
				SuppressStartupBanner="TRUE"
				WarningLevel="3"
				PreprocessorDefinitions=",_REENTRANT,"
				AssemblerListingLocation="Release"
				ObjectFile="$(IntDir)\"
/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				AdditionalIncludeDirectories="..\..\config\include;..\include;..\..\ofstd\include;"
				PreprocessorDefinitions=",_REENTRANT," />
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions=",_REENTRANT,"
				MkTypLibCompatible="FALSE"
				TargetEnvironment="1"
				GenerateStublessProxies="TRUE"
				TypeLibraryName="$(InputName).tlb"
				OutputDirectory="$(IntDir)"
				HeaderFileName="$(InputName).h"
				DLLDataFileName=""
				InterfaceIdentifierFileName="$(InputName)_i.c"
				ProxyFileName="$(InputName)_p.c"/>
			<Tool
				Name="VCManifestTool"
				UseFAT32Workaround="true"
			/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
			<Tool
				Name="VCPostBuildEventTool"/>
			<Tool
				Name="VCLibrarianTool"
				OutputFile="$(OutDir)\dcmdata.lib"/>
		</Configuration>
		<Configuration
			Name="MinSizeRel|Win32"
			OutputDirectory="MinSizeRel"
			IntermediateDirectory="dcmdata.dir\MinSizeRel"
			ConfigurationType="4"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions="       -DCMAKE_INTDIR=\&quot;MinSizeRel\&quot;"
				AdditionalIncludeDirectories="..\..\config\include;..\include;..\..\ofstd\include;"
				CompileAs="2"
				EnableFunctionLevelLinking="TRUE"
				ExceptionHandling="FALSE"
				Optimization="2"
				RuntimeLibrary="0"
				SuppressStartupBanner="TRUE"
				WarningLevel="3"
				PreprocessorDefinitions=",_REENTRANT,"
				AssemblerListingLocation="MinSizeRel"
				ObjectFile="$(IntDir)\"
/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				AdditionalIncludeDirectories="..\..\config\include;..\include;..\..\ofstd\include;"
				PreprocessorDefinitions=",_REENTRANT," />
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions=",_REENTRANT,"
				MkTypLibCompatible="FALSE"
				TargetEnvironment="1"
				GenerateStublessProxies="TRUE"
				TypeLibraryName="$(InputName).tlb"
				OutputDirectory="$(IntDir)"
				HeaderFileName="$(InputName).h"
				DLLDataFileName=""
				InterfaceIdentifierFileName="$(InputName)_i.c"
				ProxyFileName="$(InputName)_p.c"/>
			<Tool
				Name="VCManifestTool"
				UseFAT32Workaround="true"
			/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
			<Tool
				Name="VCPostBuildEventTool"/>
			<Tool
				Name="VCLibrarianTool"
				OutputFile="$(OutDir)\dcmdata.lib"/>
		</Configuration>
		<Configuration
			Name="RelWithDebInfo|Win32"
			OutputDirectory="RelWithDebInfo"
			IntermediateDirectory="dcmdata.dir\RelWithDebInfo"
			ConfigurationType="4"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions="        -DCMAKE_INTDIR=\&quot;RelWithDebInfo\&quot;"
				AdditionalIncludeDirectories="..\..\config\include;..\include;..\..\ofstd\include;"
				CompileAs="2"
				DebugInformationFormat="1"
				EnableFunctionLevelLinking="TRUE"
				ExceptionHandling="FALSE"
				Optimization="0"
				RuntimeLibrary="1"
				SuppressStartupBanner="TRUE"
				WarningLevel="3"
				PreprocessorDefinitions=",_REENTRANT,"
				AssemblerListingLocation="RelWithDebInfo"
				ObjectFile="$(IntDir)\"
/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				AdditionalIncludeDirectories="..\..\config\include;..\include;..\..\ofstd\include;"
				PreprocessorDefinitions=",_REENTRANT," />
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions=",_REENTRANT,"
				MkTypLibCompatible="FALSE"
				TargetEnvironment="1"
				GenerateStublessProxies="TRUE"
				TypeLibraryName="$(InputName).tlb"
				OutputDirectory="$(IntDir)"
				HeaderFileName="$(InputName).h"
				DLLDataFileName=""
				InterfaceIdentifierFileName="$(InputName)_i.c"
				ProxyFileName="$(InputName)_p.c"/>
			<Tool
				Name="VCManifestTool"
				UseFAT32Workaround="true"
			/>
			<Tool
				Name="VCPreBuildEventTool"/>
			<Tool
				Name="VCPreLinkEventTool"/>
			<Tool
				Name="VCPostBuildEventTool"/>
			<Tool
				Name="VCLibrarianTool"
				OutputFile="$(OutDir)\dcmdata.lib"/>
		</Configuration>
	</Configurations>
	<Files>
		<Filter
			Name="Source Files"
			Filter="">
			<File
				RelativePath="cmdlnarg.cxx">
			</File>
			<File
				RelativePath="dcbytstr.cxx">
			</File>
			<File
				RelativePath="dcchrstr.cxx">
			</File>
			<File
				RelativePath="dccodec.cxx">
			</File>
			<File
				RelativePath="dcdatset.cxx">
			</File>
			<File
				RelativePath="dcddirif.cxx">
			</File>
			<File
				RelativePath="dcdebug.cxx">
			</File>
			<File
				RelativePath="dcdicdir.cxx">
			</File>
			<File
				RelativePath="dcdicent.cxx">
			</File>
			<File
				RelativePath="dcdict.cxx">
			</File>
			<File
				RelativePath="dcdictzz.cxx">
			</File>
			<File
				RelativePath="dcdirrec.cxx">
			</File>
			<File
				RelativePath="dcelem.cxx">
			</File>
			<File
				RelativePath="dcerror.cxx">
			</File>
			<File
				RelativePath="dcfilefo.cxx">
			</File>
			<File
				RelativePath="dchashdi.cxx">
			</File>
			<File
				RelativePath="dcistrma.cxx">
			</File>
			<File
				RelativePath="dcistrmb.cxx">
			</File>
			<File
				RelativePath="dcistrmf.cxx">
			</File>
			<File
				RelativePath="dcistrmz.cxx">
			</File>
			<File
				RelativePath="dcitem.cxx">
			</File>
			<File
				RelativePath="dclist.cxx">
			</File>
			<File
				RelativePath="dcmetinf.cxx">
			</File>
			<File
				RelativePath="dcobject.cxx">
			</File>
			<File
				RelativePath="dcostrma.cxx">
			</File>
			<File
				RelativePath="dcostrmb.cxx">
			</File>
			<File
				RelativePath="dcostrmf.cxx">
			</File>
			<File
				RelativePath="dcostrmz.cxx">
			</File>
			<File
				RelativePath="dcpcache.cxx">
			</File>
			<File
				RelativePath="dcpixel.cxx">
			</File>
			<File
				RelativePath="dcpixseq.cxx">
			</File>
			<File
				RelativePath="dcpxitem.cxx">
			</File>
			<File
				RelativePath="dcrleccd.cxx">
			</File>
			<File
				RelativePath="dcrlecce.cxx">
			</File>
			<File
				RelativePath="dcrlecp.cxx">
			</File>
			<File
				RelativePath="dcrledrg.cxx">
			</File>
			<File
				RelativePath="dcrleerg.cxx">
			</File>
			<File
				RelativePath="dcrlerp.cxx">
			</File>
			<File
				RelativePath="dcsequen.cxx">
			</File>
			<File
				RelativePath="dcstack.cxx">
			</File>
			<File
				RelativePath="dcswap.cxx">
			</File>
			<File
				RelativePath="dctag.cxx">
			</File>
			<File
				RelativePath="dctagkey.cxx">
			</File>
			<File
				RelativePath="dctypes.cxx">
			</File>
			<File
				RelativePath="dcuid.cxx">
			</File>
			<File
				RelativePath="dcvm.cxx">
			</File>
			<File
				RelativePath="dcvr.cxx">
			</File>
			<File
				RelativePath="dcvrae.cxx">
			</File>
			<File
				RelativePath="dcvras.cxx">
			</File>
			<File
				RelativePath="dcvrat.cxx">
			</File>
			<File
				RelativePath="dcvrcs.cxx">
			</File>
			<File
				RelativePath="dcvrda.cxx">
			</File>
			<File
				RelativePath="dcvrds.cxx">
			</File>
			<File
				RelativePath="dcvrdt.cxx">
			</File>
			<File
				RelativePath="dcvrfd.cxx">
			</File>
			<File
				RelativePath="dcvrfl.cxx">
			</File>
			<File
				RelativePath="dcvris.cxx">
			</File>
			<File
				RelativePath="dcvrlo.cxx">
			</File>
			<File
				RelativePath="dcvrlt.cxx">
			</File>
			<File
				RelativePath="dcvrobow.cxx">
			</File>
			<File
				RelativePath="dcvrof.cxx">
			</File>
			<File
				RelativePath="dcvrpn.cxx">
			</File>
			<File
				RelativePath="dcvrpobw.cxx">
			</File>
			<File
				RelativePath="dcvrsh.cxx">
			</File>
			<File
				RelativePath="dcvrsl.cxx">
			</File>
			<File
				RelativePath="dcvrss.cxx">
			</File>
			<File
				RelativePath="dcvrst.cxx">
			</File>
			<File
				RelativePath="dcvrtm.cxx">
			</File>
			<File
				RelativePath="dcvrui.cxx">
			</File>
			<File
				RelativePath="dcvrul.cxx">
			</File>
			<File
				RelativePath="dcvrulup.cxx">
			</File>
			<File
				RelativePath="dcvrus.cxx">
			</File>
			<File
				RelativePath="dcvrut.cxx">
			</File>
			<File
				RelativePath="dcxfer.cxx">
			</File>
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
