/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#include "GeomagnetismHeader.h"
/*#include "GeomagnetismLibrary.c"*/
#include "EGM9615.h"

/*---------------------------------------------------------------------------*/

/* 
WMM Point Calculation Program.

The Geomagnetism Library is used to make a command prompt program. The program prompts
the user to enter a location, performs the computations and prints the results to the
standard output. The program expects the files GeomagnetismLibrary.c, GeomagnetismHeader.h,
WMM.COF and EGM9615.h to be in the same directory. 

Manoj.C.Nair@Noaa.Gov
April 21, 2011

 */
char MAG_GeomagIntroduction_WMM(MAGtype_MagneticModel *MagneticModel, char *VersionDate, char *ModelDate);


int main()
{
    MAGtype_MagneticModel * MagneticModels[1], *TimedMagneticModel;
    MAGtype_Ellipsoid Ellip;
    MAGtype_CoordSpherical CoordSpherical;
    MAGtype_CoordGeodetic CoordGeodetic;
    MAGtype_Date UserDate;
    MAGtype_GeoMagneticElements GeoMagneticElements, Errors;
    MAGtype_Geoid Geoid;
    char ans[20], b;
    char filename[] = "WMM.COF";
    char VersionDate[12];
    int NumTerms, Flag = 1, nMax = 0;
    int epochs = 1;
    /* Memory allocation */

    strncpy(VersionDate, VERSIONDATE_LARGE + 39, 11);
    VersionDate[11] = '\0';
    if(!MAG_robustReadMagModels(filename, &MagneticModels, epochs)) {
        printf("\n WMM.COF not found.  Press enter to exit... \n ");
        fgets(ans, 20, stdin);
        return 1;
    }
    if(nMax < MagneticModels[0]->nMax) nMax = MagneticModels[0]->nMax;
    NumTerms = ((nMax + 1) * (nMax + 2) / 2);
    TimedMagneticModel = MAG_AllocateModelMemory(NumTerms); /* For storing the time modified WMM Model parameters */
    if(MagneticModels[0] == NULL || TimedMagneticModel == NULL)
    {
        MAG_Error(2);
    }
    MAG_SetDefaults(&Ellip, &Geoid); /* Set default values and constants */
    /* Check for Geographic Poles */



    /* Set EGM96 Geoid parameters */
    Geoid.GeoidHeightBuffer = GeoidHeightBuffer;
    Geoid.Geoid_Initialized = 1;
    /* Set EGM96 Geoid parameters END */
    b = MAG_GeomagIntroduction_WMM(MagneticModels[0], VersionDate, MODEL_RELEASE_DATE);
    while(Flag == 1 && b != 'x')
    {
        if(MAG_GetUserInput(MagneticModels[0], &Geoid, &CoordGeodetic, &UserDate) == 1) /*Get User Input */
        {
            MAG_GeodeticToSpherical(Ellip, CoordGeodetic, &CoordSpherical); /*Convert from geodetic to Spherical Equations: 17-18, WMM Technical report*/
            MAG_TimelyModifyMagneticModel(UserDate, MagneticModels[0], TimedMagneticModel); /* Time adjust the coefficients, Equation 19, WMM Technical report */
            MAG_Geomag(Ellip, CoordSpherical, CoordGeodetic, TimedMagneticModel, &GeoMagneticElements); /* Computes the geoMagnetic field elements and their time change*/
            MAG_CalculateGridVariation(CoordGeodetic, &GeoMagneticElements);
            MAG_WMMErrorCalc(GeoMagneticElements.H, &Errors);
            MAG_PrintUserDataWithUncertainty(GeoMagneticElements, Errors, CoordGeodetic, UserDate, TimedMagneticModel, &Geoid); /* Print the results */
        }
        do {
            printf("\n\n Do you need more point data ? (y or n) \n ");
        } while (NULL == fgets(ans, 20, stdin));
        switch(ans[0]) {
            case 'Y':
            case 'y':
                Flag = 1;
                break;
            case 'N':
            case 'n':
                Flag = 0;
                break;
            default:
                Flag = 0;
                break;
        }

    }


    MAG_FreeMagneticModelMemory(TimedMagneticModel);
    MAG_FreeMagneticModelMemory(MagneticModels[0]);

    return 0;
}

char MAG_GeomagIntroduction_WMM(MAGtype_MagneticModel *MagneticModel, char *VersionDate, char *ModelDate)
/*Prints the introduction to the Geomagnetic program.  It needs the Magnetic model for the epoch.

 * INPUT  MagneticModel		: MAGtype_MagneticModel With Model epoch 	(input)
  OUTPUT ans   (char)  user selection
  CALLS : none
 */
{
    char help = 'h';
    char ans;
    printf("\n\n Welcome to the World Magnetic Model (WMM) %d C-Program\n\n", (int) MagneticModel->epoch);
    printf("              --- Model Release Date: %s ---\n", ModelDate);
    printf("            --- Software Release Date: %s ---\n\n", VersionDate);
    printf("\n This program estimates the strength and direction of ");
    printf("\n Earth's main Magnetic field for a given point/area.");
    while(help != 'c' && help != 'C')
    {
        printf("\n Enter h for help and contact information or c to continue.");
        printf("\n >");
        scanf("%c%*[^\n]", &help);
        getchar();

        if((help == 'h') || (help == 'H'))
        {
            printf("\n Help information ");

            printf("\n The World Magnetic Model (WMM) for %d", (int) MagneticModel->epoch);
            printf("\n is a model of Earth's main magnetic field. The WMM");
            printf("\n is recomputed every five (5) years, in years divisible by ");
            printf("\n five (i.e. 2010, 2015). See the contact information below");
            printf("\n to obtain more information on the WMM and associated software.");
            printf("\n ");
            printf("\n Input required is the location in geodetic latitude and");
            printf("\n longitude (positive for northern latitudes and eastern ");
            printf("\n longitudes), geodetic altitude in meters, and the date of ");
            printf("\n interest in years.");

            printf("\n\n\n The program computes the estimated Magnetic Declination");
            printf("\n (Decl) which is sometimes called MagneticVAR, Inclination (Incl), Total");
            printf("\n Intensity (F or TI), Horizontal Intensity (H or HI), Vertical");
            printf("\n Intensity (Z), and Grid Variation (GV). Declination and Grid");
            printf("\n Variation are measured in units of degrees and are considered");
            printf("\n positive when east or north. Inclination is measured in units");
            printf("\n of degrees and is considered positive when pointing down (into");
            printf("\n the Earth). The WMM is referenced to the WGS-84 ellipsoid and");
            printf("\n is valid for 5 years after the base epoch. Uncertainties for the");
            printf("\n WMM are one standard deviation uncertainties averaged over the globe.");
            printf("\n We represent the uncertainty as constant values in Incl, F, H, X,");
            printf("\n Y, and Z. Uncertainty in Declination varies depending on the strength");
            printf("\n of the horizontal field. For more information see the WMM Technical");
            printf("\n Report.");
            
            printf("\n\n\n It is very important to note that a degree and order 12 model,");
            printf("\n such as WMM, describes only the long wavelength spatial Magnetic ");
            printf("\n fluctuations due to Earth's core. Not included in the WMM series");
            printf("\n models are intermediate and short wavelength spatial fluctuations ");
            printf("\n that originate in Earth's mantle and crust. Consequently, isolated");
            printf("\n angular errors at various positions on the surface (primarily over");
            printf("\n land, along continental margins and over oceanic sea-mounts, ridges and");
            printf("\n trenches) of several degrees may be expected. Also not included in");
            printf("\n the model are temporal fluctuations of magnetospheric and ionospheric");
            printf("\n origin. On the days during and immediately following magnetic storms,");
            printf("\n temporal fluctuations can cause substantial deviations of the Geomagnetic");
            printf("\n field from model values. If the required declination accuracy is");
            printf("\n more stringent than the WMM series of models provide, the user is");
            printf("\n advised to request special (regional or local) surveys be performed");
            printf("\n and models prepared. The World Magnetic Model is a joint product of");
            printf("\n the United States' National Geospatial-Intelligence Agency (NGA) and");
            printf("\n the United Kingdom's Defence Geographic Centre (DGC). The WMM was");
            printf("\n developed jointly by the National Centers for Environmental Information");
            printf("\n (NCEI, Boulder CO, USA) and the British Geological Survey (BGS, ");
            printf("\n Edinburgh, Scotland).");

            printf("\n\n\n Contact Information");

            printf("\n  Software and Model Support");
            printf("\n	National Centers for Environmental Information");
            printf("\n	NOAA E/NE42");
            printf("\n	325 Broadway");
            printf("\n	Boulder, CO 80305 USA");
            printf("\n	Attn: Manoj Nair or Arnaud Chulliat");
            printf("\n	Phone:  (303) 497-4642 or -6522"); 
            printf("\n	Email:  Geomag.Models@noaa.gov \n");
        }
    }
    ans = help;
    return ans;
} /*MAG_GeomagIntroduction_WMM*/

