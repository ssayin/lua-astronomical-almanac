local aa = require("lib_aa")
local ffi = require("ffi")

aa.kinit()

print(aa.sqrt(5))
print(aa.sqrt(11))

aa.kinit()

local elobject = nil
JD = aa.zgetdate()
JD = JD + aa.gethms()
aa.update()
print("Julian day " .. string.format("%.7f", JD))

print("" .. ffi.string(aa.venus.obname, 16))
