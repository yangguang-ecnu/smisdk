<?xml version="1.0" encoding = "Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="8.00"
	Name="ZERO_CHECK"
	ProjectGUID="{E65D8B9C-F851-40D6-9422-5A40D5659861}"
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
			IntermediateDirectory="ZERO_CHECK.dir\Debug"
			ConfigurationType="10"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions=" -DCMAKE_INTDIR=\&quot;Debug\&quot;"
				AdditionalIncludeDirectories="..\config\include;include;"
				ExceptionHandling="FALSE"
				WarningLevel="3"
				PreprocessorDefinitions=",_REENTRANT,"
				AssemblerListingLocation="Debug"
				ObjectFile="$(IntDir)\"
/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				AdditionalIncludeDirectories="..\config\include;include;"
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
		</Configuration>
		<Configuration
			Name="Release|Win32"
			OutputDirectory="Release"
			IntermediateDirectory="ZERO_CHECK.dir\Release"
			ConfigurationType="10"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions=" -DCMAKE_INTDIR=\&quot;Release\&quot;"
				AdditionalIncludeDirectories="..\config\include;include;"
				ExceptionHandling="FALSE"
				WarningLevel="3"
				PreprocessorDefinitions=",_REENTRANT,"
				AssemblerListingLocation="Release"
				ObjectFile="$(IntDir)\"
/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				AdditionalIncludeDirectories="..\config\include;include;"
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
		</Configuration>
		<Configuration
			Name="MinSizeRel|Win32"
			OutputDirectory="MinSizeRel"
			IntermediateDirectory="ZERO_CHECK.dir\MinSizeRel"
			ConfigurationType="10"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions=" -DCMAKE_INTDIR=\&quot;MinSizeRel\&quot;"
				AdditionalIncludeDirectories="..\config\include;include;"
				ExceptionHandling="FALSE"
				WarningLevel="3"
				PreprocessorDefinitions=",_REENTRANT,"
				AssemblerListingLocation="MinSizeRel"
				ObjectFile="$(IntDir)\"
/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				AdditionalIncludeDirectories="..\config\include;include;"
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
		</Configuration>
		<Configuration
			Name="RelWithDebInfo|Win32"
			OutputDirectory="RelWithDebInfo"
			IntermediateDirectory="ZERO_CHECK.dir\RelWithDebInfo"
			ConfigurationType="10"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="FALSE"
			CharacterSet="2">
			<Tool
				Name="VCCLCompilerTool"
				AdditionalOptions=" -DCMAKE_INTDIR=\&quot;RelWithDebInfo\&quot;"
				AdditionalIncludeDirectories="..\config\include;include;"
				ExceptionHandling="FALSE"
				WarningLevel="3"
				PreprocessorDefinitions=",_REENTRANT,"
				AssemblerListingLocation="RelWithDebInfo"
				ObjectFile="$(IntDir)\"
/>
			<Tool
				Name="VCCustomBuildTool"/>
			<Tool
				Name="VCResourceCompilerTool"
				AdditionalIncludeDirectories="..\config\include;include;"
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
		</Configuration>
	</Configurations>
	<Files>
		<Filter
			Name="CMake Rules"
			Filter="">
			<File
				RelativePath="CMakeFiles\ZERO_CHECK.rule">
				<FileConfiguration
					Name="Debug|Win32">
					<Tool
					Name="VCCustomBuildTool"
					Description=""
					CommandLine=""
					AdditionalDependencies="CMakeFiles\ZERO_CHECK.rule"
					Outputs="CMakeFiles\ZERO_CHECK"/>
				</FileConfiguration>
				<FileConfiguration
					Name="Release|Win32">
					<Tool
					Name="VCCustomBuildTool"
					Description=""
					CommandLine=""
					AdditionalDependencies="CMakeFiles\ZERO_CHECK.rule"
					Outputs="CMakeFiles\ZERO_CHECK"/>
				</FileConfiguration>
				<FileConfiguration
					Name="MinSizeRel|Win32">
					<Tool
					Name="VCCustomBuildTool"
					Description=""
					CommandLine=""
					AdditionalDependencies="CMakeFiles\ZERO_CHECK.rule"
					Outputs="CMakeFiles\ZERO_CHECK"/>
				</FileConfiguration>
				<FileConfiguration
					Name="RelWithDebInfo|Win32">
					<Tool
					Name="VCCustomBuildTool"
					Description=""
					CommandLine=""
					AdditionalDependencies="CMakeFiles\ZERO_CHECK.rule"
					Outputs="CMakeFiles\ZERO_CHECK"/>
				</FileConfiguration>
			</File>
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
