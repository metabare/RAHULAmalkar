// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UthBlueprintStatics.generated.h"


class UUthLuaState;




/**
 * 
 */
UCLASS()
class UNREALTORCH_API UUthBlueprintStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// comment block copy at UUthLuaState::UUthLuaState()
	/** Constructs a new UthLuaState object, creating a new Lua state and initializing it.
	 *
	 * Blueprint users: Make sure to immediately store the returned object in a variable. Using the return value
	 * directly by pulling wires from the return node works only as long as no-one and nothing triggers the garbage
	 * collector; Blueprint return nodes _do not_ hold and protect objects from being collected.
	 *
	 * C++ users: Use factory helpers like UUthBlueprintStatics::CreateLuaState() or UE's NewObject() to create
	 * instances of this class. Never explicitly delete these instances; instead use the destroy() method. See the
	 * section below on object lifecycle for more details.
	 *
	 * A new Lua state is created and initialized with all Torch-related paths set, necessary packages and dlls loaded,
	 * and a set of utility functions defined.
	 *
	 * All Lua standard libraries are opened.
	 *
	 * Torch is imported into the global variable 'torch'.
	 *
	 * The UnrealTorch utility module is imported into 'uth.utility'; for available functions, see the following file:
	 *   Plugins\UnrealTorch\Source\UnrealTorch\Private\lua\uth\utility.lua.
	 *
	 * A generic logger from the utility module is copied, with its verbosity level enumeration, to the global variable
	 * LOG and ELogVerbosity. Verbosity levels follow the UE_LOG() verbosity levels. See also section 'State name and
	 * Lua output' below. Usage:
	 *   LOG( ELogVerbosity.Warning, 'Log message here' )
	 *
	 * A global variable 'uth' is created with the following structure:
	 *   uth								The main table for all UnrealTorch data
	 *     .statename						Name of this state; see UUthLuaState.setName()
	 *     .ue								Data from UE
	 *       .UE_LOG( verbosity, message )	Write log entries to UE log
	 *       .ELogVerbosity					Verbosity level enumeration for UE_LOG()
	 *       .BuildShippingOrTest			True if UE is running in Shipping or Test configuration
	 *       .F