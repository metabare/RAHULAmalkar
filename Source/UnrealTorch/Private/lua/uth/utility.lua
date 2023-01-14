--- UnrealTorch utility module
-- @module uth.utility
--
-- A collection of utility functions and global redefinitions.




local utility = {}


utility.inspect = require( 'inspect' )




--- Trap into ZeroBrane Studio remote debugger
function utility.breakpoint()
  require('mobdebug').start()
end




--- Log to io.output()
utility.LUA_LOG = (
  function()
    local ELogVerbosity_inv = tomap( map( funct