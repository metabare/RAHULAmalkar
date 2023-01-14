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
    local ELogVerbosity_inv = tomap( map( function(k,v) return v, k end, getmetatable(uth.ue.ELogVerbosity).__index ) )

    return function( verbosity, message )
      io.write( '[' .. ELogVerbosity_inv[verbosity] .. '] ' .. message .. '\n' )
      io.flush()
    end
  end
)()




--- The default logger
--
-- Log to LUA_LOG and optionally also to UE_LOG. The latter is controlled by the uth.LOG_ALSO_TO_UE flag.
utility.LOG = (
  function()
    -- We don't want later output decorations to affect our loggers
    local LUA_LOG_capture = utility.LUA_LOG
    local UE_LOG_capture  = uth.ue.UE_LOG

    return function(...)
      LUA_LOG_capture(...)
      if uth.LOG_ALSO_TO_UE the