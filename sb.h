/*  file sb.h

***************************************************************/

#ifndef NUMINTY_H
#define NUMINTY_H


// include header files
#include <stdarg.h>     // needed by some compilers for va_start, etc.

#include <iostream>     // for cin
#include <iomanip>      // for cout formatting
// allow us to call cout
using namespace std;

#include <string.h>     // for strcat and stoi
#include <stdio.h>      // for file access
#include <stdlib.h>     // standard namespace (by default in gcc & icc, but not pgi)
//  #include <quadmath.h>
#include <math.h>   // for double precision


// constants specific to this program
const char       *gcaPROGRAM_NAME="sb";
const char       *gcaPROGRAM_DESCR="primitive numeric integration benchmark";
const char       *gcaVERSION="1.0.0";
const char       *gcaCOPR_DATE="2019";

// generic constants
const int     giSTRING_LEN=512;      // room for error in long strings
// boolean constants
const int     giTRUE=1;
const int     giFALSE=0;

//**** function prototypes *****
double sqr( double ldX );
void setup_defaults ( void );
void gto_1s1s1s1s_1c_r12( void );


// ***** Global variable setup ****
char         gcaInput_prog_name[giSTRING_LEN];
char         gcaInput_ver[giSTRING_LEN];    // version
int          giaAtomicNum[4];
double       gdaCoord[4][3];
long int     giNumX, giNumY, giNumZ;
double       gdMinX, gdMinY, gdMinZ;
double       gdMaxX, gdMaxY, gdMaxZ;
double       gdaExponent[4];
double       gdaNormCoef[4];
double       gdIntegral;


/************************ 
  debug:  each element of the array is set to a value, which results in
          sending data to standard output.  
          Data for element 0 is indented to match setting - no indent for
          value=1, two spaces for value=2, etc.
          Settings of 0 turn off that function.
    element  setting  meaning 
        0       1       print prog. or top (noniterative) routine name.
        0       2       print major iterative routine name.
        0       3       print all routine names.
        1       1       print minimal pertinent data for routines from 0,1
        1       2       print minimal pertinent data for routines from 0,2
        1       3       print minimal pertinent data for routines from 0,3
*************************/

#endif

