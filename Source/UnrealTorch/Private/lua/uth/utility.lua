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

  uth.ue.UE_LOG( uth.ue.ELogVerbosity.Log, 'Redirecting Lua print() and io.write() to \'' .. logfile .. '\'' )
  print = utility.print
  io.output( logfile )

end




--- Modify a logging function so that it logs also the names of the nearest calling functions
function utility.decorate_logger( logger, message_arg_index )
  local callstack_offset = 5

  return function( ... )
    local args = table.pack(...)
    if not type(args[message_arg_index]) == "string" then return end

    local callstack = range( 1 + callstack_offset, 1 + callstack_offset + uth.LOG_CALLSTACK_DEPTH - 1 )
                      :map( function(d) return debug.getinfo(d, 'n') end )
                      :take_while( function(x) return x and x.name end )
                      :map( function(x) return x.name end )
                      :reduce( function(acc,x) return x .. '.' .. acc end, '' )

    args[message_arg_index] = '[' .. callstack:sub( 1, callstack:len() - 1 ) .. '] ' .. args[message_arg_index]

    return logger( unpack(args) )
  end

end




--- Add a log entry with the calling function's name and arguments
--
-- Logs the filename, line, name and callstack of the calling function, together with all its local variables.
-- 