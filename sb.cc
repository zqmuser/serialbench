/*  Program sb.cc
    version 1.0.0

   This is a free-standing program for high-precision numeric integration
of primitive functions.


***************************************************************/

/***************************************************************
 installation command 

linux
g++ sb.cc -O3 -lm -DERROR_TRAPS -o sb

***************************************************************/

//**** include stuff *****
#include "sb.h"


int main( int argc, char *argv[], char *environment[] )
// argc is the number of command line arguments present
// argv is a ragged array of those arguments
// environment is a ragged array of all environment variables
{  
  char      *lcpTemp;      // pointer for errors
  char      lcaOutput_line[giSTRING_LEN];


  // check number of arguments and prompt for correct usage
  if ( argc != 2 )
    {
      sprintf( lcaOutput_line, 
               "\n              %s version %s\n\n", gcaPROGRAM_NAME, gcaVERSION);
      cout << lcaOutput_line;
      sprintf( lcaOutput_line, 
               "The program %s requires one argument, i.e. \n\n", gcaPROGRAM_NAME );
      cout << lcaOutput_line;
      sprintf( lcaOutput_line, "    %s number_of_points  \n\n", gcaPROGRAM_NAME );
      cout << lcaOutput_line;
      cout << "where number_of_points are used in each of six dimensions.\n";
      cout << "Four GTO primitives on the same center\n";
      cout << "with exponents 1.2 1.3 1.4 1.5\n";
      cout << "are used to compute <1s1s|1/r12|1s1s>\n";
      cout << "The analytic value of the integral is 2.076681006931\n\n";

      // 10 points gives Integral 0.0150211
      // 20 points gives Integral 1.31468
      // 30 points gives Integral 1.73223
      // 40 points gives Integral 1.87937
      // 50 points gives Integral 1.94944
      // 60 points gives Integral 1.98796
      // 80 points gives Integral 2.02657
      // 100 points gives Integral 2.04455
      // 120 points gives Integral 2.05435
      // 150 points gives Integral 2.06238

      cout << endl;
    }
   else
    {

      // either of these work in g++ or icpc
      // pgc++ says error: namespace "std" has no member "stoi"
      //giNumX = stoi( argv[1] );
      //giNumX = std::stoi( argv[1] );
      // strtol is an alternative, again gcc and Intel work, but PGI need #include <stdlib.h>
      giNumX = strtol( argv[1], &lcpTemp, 10 );

      giNumY = giNumX;
      giNumZ = giNumX;
//cout << "giNumX=" << giNumX << endl;

      setup_defaults(); 

      // do the big computing part
      gto_1s1s1s1s_1c_r12();

      cout << "Integral " << gdIntegral << endl;

    }
  return(0);            // would return a code to graphical interface
}


//*********************************************************************
//               some more generic math functions

double sqr( double ldX )  // number to square
{
  // function to return the square of a double precision number

  //**** debug message ****
  //cout << "   procedure sqr \n";

  return( ldX * ldX );
}



//*********************************************************************
//               section to setup default parameters

void setup_defaults( void )    // mostly set pointers to NULL
{

  // this function sets default values of global variables

  sprintf( gcaInput_prog_name, "sb" );
  sprintf( gcaInput_ver, "1.0.0" );

  giaAtomicNum[0] = 8;   // oxygen
  gdaCoord[0][0] = 0.0;  // at origin
  gdaCoord[0][1] = 0.0;
  gdaCoord[0][2] = 0.0;
  
  // boundaries of integration box    
  gdMinX = -8.0;
  gdMaxX = 8.0;
  gdMinY = -8.0;
  gdMaxY = 8.0;
  gdMinZ = -8.0;
  gdMaxZ = 8.0;

  // GTO exponents
  gdaExponent[0] = 1.2;
  gdaExponent[1] = 1.3;
  gdaExponent[2] = 1.4;
  gdaExponent[3] = 1.5;

}


//*********************************************************************
//               section to read input .inp file

void gto_1s1s1s1s_1c_r12( void )
{
  long int     liX1, liY1, liZ1;  // loop counters
  long int     liX2, liY2, liZ2;  // loop counters
  double   ldX1, ldY1, ldZ1;
  double   ldX2, ldY2, ldZ2;
  double   ldRowSum1, ldPlaneSum1, ldVolumeSum1;
  double   ldRowSum2, ldPlaneSum2, ldVolumeSum2;
  double   ldStepX, ldStepY, ldStepZ;
  double   ldHalfStepX, ldHalfStepY, ldHalfStepZ;
  double   ldR2a, ldR2b;   // r squared
  double   ldR12;   // r12 
  double   ldTemp;  
  double   ldExponSumA, ldExponSumB;

  // DEBUG
  //cout << "1 procedure gto_1s1s1s1s_1c_r12 \n"; 

  // we are doing a high-precision rectangular grid rieman sum

  // initialize variables
  ldRowSum1 = 0.0;
  ldPlaneSum1 = 0.0;
  ldVolumeSum1 = 0.0;
  ldRowSum2 = 0.0;
  ldPlaneSum2 = 0.0;
  ldVolumeSum2 = 0.0;
  gdIntegral = 0.0;

  ldExponSumA = gdaExponent[0] + gdaExponent[1];
  ldExponSumB = gdaExponent[2] + gdaExponent[3];

  // work out step sizes
  ldStepX = (gdMaxX - gdMinX) / double( giNumX );
  ldStepY = (gdMaxY - gdMinY) / double( giNumY );
  ldStepZ = (gdMaxZ - gdMinZ) / double( giNumZ );
  ldHalfStepX = ldStepX / 2.0;
  ldHalfStepY = ldStepY / 2.0;
  ldHalfStepZ = ldStepZ / 2.0;

  ldVolumeSum1 = 0.0;

  // WARNING:  This works with Intel compiler, not with GCC or PGI
#pragma omp parallel for schedule(static) reduction(+:ldVolumeSum1)
  for ( liX1=0 ; liX1<giNumX ; liX1++ )
  {
    // midpoint rule formula
    ldX1 = liX1 * ldStepX + gdMinX + ldHalfStepX;

    ldVolumeSum2 = 0.0;

    for ( liX2=0 ; liX2<giNumX ; liX2++ )
    {
      // midpoint rule formula
      ldX2 = liX2 * ldStepX + gdMinX + ldHalfStepX;

      ldPlaneSum1 = 0.0;

      for ( liY1=0 ; liY1<giNumY ; liY1++ )
      {
        ldY1 = liY1 * ldStepY + gdMinY + ldHalfStepY;

        ldPlaneSum2 = 0.0;

        for ( liY2=0 ; liY2<giNumY ; liY2++ )
        {
          ldY2 = liY2 * ldStepY + gdMinY + ldHalfStepY;

          ldRowSum1 = 0.0;

          for ( liZ1=0 ; liZ1<giNumZ ; liZ1++ )
          {
            ldZ1 = liZ1 * ldStepZ + gdMinZ + ldHalfStepZ;

            ldRowSum2 = 0.0;

            // exclude the singularity
            if ( (liX1==liX2) && (liY1==liY2) )
            {

              for ( liZ2=0 ; liZ2<liZ1 ; liZ2++ )
              {
                ldZ2 = liZ2 * ldStepZ + gdMinZ + ldHalfStepZ;

                // evaluate R^2
                ldR2a = ldX1*ldX1 + ldY1*ldY1 + ldZ1*ldZ1;
                ldR2b = ldX2*ldX2 + ldY2*ldY2 + ldZ2*ldZ2;

                //ldRa = sqrtq( ldR2a );
                //ldRb = sqrtq( ldR2b );

                ldR12 = sqrt( (ldX1-ldX2)*(ldX1-ldX2)
                              + (ldY1-ldY2)*(ldY1-ldY2) 
                              + (ldZ1-ldZ2)*(ldZ1-ldZ2) );

                // DANGER WILL ROBINSON DANGER
                // The equations here must be edited in 3 locations

                // choice to make here 
                //  serial execution is faster without ldTemp
                //     ( better for pipelining )
                //  OpenMP execution faster with it
                //     ( less critical code )
                ldTemp = exp( -ldR2a * ldExponSumA )
                         * exp( -ldR2b * ldExponSumB ) 
                         / ldR12;

                ldRowSum2 += ldTemp;

              } // loop over Z2

              for ( liZ2=liZ1+1 ; liZ2<giNumZ ; liZ2++ )
              {
                ldZ2 = liZ2 * ldStepZ + gdMinZ + ldHalfStepZ;

                // evaluate R^2
                ldR2a = ldX1*ldX1 + ldY1*ldY1 + ldZ1*ldZ1;
                ldR2b = ldX2*ldX2 + ldY2*ldY2 + ldZ2*ldZ2;

                //ldRa = sqrtq( ldR2a );
                //ldRb = sqrtq( ldR2b );

                ldR12 = sqrt( (ldX1-ldX2)*(ldX1-ldX2)
                              + (ldY1-ldY2)*(ldY1-ldY2) 
                              + (ldZ1-ldZ2)*(ldZ1-ldZ2) );

                // DANGER WILL ROBINSON DANGER
                // The equations here must be edited in 3 locations

                // choice to make here 
                //  serial execution is faster without ldTemp
                //     ( better for pipelining )
                //  OpenMP execution faster with it
                //     ( less critical code )
                ldTemp = exp( -ldR2a * ldExponSumA )
                         * exp( -ldR2b * ldExponSumB ) 
                         / ldR12;

                ldRowSum2 += ldTemp;

              } // loop over Z2

            } // x&y same, avoid singularity
            else
            {  // no singularity

              for ( liZ2=0 ; liZ2<giNumZ ; liZ2++ )
              {
                ldZ2 = liZ2 * ldStepZ + gdMinZ + ldHalfStepZ;

                // evaluate R^2
                ldR2a = ldX1*ldX1 + ldY1*ldY1 + ldZ1*ldZ1;
                ldR2b = ldX2*ldX2 + ldY2*ldY2 + ldZ2*ldZ2;

                //ldRa = sqrtq( ldR2a );
                //ldRb = sqrtq( ldR2b );

                ldR12 = sqrt( (ldX1-ldX2)*(ldX1-ldX2)
                              + (ldY1-ldY2)*(ldY1-ldY2) 
                              + (ldZ1-ldZ2)*(ldZ1-ldZ2) );

                // DANGER WILL ROBINSON DANGER
                // The equations here must be edited in 3 locations

                // choice to make here 
                //  serial execution is faster without ldTemp
                //     ( better for pipelining )
                //  OpenMP execution faster with it
                //     ( less critical code )
                ldTemp = exp( -ldR2a * ldExponSumA )
                         * exp( -ldR2b * ldExponSumB ) 
                         / ldR12;

                ldRowSum2 += ldTemp;

              } // loop over Z2

            }  // no singularity

          ldRowSum1 += ldRowSum2;

          } // loop over Z1

          ldPlaneSum2 += ldRowSum1;

        } // loop over Y2

        ldPlaneSum1 += ldPlaneSum2;

      } // loop over Y1

      ldVolumeSum2 += ldPlaneSum1;

    } // loop over X2

    ldVolumeSum1 += ldVolumeSum2;

  } // loop over X1

  // regular midpoint rule is like this
  // six fold integral requires six dxdxdydydzdz
  gdIntegral = ldVolumeSum1 * ldStepX*ldStepX * ldStepY*ldStepY 
               * ldStepZ*ldStepZ;

}  // end of gto_1s1s1s1s_1c_r12



