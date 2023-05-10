local aa = require("lib_aa")
local ffi = require("ffi")

aa.kinit()

-- print("" .. ffi.string(aa.venus.obname, 16))

::loop::
aa.prtflg = 1
print("Enter starting date of tabulation\n")
aa.JD = aa.zgetdate()
aa.JD = aa.JD + aa.gethms()
aa.update()

print("Julian day " .. string.format("%.7f", aa.JD))
local djd = 1
local ntab = 1
if ntab <= 0 then
  ntab = 1
end

print("Planet number 0-9 or 88 to read star, 99 to read orbit")
aa.objnum = tonumber(io.read())

--if aa.objnum== -1 then
--end
if aa.objnum == 0 then
  print("\n                   The Sun\n")
end
if aa.objnum == 1 then
  aa.elobject = ffi.cast("struct orbit*", aa.mercury)
end
if aa.objnum == 2 then
  aa.elobject = ffi.cast("struct orbit*", aa.venus)
end
if aa.objnum == 3 then
  print("\n                   The Moon\n")
end
if aa.objnum == 4 then
  aa.elobject = ffi.cast("struct orbit*", aa.mars)
end
if aa.objnum == 5 then
  aa.elobject = ffi.cast("struct orbit*", aa.jupiter)
end
if aa.objnum == 6 then
  aa.elobject = ffi.cast("struct orbit*", aa.saturn)
end
if aa.objnum == 7 then
  aa.elobject = ffi.cast("struct orbit*", aa.uranus)
end
if aa.objnum == 8 then
  aa.elobject = ffi.cast("struct orbit*", aa.neptune)
end
if aa.objnum == 9 then
  aa.elobject = ffi.cast("struct orbit*", aa.pluto)
end
-- if aa.objnum == 10 then
-- elobject = aa.test
-- end
-- if aa.objnum == 88 then
-- end

-- if( elobject == (struct orbit *)&fstar )
--	showcname( &elobject->obname[0] );
if aa.elobject then
  print("\n                  " .. ffi.string(aa.elobject[0].obname, 16) .. "\n")
end

for i = 1, ntab do
  -- print Julian date
  print("\nJD " .. aa.JD .. ",  ")
  aa.update()

  -- Always calculate heliocentric position of the earth
  aa.kepler(aa.TDT, aa.earth, aa.rearth, aa.eapolar)

  if aa.objnum == 0 then
    aa.dosun()
    aa.iter_trnsit(aa.dosun)
  elseif aa.objnum == 3 then
    aa.domoon()
    aa.iter_trnsit(aa.domoon)
  elseif aa.objnum == 88 then
    aa.dostar()
    aa.iter_trnsit(aa.star)
  else
    aa.doplanet()
    aa.iter_trnsit(aa.doplanet)
    print("\n")
    aa.JD = aa.JD + djd
  end
end

goto loop
