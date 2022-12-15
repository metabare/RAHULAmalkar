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
	else if( verbosity == ELogVerbosity::Display ) { UE_LOG( LogLua, Display, TEXT( "%s" ),