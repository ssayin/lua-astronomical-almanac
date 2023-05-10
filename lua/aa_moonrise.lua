local aa = require("lib_aa")

local u

aa.kinit()

print("\nTable of lunar rise, transit, and set times.\n\n")

local STARTDATE = 2448431.5
local ENDDATE = 2448831.5

local objnum = 0
u = STARTDATE

while u <= ENDDATE do
	aa.prtflg = 0
	u = aa.search_moon(u)
	aa.prtflg = 1
	if aa.f_trnsit then
		aa.jtocal(aa.t_rise)
	else
		print("\n")
	end
	aa.jtocal(aa.t_trnsit)
	if aa.f_trnsit then
		aa.jtocal(aa.t_set)
	else
		print("\n")
	end
	aa.prtflg = 0
	print("\n")
	u = u + 1.0
end
