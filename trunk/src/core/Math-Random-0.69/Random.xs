#ifdef __cplusplus
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#ifdef __cplusplus
}
#endif

#include "randlib.h"
#include "helper.h"

static int
not_here(s)
char *s;
{
    croak("%s not implemented on this architecture", s);
    return -1;
}

static double
constant(name, arg)
char *name;
int arg;
{
    errno = 0;
    switch (*name) {
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}


MODULE = Math::Random		PACKAGE = Math::Random		


double
genbet (aa,bb)
	INPUT:
	double  aa
	double  bb

double
genchi (df)
	INPUT:
	double  df

double
genexp (av)
	INPUT:
	double  av

double
genf (dfn,dfd)
	INPUT:
	double  dfn
	double  dfd

double
gengam (a,r)
	INPUT:
	double  a
	double  r

int
psetmn (p)
	INPUT:
	long  p

int
pgenmn ()
	PROTOTYPE:
	INPUT:
	CODE:
	RETVAL = pgenmn();
	OUTPUT:
	RETVAL

int
rspriw (size)
	INPUT:
	long  size

int
rsprfw (size)
	INPUT:
	long  size

void
svprfw (index,value)
	INPUT:
	long  index
	double  value

void
pgnmul (n,ncat)
	INPUT:
	long  n
	long  ncat

long
gvpriw (index)
	INPUT:
	long  index

double
gennch (df,xnonc)
	INPUT:
	double  df
	double  xnonc

double
gennf (dfn,dfd,xnonc)
	INPUT:
	double  dfn
	double  dfd
	double  xnonc

double
gennor (av,sd)
	INPUT:
	double  av
	double  sd

void
pgnprm (n)
	PROTOTYPE: $
	INPUT:
	long  n
	CODE:
	pgnprm(n);
	OUTPUT:

double
genunf (low,high)
	INPUT:
	double  low
	double  high

long
ignpoi (mu)
	INPUT:
	double  mu

long
ignuin (low,high)
	INPUT:
	long  low
	long  high

long
ignnbn (n,p)
	INPUT:
	long  n
	double  p

long
ignbin (n,pp)
	INPUT:
	long  n
	double  pp

void
phrtsd (phrase)
	PROTOTYPE: $
	INPUT:
	char *  phrase
	PREINIT:
	long  newseed1;
	long  newseed2;
	PPCODE:
	phrtsd(phrase,&newseed1,&newseed2);
	EXTEND(sp, 2);
	PUSHs(sv_2mortal(newSViv(newseed1)));
	PUSHs(sv_2mortal(newSViv(newseed2)));

void
getsd ()
	PROTOTYPE:
	PREINIT:
	long  newseed1;
	long  newseed2;
	PPCODE:
	getsd(&newseed1,&newseed2);
	EXTEND(sp, 2);
	PUSHs(sv_2mortal(newSViv(newseed1)));
	PUSHs(sv_2mortal(newSViv(newseed2)));

void
salfph (phrase)
	PROTOTYPE: $
	INPUT:
	char *  phrase
	CODE:
	salfph(phrase);
	OUTPUT:

void
setall (iseed1,iseed2)
	PROTOTYPE: $$
	INPUT:
	long  iseed1
	long  iseed2
	CODE:
	setall(iseed1,iseed2);
	OUTPUT:

double
gvprfw (index)
	INPUT:
	long  index

