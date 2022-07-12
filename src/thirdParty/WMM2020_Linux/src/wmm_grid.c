#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#include "GeomagnetismHeader.h"
#include "EGM9615.h"
/*#include "GeomagnetismLibrary.c"*/

/*


WMM grid program.

The Geomagnetism Library is used to make a command prompt program. The program prompts
the user to enter a location, performs the computations and prints the results to the
standard output. The program expects the files GeomagnetismLibrary.c, GeomagnetismHeader.h,
WMM2010SHDF.COF and EGM9615.h to be in the same directory. 

Manoj.C.Nair@Noaa.Gov
April 21, 2011

 */

int MAG_Grid(MAGtype_CoordGeodetic minimum,
        MAGtype_CoordGeodetic maximum, 
        double cord_step_size, 
        double altitude_step_size, 
        double time_step, 
        MAGtype_MagneticModel *MagneticModel, 
        MAGtype_Geoid *Geoid, 
        MAGtype_Ellipsoid Ellip, 
        MAGtype_Date StartDate, 
        MAGtype_Date EndDate, 
        int ElementOption, 
        int UncertaintyOption, 
        int PrintOption, 
        char *OutputFile);

const char* BOZ_WARN_TEXT_STRONG = "Warning: some calculated locations are "
                                   "in the blackout zone "
                                   "around the magnetic pole\nas defined by "
                                   "the WMM military specification \n(https://"
                                   "www.ngdc.noaa.gov/geomag/WMM/data/MIL-PRF-"
                                   "89500B.pdf).\nCompass accuracy is highly "
                                   "degraded in this region.\n";
const char* BOZ_WARN_TEXT_WEAK = "Caution: some calculated locations approach "
                                "the blackout zone around the magnetic\npole "
                                "as defined by the WMM military specification "
                                "\n(https://www.ngdc.noaa.gov/geomag/WMM/data/"
                                "MIL-PRF-89500B.pdf).\n Compass accuracy may "
                                "be degraded in this region.\n";

int main()
{
    MAGtype_MagneticModel * MagneticModels[1];
    MAGtype_Ellipsoid Ellip;
    MAGtype_CoordGeodetic minimum, maximum;
    MAGtype_Geoid Geoid;
    MAGtype_Date startdate, enddate;
    int ElementOption, PrintOption, i, epochs = 1, UncertaintyOption = 1;
    double cord_step_size, altitude_step_size, time_step_size;
    char filename[] = "WMM.COF";
    char OutputFilename[32];
    char VersionDate[12];
    char ans[20];

    if(!MAG_robustReadMagModels(filename, &MagneticModels, 1)) {
        printf("\n WMM.COF not found.  Press enter to exit... \n ");
        fgets(ans, 20, stdin);
        return 1;
    }
    strncpy(VersionDate, VERSIONDATE_LARGE + 39, 11);
    VersionDate[11] = '\0';

    MAG_SetDefaults(&Ellip, &Geoid);
    /* Set EGM96 Geoid parameters */
    Geoid.GeoidHeightBuffer = GeoidHeightBuffer;
    Geoid.Geoid_Initialized = 1;
    /* Set EGM96 Geoid parameters END */
    printf("\n\n Welcome to the World Magnetic Model (WMM) %d C-Program\n",(int) MagneticModels[0]->epoch);
    printf("of the US National Centers for Environmental Information\n\t\t--- Grid Calculation Program ----\n\t");
    printf("       --- Model Release Date: %s ---\n\t", MODEL_RELEASE_DATE);
    printf("      --- Software Release Date: %s ---\n", VersionDate);
    printf("This program may be used to generate a grid/volume of magnetic field values\n");
    printf("over latitude, longitude, altitude and time axes. To skip an axis,\n");
    printf("keep the start and end values the same and enter zero for the step size.\n");
    printf("Output values are space separated latitude, longitude, altitude, date,\n");
    printf("Magnetic Element, and Uncertainty in that element.  For more information\n");
    printf("see accompanying software manual.");

    printf("\n\n                     Enter grid parameters \n\n");


    /* Get the Lat/Long, Altitude, Time limits from a user interface and print the grid to screen */

    MAG_GetUserGrid(&minimum, &maximum, &cord_step_size, &altitude_step_size, &time_step_size, &startdate, &enddate, &ElementOption, &PrintOption, OutputFilename, &Geoid);
    MAG_Grid(minimum, maximum, cord_step_size, altitude_step_size, time_step_size, MagneticModels[0], &Geoid, Ellip, startdate, enddate, ElementOption, UncertaintyOption, PrintOption, OutputFilename);

    for(i = 0; i < epochs; i++) MAG_FreeMagneticModelMemory(MagneticModels[i]);




    printf("\nPress any key to exit...\n");
    getchar();

    return 0;
}

int MAG_Grid(MAGtype_CoordGeodetic minimum, MAGtype_CoordGeodetic maximum, double
        cord_step_size, double altitude_step_size, double time_step, MAGtype_MagneticModel *MagneticModel, MAGtype_Geoid
        *Geoid, MAGtype_Ellipsoid Ellip, MAGtype_Date StartDate, MAGtype_Date EndDate, int ElementOption, int UncertaintyOption, int PrintOption, char *OutputFile)

/*This function calls WMM subroutines to generate a grid as defined by the user. The function may be used
to generate a grid of magnetic field elements, time series or a profile. The selected geomagnetic element
is either printed to the file GridResults.txt or to the screen depending on user option.

INPUT: minimum :Data structure with the following elements (minimum limits of the grid)
                                double lambda; (longitude)
                                double phi; ( geodetic latitude)
                                double HeightAboveEllipsoid; (height above the ellipsoid (HaE) )
                                double HeightAboveGeoid;(height above the Geoid )
                maximum : same as the above (maximum limist of the grid)
                step_size  : double  : spatial step size, in decimal degrees
                a_step_size : double  :  double altitude step size (km)
                step_time : double  : time step size (decimal years)
                StartDate :  data structure with the following elements used
                                        double DecimalYear;     ( decimal years )
                EndDate :	Same as the above;
                MagneticModel :	 data structure with the following elements
                        double EditionDate;
                        double epoch;       Base time of Geomagnetic model epoch (yrs)
                        char  ModelName[20];
                        double *Main_Field_Coeff_G;          C - Gauss coefficients of main geomagnetic model (nT)
                        double *Main_Field_Coeff_H;          C - Gauss coefficients of main geomagnetic model (nT)
                        double *Secular_Var_Coeff_G;  CD - Gauss coefficients of secular geomagnetic model (nT/yr)
                        double *Secular_Var_Coeff_H;  CD - Gauss coefficients of secular geomagnetic model (nT/yr)
                        int nMax;  Maximum degree of spherical harmonic model
                        int nMaxSecVar; Maxumum degree of spherical harmonic secular model
                        int SecularVariationUsed; Whether or not the magnetic secular variation vector will be needed by program
                Geoid :  data structure with the following elements
        Pointer to data structure Geoid with the following elements
                        int NumbGeoidCols ;   ( 360 degrees of longitude at 15 minute spacing )
                        int NumbGeoidRows ;   ( 180 degrees of latitude  at 15 minute spacing )
                        int NumbHeaderItems ;    ( min, max lat, min, max long, lat, long spacing )
                        int	ScaleFactor;    ( 4 grid cells per degree at 15 minute spacing  )
                        float *GeoidHeightBuffer;   (Pointer to the memory to store the Geoid elevation data )
                        int NumbGeoidElevs;    (number of points in the gridded file )
                        int  Geoid_Initialized ;  ( indicates successful initialization )
   Ellip  data  structure with the following elements
                        double a; semi-major axis of the ellipsoid
                        double b; semi-minor axis of the ellipsoid
                        double fla;  flattening
                        double epssq; first eccentricity squared
                        double eps;  first eccentricity
                        double re; mean radius of  ellipsoid
          ElementOption : int : Geomagnetic Element to print
 *        UncertaintyOption: int: 1-Append uncertainties.  Otherwise do not append uncertainties.
          PrintOption : int : 1 Print to File, Otherwise, print to screen

   OUTPUT: none (prints the output to a file )

   CALLS : MAG_AllocateModelMemory To allocate memory for model coefficients
      MAG_TimelyModifyMagneticModel This modifies the Magnetic coefficients to the correct date.
                  MAG_ConvertGeoidToEllipsoidHeight (&CoordGeodetic, &Geoid);   Convert height above msl to height above WGS-84 ellipsoid
                  MAG_GeodeticToSpherical Convert from geodeitic to Spherical Equations: 7-8, WMM Technical report
                  MAG_ComputeSphericalHarmonicVariables Compute Spherical Harmonic variables
                  MAG_AssociatedLegendreFunction Compute ALF  Equations 5-6, WMM Technical report
                  MAG_Summation Accumulate the spherical harmonic coefficients Equations 10:12 , WMM Technical report
                  MAG_RotateMagneticVector Map the computed Magnetic fields to Geodeitic coordinates Equation 16 , WMM Technical report
                  MAG_CalculateGeoMagneticElements Calculate the geoMagnetic elements, Equation 18 , WMM Technical report

 */
{
    int NumTerms;
    double a, b, c, d, PrintElement, ErrorElement = 0;

    MAGtype_MagneticModel *TimedMagneticModel;
    MAGtype_CoordSpherical CoordSpherical;
    MAGtype_MagneticResults MagneticResultsSph, MagneticResultsGeo, MagneticResultsSphVar, MagneticResultsGeoVar;
    MAGtype_SphericalHarmonicVariables *SphVariables;
    MAGtype_GeoMagneticElements GeoMagneticElements, Errors;
    MAGtype_LegendreFunction *LegendreFunction;
    MAGtype_Gradient Gradient;
    int print_boz_warning_weak = FALSE;
    int print_boz_warning_strong = FALSE;
    
    FILE *fileout = NULL;

    if(PrintOption == 1)
    {
        fileout = fopen(OutputFile, "w");
        if(!fileout)
        {
            printf("Error opening %s to write", OutputFile);
            return FALSE;
        }
    }



    if(fabs(cord_step_size) < 1.0e-10) cord_step_size = 99999.0; /*checks to make sure that the step_size is not too small*/
    if(fabs(altitude_step_size) < 1.0e-10) altitude_step_size = 99999.0;
    if(fabs(time_step) < 1.0e-10) time_step = 99999.0;


    NumTerms = ((MagneticModel->nMax + 1) * (MagneticModel->nMax + 2) / 2);
    TimedMagneticModel = MAG_AllocateModelMemory(NumTerms);
    LegendreFunction = MAG_AllocateLegendreFunctionMemory(NumTerms); /* For storing the ALF functions */
    SphVariables = MAG_AllocateSphVarMemory(MagneticModel->nMax);
    a = minimum.HeightAboveGeoid; /*sets the loop initialization values*/
    b = minimum.phi;
    c = minimum.lambda;
    d = StartDate.DecimalYear;



    for(minimum.HeightAboveGeoid = a; minimum.HeightAboveGeoid <= maximum.HeightAboveGeoid; minimum.HeightAboveGeoid += altitude_step_size) /* Altitude loop*/
    {

        for(minimum.phi = b; minimum.phi <= maximum.phi; minimum.phi += cord_step_size) /*Latitude loop*/
        {

            for(minimum.lambda = c; minimum.lambda <= maximum.lambda; minimum.lambda += cord_step_size) /*Longitude loop*/
            {
                if(Geoid->UseGeoid == 1)
                    MAG_ConvertGeoidToEllipsoidHeight(&minimum, Geoid); /* This converts the height above mean sea level to height above the WGS-84 ellipsoid */
                else
                    minimum.HeightAboveEllipsoid = minimum.HeightAboveGeoid;
                MAG_GeodeticToSpherical(Ellip, minimum, &CoordSpherical);
                MAG_ComputeSphericalHarmonicVariables(Ellip, CoordSpherical, MagneticModel->nMax, SphVariables); /* Compute Spherical Harmonic variables  */
                MAG_AssociatedLegendreFunction(CoordSpherical, MagneticModel->nMax, LegendreFunction); /* Compute ALF  Equations 5-6, WMM Technical report*/

                for(StartDate.DecimalYear = d; StartDate.DecimalYear <= EndDate.DecimalYear; StartDate.DecimalYear += time_step) /*Year loop*/
                {

                    MAG_TimelyModifyMagneticModel(StartDate, MagneticModel, TimedMagneticModel); /*This modifies the Magnetic coefficients to the correct date. */
                    MAG_Summation(LegendreFunction, TimedMagneticModel, *SphVariables, CoordSpherical, &MagneticResultsSph); /* Accumulate the spherical harmonic coefficients Equations 10:12 , WMM Technical report*/
                    MAG_SecVarSummation(LegendreFunction, TimedMagneticModel, *SphVariables, CoordSpherical, &MagneticResultsSphVar); /*Sum the Secular Variation Coefficients, Equations 13:15 , WMM Technical report  */
                    MAG_RotateMagneticVector(CoordSpherical, minimum, MagneticResultsSph, &MagneticResultsGeo); /* Map the computed Magnetic fields to Geodetic coordinates Equation 16 , WMM Technical report */
                    MAG_RotateMagneticVector(CoordSpherical, minimum, MagneticResultsSphVar, &MagneticResultsGeoVar); /* Map the secular variation field components to Geodetic coordinates, Equation 17 , WMM Technical report*/
                    MAG_CalculateGeoMagneticElements(&MagneticResultsGeo, &GeoMagneticElements); /* Calculate the Geomagnetic elements, Equation 18 , WMM Technical report */
                    MAG_CalculateGridVariation(minimum, &GeoMagneticElements);
                    MAG_CalculateSecularVariationElements(MagneticResultsGeoVar, &GeoMagneticElements); /*Calculate the secular variation of each of the Geomagnetic elements, Equation 19, WMM Technical report*/
                    MAG_WMMErrorCalc(GeoMagneticElements.H, &Errors);
                    if(GeoMagneticElements.H <= 2000.0) {
                        print_boz_warning_strong = TRUE;
                    } else if (GeoMagneticElements.H <= 6000.0) {
                        print_boz_warning_weak = TRUE;
                    }
                    
                    if(ElementOption >= 17)
                        MAG_Gradient(Ellip, minimum, TimedMagneticModel, &Gradient);

                    switch(ElementOption) {
                        case 1:
                            PrintElement = GeoMagneticElements.Decl; /*1. Angle between the magnetic field vector and true north, positive east*/
                            ErrorElement = Errors.Decl;
                            break;
                        case 2:
                            PrintElement = GeoMagneticElements.Incl; /*2. Angle between the magnetic field vector and the horizontal plane, positive downward*/
                            ErrorElement = Errors.Incl;                            
                            break;
                        case 3:
                            PrintElement = GeoMagneticElements.F; /*3. Magnetic Field Strength*/
                            ErrorElement = Errors.F;
                            break;
                        case 4:
                            PrintElement = GeoMagneticElements.H; /*4. Horizontal Magnetic Field Strength*/
                            ErrorElement = Errors.H;
                            break;
                        case 5:
                            PrintElement = GeoMagneticElements.X; /*5. Northern component of the magnetic field vector*/
                            ErrorElement = Errors.X;
                            break;
                        case 6:
                            PrintElement = GeoMagneticElements.Y; /*6. Eastern component of the magnetic field vector*/
                            ErrorElement = Errors.Y;
                            break;
                        case 7:
                            PrintElement = GeoMagneticElements.Z; /*7. Downward component of the magnetic field vector*/
                            ErrorElement = Errors.Z;
                            break;
                        case 8:
                            PrintElement = GeoMagneticElements.GV; /*8. The Grid Variation*/
                            ErrorElement = Errors.Decl;
                            break;
                        case 9:
                            PrintElement = GeoMagneticElements.Decldot * 60; /*9. Yearly Rate of change in declination*/
                            UncertaintyOption = 0;
                            break;
                        case 10:
                            PrintElement = GeoMagneticElements.Incldot * 60; /*10. Yearly Rate of change in inclination*/
                            UncertaintyOption = 0;
                            break;
                        case 11:
                            PrintElement = GeoMagneticElements.Fdot; /*11. Yearly rate of change in Magnetic field strength*/
                            UncertaintyOption = 0;
                            break;
                        case 12:
                            PrintElement = GeoMagneticElements.Hdot; /*12. Yearly rate of change in horizontal field strength*/
                            UncertaintyOption = 0;
                            break;
                        case 13:
                            PrintElement = GeoMagneticElements.Xdot; /*13. Yearly rate of change in the northern component*/
                            UncertaintyOption = 0;
                            break;
                        case 14:
                            PrintElement = GeoMagneticElements.Ydot; /*14. Yearly rate of change in the eastern component*/
                            UncertaintyOption = 0;
                            break;
                        case 15:
                            PrintElement = GeoMagneticElements.Zdot; /*15. Yearly rate of change in the downward component*/
                            UncertaintyOption = 0;
                            break;
                        case 16:
                            PrintElement = GeoMagneticElements.GVdot;
                            UncertaintyOption = 0;
                            /*16. Yearly rate of change in grid variation*/;
                            break;
                        case 17:
                            PrintElement = Gradient.GradPhi.X;
                            UncertaintyOption = 0;
                            break;
                        case 18:
                            PrintElement = Gradient.GradPhi.Y;
                            UncertaintyOption = 0;
                            break;
                        case 19:
                            PrintElement = Gradient.GradPhi.Z;
                            UncertaintyOption = 0;
                            break;
                        case 20:
                            PrintElement = Gradient.GradLambda.X;
                            UncertaintyOption = 0;
                            break;
                        case 21:
                            PrintElement = Gradient.GradLambda.Y;
                            UncertaintyOption = 0;
                            break;
                        case 22:
                            PrintElement = Gradient.GradLambda.Z;
                            UncertaintyOption = 0;
                            break;
                        case 23:
                            PrintElement = Gradient.GradZ.X;
                            UncertaintyOption = 0;
                            break;
                        case 24:
                            PrintElement = Gradient.GradZ.Y;
                            UncertaintyOption = 0;
                            break;
                        case 25:
                            PrintElement = Gradient.GradZ.Z;
                            UncertaintyOption = 0;
                            break;
                        default:
                            PrintElement = GeoMagneticElements.Decl; /* 1. Angle between the magnetic field vector and true north, positive east*/
                            ErrorElement = Errors.Decl;
                    }
                    
                    if(Geoid->UseGeoid == 1)
                    {
                        if(PrintOption == 1) fprintf(fileout, "%5.2f %6.2f %8.4f %7.2f %10.2f", minimum.phi, minimum.lambda, minimum.HeightAboveGeoid, StartDate.DecimalYear, PrintElement);
                        else printf("%5.2f %6.2f %8.4f %7.2f %10.2f", minimum.phi, minimum.lambda, minimum.HeightAboveGeoid, StartDate.DecimalYear, PrintElement);
                    } else
                    {
                        if(PrintOption == 1) fprintf(fileout, "%5.2f %6.2f %8.4f %7.2f %10.2f", minimum.phi, minimum.lambda, minimum.HeightAboveEllipsoid, StartDate.DecimalYear, PrintElement);
                        else printf("%5.2f %6.2f %8.4f %7.2f %10.2f", minimum.phi, minimum.lambda, minimum.HeightAboveEllipsoid, StartDate.DecimalYear, PrintElement);
                    }
                    if(UncertaintyOption == 1) {
                        if(PrintOption == 1) fprintf(fileout, " %7.2f", ErrorElement);
                        else printf(" %7.2f", ErrorElement);
                    }
                    if(PrintOption == 1) fprintf(fileout, "\n");
                    else printf("\n"); /* Complete line */

                     /**Below can be used for XYZ Printing format (longitude latitude output_data)
                     *  fprintf(fileout, "%5.2f %6.2f %10.4f\n", minimum.lambda, minimum.phi, PrintElement); **/
                        
                } /* year loop */

            } /*Longitude Loop */

        } /* Latitude Loop */

    } /* Altitude Loop */
    
    if(PrintOption == 1){
        if(print_boz_warning_strong){ 
            fprintf(fileout, "%s", BOZ_WARN_TEXT_STRONG);
        } else if (print_boz_warning_weak) {
            fprintf(fileout, "%s", BOZ_WARN_TEXT_WEAK);
        }
        fclose(fileout);
    } else {
        if(print_boz_warning_strong){ 
            printf("%s", BOZ_WARN_TEXT_STRONG);
        } else if (print_boz_warning_weak) {
            printf("%s", BOZ_WARN_TEXT_WEAK);
        }
    }


    MAG_FreeMagneticModelMemory(TimedMagneticModel);
    MAG_FreeLegendreMemory(LegendreFunction);
    MAG_FreeSphVarMemory(SphVariables);

    return TRUE;
} /*MAG_Grid*/

