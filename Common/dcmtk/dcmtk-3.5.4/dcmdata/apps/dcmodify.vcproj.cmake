<?xml version="1.0" encoding = "Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="8.00"
	Name="dcmodify"
	ProjectGUID="{9A2D5A6A-D145-45FB-9AEA-84EDDC0D9D24}"
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
			IntermediateDirectory="dcmodify.dir\Debug"
			ConfigurationType="1"
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
				Name="VCLinkerTool"
				AdditionalOptions="   /STACK:10000000 /machine:I386 /debug "
				AdditionalDependencies="$(NOINHERIT) kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  netapi32.lib wsock32.lib ofstd.lib dcmdata.lib "
				OutputFile="Debug\dcmodify.exe"
				Version="0.0"
				GenerateManifest="TRUE"
				LinkIncremental="2"
				AdditionalLibraryDirectories="..\$(OutDir),..,..\..\ofstd\$(OutDir),..\..\ofstd,..\..\ofstd\libsrc\$(OutDir),..\..\ofstd\libsrc,..\libsrc\$(OutDir),..\libsrc"
				ProgramDataBaseFile="$(OutDir)\dcmodify.pdb"
				GenerateDebugInformation="TRUE"
				SubSystem="1"
				StackReserveSize="10000000"/>
		</Configuration>
		<Configuration
			Name="Release|Win32"
			OutputDirectory="Release"
			IntermediateDirectory="dcmodify.dir\Release"
			ConfigurationType="1"
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
				Name="VCLinkerTool"
				AdditionalOptions="   /STACK:10000000 /machine:I386 "
				AdditionalDependencies="$(NOINHERIT) kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  netapi32.lib wsock32.lib ofstd.lib dcmdata.lib "
				OutputFile="Release\dcmodify.exe"
				Version="0.0"
				GenerateManifest="TRUE"
				LinkIncremental="1"
				AdditionalLibraryDirectories="..\$(OutDir),..,..\..\ofstd\$(OutDir),..\..\ofstd,..\..\ofstd\libsrc\$(OutDir),..\..\ofstd\libsrc,..\libsrc\$(OutDir),..\libsrc"
				ProgramDataBaseFile="$(OutDir)\dcmodify.pdb"
				SubSystem="1"
				StackReserveSize="10000000"/>
		</Configuration>
		<Configuration
			Name="MinSizeRel|Win32"
			OutputDirectory="MinSizeRel"
			IntermediateDirectory="dcmodify.dir\MinSizeRel"
			ConfigurationType="1"
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
				Name="VCLinkerTool"
				AdditionalOptions="   /STACK:10000000 /machine:I386 "
				AdditionalDependencies="$(NOINHERIT) kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  netapi32.lib wsock32.lib ofstd.lib dcmdata.lib "
				OutputFile="MinSizeRel\dcmodify.exe"
				Version="0.0"
				GenerateManifest="TRUE"
				LinkIncremental="1"
				AdditionalLibraryDirectories="..\$(OutDir),..,..\..\ofstd\$(OutDir),..\..\ofstd,..\..\ofstd\libsrc\$(OutDir),..\..\ofstd\libsrc,..\libsrc\$(OutDir),..\libsrc"
				ProgramDataBaseFile="$(OutDir)\dcmodify.pdb"
				SubSystem="1"
				StackReserveSize="10000000"/>
		</Configuration>
		<Configuration
			Name="RelWithDebInfo|Win32"
			OutputDirectory="RelWithDebInfo"
			IntermediateDirectory="dcmodify.dir\RelWithDebInfo"
			ConfigurationType="1"
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
				Name="VCLinkerTool"
				AdditionalOptions="   /STACK:10000000 /machine:I386 /debug "
				AdditionalDependencies="$(NOINHERIT) kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  netapi32.lib wsock32.lib ofstd.lib dcmdata.lib "
				OutputFile="RelWithDebInfo\dcmodify.exe"
				Version="0.0"
				GenerateManifest="TRUE"
				LinkIncremental="2"
				AdditionalLibraryDirectories="..\$(OutDir),..,..\..\ofstd\$(OutDir),..\..\ofstd,..\..\ofstd\libsrc\$(OutDir),..\..\ofstd\libsrc,..\libsrc\$(OutDir),..\libsrc"
				ProgramDataBaseFile="$(OutDir)\dcmodify.pdb"
				GenerateDebugInformation="TRUE"
				SubSystem="1"
				StackReserveSize="10000000"/>
		</Configuration>
	</Configurations>
	<Files>
		<Filter
			Name="Source Files"
			Filter="">
			<File
				RelativePath="dcmodify.cxx">
			</File>
			<File
				RelativePath="mdfconen.cxx">
			</File>
			<File
				RelativePath="mdfdsman.cxx">
			</File>
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
