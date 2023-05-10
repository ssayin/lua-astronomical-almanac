local ffi = require("ffi")

local library_path = (function()
	local prefix = string.sub(package.config, 1, 1)
	local dirname = debug.getinfo(1, "S").source:sub(2):match("(.*[/\\])")
	local libname = prefix == "\\" and "liblua_aa.dll" or "liblua_aa.so"
	return dirname .. "../build/" .. libname
end)()

local aa = ffi.load(library_path)

ffi.cdef([[

static const int NARGS = 18;

struct plantbl {
  char maxargs;
  char max_harmonic[NARGS];
  char max_power_of_t;
  char  *arg_tbl;
  void  *lon_tbl;
  void  *lat_tbl;
  void  *rad_tbl;
  double distance;
  double timescale;
  double trunclvl;
};
struct orbit
	{
	char obname[16]; /* name of the object */
	double epoch;	/* epoch of orbital elements */
	double i;	/* inclination	*/
	double W;	/* longitude of the ascending node */
	double w;	/* argument of the perihelion */
	double a;	/* mean distance (semimajor axis) */
	double dm;	/* daily motion */
	double ecc;	/* eccentricity */
	double M;	/* mean anomaly */
	double equinox;	/* epoch of equinox and ecliptic */
	double mag;	/* visual magnitude at 1AU from earth and sun */
	double sdiam;	/* equatorial semidiameter at 1au, arc seconds */
/* The following used by perterbation formulas: */
	struct plantbl *ptable;
	double L;	/* computed mean longitude */
	double r;	/* computed radius vector */
	double plat;	/* perturbation in ecliptic latitude */
	};

struct star
	{
	char obname[32];	/* Object name (31 chars) */
	double epoch;		/* Epoch of coordinates */
	double ra;		/* Right Ascension, radians */
	double dec;		/* Declination, radians */
	double px;		/* Parallax, radians */
	double mura;		/* proper motion in R.A., rad/century */
	double mudec;		/* proper motion in Dec., rad/century */
	double v;		/* radial velocity, km/s */
	double equinox;		/* Epoch of equinox and ecliptic */
	double mag;		/* visual magnitude */
	};
/* Note the items for a star are in different measurement units
 * in the ASCII file description.
 */

/* aa.c */
extern double DTR;
extern double RTD;
extern double RTS;
extern double STR;
extern double PI;
extern double J2000;
extern double B1950;
extern double J1900;
extern double JD;
extern double TDT;
extern double UT;
extern double dradt;
extern double ddecdt;
extern int objnum, jdflag, prtflg;
extern double obpolar[];
extern double  eapolar[];
extern double  rearth[];
extern double dp[];
/* angles.c */
extern double  SE;
extern double  SO;
extern double  EO;
extern double  pq;
extern double  ep;
extern double  qe;

/* nutate.c */
extern double jdnut, nutl, nuto;
/* epsiln.c */
extern double jdeps, eps, coseps, sineps;
/* vearth.c */
extern double jvearth, vearth[];

/* DEBUG = 1 to enable miscellaneous printouts. */
extern int altaz ( double pol[3], double J );
extern int angles ( double p[], double q[], double e[] );
extern int annuab ( double p[] );
extern double arcdot ( double p[], double q[] );
extern int domoon ( void );
extern int iter_trnsit ( int (*func)() );
extern int moonll ( double J, double moonpp[], double moonpol[] );
extern int gmoon ( double J, double rect[], double pol[] );
extern double mods3600 ( double x );
extern int moon1 ( void );
extern int moon2 ( void );
extern int moon3 ( void );
extern int moon4 ( int ltflag );
extern int chewm ( short *p, int nlines, int nangles, int typflg, double ans[] );
extern int showcname ( char *in );
extern int relativity ( double p[], double q[], double e[] );
extern double deltat ( double Y );
extern int update ( void );
extern int diurab ( double last, double *ra, double *dec );
extern int diurpx ( double last, double *ra, double *dec, double dist );
extern int showrd ( char *msg, double p[], double pol[] );
extern int showcor ( char *strng, double p[], double dp[] );
extern int dms ( double x );
extern int hms ( double x );
extern double zgetdate ( void );
extern double caltoj ( long year, int month, double day );
extern int jtocal ( double J );
extern double mod360 ( double x );
extern double modtp ( double x );
extern double gethms ( void );
extern int getnum ( char *msg, void *num, char *format );
extern int deltap ( double p0[], double p1[], double *dr, double *dd );
extern int epsiln ( double J );
extern int fk4fk5 ( double p[], double m[], struct star *el );
extern int gplan ( double J, struct plantbl *plan, double pobj[] );
extern int g3plan ( double J, struct plantbl *plan, double pobj[], int obj );
extern int kepler ( double J, struct orbit *e, double rect[], double polar[] );
extern int embofs ( double J, double emb[], double *r );
extern int kinit ( void );
extern int getorbit ( struct orbit *el );
extern int getstar ( struct star *el );
extern void *fincat ( char *name, int n, char *str1, char *str2 );
extern int lightt ( struct orbit *elemnt, double q[], double e[] );
extern int lonlat ( double pp[], double J, double polar[], int precess );
extern int nutlo ( double J );
extern int nutate ( double J, double p[] );
extern int sscc ( int k, double arg, int n );
extern int precess ( double R[], double J, int direction );
extern double refrac ( double alt );
extern int reduce ( struct orbit *elemnt, double q[], double e[] );
extern int doplanet (void);
extern int rstar ( struct star *el );
extern int dostar (void);
extern double sidrlt ( double j, double tlong );
extern int dosun ( void );
extern double tdb ( double JED );
extern int trnsit ( double J, double lha, double dec );
extern int velearth ( double J );
extern double zatan2 ( double x, double y );
extern double acos(double);
extern double asin(double);
extern double atan(double);
extern double cos(double);
extern double sin(double);
extern double tan(double);
extern double sqrt(double);
extern double fabs(double);
extern double log(double);
extern double floor(double);
extern double sinh(double);
extern double cosh(double);
extern double tanh(double);
extern char *whatconstel (double *, double);

/* see: lua_aa.c */
extern struct orbit venus;
extern struct orbit mercury;
extern struct orbit venus;
extern struct orbit earth;
extern struct orbit mars;
extern struct orbit jupiter;
extern struct orbit saturn;
extern struct orbit uranus;
extern struct orbit neptune;
extern struct orbit pluto;
]])

return aa
