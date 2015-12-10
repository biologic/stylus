#include "randlib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define max(a,b) ((a) >= (b) ? (a) : (b))
void ftnstop(char*);

double genbet(double aa,double bb)
/*
**********************************************************************
     double genbet(double aa,double bb)
               GeNerate BETa random deviate
                              Function
     Returns a single random deviate from the beta distribution with
     parameters A and B.  The density of the beta is
               x^(a-1) * (1-x)^(b-1) / B(a,b) for 0 < x < 1
                              Arguments
     aa --> First parameter of the beta distribution
       
     bb --> Second parameter of the beta distribution

                              Method
     R. C. H. Cheng
     Generating Beta Variates with Nonintegral Shape Parameters
     Communications of the ACM, 21:317-322  (1978)
     (Algorithms BB and BC)
**********************************************************************
*/
{
/* JJV changed expmax (log(1.0E38)==87.49823), and added minlog */
#define expmax 87.4982335337737
#define infnty 1.0E38
#define minlog 1.0E-37
static double olda = -1.0E37;
static double oldb = -1.0E37;
static double genbet,a,alpha,b,beta,delta,gamma,k1,k2,r,s,t,u1,u2,v,w,y,z;
static long qsame;

    qsame = olda == aa && oldb == bb;
    if(qsame) goto S20;
    if(!(aa < minlog || bb < minlog)) goto S10;
    fputs(" AA or BB < 1.0E-37 in GENBET - Abort!\n",stderr);
    fprintf(stderr," AA: %16.6E BB %16.6E\n",aa,bb);
    exit(1);
S10:
    olda = aa;
    oldb = bb;
S20:
    if(!(min(aa,bb) > 1.0)) goto S100;
/*
     Algorithm BB
     Initialize
*/
    if(qsame) goto S30;
    a = min(aa,bb);
    b = max(aa,bb);
    alpha = a+b;
    beta = sqrt((alpha-2.0)/(2.0*a*b-alpha));
    gamma = a+1.0/beta;
S30:
    u1 = ranf();
/*
     Step 1
*/
    u2 = ranf();
    v = beta*log(u1/(1.0-u1));
/* JJV altered this */
    if(v > expmax) goto S55;
/*
 * JJV added checker to see if a*exp(v) will overflow
 * JJV S50 _was_ w = a*exp(v); also note here a > 1.0
 */
    w = exp(v);
    if(w > infnty/a) goto S55;
    w *= a;
    goto S60;
S55:
    w = infnty;
S60:
    z = pow(u1,2.0)*u2;
    r = gamma*v-1.38629436111989;
    s = a+r-w;
/*
     Step 2
*/
    if(s+2.60943791243410 >= 5.0*z) goto S70;
/*
     Step 3
*/
    t = log(z);
    if(s > t) goto S70;
/*
 *   Step 4
 *
 *    JJV added checker to see if log(alpha/(b+w)) will 
 *    JJV overflow.  If so, we count the log as -INF, and
 *    JJV consequently evaluate conditional as true, i.e.
 *    JJV the algorithm rejects the trial and starts over
 *    JJV May not need this here since alpha > 2.0
 */
    if(alpha/(b+w) < minlog) goto S30;
    if(r+alpha*log(alpha/(b+w)) < t) goto S30;
S70:
/*
     Step 5
*/
    if(aa == a) {
       genbet = w/(b+w);
    } else {
       genbet = b/(b+w);
    }
    goto S230;
S100:
/*
     Algorithm BC
     Initialize
*/
    if(qsame) goto S110;
    a = max(aa,bb);
    b = min(aa,bb);
    alpha = a+b;
    beta = 1.0/b;
    delta = 1.0+a-b;
    k1 = delta*(1.38888888888889E-2+4.16666666666667E-2*b) /
         (a*beta-0.777777777777778);
    k2 = 0.25+(0.5+0.25/delta)*b;
S110:
S120:
    u1 = ranf();
/*
     Step 1
*/
    u2 = ranf();
    if(u1 >= 0.5) goto S130;
/*
     Step 2
*/
    y = u1*u2;
    z = u1*y;
    if(0.25*u2+z-y >= k1) goto S120;
    goto S170;
S130:
/*
     Step 3
*/
    z = pow(u1,2.0)*u2;
    if(!(z <= 0.25)) goto S160;
    v = beta*log(u1/(1.0-u1));
/*
 *    JJV instead of checking v > expmax at top, I will check
 *    JJV if a < 1, then check the appropriate values
 */
    if(a > 1.0) goto S135;
/*   JJV a < 1 so it can help out if exp(v) would overflow */
    if(v > expmax) goto S132;
    w = a*exp(v);
    goto S200;
S132:
    w = v + log(a);
    if(w > expmax) goto S140;
    w = exp(w);
    goto S200;
S135:
/*   JJV in this case a > 1 */
    if(v > expmax) goto S140;
    w = exp(v);
    if(w > infnty/a) goto S140;
    w *= a;
    goto S200;
S140:
    w = infnty;
    goto S200;
/*
 * JJV old code
 *    if(!(v > expmax)) goto S140;
 *    w = infnty;
 *    goto S150;
 *S140:
 *    w = a*exp(v);
 *S150:
 *    goto S200;
 */
S160:
    if(z >= k2) goto S120;
S170:
/*
     Step 4
     Step 5
*/
    v = beta*log(u1/(1.0-u1));
/*   JJV same kind of checking as above */
    if(a > 1.0) goto S175;
/* JJV a < 1 so it can help out if exp(v) would overflow */
    if(v > expmax) goto S172;
    w = a*exp(v);
    goto S190;
S172:
    w = v + log(a);
    if(w > expmax) goto S180;
    w = exp(w);
    goto S190;
S175:
/* JJV in this case a > 1.0 */
    if(v > expmax) goto S180;
    w = exp(v);
    if(w > infnty/a) goto S180;
    w *= a;
    goto S190;
S180:
    w = infnty;
/*
 *   JJV old code
 *    if(!(v > expmax)) goto S180;
 *    w = infnty;
 *    goto S190;
 *S180:
 *    w = a*exp(v);
 */
S190:
/*
 * JJV here we also check to see if log overlows; if so, we treat it
 * JJV as -INF, which means condition is true, i.e. restart
 */
    if(alpha/(b+w) < minlog) goto S120;
    if(alpha*(log(alpha/(b+w))+v)-1.38629436111989 < log(z)) goto S120;
S200:
/*
     Step 6
*/
    if(a == aa) {
       genbet = w/(b+w);
    } else {
       genbet = b/(b+w);
    }
S230:
    return genbet;
#undef expmax
#undef infnty
#undef minlog
}

double genchi(double df)
/*
**********************************************************************
     double genchi(double df)
                Generate random value of CHIsquare variable
                              Function
     Generates random deviate from the distribution of a chisquare
     with DF degrees of freedom random variable.
                              Arguments
     df --> Degrees of freedom of the chisquare
            (Must be positive)
       
                              Method
     Uses relation between chisquare and gamma.
**********************************************************************
*/
{
static double genchi;

    if(!(df <= 0.0)) goto S10;
    fputs(" DF <= 0 in GENCHI - ABORT\n",stderr);
    fprintf(stderr," Value of DF: %16.6E\n",df);
    exit(1);
S10:
/*
 * JJV changed the code to call SGAMMA directly
 *    genchi = 2.0*gengam(1.0,df/2.0); <- OLD
 */
    genchi = 2.0*sgamma(df/2.0);
    return genchi;
}

double genexp(double av)
/*
**********************************************************************
     double genexp(double av)
                    GENerate EXPonential random deviate
                              Function
     Generates a single random deviate from an exponential
     distribution with mean AV.
                              Arguments
     av --> The mean of the exponential distribution from which
            a random deviate is to be generated.
        JJV (av >= 0)
                              Method
     Renames SEXPO from TOMS as slightly modified by BWB to use RANF
     instead of SUNIF.
     For details see:
               Ahrens, J.H. and Dieter, U.
               Computer Methods for Sampling From the
               Exponential and Normal Distributions.
               Comm. ACM, 15,10 (Oct. 1972), 873 - 882.
**********************************************************************
*/
{
static double genexp;

/* JJV added check that av >= 0 */
    if(av >= 0.0) goto S10;
    fputs(" AV < 0 in GENEXP - ABORT\n",stderr);
    fprintf(stderr," Value of AV: %16.6E\n",av);
    exit(1);
S10:
    genexp = sexpo()*av;
    return genexp;
}

double genf(double dfn,double dfd)
/*
**********************************************************************
     double genf(double dfn,double dfd)
                GENerate random deviate from the F distribution
                              Function
     Generates a random deviate from the F (variance ratio)
     distribution with DFN degrees of freedom in the numerator
     and DFD degrees of freedom in the denominator.
                              Arguments
     dfn --> Numerator degrees of freedom
             (Must be positive)
     dfd --> Denominator degrees of freedom
             (Must be positive)
                              Method
     Directly generates ratio of chisquare variates
**********************************************************************
*/
{
static double genf,xden,xnum;

    if(!(dfn <= 0.0 || dfd <= 0.0)) goto S10;
    fputs(" Degrees of freedom nonpositive in GENF - abort!\n",stderr);
    fprintf(stderr," DFN value: %16.6E DFD value: %16.6E\n",dfn,dfd);
    exit(1);
S10:
/*
 * JJV changed this to call SGAMMA directly
 *
 *     GENF = ( GENCHI( DFN ) / DFN ) / ( GENCHI( DFD ) / DFD )
 *   xnum = genchi(dfn)/dfn; <- OLD
 *   xden = genchi(dfd)/dfd; <- OLD
 */
    xnum = 2.0*sgamma(dfn/2.0)/dfn;
    xden = 2.0*sgamma(dfd/2.0)/dfd;
/*
 * JJV changed constant to prevent underflow at compile time.
 *   if(!(xden <= 9.999999999998E-39*xnum)) goto S20;
 */
    if(!(xden <= 1.0E-37*xnum)) goto S20;
    fputs(" GENF - generated numbers would cause overflow\n",stderr);
    fprintf(stderr," Numerator %16.6E Denominator %16.6E\n",xnum,xden);
/*
 * JJV changed next 2 lines to reflect constant change above in the
 * JJV truncated value returned.
 *   fputs(" GENF returning 1.0E38\n",stderr);
 *   genf = 1.0E38;
 */
    fputs(" GENF returning 1.0E37\n",stderr);
    genf = 1.0E37;
    goto S30;
S20:
    genf = xnum/xden;
S30:
    return genf;
}

double gengam(double a,double r)
/*
**********************************************************************
     double gengam(double a,double r)
           GENerates random deviates from GAMma distribution
                              Function
     Generates random deviates from the gamma distribution whose
     density is
          (A**R)/Gamma(R) * X**(R-1) * Exp(-A*X)
                              Arguments
     a --> Location parameter of Gamma distribution
     JJV   (a > 0)
     r --> Shape parameter of Gamma distribution
     JJV   (r > 0)
                              Method
     Renames SGAMMA from TOMS as slightly modified by BWB to use RANF
     instead of SUNIF.
     For details see:
               (Case R >= 1.0)
               Ahrens, J.H. and Dieter, U.
               Generating Gamma Variates by a
               Modified Rejection Technique.
               Comm. ACM, 25,1 (Jan. 1982), 47 - 54.
     Algorithm GD
     JJV altered following to reflect argument ranges
               (Case 0.0 < R < 1.0)
               Ahrens, J.H. and Dieter, U.
               Computer Methods for Sampling from Gamma,
               Beta, Poisson and Binomial Distributions.
               Computing, 12 (1974), 223-246/
     Adapted algorithm GS.
**********************************************************************
*/
{
static double gengam;
/* JJV added argument checker */
    if(a > 0.0 && r > 0.0) goto S10;
    fputs(" A or R nonpositive in GENGAM - abort!\n",stderr);
    fprintf(stderr," A value: %16.6E R value: %16.6E\n",a,r);
    exit(1);
S10:
    gengam = sgamma(r);
    gengam /= a;
    return gengam;
}

void genmn(double *parm,double *x,double *work)
/*
**********************************************************************
     void genmn(double *parm,double *x,double *work)
              GENerate Multivariate Normal random deviate
                              Arguments
     parm --> Parameters needed to generate multivariate normal
               deviates (MEANV and Cholesky decomposition of
               COVM). Set by a previous call to SETGMN.
               1 : 1                - size of deviate, P
               2 : P + 1            - mean vector
               P+2 : P*(P+3)/2 + 1  - upper half of cholesky
                                       decomposition of cov matrix
     x    <-- Vector deviate generated.
     work <--> Scratch array
                              Method
     1) Generate P independent standard normal deviates - Ei ~ N(0,1)
     2) Using Cholesky decomposition find A s.t. trans(A)*A = COVM
     3) trans(A)E + MEANV ~ N(MEANV,COVM)
**********************************************************************
*/
{
static long i,icount,j,p,D1,D2,D3,D4;
static double ae;

    p = (long) (*parm);
/*
     Generate P independent normal deviates - WORK ~ N(0,1)
*/
    for(i=1; i<=p; i++) *(work+i-1) = snorm();
    for(i=1,D3=1,D4=(p-i+D3)/D3; D4>0; D4--,i+=D3) {
/*
     PARM (P+2 : P*(P+3)/2 + 1) contains A, the Cholesky
      decomposition of the desired covariance matrix.
          trans(A)(1,1) = PARM(P+2)
          trans(A)(2,1) = PARM(P+3)
          trans(A)(2,2) = PARM(P+2+P)
          trans(A)(3,1) = PARM(P+4)
          trans(A)(3,2) = PARM(P+3+P)
          trans(A)(3,3) = PARM(P+2-1+2P)  ...
     trans(A)*WORK + MEANV ~ N(MEANV,COVM)
*/
        icount = 0;
        ae = 0.0;
        for(j=1,D1=1,D2=(i-j+D1)/D1; D2>0; D2--,j+=D1) {
            icount += (j-1);
            ae += (*(parm+i+(j-1)*p-icount+p)**(work+j-1));
        }
        *(x+i-1) = ae+*(parm+i);
    }
}

void genmul(long n,double *p,long ncat,long *ix)
/*
**********************************************************************
 
     void genmul(int n,double *p,int ncat,int *ix)
     GENerate an observation from the MULtinomial distribution
                              Arguments
     N --> Number of events that will be classified into one of
           the categories 1..NCAT
     P --> Vector of probabilities.  P(i) is the probability that
           an event will be classified into category i.  Thus, P(i)
           must be [0,1]. Only the first NCAT-1 P(i) must be defined
           since P(NCAT) is 1.0 minus the sum of the first
           NCAT-1 P(i).
     NCAT --> Number of categories.  Length of P and IX.
     IX <-- Observation from multinomial distribution.  All IX(i)
            will be nonnegative and their sum will be N.
                              Method
     Algorithm from page 559 of
 
     Devroye, Luc
 
     Non-Uniform Random Variate Generation.  Springer-Verlag,
     New York, 1986.
 
**********************************************************************
*/
{
static double prob,ptot,sum;
static long i,icat,ntot;
    if(n < 0) ftnstop("N < 0 in GENMUL");
    if(ncat <= 1) ftnstop("NCAT <= 1 in GENMUL");
    ptot = 0.0F;
    for(i=0; i<ncat-1; i++) {
        if(*(p+i) < 0.0F) ftnstop("Some P(i) < 0 in GENMUL");
        if(*(p+i) > 1.0F) ftnstop("Some P(i) > 1 in GENMUL");
        ptot += *(p+i);
    }
    if(ptot > 0.99999F) ftnstop("Sum of P(i) > 1 in GENMUL");
/*
     Initialize variables
*/
    ntot = n;
    sum = 1.0F;
    for(i=0; i<ncat; i++) ix[i] = 0;
/*
     Generate the observation
*/
    for(icat=0; icat<ncat-1; icat++) {
        prob = *(p+icat)/sum;
        *(ix+icat) = ignbin(ntot,prob);
        ntot -= *(ix+icat);
	if(ntot <= 0) return;
        sum -= *(p+icat);
    }
    *(ix+ncat-1) = ntot;
/*
     Finished
*/
    return;
}

double gennch(double df,double xnonc)
/*
**********************************************************************
     double gennch(double df,double xnonc)
           Generate random value of Noncentral CHIsquare variable
                              Function
     Generates random deviate  from the  distribution  of a  noncentral
     chisquare with DF degrees  of freedom and noncentrality  parameter
     xnonc.
                              Arguments
     df --> Degrees of freedom of the chisquare
            (Must be >= 1.0)
     xnonc --> Noncentrality parameter of the chisquare
               (Must be >= 0.0)
                              Method
     Uses fact that  noncentral chisquare  is  the  sum of a  chisquare
     deviate with DF-1  degrees of freedom plus the  square of a normal
     deviate with mean XNONC and standard deviation 1.
**********************************************************************
*/
{
static double gennch;

    if(!(df < 1.0 || xnonc < 0.0)) goto S10;
    fputs("DF < 1 or XNONC < 0 in GENNCH - ABORT\n",stderr);
    fprintf(stderr,"Value of DF: %16.6E Value of XNONC: %16.6E\n",df,xnonc);
    exit(1);
/* JJV changed code to call SGAMMA, SNORM directly */
S10:
    if(df >= 1.000000001) goto S20;
/*
 * JJV case df == 1.0
 * gennch = pow(gennor(sqrt(xnonc),1.0),2.0); <- OLD
 */
    gennch = pow(snorm()+sqrt(xnonc),2.0);
    goto S30;
S20:
/*
 * JJV case df > 1.0
 * gennch = genchi(df-1.0)+pow(gennor(sqrt(xnonc),1.0),2.0); <- OLD
 */
    gennch = 2.0*sgamma((df-1.0)/2.0)+pow(snorm()+sqrt(xnonc),2.0);
S30:
    return gennch;
}

double gennf(double dfn,double dfd,double xnonc)
/*
**********************************************************************
     double gennf(double dfn,double dfd,double xnonc)
           GENerate random deviate from the Noncentral F distribution
                              Function
     Generates a random deviate from the  noncentral F (variance ratio)
     distribution with DFN degrees of freedom in the numerator, and DFD
     degrees of freedom in the denominator, and noncentrality parameter
     XNONC.
                              Arguments
     dfn --> Numerator degrees of freedom
             (Must be >= 1.0)
     dfd --> Denominator degrees of freedom
             (Must be positive)
     xnonc --> Noncentrality parameter
               (Must be nonnegative)
                              Method
     Directly generates ratio of noncentral numerator chisquare variate
     to central denominator chisquare variate.
**********************************************************************
*/
{
static double gennf,xden,xnum;
static long qcond;

    /* JJV changed qcond, error message to allow dfn == 1.0 */
    qcond = dfn < 1.0 || dfd <= 0.0 || xnonc < 0.0;
    if(!qcond) goto S10;
    fputs("In GENNF - Either (1) Numerator DF < 1.0 or\n",stderr);
    fputs(" (2) Denominator DF <= 0.0 or\n",stderr);
    fputs(" (3) Noncentrality parameter < 0.0\n",stderr);
    fprintf(stderr,
      "DFN value: %16.6E DFD value: %16.6E XNONC value: \n%16.6E\n",dfn,dfd,
      xnonc);
    exit(1);
S10:
/*
 * JJV changed the code to call SGAMMA and SNORM directly
 * GENNF = ( GENNCH( DFN, XNONC ) / DFN ) / ( GENCHI( DFD ) / DFD )
 * xnum = gennch(dfn,xnonc)/dfn; <- OLD
 * xden = genchi(dfd)/dfd; <- OLD
 */
    if(dfn >= 1.000001) goto S20;
/* JJV case dfn == 1.0, dfn is counted as exactly 1.0 */
    xnum = pow(snorm()+sqrt(xnonc),2.0);
    goto S30;
S20:
/* JJV case df > 1.0 */
    xnum = (2.0*sgamma((dfn-1.0)/2.0)+pow(snorm()+sqrt(xnonc),2.0))/dfn;
S30:
    xden = 2.0*sgamma(dfd/2.0)/dfd;
/*
 * JJV changed constant to prevent underflow at compile time.
 *   if(!(xden <= 9.999999999998E-39*xnum)) goto S40;
 */
    if(!(xden <= 1.0E-37*xnum)) goto S40;
    fputs(" GENNF - generated numbers would cause overflow\n",stderr);
    fprintf(stderr," Numerator %16.6E Denominator %16.6E\n",xnum,xden);
/*
 * JJV changed next 2 lines to reflect constant change above in the
 * JJV truncated value returned.
 *   fputs(" GENNF returning 1.0E38\n",stderr);
 *   gennf = 1.0E38;
 */
    fputs(" GENNF returning 1.0E37\n",stderr);
    gennf = 1.0E37;
    goto S50;
S40:
    gennf = xnum/xden;
S50:
    return gennf;
}

double gennor(double av,double sd)
/*
**********************************************************************
     double gennor(double av,double sd)
         GENerate random deviate from a NORmal distribution
                              Function
     Generates a single random deviate from a normal distribution
     with mean, AV, and standard deviation, SD.
                              Arguments
     av --> Mean of the normal distribution.
     sd --> Standard deviation of the normal distribution.
     JJV    (sd >= 0)
                              Method
     Renames SNORM from TOMS as slightly modified by BWB to use RANF
     instead of SUNIF.
     For details see:
               Ahrens, J.H. and Dieter, U.
               Extensions of Forsythe's Method for Random
               Sampling from the Normal Distribution.
               Math. Comput., 27,124 (Oct. 1973), 927 - 937.
**********************************************************************
*/
{
static double gennor;

/* JJV added argument checker */
    if(sd >= 0.0) goto S10;
    fputs(" SD < 0 in GENNOR - ABORT\n",stderr);
    fprintf(stderr," Value of SD: %16.6E\n",sd);
    exit(1);
S10:
    gennor = sd*snorm()+av;
    return gennor;
}

void genprm(long *iarray,int larray)
/*
**********************************************************************
    void genprm(long *iarray,int larray)
               GENerate random PeRMutation of iarray
                              Arguments
     iarray <--> On output IARRAY is a random permutation of its
                 value on input
     larray <--> Length of IARRAY
**********************************************************************
*/
{
static long i,itmp,iwhich,D1,D2;

    for(i=1,D1=1,D2=(larray-i+D1)/D1; D2>0; D2--,i+=D1) {
        iwhich = ignuin(i,larray);
        itmp = *(iarray+iwhich-1);
        *(iarray+iwhich-1) = *(iarray+i-1);
        *(iarray+i-1) = itmp;
    }
}

double genunf(double low,double high)
/*
**********************************************************************
     double genunf(double low,double high)
               GeNerate Uniform Real between LOW and HIGH
                              Function
     Generates a real uniformly distributed between LOW and HIGH.
                              Arguments
     low --> Low bound (exclusive) on real value to be generated
     high --> High bound (exclusive) on real value to be generated
**********************************************************************
*/
{
static double genunf;

    if(!(low > high)) goto S10;
    fprintf(stderr,"LOW > HIGH in GENUNF: LOW %16.6E HIGH: %16.6E\n",low,high);
    fputs("Abort\n",stderr);
    exit(1);
S10:
    genunf = low+(high-low)*ranf();
    return genunf;
}

void gscgn(long getset,long *g)
/*
**********************************************************************
     void gscgn(long getset,long *g)
                         Get/Set GeNerator
     Gets or returns in G the number of the current generator
                              Arguments
     getset --> 0 Get
                1 Set
     g <-- Number of the current random number generator (1..32)
**********************************************************************
*/
{
#define numg 32L
static long curntg = 1;
    if(getset == 0) *g = curntg;
    else  {
        if(*g < 0 || *g > numg) {
            fputs(" Generator number out of range in GSCGN\n",stderr);
            exit(0);
        }
        curntg = *g;
    }
#undef numg
}

void gsrgs(long getset,long *qvalue)
/*
**********************************************************************
     void gsrgs(long getset,long *qvalue)
               Get/Set Random Generators Set
     Gets or sets whether random generators set (initialized).
     Initially (data statement) state is not set
     If getset is 1 state is set to qvalue
     If getset is 0 state returned in qvalue
**********************************************************************
*/
{
static long qinit = 0;

    if(getset == 0) *qvalue = qinit;
    else qinit = *qvalue;
}

void gssst(long getset,long *qset)
/*
**********************************************************************
     void gssst(long getset,long *qset)
          Get or Set whether Seed is Set
     Initialize to Seed not Set
     If getset is 1 sets state to Seed Set
     If getset is 0 returns T in qset if Seed Set
     Else returns F in qset
**********************************************************************
*/
{
static long qstate = 0;
    if(getset != 0) qstate = 1;
    else  *qset = qstate;
}

long ignbin(long n,double pp)
/*
**********************************************************************
     long ignbin(long n,double pp)
                    GENerate BINomial random deviate
                              Function
     Generates a single random deviate from a binomial
     distribution whose number of trials is N and whose
     probability of an event in each trial is P.
                              Arguments
     n  --> The number of trials in the binomial distribution
            from which a random deviate is to be generated.
	    JJV (N >= 0)
     pp --> The probability of an event in each trial of the
            binomial distribution from which a random deviate
            is to be generated.
	    JJV (0.0 <= PP <= 1.0)
     ignbin <-- A random deviate yielding the number of events
                from N independent trials, each of which has
                a probability of event P.
                              Method
     This is algorithm BTPE from:
         Kachitvichyanukul, V. and Schmeiser, B. W.
         Binomial Random Variate Generation.
         Communications of the ACM, 31, 2
         (February, 1988) 216.
**********************************************************************
     SUBROUTINE BTPEC(N,PP,ISEED,JX)
     BINOMIAL RANDOM VARIATE GENERATOR
     MEAN .LT. 30 -- INVERSE CDF
       MEAN .GE. 30 -- ALGORITHM BTPE:  ACCEPTANCE-REJECTION VIA
       FOUR REGION COMPOSITION.  THE FOUR REGIONS ARE A TRIANGLE
       (SYMMETRIC IN THE CENTER), A PAIR OF PARALLELOGRAMS (ABOVE
       THE TRIANGLE), AND EXPONENTIAL LEFT AND RIGHT TAILS.
     BTPE REFERS TO BINOMIAL-TRIANGLE-PARALLELOGRAM-EXPONENTIAL.
     BTPEC REFERS TO BTPE AND "COMBINED."  THUS BTPE IS THE
       RESEARCH AND BTPEC IS THE IMPLEMENTATION OF A COMPLETE
       USABLE ALGORITHM.
     REFERENCE:  VORATAS KACHITVICHYANUKUL AND BRUCE SCHMEISER,
       "BINOMIAL RANDOM VARIATE GENERATION,"
       COMMUNICATIONS OF THE ACM, FORTHCOMING
     WRITTEN:  SEPTEMBER 1980.
       LAST REVISED:  MAY 1985, JULY 1987
     REQUIRED SUBPROGRAM:  RAND() -- A UNIFORM (0,1) RANDOM NUMBER
                           GENERATOR
     ARGUMENTS
       N : NUMBER OF BERNOULLI TRIALS            (INPUT)
       PP : PROBABILITY OF SUCCESS IN EACH TRIAL (INPUT)
       ISEED:  RANDOM NUMBER SEED                (INPUT AND OUTPUT)
       JX:  RANDOMLY GENERATED OBSERVATION       (OUTPUT)
     VARIABLES
       PSAVE: VALUE OF PP FROM THE LAST CALL TO BTPEC
       NSAVE: VALUE OF N FROM THE LAST CALL TO BTPEC
       XNP:  VALUE OF THE MEAN FROM THE LAST CALL TO BTPEC
       P: PROBABILITY USED IN THE GENERATION PHASE OF BTPEC
       FFM: TEMPORARY VARIABLE EQUAL TO XNP + P
       M:  INTEGER VALUE OF THE CURRENT MODE
       FM:  FLOATING POINT VALUE OF THE CURRENT MODE
       XNPQ: TEMPORARY VARIABLE USED IN SETUP AND SQUEEZING STEPS
       P1:  AREA OF THE TRIANGLE
       C:  HEIGHT OF THE PARALLELOGRAMS
       XM:  CENTER OF THE TRIANGLE
       XL:  LEFT END OF THE TRIANGLE
       XR:  RIGHT END OF THE TRIANGLE
       AL:  TEMPORARY VARIABLE
       XLL:  RATE FOR THE LEFT EXPONENTIAL TAIL
       XLR:  RATE FOR THE RIGHT EXPONENTIAL TAIL
       P2:  AREA OF THE PARALLELOGRAMS
       P3:  AREA OF THE LEFT EXPONENTIAL TAIL
       P4:  AREA OF THE RIGHT EXPONENTIAL TAIL
       U:  A U(0,P4) RANDOM VARIATE USED FIRST TO SELECT ONE OF THE
           FOUR REGIONS AND THEN CONDITIONALLY TO GENERATE A VALUE
           FROM THE REGION
       V:  A U(0,1) RANDOM NUMBER USED TO GENERATE THE RANDOM VALUE
           (REGION 1) OR TRANSFORMED INTO THE VARIATE TO ACCEPT OR
           REJECT THE CANDIDATE VALUE
       IX:  INTEGER CANDIDATE VALUE
       X:  PRELIMINARY CONTINUOUS CANDIDATE VALUE IN REGION 2 LOGIC
           AND A FLOATING POINT IX IN THE ACCEPT/REJECT LOGIC
       K:  ABSOLUTE VALUE OF (IX-M)
       F:  THE HEIGHT OF THE SCALED DENSITY FUNCTION USED IN THE
           ACCEPT/REJECT DECISION WHEN BOTH M AND IX ARE SMALL
           ALSO USED IN THE INVERSE TRANSFORMATION
       R: THE RATIO P/Q
       G: CONSTANT USED IN CALCULATION OF PROBABILITY
       MP:  MODE PLUS ONE, THE LOWER INDEX FOR EXPLICIT CALCULATION
            OF F WHEN IX IS GREATER THAN M
       IX1:  CANDIDATE VALUE PLUS ONE, THE LOWER INDEX FOR EXPLICIT
             CALCULATION OF F WHEN IX IS LESS THAN M
       I:  INDEX FOR EXPLICIT CALCULATION OF F FOR BTPE
       AMAXP: MAXIMUM ERROR OF THE LOGARITHM OF NORMAL BOUND
       YNORM: LOGARITHM OF NORMAL BOUND
       ALV:  NATURAL LOGARITHM OF THE ACCEPT/REJECT VARIATE V
       X1,F1,Z,W,Z2,X2,F2, AND W2 ARE TEMPORARY VARIABLES TO BE
       USED IN THE FINAL ACCEPT/REJECT TEST
       QN: PROBABILITY OF NO SUCCESS IN N TRIALS
     REMARK
       IX AND JX COULD LOGICALLY BE THE SAME VARIABLE, WHICH WOULD
       SAVE A MEMORY POSITION AND A LINE OF CODE.  HOWEVER, SOME
       COMPILERS (E.G.,CDC MNF) OPTIMIZE BETTER WHEN THE ARGUMENTS
       ARE NOT INVOLVED.
     ISEED NEEDS TO BE DOUBLE PRECISION IF THE IMSL ROUTINE
     GGUBFS IS USED TO GENERATE UNIFORM RANDOM NUMBER, OTHERWISE
     TYPE OF ISEED SHOULD BE DICTATED BY THE UNIFORM GENERATOR
**********************************************************************
*****DETERMINE APPROPRIATE ALGORITHM AND WHETHER SETUP IS NECESSARY
*/
{
/* JJV changed initial values to ridiculous values */
static double psave = -1.0E37;
static long nsave = -214748365;
static long ignbin,i,ix,ix1,k,m,mp,T1;
static double al,alv,amaxp,c,f,f1,f2,ffm,fm,g,p,p1,p2,p3,p4,q,qn,r,u,v,w,w2,x,x1,
    x2,xl,xll,xlr,xm,xnp,xnpq,xr,ynorm,z,z2;

    if(pp != psave) goto S10;
    if(n != nsave) goto S20;
    if(xnp < 30.0) goto S150;
    goto S30;
S10:
/*
*****SETUP, PERFORM ONLY WHEN PARAMETERS CHANGE
JJV added checks to ensure 0.0 <= PP <= 1.0
*/
    if(pp < 0.0F) ftnstop("PP < 0.0 in IGNBIN");
    if(pp > 1.0F) ftnstop("PP > 1.0 in IGNBIN");
    psave = pp;
    p = min(psave,1.0-psave);
    q = 1.0-p;
S20:
/*
JJV added check to ensure N >= 0
*/
    if(n < 0L) ftnstop("N < 0 in IGNBIN");
    xnp = n*p;
    nsave = n;
    if(xnp < 30.0) goto S140;
    ffm = xnp+p;
    m = ffm;
    fm = m;
    xnpq = xnp*q;
    p1 = (long) (2.195*sqrt(xnpq)-4.6*q)+0.5;
    xm = fm+0.5;
    xl = xm-p1;
    xr = xm+p1;
    c = 0.134+20.5/(15.3+fm);
    al = (ffm-xl)/(ffm-xl*p);
    xll = al*(1.0+0.5*al);
    al = (xr-ffm)/(xr*q);
    xlr = al*(1.0+0.5*al);
    p2 = p1*(1.0+c+c);
    p3 = p2+c/xll;
    p4 = p3+c/xlr;
S30:
/*
*****GENERATE VARIATE
*/
    u = ranf()*p4;
    v = ranf();
/*
     TRIANGULAR REGION
*/
    if(u > p1) goto S40;
    ix = xm-p1*v+u;
    goto S170;
S40:
/*
     PARALLELOGRAM REGION
*/
    if(u > p2) goto S50;
    x = xl+(u-p1)/c;
    v = v*c+1.0-ABS(xm-x)/p1;
    if(v > 1.0 || v <= 0.0) goto S30;
    ix = x;
    goto S70;
S50:
/*
     LEFT TAIL
*/
    if(u > p3) goto S60;
    ix = xl+log(v)/xll;
    if(ix < 0) goto S30;
    v *= ((u-p2)*xll);
    goto S70;
S60:
/*
     RIGHT TAIL
*/
    ix = xr-log(v)/xlr;
    if(ix > n) goto S30;
    v *= ((u-p3)*xlr);
S70:
/*
*****DETERMINE APPROPRIATE WAY TO PERFORM ACCEPT/REJECT TEST
*/
    k = ABS(ix-m);
    if(k > 20 && k < xnpq/2-1) goto S130;
/*
     EXPLICIT EVALUATION
*/
    f = 1.0;
    r = p/q;
    g = (n+1)*r;
    T1 = m-ix;
    if(T1 < 0) goto S80;
    else if(T1 == 0) goto S120;
    else  goto S100;
S80:
    mp = m+1;
    for(i=mp; i<=ix; i++) f *= (g/i-r);
    goto S120;
S100:
    ix1 = ix+1;
    for(i=ix1; i<=m; i++) f /= (g/i-r);
S120:
    if(v <= f) goto S170;
    goto S30;
S130:
/*
     SQUEEZING USING UPPER AND LOWER BOUNDS ON ALOG(F(X))
*/
    amaxp = k/xnpq*((k*(k/3.0+0.625)+0.1666666666666)/xnpq+0.5);
    ynorm = -(k*k/(2.0*xnpq));
    alv = log(v);
    if(alv < ynorm-amaxp) goto S170;
    if(alv > ynorm+amaxp) goto S30;
/*
     STIRLING'S FORMULA TO MACHINE ACCURACY FOR
     THE FINAL ACCEPTANCE/REJECTION TEST
*/
    x1 = ix+1.0;
    f1 = fm+1.0;
    z = n+1.0-fm;
    w = n-ix+1.0;
    z2 = z*z;
    x2 = x1*x1;
    f2 = f1*f1;
    w2 = w*w;
    if(alv <= xm*log(f1/x1)+(n-m+0.5)*log(z/w)+(ix-m)*log(w*p/(x1*q))+(13860.0-
      (462.0-(132.0-(99.0-140.0/f2)/f2)/f2)/f2)/f1/166320.0+(13860.0-(462.0-
      (132.0-(99.0-140.0/z2)/z2)/z2)/z2)/z/166320.0+(13860.0-(462.0-(132.0-
      (99.0-140.0/x2)/x2)/x2)/x2)/x1/166320.0+(13860.0-(462.0-(132.0-(99.0
      -140.0/w2)/w2)/w2)/w2)/w/166320.0) goto S170;
    goto S30;
S140:
/*
     INVERSE CDF LOGIC FOR MEAN LESS THAN 30
*/
/* The following change was recommended by Paul B. to get around an
   error when using gcc under AIX. 2006-09-12. */
/**    qn = pow(q,(double)n); <- OLD **/
    qn = exp( (double)n * log(q) );
    r = p/q;
    g = r*(n+1);
S150:
    ix = 0;
    f = qn;
    u = ranf();
S160:
    if(u < f) goto S170;
    if(ix > 110) goto S150;
    u -= f;
    ix += 1;
    f *= (g/ix-r);
    goto S160;
S170:
    if(psave > 0.5) ix = n-ix;
    ignbin = ix;
    return ignbin;
}

long ignnbn(long n,double p)
/*
**********************************************************************
 
     long ignnbn(long n,double p)
                GENerate Negative BiNomial random deviate
                              Function
     Generates a single random deviate from a negative binomial
     distribution.
                              Arguments
     N  --> The number of trials in the negative binomial distribution
            from which a random deviate is to be generated.
	    JJV (N > 0)
     P  --> The probability of an event.
     JJV    (0.0 < P < 1.0)
                              Method
     Algorithm from page 480 of
 
     Devroye, Luc
 
     Non-Uniform Random Variate Generation.  Springer-Verlag,
     New York, 1986.
**********************************************************************
*/
{
static long ignnbn;
static double y,a,r;
/*
     ..
     .. Executable Statements ..
*/
/*
     Check Arguments
*/
    if(n <= 0L) ftnstop("N <= 0 in IGNNBN");
    if(p <= 0.0F) ftnstop("P <= 0.0 in IGNNBN");
    if(p >= 1.0F) ftnstop("P >= 1.0 in IGNNBN");
/*
     Generate Y, a random gamma (n,(1-p)/p) variable
     JJV Note: the above parametrization is consistent with Devroye,
     JJV       but gamma (p/(1-p),n) is the equivalent in our code
*/
    r = (double)n;
    a = p/(1.0F-p);
/*
 * JJV changed this to call SGAMMA directly
 *  y = gengam(a,r); <- OLD
 */
    y = sgamma(r)/a;
/*
     Generate a random Poisson(y) variable
*/
    ignnbn = ignpoi(y);
    return ignnbn;
}

long ignpoi(double mu)
/*
**********************************************************************
     long ignpoi(double mu)
                    GENerate POIsson random deviate
                              Function
     Generates a single random deviate from a Poisson
     distribution with mean MU.
                              Arguments
     mu --> The mean of the Poisson distribution from which
            a random deviate is to be generated.
	    (mu >= 0.0)
     ignpoi <-- The random deviate.
                              Method
     Renames KPOIS from TOMS as slightly modified by BWB to use RANF
     instead of SUNIF.
     For details see:
               Ahrens, J.H. and Dieter, U.
               Computer Generation of Poisson Deviates
               From Modified Normal Distributions.
               ACM Trans. Math. Software, 8, 2
               (June 1982),163-179
**********************************************************************
**********************************************************************
                                                                      
                                                                      
     P O I S S O N  DISTRIBUTION                                      
                                                                      
                                                                      
**********************************************************************
**********************************************************************
                                                                      
     FOR DETAILS SEE:                                                 
                                                                      
               AHRENS, J.H. AND DIETER, U.                            
               COMPUTER GENERATION OF POISSON DEVIATES                
               FROM MODIFIED NORMAL DISTRIBUTIONS.                    
               ACM TRANS. MATH. SOFTWARE, 8,2 (JUNE 1982), 163 - 179. 
                                                                      
     (SLIGHTLY MODIFIED VERSION OF THE PROGRAM IN THE ABOVE ARTICLE)  
                                                                      
**********************************************************************
      INTEGER FUNCTION IGNPOI(IR,MU)
     INPUT:  IR=CURRENT STATE OF BASIC RANDOM NUMBER GENERATOR
             MU=MEAN MU OF THE POISSON DISTRIBUTION
     OUTPUT: IGNPOI=SAMPLE FROM THE POISSON-(MU)-DISTRIBUTION
     MUPREV=PREVIOUS MU, MUOLD=MU AT LAST EXECUTION OF STEP P OR B.
     TABLES: COEFFICIENTS A0-A7 FOR STEP F. FACTORIALS FACT
     COEFFICIENTS A(K) - FOR PX = FK*V*V*SUM(A(K)*V**K)-DEL
     SEPARATION OF CASES A AND B
*/
{
extern double fsign( double num, double sign );
static double a0 = -0.5;
static double a1 =  0.3333333343;
static double a2 = -0.2499998565;
static double a3 =  0.1999997049;
static double a4 = -0.1666848753;
static double a5 =  0.1428833286;
static double a6 = -0.1241963125;
static double a7 =  0.1101687109;
static double a8 = -0.1142650302;
static double a9 =  0.1055093006;
/* JJV changed the initial values of MUPREV and MUOLD */
static double muold = -1.0E37;
static double muprev = -1.0E37;
static double fact[10] = {
    1.0,1.0,2.0,6.0,24.0,120.0,720.0,5040.0,40320.0,362880.0
};
/* JJV added ll to the list, for Case A */
static long ignpoi,j,k,kflag,l,ll,m;
static double b1,b2,c,c0,c1,c2,c3,d,del,difmuk,e,fk,fx,fy,g,omega,p,p0,px,py,q,s,
    t,u,v,x,xx,pp[35];

    if(mu == muprev) goto S10;
    if(mu < 10.0) goto S120;
/*
     C A S E  A. (RECALCULATION OF S,D,LL IF MU HAS CHANGED)
     JJV changed l in Case A to ll
*/
    muprev = mu;
    s = sqrt(mu);
    d = 6.0*mu*mu;
/*
             THE POISSON PROBABILITIES PK EXCEED THE DISCRETE NORMAL
             PROBABILITIES FK WHENEVER K >= M(MU). LL=IFIX(MU-1.1484)
             IS AN UPPER BOUND TO M(MU) FOR ALL MU >= 10 .
*/
    ll = (long) (mu-1.1484);
S10:
/*
     STEP N. NORMAL SAMPLE - SNORM(IR) FOR STANDARD NORMAL DEVIATE
*/
    g = mu+s*snorm();
    if(g < 0.0) goto S20;
    ignpoi = (long) (g);
/*
     STEP I. IMMEDIATE ACCEPTANCE IF IGNPOI IS LARGE ENOUGH
*/
    if(ignpoi >= ll) return ignpoi;
/*
     STEP S. SQUEEZE ACCEPTANCE - SUNIF(IR) FOR (0,1)-SAMPLE U
*/
    fk = (double)ignpoi;
    difmuk = mu-fk;
    u = ranf();
    if(d*u >= difmuk*difmuk*difmuk) return ignpoi;
S20:
/*
     STEP P. PREPARATIONS FOR STEPS Q AND H.
             (RECALCULATIONS OF PARAMETERS IF NECESSARY)
             .3989423=(2*PI)**(-.5)  .416667E-1=1./24.  .1428571=1./7.
             THE QUANTITIES B1, B2, C3, C2, C1, C0 ARE FOR THE HERMITE
             APPROXIMATIONS TO THE DISCRETE NORMAL PROBABILITIES FK.
             C=.1069/MU GUARANTEES MAJORIZATION BY THE 'HAT'-FUNCTION.
*/
    if(mu == muold) goto S30;
    muold = mu;
    omega = 0.398942280401433/s;
    b1 = 4.16666666666667E-2/mu;
    b2 = 0.3*b1*b1;
    c3 = 0.142857142857143*b1*b2;
    c2 = b2-15.0*c3;
    c1 = b1-6.0*b2+45.0*c3;
    c0 = 1.0-b1+3.0*b2-15.0*c3;
    c = 0.1069/mu;
S30:
    if(g < 0.0) goto S50;
/*
             'SUBROUTINE' F IS CALLED (KFLAG=0 FOR CORRECT RETURN)
*/
    kflag = 0;
    goto S70;
S40:
/*
     STEP Q. QUOTIENT ACCEPTANCE (RARE CASE)
*/
    if(fy-u*fy <= py*exp(px-fx)) return ignpoi;
S50:
/*
     STEP E. EXPONENTIAL SAMPLE - SEXPO(IR) FOR STANDARD EXPONENTIAL
             DEVIATE E AND SAMPLE T FROM THE LAPLACE 'HAT'
             (IF T <= -.6744 THEN PK < FK FOR ALL MU >= 10.)
*/
    e = sexpo();
    u = ranf();
    u += (u-1.0);
    t = 1.8+fsign(e,u);
    if(t <= -0.6744) goto S50;
    ignpoi = (long) (mu+s*t);
    fk = (double)ignpoi;
    difmuk = mu-fk;
/*
             'SUBROUTINE' F IS CALLED (KFLAG=1 FOR CORRECT RETURN)
*/
    kflag = 1;
    goto S70;
S60:
/*
     STEP H. HAT ACCEPTANCE (E IS REPEATED ON REJECTION)
*/
    if(c*fabs(u) > py*exp(px+e)-fy*exp(fx+e)) goto S50;
    return ignpoi;
S70:
/*
     STEP F. 'SUBROUTINE' F. CALCULATION OF PX,PY,FX,FY.
             CASE IGNPOI .LT. 10 USES FACTORIALS FROM TABLE FACT
*/
    if(ignpoi >= 10) goto S80;
    px = -mu;
    py = pow(mu,(double)ignpoi)/ *(fact+ignpoi);
    goto S110;
S80:
/*
             CASE IGNPOI .GE. 10 USES POLYNOMIAL APPROXIMATION
             A0-A7 FOR ACCURACY WHEN ADVISABLE
             .8333333E-1=1./12.  .3989423=(2*PI)**(-.5)
*/
    del = 8.33333333E-2/fk;
    del -= (4.8*del*del*del);
    v = difmuk/fk;
    if(fabs(v) <= 0.25) goto S90;
    px = fk*log(1.0+v)-difmuk-del;
    goto S100;
S90:
    px = fk*v*v*((((((((a8*v+a7)*v+a6)*v+a5)*v+a4)*v+a3)*v+a2)*v+a1)*v+a0)-del;
S100:
    py = 0.398942280401433/sqrt(fk);
S110:
    x = (0.5-difmuk)/s;
    xx = x*x;
    fx = -0.5*xx;
    fy = omega*(((c3*xx+c2)*xx+c1)*xx+c0);
    if(kflag <= 0) goto S40;
    goto S60;
S120:
/*
     C A S E  B. (START NEW TABLE AND CALCULATE P0 IF NECESSARY)
     JJV changed MUPREV assignment to initial value
*/
    muprev = -1.0E37;
    if(mu == muold) goto S130;
/* JJV added argument checker here */
    if(mu >= 0.0) goto S125;
    fprintf(stderr,"MU < 0 in IGNPOI: MU %16.6E\n",mu);
    fputs("Abort\n",stderr);
    exit(1);
S125:
    muold = mu;
    m = max(1L,(long) (mu));
    l = 0;
    p = exp(-mu);
    q = p0 = p;
S130:
/*
     STEP U. UNIFORM SAMPLE FOR INVERSION METHOD
*/
    u = ranf();
    ignpoi = 0;
    if(u <= p0) return ignpoi;
/*
     STEP T. TABLE COMPARISON UNTIL THE END PP(L) OF THE
             PP-TABLE OF CUMULATIVE POISSON PROBABILITIES
             (0.458=PP(9) FOR MU=10)
*/
    if(l == 0) goto S150;
    j = 1;
    if(u > 0.458) j = min(l,m);
    for(k=j; k<=l; k++) {
        if(u <= *(pp+k-1)) goto S180;
    }
    if(l == 35) goto S130;
S150:
/*
     STEP C. CREATION OF NEW POISSON PROBABILITIES P
             AND THEIR CUMULATIVES Q=PP(K)
*/
    l += 1;
    for(k=l; k<=35; k++) {
        p = p*mu/(double)k;
        q += p;
        *(pp+k-1) = q;
        if(u <= q) goto S170;
    }
    l = 35;
    goto S130;
S170:
    l = k;
S180:
    ignpoi = k;
    return ignpoi;
}

long ignuin(long low,long high)
/*
**********************************************************************
     long ignuin(long low,long high)
               GeNerate Uniform INteger
                              Function
     Generates an integer uniformly distributed between LOW and HIGH.
                              Arguments
     low --> Low bound (inclusive) on integer value to be generated
     high --> High bound (inclusive) on integer value to be generated
                              Note
     If (HIGH-LOW) > 2,147,483,561 prints error message on * unit and
     stops the program.
**********************************************************************
     IGNLGI generates integers between 1 and 2147483562
     MAXNUM is 1 less than maximum generable value
*/
{
#define maxnum 2147483561L
static long ignuin,ign,maxnow,range,ranp1;

    if(!(low > high)) goto S10;
    fputs(" low > high in ignuin - ABORT\n",stderr);
    exit(1);

S10:
    range = high-low;
    if(!(range > maxnum)) goto S20;
    fputs(" high - low too large in ignuin - ABORT\n",stderr);
    exit(1);

S20:
    if(!(low == high)) goto S30;
    ignuin = low;
    return ignuin;

S30:
/*
     Number to be generated should be in range 0..RANGE
     Set MAXNOW so that the number of integers in 0..MAXNOW is an
     integral multiple of the number in 0..RANGE
*/
    ranp1 = range+1;
    maxnow = maxnum/ranp1*ranp1;
S40:
    ign = ignlgi()-1;
    if(!(ign <= maxnow)) goto S40;
    ignuin = low+ign%ranp1;
    return ignuin;
#undef maxnum
#undef err1
#undef err2
}

long lennob( char *str )
/* 
Returns the length of str ignoring trailing blanks but not 
other white space.
*/
{
long i, i_nb;

for (i=0, i_nb= -1L; *(str+i); i++)
    if ( *(str+i) != ' ' ) i_nb = i;
return (i_nb+1);
}

long mltmod(long a,long s,long m)
/*
**********************************************************************
     long mltmod(long a,long s,long m)
                    Returns (A*S) MOD M
     This is a transcription from Pascal to C of routine
     MultMod_Decompos from the paper
     L'Ecuyer, P. and Cote, S. "Implementing a Random Number Package
     with Splitting Facilities." ACM Transactions on Mathematical
     Software, 17:98-111 (1991)
                              Arguments
     a, s, m  -->
     WGR, 12/19/00: replaced S10, S20, etc. with C blocks {} per
     original paper.
**********************************************************************
*/
{
#define h 32768L
static long a0,a1,k,p,q,qh,rh;
/*
     H = 2**((b-2)/2) where b = 32 because we are using a 32 bit
     machine.  On a different machine recompute H.
*/
 if (a <= 0 || a >= m || s <= 0 || s >= m) {
    fputs(" a, m, s out of order in mltmod - ABORT!\n",stderr);
    fprintf(stderr," a = %12ld s = %12ld m = %12ld\n",a,s,m);
    fputs(" mltmod requires: 0 < a < m; 0 < s < m\n",stderr);
    exit(1);
 }

 if (a < h) {
    a0 = a;
    p = 0;
 } else {
    a1 = a/h;
    a0 = a - h*a1;
    qh = m/h;
    rh = m - h*qh;
    if (a1 >= h) {  /* A2=1 */
       a1 -= h;
       k = s/qh;
       p = h*(s-k*qh) - k*rh;
       while (p < 0) { p += m; }
    } else {
       p = 0;
    }
/*
     P = (A2*S*H)MOD M
*/
    if (a1 != 0) {
       q = m/a1;
       k = s/q;
       p -= k*(m - a1*q);
       if (p > 0) { p -= m; }
       p += a1*(s - k*q);
       while (p < 0) { p += m; }
    }
/*
     P = ((A2*H + A1)*S)MOD M
*/
    k = p/qh;
    p = h*(p-k*qh) - k*rh;
    while (p < 0) { p += m; }
 }
/*
     P = ((A2*H + A1)*H*S)MOD M
*/
 if (a0 != 0) {
    q = m/a0;
    k = s/q;
    p -= k*(m-a0*q);
    if (p > 0) { p -= m; }
    p += a0*(s-k*q);
    while (p < 0) { p += m; }
 }
 return p;
#undef h
}

void phrtsd(char* phrase,long *seed1,long *seed2)
/*
**********************************************************************
     void phrtsd(char* phrase,long *seed1,long *seed2)
               PHRase To SeeDs

                              Function

     Uses a phrase (character string) to generate two seeds for the RGN
     random number generator.
                              Arguments
     phrase --> Phrase to be used for random number generation
      
     seed1 <-- First seed for generator
                        
     seed2 <-- Second seed for generator
                        
                              Note

     Trailing blanks are eliminated before the seeds are generated.
     Generated seed values will fall in the range 1..2^30
     (1..1,073,741,824)
**********************************************************************
*/
{

static char table[] =
"abcdefghijklmnopqrstuvwxyz\
ABCDEFGHIJKLMNOPQRSTUVWXYZ\
0123456789\
!@#$%^&*()_+[];:'\\\"<>?,./ ";  /* WGR added space, 5/19/1999 */

long ix;

static long twop30 = 1073741824L;
static long shift[5] = {
    1L,64L,4096L,262144L,16777216L
};

#ifdef PHRTSD_ORIG
 /*----------------------------- Original phrtsd */
static long i,ichr,j,lphr,values[5];
extern long lennob(char *str);

    *seed1 = 1234567890L;
    *seed2 = 123456789L;
    lphr = lennob(phrase); 
    if(lphr < 1) return;
    for(i=0; i<=(lphr-1); i++) {
	for (ix=0; table[ix]; ix++) if (*(phrase+i) == table[ix]) break;
	/* JJV added ix++; to bring index in line with fortran's index*/
	ix++;
        if (!table[ix]) ix = 0;
        ichr = ix % 64;
        if(ichr == 0) ichr = 63;
        for(j=1; j<=5; j++) {
            *(values+j-1) = ichr-j;
            if(*(values+j-1) < 1) *(values+j-1) += 63;
        }
        for(j=1; j<=5; j++) {
            *seed1 = ( *seed1+*(shift+j-1)**(values+j-1) ) % twop30;
            *seed2 = ( *seed2+*(shift+j-1)**(values+6-j-1) )  % twop30;
        }
    }
#else
 /*----------------------------- New phrtsd */
static long i,j, ichr,lphr;
static long values[8] = { 8521739, 5266711, 3254959, 2011673,
  1243273, 768389, 474899, 293507 };
extern long lennob(char *str);

    *seed1 = 1234567890L;
    *seed2 = 123456789L;
    lphr = lennob(phrase); 
    if(lphr < 1) return;
    for(i=0; i<(lphr-1); i++) {
        ichr = phrase[i];
        j = i % 8;
        *seed1 = ( *seed1 + (values[j] * ichr) ) % twop30;
        *seed2 = ( *seed2 + (values[7-j] * ichr) ) % twop30;
    }
#endif
}

double ranf(void)
/*
**********************************************************************
     double ranf(void)
                RANDom number generator as a Function
     Returns a random floating point number from a uniform distribution
     over 0 - 1 (endpoints of this interval are not returned) using the
     current generator.
     This is a transcription from Pascal to C of routine
     Uniform_01 from the paper
     L'Ecuyer, P. and Cote, S. "Implementing a Random Number Package
     with Splitting Facilities." ACM Transactions on Mathematical
     Software, 17:98-111 (1991)
     WGR, 2/12/01: increased precision.
**********************************************************************
*/
{
static double ranf;
/*
     4.656613057E-10 is 1/M1  M1 is set in a data statement in IGNLGI
      and is currently 2147483563. If M1 changes, change this also.
*/
    ranf = ignlgi()*4.65661305739177E-10;
    return ranf;
}

void setgmn(double *meanv,double *covm,long p,double *parm)
/*
**********************************************************************
     void setgmn(double *meanv,double *covm,long p,double *parm)
            SET Generate Multivariate Normal random deviate
                              Function
      Places P, MEANV, and the Cholesky factorization of COVM
      in GENMN.
                              Arguments
     meanv --> Mean vector of multivariate normal distribution.
     covm   <--> (Input) Covariance   matrix    of  the  multivariate
                 normal distribution
                 (Output) Destroyed on output
     p     --> Dimension of the normal, or length of MEANV.
     parm <-- Array of parameters needed to generate multivariate norma
                deviates (P, MEANV and Cholesky decomposition of
                COVM).
                1 : 1                - P
                2 : P + 1            - MEANV
                P+2 : P*(P+3)/2 + 1  - Cholesky decomposition of COVM
               Needed dimension is (p*(p+3)/2 + 1)
**********************************************************************
*/
{
extern void spofa(double *a,long lda,long n,long *info);
static long T1;
static long i,icount,info,j,D2,D3,D4,D5;
    T1 = p*(p+3)/2+1;
/*
     TEST THE INPUT
*/
    if(!(p <= 0)) goto S10;
    fputs("P nonpositive in SETGMN\n",stderr);
    fprintf(stderr,"Value of P: %12ld\n",p);
    exit(1);
S10:
    *parm = p;
/*
     PUT P AND MEANV INTO PARM
*/
    for(i=2,D2=1,D3=(p+1-i+D2)/D2; D3>0; D3--,i+=D2) *(parm+i-1) = *(meanv+i-2);
/*
      Cholesky decomposition to find A s.t. trans(A)*(A) = COVM
*/
    spofa(covm,p,p,&info);
    if(!(info != 0)) goto S30;
    fputs(" COVM not positive definite in SETGMN\n",stderr);
    exit(1);
S30:
    icount = p+1;
/*
     PUT UPPER HALF OF A, WHICH IS NOW THE CHOLESKY FACTOR, INTO PARM
          COVM(1,1) = PARM(P+2)
          COVM(1,2) = PARM(P+3)
                    :
          COVM(1,P) = PARM(2P+1)
          COVM(2,2) = PARM(2P+2)  ...
*/
    for(i=1,D4=1,D5=(p-i+D4)/D4; D5>0; D5--,i+=D4) {
        for(j=i-1; j<p; j++) {
            icount += 1;
            *(parm+icount-1) = *(covm+i-1+j*p);
        }
    }
}

double sexpo(void)
/*
**********************************************************************
                                                                      
                                                                      
     (STANDARD-)  E X P O N E N T I A L   DISTRIBUTION                
                                                                      
                                                                      
**********************************************************************
**********************************************************************
                                                                      
     FOR DETAILS SEE:                                                 
                                                                      
               AHRENS, J.H. AND DIETER, U.                            
               COMPUTER METHODS FOR SAMPLING FROM THE                 
               EXPONENTIAL AND NORMAL DISTRIBUTIONS.                  
               COMM. ACM, 15,10 (OCT. 1972), 873 - 882.               
                                                                      
     ALL STATEMENT NUMBERS CORRESPOND TO THE STEPS OF ALGORITHM       
     'SA' IN THE ABOVE PAPER (SLIGHTLY MODIFIED IMPLEMENTATION)       
                                                                      
     Modified by Barry W. Brown, Feb 3, 1988 to use RANF instead of   
     SUNIF.  The argument IR thus goes away.                          
                                                                      
**********************************************************************
     Q(N) = SUM(ALOG(2.0)**K/K!)    K=1,..,N ,      THE HIGHEST N
     (HERE 8) IS DETERMINED BY Q(N)=1.0 WITHIN STANDARD PRECISION
*/
{
static double q[8] = {
    0.69314718055995, 0.93337368751905, 0.98887779618387, 0.99849592529150,
    0.99982928110614, 0.99998331641007, 0.99999856914388, 0.99999989069256
};
static long i;
static double sexpo,a,u,ustar,umin;
static double *q1 = q;
    a = 0.0;
    u = ranf();
    goto S30;
S20:
    a += *q1;
S30:
    u += u;
/*
 * JJV changed the following to reflect the true algorithm and prevent
 * JJV unpredictable behavior if U is initially 0.5.
 *  if(u <= 1.0) goto S20;
 */
    if(u < 1.0) goto S20;
    u -= 1.0;
    if(u > *q1) goto S60;
    sexpo = a+u;
    return sexpo;
S60:
    i = 1;
    ustar = ranf();
    umin = ustar;
S70:
    ustar = ranf();
    if(ustar < umin) umin = ustar;
    i += 1;
    if(u > *(q+i-1)) goto S70;
    sexpo = a+umin**q1;
    return sexpo;
}

double sgamma(double a)
/*
**********************************************************************
                                                                      
                                                                      
     (STANDARD-)  G A M M A  DISTRIBUTION                             
                                                                      
                                                                      
**********************************************************************
**********************************************************************
                                                                      
               PARAMETER  A >= 1.0  !                                 
                                                                      
**********************************************************************
                                                                      
     FOR DETAILS SEE:                                                 
                                                                      
               AHRENS, J.H. AND DIETER, U.                            
               GENERATING GAMMA VARIATES BY A                         
               MODIFIED REJECTION TECHNIQUE.                          
               COMM. ACM, 25,1 (JAN. 1982), 47 - 54.                  
                                                                      
     STEP NUMBERS CORRESPOND TO ALGORITHM 'GD' IN THE ABOVE PAPER     
                                 (STRAIGHTFORWARD IMPLEMENTATION)     
                                                                      
     Modified by Barry W. Brown, Feb 3, 1988 to use RANF instead of   
     SUNIF.  The argument IR thus goes away.                          
                                                                      
**********************************************************************
                                                                      
               PARAMETER  0.0 < A < 1.0  !                            
                                                                      
**********************************************************************
                                                                      
     FOR DETAILS SEE:                                                 
                                                                      
               AHRENS, J.H. AND DIETER, U.                            
               COMPUTER METHODS FOR SAMPLING FROM GAMMA,              
               BETA, POISSON AND BINOMIAL DISTRIBUTIONS.              
               COMPUTING, 12 (1974), 223 - 246.                       
                                                                      
     (ADAPTED IMPLEMENTATION OF ALGORITHM 'GS' IN THE ABOVE PAPER)    
                                                                      
**********************************************************************
     INPUT: A =PARAMETER (MEAN) OF THE STANDARD GAMMA DISTRIBUTION
     OUTPUT: SGAMMA = SAMPLE FROM THE GAMMA-(A)-DISTRIBUTION
     COEFFICIENTS Q(K) - FOR Q0 = SUM(Q(K)*A**(-K))
     COEFFICIENTS A(K) - FOR Q = Q0+(T*T/2)*SUM(A(K)*V**K)
     COEFFICIENTS E(K) - FOR EXP(Q)-1 = SUM(E(K)*Q**K)
     PREVIOUS A PRE-SET TO ZERO - AA IS A', AAA IS A"
     SQRT32 IS THE SQUAREROOT OF 32 = 5.656854249492380
*/
{
extern double fsign( double num, double sign );
static double q1 = 4.16666664E-2;
static double q2 = 2.08333723E-2;
static double q3 = 7.9849875E-3;
static double q4 = 1.5746717E-3;
static double q5 = -3.349403E-4;
static double q6 = 3.340332E-4;
static double q7 = 6.053049E-4;
static double q8 = -4.701849E-4;
static double q9 = 1.710320E-4;
static double a1 =  0.333333333;
static double a2 = -0.249999949;
static double a3 =  0.199999867;
static double a4 = -0.166677482;
static double a5 =  0.142873973;
static double a6 = -0.124385581;
static double a7 =  0.110368310;
static double a8 = -0.112750886;
static double a9 =  0.104089866;
static double e1 = 1.0;
static double e2 = 0.499999994;
static double e3 = 0.166666848;
static double e4 = 4.1664508E-2;
static double e5 = 8.345522E-3;
static double e6 = 1.353826E-3;
static double e7 = 2.47453E-4;
static double aa = 0.0;
static double aaa = 0.0;
static double sqrt32 = 5.65685424949238;
/* JJV added b0 to fix rare and subtle bug */
static double sgamma,s2,s,d,t,x,u,r,q0,b,b0,si,c,v,q,e,w,p;
    if(a == aa) goto S10;
    if(a < 1.0) goto S120;
/*
     STEP  1:  RECALCULATIONS OF S2,S,D IF A HAS CHANGED
*/
    aa = a;
    s2 = a-0.5;
    s = sqrt(s2);
    d = sqrt32-12.0*s;
S10:
/*
     STEP  2:  T=STANDARD NORMAL DEVIATE,
               X=(S,1/2)-NORMAL DEVIATE.
               IMMEDIATE ACCEPTANCE (I)
*/
    t = snorm();
    x = s+0.5*t;
    sgamma = x*x;
    if(t >= 0.0) return sgamma;
/*
     STEP  3:  U= 0,1 -UNIFORM SAMPLE. SQUEEZE ACCEPTANCE (S)
*/
    u = ranf();
    if(d*u <= t*t*t) return sgamma;
/*
     STEP  4:  RECALCULATIONS OF Q0,B,SI,C IF NECESSARY
*/
    if(a == aaa) goto S40;
    aaa = a;
    r = 1.0/a;
    q0 = ((((((((q9*r+q8)*r+q7)*r+q6)*r+q5)*r+q4)*r+q3)*r+q2)*r+q1)*r;
/*
               APPROXIMATION DEPENDING ON SIZE OF PARAMETER A
               THE CONSTANTS IN THE EXPRESSIONS FOR B, SI AND
               C WERE ESTABLISHED BY NUMERICAL EXPERIMENTS
*/
    if(a <= 3.686) goto S30;
    if(a <= 13.022) goto S20;
/*
               CASE 3:  A .GT. 13.022
*/
    b = 1.77;
    si = 0.75;
    c = 0.1515/s;
    goto S40;
S20:
/*
               CASE 2:  3.686 .LT. A .LE. 13.022
*/
    b = 1.654+7.6E-3*s2;
    si = 1.68/s+0.275;
    c = 6.2E-2/s+2.4E-2;
    goto S40;
S30:
/*
               CASE 1:  A .LE. 3.686
*/
    b = 0.463+s+0.178*s2;
    si = 1.235;
    c = 0.195/s-7.9E-2+1.6E-1*s;
S40:
/*
     STEP  5:  NO QUOTIENT TEST IF X NOT POSITIVE
*/
    if(x <= 0.0) goto S70;
/*
     STEP  6:  CALCULATION OF V AND QUOTIENT Q
*/
    v = t/(s+s);
    if(fabs(v) <= 0.25) goto S50;
    q = q0-s*t+0.25*t*t+(s2+s2)*log(1.0+v);
    goto S60;
S50:
    q = q0+0.5*t*t*((((((((a9*v+a8)*v+a7)*v+a6)*v+a5)*v+a4)*v+a3)*v+a2)*v+a1)*v;
S60:
/*
     STEP  7:  QUOTIENT ACCEPTANCE (Q)
*/
    if(log(1.0-u) <= q) return sgamma;
S70:
/*
     STEP  8:  E=STANDARD EXPONENTIAL DEVIATE
               U= 0,1 -UNIFORM DEVIATE
               T=(B,SI)-DOUBLE EXPONENTIAL (LAPLACE) SAMPLE
*/
    e = sexpo();
    u = ranf();
    u += (u-1.0);
    t = b+fsign(si*e,u);
/*
     STEP  9:  REJECTION IF T .LT. TAU(1) = -.71874483771719
*/
    if(t < -0.71874483771719) goto S70;
/*
     STEP 10:  CALCULATION OF V AND QUOTIENT Q
*/
    v = t/(s+s);
    if(fabs(v) <= 0.25) goto S80;
    q = q0-s*t+0.25*t*t+(s2+s2)*log(1.0+v);
    goto S90;
S80:
    q = q0+0.5*t*t*((((((((a9*v+a8)*v+a7)*v+a6)*v+a5)*v+a4)*v+a3)*v+a2)*v+a1)*v;
S90:
/*
     STEP 11:  HAT ACCEPTANCE (H) (IF Q NOT POSITIVE GO TO STEP 8)
*/
    if(q <= 0.0) goto S70;
    if(q <= 0.5) goto S100;
/*
 * JJV modified the code through line 115 to handle large Q case
 */
    if(q < 15.0) goto S95;
/*
 * JJV Here Q is large enough that Q = log(exp(Q) - 1.0) (for real Q)
 * JJV so reformulate test at 110 in terms of one EXP, if not too big
 * JJV 87.49823 is close to the largest real which can be
 * JJV exponentiated (87.49823 = log(1.0E38))
 */
    if((q+e-0.5*t*t) > 87.4982335337737) goto S115;
    if(c*fabs(u) > exp(q+e-0.5*t*t)) goto S70;
    goto S115;
S95:
    w = exp(q)-1.0;
    goto S110;
S100:
    w = ((((((e7*q+e6)*q+e5)*q+e4)*q+e3)*q+e2)*q+e1)*q;
S110:
/*
               IF T IS REJECTED, SAMPLE AGAIN AT STEP 8
*/
    if(c*fabs(u) > w*exp(e-0.5*t*t)) goto S70;
S115:
    x = s+0.5*t;
    sgamma = x*x;
    return sgamma;
S120:
/*
     ALTERNATE METHOD FOR PARAMETERS A BELOW 1  (.3678794=EXP(-1.))

     JJV changed B to B0 (which was added to declarations for this)
     JJV in 120 to END to fix rare and subtle bug.
     JJV Line: 'aa = 0.0' was removed (unnecessary, wasteful).
     JJV Reasons: the state of AA only serves to tell the A >= 1.0
     JJV case if certain A-dependent constants need to be recalculated.
     JJV The A < 1.0 case (here) no longer changes any of these, and
     JJV the recalculation of B (which used to change with an
     JJV A < 1.0 call) is governed by the state of AAA anyway.
    aa = 0.0;
*/
    b0 = 1.0+ 0.3678794411714423*a;
S130:
    p = b0*ranf();
    if(p >= 1.0) goto S140;
    sgamma = exp(log(p)/ a);
    if(sexpo() < sgamma) goto S130;
    return sgamma;
S140:
    sgamma = -log((b0-p)/ a);
    if(sexpo() < (1.0-a)*log(sgamma)) goto S130;
    return sgamma;
}

double snorm(void)
/*
**********************************************************************
                                                                      
                                                                      
     (STANDARD-)  N O R M A L  DISTRIBUTION                           
                                                                      
                                                                      
**********************************************************************
**********************************************************************
                                                                      
     FOR DETAILS SEE:                                                 
                                                                      
               AHRENS, J.H. AND DIETER, U.                            
               EXTENSIONS OF FORSYTHE'S METHOD FOR RANDOM             
               SAMPLING FROM THE NORMAL DISTRIBUTION.                 
               MATH. COMPUT., 27,124 (OCT. 1973), 927 - 937.          
                                                                      
     ALL STATEMENT NUMBERS CORRESPOND TO THE STEPS OF ALGORITHM 'FL'  
     (M=5) IN THE ABOVE PAPER     (SLIGHTLY MODIFIED IMPLEMENTATION)  
                                                                      
     Modified by Barry W. Brown, Feb 3, 1988 to use RANF instead of   
     SUNIF.  The argument IR thus goes away.                          
                                                                      
**********************************************************************
     THE DEFINITIONS OF THE CONSTANTS A(K), D(K), T(K) AND
     H(K) ARE ACCORDING TO THE ABOVEMENTIONED ARTICLE
*/
{
static double a[32] = {
    0.0,              0.03917608550309, 0.07841241273311, 0.11776987457909,
    0.15731068461017, 0.19709908429430, 0.23720210932878, 0.27769043982157,
    0.31863936396437, 0.36012989178957, 0.40225006532172, 0.44509652498551,
    0.48877641111466, 0.53340970624127, 0.57913216225555, 0.62609901234641,
    0.67448975019607, 0.72451438349236, 0.77642176114792, 0.83051087820539,
    0.88714655901887, 0.94678175630104, 1.00999016924958, 1.07751556704027,
    1.15034938037600, 1.22985875921658, 1.31801089730353, 1.41779713799625,
    1.53412054435253, 1.67593972277344, 1.86273186742164, 2.15387469406144
};
static double d[31] = {
    0.0, 0.0, 0.0, 0.0,
    0.0,              0.26368432217502, 0.24250845238097, 0.22556744380930,
    0.21163416577204, 0.19992426749317, 0.18991075842246, 0.18122518100691,
    0.17360140038056, 0.16684190866667, 0.16079672918053, 0.15534971747692,
    0.15040938382813, 0.14590257684509, 0.14177003276856, 0.13796317369537,
    0.13444176150074, 0.13117215026483, 0.12812596512583, 0.12527909006226,
    0.12261088288608, 0.12010355965651, 0.11774170701949, 0.11551189226063,
    0.11340234879117, 0.11140272044119, 0.10950385201710
};
static double t[31] = {
    7.6738283767E-4,   2.30687039764E-3,  3.86061844387E-3,  5.43845406707E-3,
    7.05069876857E-3,  8.70839582019E-3,  1.042356984914E-2, 1.220953194966E-2,
    1.408124734637E-2, 1.605578804548E-2, 1.815290075142E-2, 2.039573175398E-2,
    2.281176732513E-2, 2.543407332319E-2, 2.830295595118E-2, 3.146822492920E-2,
    3.499233438388E-2, 3.895482964836E-2, 4.345878381672E-2, 4.864034918076E-2,
    5.468333844273E-2, 6.184222395816E-2, 7.047982761667E-2, 8.113194985866E-2,
    9.462443534514E-2, 0.11230007889456,  0.13649799954975,  0.17168856004707,
    0.22762405488269,  0.33049802776911,  0.58470309390507
};
static double h[31] = {
    3.920617164634E-2, 3.932704963665E-2, 3.950999486086E-2, 3.975702679515E-2,
    4.007092772490E-2, 4.045532602655E-2, 4.091480886081E-2, 4.145507115859E-2,
    4.208311051344E-2, 4.280748137995E-2, 4.363862733472E-2, 4.458931789605E-2,
    4.567522779560E-2, 4.691571371696E-2, 4.833486978119E-2, 4.996298427702E-2,
    5.183858644724E-2, 5.401138183398E-2, 5.654656186515E-2, 5.953130423884E-2,
    6.308488965373E-2, 6.737503494905E-2, 7.264543556657E-2, 7.926471414968E-2,
    8.781922325338E-2, 9.930398323927E-2, 0.11555994154118,  0.14043438342816,
    0.18361418337460,  0.27900163464163,  0.70104742502766
};
static long i;
static double snorm,u,s,ustar,aa,w,y,tt;
    u = ranf();
    s = 0.0;
    if(u > 0.5) s = 1.0;
    u += (u-s);
    u = 32.0*u;
    i = (long) (u);
    if(i == 32) i = 31;
    if(i == 0) goto S100;
/*
                                START CENTER
*/
    ustar = u-(double)i;
    aa = *(a+i-1);
S40:
    if(ustar <= *(t+i-1)) goto S60;
    w = (ustar-*(t+i-1))**(h+i-1);
S50:
/*
                                EXIT   (BOTH CASES)
*/
    y = aa+w;
    snorm = y;
    if(s == 1.0) snorm = -y;
    return snorm;
S60:
/*
                                CENTER CONTINUED
*/
    u = ranf();
    w = u*(*(a+i)-aa);
    tt = (0.5*w+aa)*w;
    goto S80;
S70:
    tt = u;
    ustar = ranf();
S80:
    if(ustar > tt) goto S50;
    u = ranf();
    if(ustar >= u) goto S70;
    ustar = ranf();
    goto S40;
S100:
/*
                                START TAIL
*/
    i = 6;
    aa = *(a+31);
    goto S120;
S110:
    aa += *(d+i-1);
    i += 1;
S120:
    u += u;
    if(u < 1.0) goto S110;
    u -= 1.0;
S140:
    w = u**(d+i-1);
    tt = (0.5*w+aa)*w;
    goto S160;
S150:
    tt = u;
S160:
    ustar = ranf();
    if(ustar > tt) goto S50;
    u = ranf();
    if(ustar >= u) goto S150;
    u = ranf();
    goto S140;
}

double fsign( double num, double sign )
/* Transfers sign of argument sign to argument num */
{
if ( ( sign>0.0f && num<0.0f ) || ( sign<0.0f && num>0.0f ) )
    return -num;
else return num;
}

/************************************************************************
FTNSTOP:
Prints msg to standard error and then exits
************************************************************************/
void ftnstop(char* msg)
/* msg - error message */
{
  if (msg != NULL) fprintf(stderr,"%s\n",msg);
  exit(0);
}
