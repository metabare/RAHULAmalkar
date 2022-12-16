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
	lu