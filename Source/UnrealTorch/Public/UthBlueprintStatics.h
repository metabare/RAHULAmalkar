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
	 * section below on object lifecycle for mo