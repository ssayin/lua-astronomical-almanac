find_library(MATH_LIBRARY m)

add_library(
  ${PROJECT_NAME} SHARED
  lua_aa.c
  aa/altaz.c
  aa/angles.c
  aa/annuab.c
  aa/constel.c
  aa/deflec.c
  aa/deltat.c
  aa/diurab.c
  aa/diurpx.c
  aa/dms.c
  aa/epsiln.c
  aa/fk4fk5.c
  aa/kepler.c
  aa/kfiles.c
  aa/lightt.c
  aa/lonlat.c
  aa/nutate.c
  aa/precess.c
  aa/refrac.c
  aa/rplanet.c
  aa/rstar.c
  aa/sidrlt.c
  aa/sun.c
  aa/domoon.c
  aa/trnsit.c
  aa/vearth.c
  aa/zatan2.c
  aa/gplan.c
  aa/mer404.c
  aa/ven404.c
  aa/ear404.c
  aa/mar404.c
  aa/jup404.c
  aa/sat404.c
  aa/ura404.c
  aa/nep404.c
  aa/plu404.c
  aa/mlr404.c
  aa/mlat404.c)

target_include_directories(${PROJECT_NAME} PUBLIC aa)

if(MATH_LIBRARY)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${MATH_LIBRARY})
endif()
