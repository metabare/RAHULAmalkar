--- UnrealTorch manual tests
-- @module uth.tests
--
-- A set of tests to be run and checked manually.


local tests = {}


tests.inspect = require('inspect')




--- Run all manual tests
function tests.run()
  uth.utility.log_function_start()

  -- Test output redirection and logging
  print( 'Testing redirection of print().' )
  io.write( 'Testing redirection of io.write().\n' )
  uth.ue.UE_LOG( ELogVerbosity.Log, 'Testing UE_LOG().' )
  uth.utility.LUA_LOG( ELogVerbosity.Log, 'Testing LUA_LOG().' )
  LOG( ELogVerbosity.Log, 'Testing generic LOG().' )
  function ne