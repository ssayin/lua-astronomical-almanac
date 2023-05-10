/* This program calculates orbits of planetary bodies and reduces
 * the coordinates of planets or stars to geocentric and topocentric
 * place.  An effort has been made to use rigorous methods throughout.
 *
 * References to AA page numbers are to The Astronomical Almanac, 1986
 * published by the U.S. Government Printing Office.
 *
 * The program uses as a default the PLAN404 approximations to DE404
 * for planetary positions.
 *
 * Warning! Your atan2() function may not work the same as the one
 * assumed by this program.
 * atan2(x,y) computes atan(y/x), result between 0 and 2pi.
 *
 * S. L. Moshier, November, 1987
 */

/*
 * lua_aa.c: Removed extern main(...) and merged aa.c, conjunct.c, and
 * moonrise.c together. - Serdar Sayın
 *
 * All credit goes to S. L. Moshier
 * */

#include <stdlib.h>

double DTR = 1.7453292519943295769e-2;
double RTD = 5.7295779513082320877e1;
double RTS = 2.0626480624709635516e5;  /* arc seconds per radian */
double STR = 4.8481368110953599359e-6; /* radians per arc second */
double PI = 3.14159265358979323846;
extern double PI;

// conjunct.c
double TPI = 6.28318530717958647693;

/* Standard epochs.  Note Julian epochs (J) are measured in
 * years of 365.25 days.
 */
double J2000 = 2451545.0;   /* 2000 January 1.5 */
double B1950 = 2433282.423; /* 1950 January 0.923 Besselian epoch */
double J1900 = 2415020.0;   /* 1900 January 0, 12h UT */

/* Data structures containing orbital elements of
 * objects that orbit the sun.  See kep.h for the definition.
 */
#include "aa/kep.h"

#ifdef __BORLANDC__
#include <stdlib.h>
#endif

#ifdef _MSC_VER
#if _MSC_VER >= 1000
#include <stdlib.h>
#include <string.h>
#endif
#endif

/* approximate motion of right ascension and declination
 * of object, in radians per day
 */
double FAR dradt;
double FAR ddecdt;

/* Space for star description read from a disc file.
 */
struct star fstar;

/* Space for orbit read from a disc file. Entering 99 for the
 * planet number yields a prompt for a file name containg ASCII
 * strings specifying the elements.
 */
struct orbit forbit;

/* Orbits for each planet.  The indicated orbital elements are
 * not actually used, since the positions are are now calculated
 * from a formula.  Magnitude and semidiameter are still used.
 */
/* Programs to compute perturbations. */
extern struct plantbl mer404, ven404, ear404, mar404;
extern struct plantbl jup404, sat404, ura404, nep404, plu404;

struct orbit mercury = {"Mercury        ",
                        2446800.5, /* January 5.0, 1987 */
                        7.0048,
                        48.177,
                        29.074,
                        0.387098,
                        4.09236,
                        0.205628,
                        198.7199,
                        2446800.5,
                        -0.42,
                        3.36,
                        &mer404,
                        0.0,
                        0.0,
                        0.0};

struct orbit venus = {"Venus          ", 2446800.5, 3.3946, 76.561, 54.889,
                      0.723329, 1.60214, 0.006757, 9.0369, 2446800.5,
                      /* Note the calculated apparent visual magnitude for Venus
                       * is not very accurate.
                       */
                      -4.40, 8.34, &ven404, 0.0, 0.0, 0.0};

/* Fixed numerical values will be used for earth if read in from a file
 * named earth.orb.  See kfiles.c, kep.h.
 */
struct orbit earth = {
    "Earth          ", 2446800.5, 0.0,    0.0,       102.884, 0.999999,
    0.985611,          0.016713,  1.1791, 2446800.5, -3.86,   0.0,
    &ear404,           0.0,       0.0,    0.0};
extern struct orbit earth;

struct orbit mars = {"Mars           ", 2446800.5, 1.8498,   49.457,
                     286.343,           1.523710,  0.524023, 0.093472,
                     53.1893,           2446800.5, -1.52,    4.68,
                     &mar404,           0.0,       0.0,      0.0};

struct orbit jupiter = {"Jupiter        ", 2446800.5, 1.3051,    100.358,
                        275.129,           5.20265,   0.0830948, 0.048100,
                        344.5086,          2446800.5, -9.40,     98.44,
                        &jup404,           0.0,       0.0,       0.0};

struct orbit saturn = {"Saturn         ", 2446800.5, 2.4858,    113.555,
                       337.969,           9.54050,   0.0334510, 0.052786,
                       159.6327,          2446800.5, -8.88,     82.73,
                       &sat404,           0.0,       0.0,       0.0};

struct orbit uranus = {
    "Uranus         ", 2446800.5, 0.7738,  73.994,    98.746, 19.2233,
    0.0116943,         0.045682,  84.8516, 2446800.5, -7.19,  35.02,
    &ura404,           0.0,       0.0,     0.0};

struct orbit neptune = {"Neptune        ", 2446800.5, 1.7697,     131.677,
                        250.623,           30.1631,   0.00594978, 0.009019,
                        254.2568,          2446800.5, -6.87,      33.50,
                        &nep404,           0.0,       0.0,        0.0};

struct orbit pluto = {
    "Pluto          ", 2446640.5, 17.1346,  110.204,   114.21, 39.4633,
    0.00397570,        0.248662,  355.0554, 2446640.5, -1.0,   2.07,
    &plu404,           0.0,       0.0,      0.0};

/*
int otest(), ctest();
*/
struct orbit test = {"Test orbit     ",
                     2446800.5,
                     1.8498,
                     49.457,
                     286.343,
                     1.523710,
                     0.524023,
                     0.093472,
                     53.1893,
                     2446800.5,
                     -1.52,
                     4.68,
                     0,
                     0.0,
                     0.0,
                     0.0};

/* coordinates of object
 */
int objnum = 0;            /* I.D. number of object */
double robject[3] = {0.0}; /* position */
/* ecliptic polar coordinates:
 * longitude, latitude in radians
 * radius in au
 */
double FAR obpolar[3];

/* coordinates of Earth
 */
/* Heliocentric rectangular equatorial position
 * of the earth at time TDT re equinox J2000
 */
double FAR rearth[3];
/* Corresponding polar coordinates of earth:
 * longitude and latitude in radians, radius in au
 */
double FAR eapolar[3];

/* Julian date of ephemeris
 */
double JD;
double TDT;
double UT;

/* flag = 0 if TDT assumed = UT,
 *      = 1 if input time is TDT,
 *      = 2 if input time is UT.
 */
int jdflag = 0;

/* correction vector, saved for display  */
double dp[3];

/* display formats for printf()
 */
extern char *intfmt, *dblfmt;

/* display enable flag
 */
int prtflg = 1;

/* Tabulation parameters
 */
static double djd = 1.0;
static int ntab = 1;

struct orbit *elobject;

// conjunct.c

struct orbit objorb = {"Object         ",
                       2446800.5,
                       0.0,
                       0.0,
                       102.884,
                       0.999999,
                       0.985611,
                       0.016713,
                       1.1791,
                       2446800.5,
                       -3.86,
                       0.0,
                       0, /* &ear404, */
                       0.0,
                       0.0,
                       0.0};

double func(double t);
int apparent(double p[], double q[]);

/* pointer to orbital elements of object */
/* Find apparent coordinates of object at Julian date TDT.
   Q is heliocentric position of object.
   P is geocentric, object minus earth.
   Both outputs are in equatorial rectangular coordinates
   and are referred to the equinox and ecliptic of date.  */
int apparent(double p[], double q[]) {
  double polar[3];
  int i;
  static double TDTearth = -1.0e38;

  /* Calculate heliocentric position of the earth */
  if (TDTearth != TDT) {
    kepler(TDT, &earth, rearth, eapolar);
    TDTearth = TDT;
  }

  if (objnum == 3) {
    moonll(TDT, p, polar);
    for (i = 0; i < 3; i++) {
      q[i] = p[i] + rearth[i];
    }
    return 0;
  } else if (objnum == 0) {
    for (i = 0; i < 3; i++) {
      q[i] = 0.0;
      p[i] = -rearth[i];
    }
  } else if ((objnum > 0) && (objnum < 10)) {
    kepler(TDT, &objorb, q, polar);
  } else
    exit(1);

  /* Adjust for light time (planetary aberration)
   */
  if (objnum != 0)
    lightt(&objorb, q, rearth);

  /* Find Euclidean vectors between earth, object, and the sun
   */
  for (i = 0; i < 3; i++)
    p[i] = q[i] - rearth[i];

  angles(p, q, rearth);

  /* Find unit vector from earth in direction of object
   */
  for (i = 0; i < 3; i++)
    p[i] /= EO;

  /* Correct position for light deflection
   */
  if (objnum != 0)
    relativity(p, q, rearth);

  /* Correct for annual aberration
   */
  annuab(p);

  /* Precession of the equinox and ecliptic
   * from J2000.0 to ephemeris date
   */
  precess(p, TDT, -1);

  /* Ajust for nutation
   * at current ecliptic.
   */
  epsiln(TDT);
  nutate(TDT, p);

  /* Return dimensions in au.  */
  for (i = 0; i < 3; i++)
    p[i] *= EO;
  return 0;
}

/* Search for desired conjunction.
   Step forward DELTA days from date T.  Then begin searching
   for the date at which the desired function equals Y.  */
double search(double t, double y, double delta) {
  double tl, tm, th, yl, ym, yh, e;

  tl = t;
  yl = func(t);
  th = tl + delta;
  yh = yl;
  /* Bracket the solution date.  */
  do {
    tl = th;
    yl = yh;
    th = th + 1.0;
    yh = func(th);
    e = yh - y;
    if (e > PI) {
      e -= TPI;
      yh -= TPI;
    }
    if (e < -PI) {
      e += TPI;
      yh += TPI;
    }
  } while (e < 0);

  /* Search by simple interval halving.  */
  while ((th - tl) > .00001) {
    tm = 0.5 * (tl + th);
    ym = func(tm);
    e = ym - y;
    if (e > PI) {
      e -= TPI;
      ym -= TPI;
    }
    if (e < -PI) {
      e += TPI;
      ym += TPI;
    }
    if (e > 0) {
      yh = ym;
      th = tm;
    } else {
      yl = ym;
      tl = tm;
    }
  }
  tm = tl + (th - tl) * (y - yl) / (yh - yl);
  return (tm);
}

// I will figure out what to do with these later - SS
#define NEWMOON 1
#define FULLMOON 1
#undef EQUINOX

/* Compute desired relation of apperent ecliptic longitude
   as a function of the ephemeris date.  */
double func(double t) {
  double p[3], q[3], polar[3];
  double s;
#if NEWMOON || FULLMOON
  double m;
#endif

  TDT = t;
  /* Longitude of the sun.  */
  objnum = 0;
  apparent(p, q);
  lonlat(p, TDT, polar, 0);
  s = polar[0];
#if NEWMOON || FULLMOON
  /* Longitude of the moon.  */
  objnum = 3;
  apparent(p, q);
  lonlat(p, TDT, polar, 0);
  m = polar[0];
  return (s - m);
#endif
#if EQUINOX
  return s;
#endif
}

// moonrise.c

/* Results computed by domoon.c  */
/* Transit, rise, and set times in radians (2 pi = 1 day) */
extern int f_trnsit;
extern double r_trnsit;
extern double r_rise;
extern double r_set;

// marking these extern
extern double t_trnsit;
extern double t_rise;
extern double t_set;

void func_moon(double t);
double search_moon(double t);

double search_moon(double t) {
  double JDsave, TDTsave, UTsave;
  double date_save, date, t0, t1;
  double rise1, set1, trnsit1;

  JDsave = JD;
  TDTsave = TDT;
  UTsave = UT;
  date_save = floor(t - 0.5) + 0.5;

  /* Find transit time. */
  date = date_save;
  func_moon(t);
  do {
    if (r_trnsit < 0.0) {
      date -= 1.0;
      r_trnsit += TPI;
    }
    if (r_trnsit > TPI) {
      date += 1.0;
      r_trnsit -= TPI;
    }
    t0 = date + r_trnsit / TPI;
    func_moon(t0);
    t1 = date + r_trnsit / TPI;
  } while (fabs(t1 - t0) > .0001);

  t_trnsit = t1;
  trnsit1 = r_trnsit;
  set1 = r_set;
  if (f_trnsit == 0)
    goto sdone;

  /* Set current date to be that of the transit just found.  */
  date_save = date;
  do {
    if (r_rise < 0.0) {
      date -= 1.0;
      r_rise += TPI;
    }
    if (r_rise > TPI) {
      date += 1.0;
      r_rise -= TPI;
    }
    t0 = date + r_rise / TPI;
    func_moon(t0);
    t1 = date + r_rise / TPI;
  } while (fabs(t1 - t0) > .0001);
  rise1 = r_rise;
  t_rise = t1;

  date = date_save;
  r_set = set1;
  do {
    if (r_set < 0.0) {
      date -= 1.0;
      r_set += TPI;
    }
    if (r_set > TPI) {
      date += 1.0;
      r_set -= TPI;
    }
    t0 = date + r_set / TPI;
    func_moon(t0);
    t1 = date + r_set / TPI;
  } while (fabs(t1 - t0) > .0001);

  t_set = t1;
  r_trnsit = trnsit1;
  r_rise = rise1;
  /*  printf("%.15e %.15e %.15e\n", rise1, trnsit1, set1); */
sdone:
  JD = JDsave;
  TDT = TDTsave;
  UT = UTsave;
  return t_trnsit;
}

/* Compute estimate of lunar rise and set times.  */
void func_moon(double t) {
  int prtsave;

  prtsave = prtflg;
  prtflg = 0;
  objnum = 3;
  JD = t;
  update(); /* find UT */
  kepler(TDT, &earth, rearth, eapolar);
  domoon();
  prtflg = prtsave;
}
