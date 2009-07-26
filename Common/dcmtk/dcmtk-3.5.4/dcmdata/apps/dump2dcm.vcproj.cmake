<?xml version="1.0" encoding = "Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="8.00"
	Name="dump2dcm"
	ProjectGUID="{60CF8A6E-2F27-4F8E-A1DF-9B7837D0737F}"
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
			IntermediateDirectory="dump2dcm.dir\Debug"
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
				OutputFile="Debug\dump2dcm.exe"
				Version="0.0"
				GenerateManifest="TRUE"
				LinkIncremental="2"
				AdditionalLibraryDirectories="..\$(OutDir),..,..\..\ofstd\$(OutDir),..\..\ofstd,..\..\ofstd\libsrc\$(OutDir),..\..\ofstd\libsrc,..\libsrc\$(OutDir),..\libsrc"
				ProgramDataBaseFile="$(OutDir)\dump2dcm.pdb"
				GenerateDebugInformation="TRUE"
				SubSystem="1"
				StackReserveSize="10000000"/>
		</Configuration>
		<Configuration
			Name="Release|Win32"
			OutputDirectory="Release"
			IntermediateDirectory="dump2dcm.dir\Release"
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
				OutputFile="Release\dump2dcm.exe"
				Version="0.0"
				GenerateManifest="TRUE"
				LinkIncremental="1"
				AdditionalLibraryDirectories="..\$(OutDir),..,..\..\ofstd\$(OutDir),..\..\ofstd,..\..\ofstd\libsrc\$(OutDir),..\..\ofstd\libsrc,..\libsrc\$(OutDir),..\libsrc"
				ProgramDataBaseFile="$(OutDir)\dump2dcm.pdb"
				SubSystem="1"
				StackReserveSize="10000000"/>
		</Configuration>
		<Configuration
			Name="MinSizeRel|Win32"
			OutputDirectory="MinSizeRel"
			IntermediateDirectory="dump2dcm.dir\MinSizeRel"
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
				OutputFile="MinSizeRel\dump2dcm.exe"
				Version="0.0"
				GenerateManifest="TRUE"
				LinkIncremental="1"
				AdditionalLibraryDirectories="..\$(OutDir),..,..\..\ofstd\$(OutDir),..\..\ofstd,..\..\ofstd\libsrc\$(OutDir),..\..\ofstd\libsrc,..\libsrc\$(OutDir),..\libsrc"
				ProgramDataBaseFile="$(OutDir)\dump2dcm.pdb"
				SubSystem="1"
				StackReserveSize="10000000"/>
		</Configuration>
		<Configuration
			Name="RelWithDebInfo|Win32"
			OutputDirectory="RelWithDebInfo"
			IntermediateDirectory="dump2dcm.dir\RelWithDebInfo"
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
				OutputFile="RelWithDebInfo\dump2dcm.exe"
				Version="0.0"
				GenerateManifest="TRUE"
				LinkIncremental="2"
				AdditionalLibraryDirectories="..\$(OutDir),..,..\..\ofstd\$(OutDir),..\..\ofstd,..\..\ofstd\libsrc\$(OutDir),..\..\ofstd\libsrc,..\libsrc\$(OutDir),..\libsrc"
				ProgramDataBaseFile="$(OutDir)\dump2dcm.pdb"
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
				RelativePath="dump2dcm.cxx">
			</File>
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
