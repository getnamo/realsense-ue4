// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    using System.IO;

	public class RealSensePlugin : ModuleRules
	{
        private string ModulePath
        {
            get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
        }

        private string ThirdPartyPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
        }

		public RealSensePlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
                    "RealSensePlugin/Public",
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"RealSensePlugin/Private",
                    Path.Combine(ThirdPartyPath, "RealSense", "Include"),
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "Engine",
                    "InputCore",
                    "Slate",
                    "SlateCore"
					// ... add other public dependencies that you statically link with here ...
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);

            LoadRealSenseLib(Target);
		}

        public bool LoadRealSenseLib(TargetInfo Target)
        {
            bool isLibrarySupported = false;

            if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
            {
                isLibrarySupported = true;

                string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "64" : "32";
                string LibrariesPath = Path.Combine(ThirdPartyPath, "RSSDK", "Lib");

                PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "Win" + PlatformString, "libpxc.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "Win" + PlatformString, "libpxcmd.lib"));
            }

            if (isLibrarySupported)
            {
                // Include path
                PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "RSSDK", "Include"));
            }

            Definitions.Add(string.Format("WITH_RealSense_BINDING={0}", isLibrarySupported ? 1 : 0));

            return isLibrarySupported;
        }
	}

}