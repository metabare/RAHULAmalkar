// Fill out your copyright notice in the Description page of Project Settings.

#include "UthLuaState.h"

#include "IPluginManager.h"
#include "Paths.h"

#include "UEWrappedSol2.h"

#include <set>
#include <memory>
#include <string>




DECLARE_LOG_CATEGORY_EXTERN( LogLua, Log, All );
DEFINE_LOG_CATEGORY( LogLua );

void UeLogProxy( ELogVerbosity::Type verbosity, const std::string & message )
{
	// Need to copy-paste due to the nature of the UE_LOG macro definition
	if( verbosity == ELogVerbosity::Fatal ) { UE_LOG( LogLua, Fatal, TEXT( "%s" ), UTF8_TO_TCHAR( message.c_str() ) ); }
	else if( verbosity == ELogVerbosity::Error ) { UE_LOG( LogLua, Error, TEXT( "%s" ), UTF8_TO_TCHAR( message.c_str() ) ); }
	else if( verbosity == ELogVerbosity::Warning ) { UE_LOG( LogLua, Warning, TEXT( "%s" ), UTF8_TO_TCHAR( message.c_str() ) ); }
	else if( verbosity == ELogVerbosity::Display ) { UE_LOG( LogLua, Display, TEXT( "%s" ), UTF8_TO_TCHAR( message.c_str() ) ); }
	else if( verbosity == ELogVerbosity::Log ) { UE_LOG( LogLua, Log, TEXT( "%s" ), UTF8_TO_TCHAR( message.c_str() ) ); }
	else if( verbosity == ELogVerbosity::Verbose ) { UE_LOG( LogLua, Verbose, TEXT( "%s" ), UTF8_TO_TCHAR( message.c_str() ) ); }
	else if( verbosity == ELogVerbosity::VeryVerbose ) { UE_LOG( LogLua, VeryVerbose, TEXT( "%s" ), UTF8_TO_TCHAR( message.c_str() ) ); }
	else checkf( false, TEXT( "(%s) Unknown verbosity level: %d" ), TEXT( __FUNCTION__ ), verbosity );
}




UUthLuaState::UUthLuaState()
{
	// Stop if the plugin is not loaded yet (probably UE is just creating an internal instance of us).
	// The field 'lua' will stay null and isValid() will return false for this object.
	// 
	// Note: IsModuleLoaded() returns true even if StartupModule() is still running.
	if( !FModuleManager::Get().IsModuleLoaded( "UnrealTorch" ) ) return;


	// Get base directories
	std::string BaseDirPlugin = TCHAR_TO_UTF8( *IPluginManager::Get().FindPlugin( "UnrealTorch" )->GetBaseDir() );
	std::string BaseDirGameContent = TCHAR_TO_UTF8( *FPaths::GameContentDir() );
	std::string BaseDirGameLogs = TCHAR_TO_UTF8( *FPaths::GameLogDir() );

	// Create a sol-wrapped Lua state
	lua = std::make_unique<sol::state>();

	// Open all standard libraries
	lua->open_libraries();

	// Set paths
	(*lua)["package"]["path"] =
		BaseDirPlugin + "/Source/UnrealTorch/Private/lua/?.lua;" +
		BaseDirPlugin + "/Source/ThirdParty/Torch/WindowsTorch/lua/?.lua;" +
		BaseDirPlugin + "/Source/ThirdParty/Torch/WindowsTorch/lua/?/init.lua;" +
		BaseDirGameContent + "/Lua/?.lua;" +
		BaseDirGameContent + "/Lua/?/init.lua";   // no semicolon at end to avoid getting ;; by accident later
	(*lua)["package"]["cpath"] =
		BaseDirPlugin + "/Source/ThirdParty/Torch/WindowsTorch/bin/?.dll;" +
		BaseDirGameContent + "/Lua/bin/?.dll";   // no semicolon at end to avoid getting ;; by accident later

	// Create and populate the global table 'uth'
	sol::table uth_ue = lua->create_table_with(
		"UE_LOG", UeLogProxy,
		"BuildShippingOrTest", UE_BUILD_SHIPPING || UE_BUILD_TEST,
		"FPaths", lua->create_table_with(
			"GameLogDir", BaseDirGameLogs
		)
	);
	uth_ue.new_enum( "ELogVerbosity",
					 "Fatal", ELogVerbosity::Fatal,
					 "Error", ELogVerbosity::Error,
					 "Warning", ELogVerbosity::Warning,
					 "Display", ELogVerbosity::Display,
					 "Log", ELogVerbosity::Log,
					 "Verbose", ELogVerbosity::Verbose,
					 "VeryVerbose", ELogVerbosity::VeryVerbose
	);
	(*lua)["uth"] = lua->create_table_with(
		"statename", *name.ToString(),
		"ue", uth_ue
	);

	// Call Lua-side initialization script
	try
	{
		lua->script_file( BaseDirPlugin + "/Source/UnrealTorch/Private/lua/uth/init.lua", sol::default_on_error );
	}
	catch( const sol::error & error )
	{
		// We won't get a stack traceback this way; to get one, we should use a protected_function (cf. https://github.com/ThePhD/sol2/issues/280)
		// but that leads to a rather convoluted call syntax
		UE_LOG( LogUnrealTorch, Error, TE