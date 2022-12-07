#include "CoreTypes.h"
#include "Containers/UnrealString.h"
#include "Misc/AutomationTest.h"

#include "UthLuaState.h"

#include "UObjectGlobals.h"
#include "Paths.h"

#include <memory>
#include <fstream>


#if WITH_DEV_AUTOMATION_TESTS


IMPLEMENT_SIMPLE_AUTOMATION_TEST( FUthLuaStateTest, "Project.UnrealTorch.UthLuaState",
								  EAutomationTestFlags::ApplicationContextMask |
								  EAutomationTestFlags::ProductFilter )




bool FUthLuaStateTest::RunTest( const FString & Parameters )
{
	// Object construction, destruction and naming
	{
		UUthLuaState * luaC1{ NewObject<UUthLuaState>( GetTransientPackage(), FName() ) };    // becomes a dangling pointer after next GC round
		UUthLuaState * luaC2{ NewObject<UUthLuaState>( GetTransientPackage(), FName(), RF_MarkAsRootSet ) };

		TestNotNull( TEXT( "UthLuaState created in C++ via NewObject()" ),
					 luaC1 );
		TestNotNull( TEXT( "UthLuaState created in C++ via NewObject() with MarkAsRootSe