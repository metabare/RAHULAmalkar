--- UnrealTorch Lua state initialization script
-- @script init
--
-- Called by UE from UUthLuaState::construct().
-- This script performs Lua-side initialization of the Lua state.
--
-- The global variable 'uth' has been already set from the C++ side with the following structure:
--   uth                                  The main table for all UnrealTorch data
--     .statename                         Name of this state; see UUthLuaState.setName()
--     .ue                                Static data from UE
--       .UE_LOG( verbosity, message )    Write log entries to UE log
--       .ELogVerbosity                   Verbosity level enumeration for UE_LOG()
--       .BuildShippingOrTest             True if UE is built in Shipping or Test configuration
--       .FPaths                          UE path information
--         .GameLogDir                    Log directory path from FPaths::GameLogDir()
--
-- Output from io.write() and print() is directed to a log file in the UE log directory 'Saved\Logs\'.
--
-- The utility module is imported into 'uth.utility'. A generic logger from the utility module is copied, with its
-- verbosity level enumeration, to the global variable LOG and ELogVerbosity. Usage:
--   LOG( ELogVerbosity.Warning, 'Log message here' )
--
-- Torch is imported into the global variable 'torch'.

uth.ue.UE_LOG( uth.ue.ELogVerbosity.Log, '[init.lua] Initializing Lua state..' )
assert( uth, 'The global variable \'uth\'