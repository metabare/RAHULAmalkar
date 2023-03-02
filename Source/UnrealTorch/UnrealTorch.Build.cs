// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UnrealTorch : ModuleRules
{
	public UnrealTorch(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// Including Sol2 without this gives the error C4577 from <utility>
		// Alternative solution: disable 4577 in WindowsPlatformCompilerSetup.h
		//   (disabling 4577 in your own code is not enough, for some reason)
		// Also, we now actually use exceptions as they now work with the latest versions of everything.
		bEnableExceptions = true;

		PublicIncludePaths.AddRange(
			new string[] {
				"UnrealTor