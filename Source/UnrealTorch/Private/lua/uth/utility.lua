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
      if uth.LOG_ALSO_TO_UE then UE_LOG_capture(...) end
    end
  end
)()




--- Redirected print
--
-- Redirected print() that writes all output via io.write()
function utility.print( ... )
  local arg = table.pack(...)

  io.write( '[print] ' )
  for i = 1, arg.n do
    io.write( tostring( arg[i] ) .. (i ~= arg.n and '\t' or '\n') )
  end
  if arg.n == 0 then io.write( '\n' ) end
  io.flush()

end




--- Redirect print and io.output() to a log file
--
-- Redirecting stderr seems tricky so we ignore that.
function utility.redirect_output()

  local logfile = uth.ue.FPaths.GameLogDir .. '/lua_' .. uth.statename .. '.log'

  uth.