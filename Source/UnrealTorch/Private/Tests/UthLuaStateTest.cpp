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
		TestNotNull( TEXT( "UthLuaState created in C++ via NewObject() with MarkAsRootSet flag" ),
					 luaC2 );

		TestTrue( TEXT( "UthLuaState created in C++ via NewObject(): isValid()" ),
				  luaC1->isValid() );
		TestTrue( TEXT( "UthLuaState created in C++ via NewObject() with MarkAsRootSet flag: isValid()" ),
				  luaC2->isValid() );

		TestEqual( TEXT( "UthLuaState created in C++ via NewObject(): getName() == 'default'" ),
				   luaC1->getName(),
				   FName( "default" ) );

		luaC1->setName( "TestStateName" );
		std::string luaStateNameActual = luaC1->getLuaState()["uth"]["statename"];    // Sol requires assignment
		TestEqual( TEXT( "UthLuaState created in C++ via NewObject(): setName( <name> ) => getName() == <name>" ),
				   luaC1->getName(),
				   FName( "TestStateName" ) );
		TestEqual( TEXT( "UthLuaState created in C++ via NewObject(): setName( <name> ) => Lua uth.statename == <name>" ),
				   luaStateNameActual,
				   std::string( "TestStateName" ) );

		UUthLuaState * luaC1_PendingKill = luaC1;    // becomes a dangling pointer after next GC round
		UUthLuaState * luaC2_PendingKill = luaC2;    // becomes a dangling pointer after next GC round
		luaC1->destroy(); luaC1 = nullptr;
		luaC2->destroy(); luaC2 = nullptr;

		TestFalse( TEXT( "UthLuaState created in C++ via NewObject(): destroy() => isValid()" ),
				   lu