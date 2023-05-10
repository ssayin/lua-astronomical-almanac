local aa = require("lib_aa")

local STARTDATE = 2439935.5
local ENDDATE = 2445000.5
aa.kinit()
local t
local t0
t0 = STARTDATE

local EQUINOX = 0
local NEWMOON = 1
local FULLMOON = 2
local type = NEWMOON

while t0 <= ENDDATE do
	aa.prtflg = 0
	if type == EQUINOX then
		t = aa.search(t0, 0.0, 364.0)
	end
	if type == NEWMOON then
		t = aa.search(t0, aa.PI, 27.0)
	end
	if type == FULLMOON then
		t = aa.search(t0, 0.0, 27.0)
	end

	aa.TDT = t
	print(t)
	aa.prtflg = 1
	aa.jtocal(t)
	t0 = t
end
