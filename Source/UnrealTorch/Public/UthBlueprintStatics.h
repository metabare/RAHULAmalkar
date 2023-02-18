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
	/** Constructs a new UthLuaState object, creating a new Lua state and initial