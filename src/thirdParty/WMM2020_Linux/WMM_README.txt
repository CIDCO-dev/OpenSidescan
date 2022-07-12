World Magnetic Model Software and support documents
=======================================================

Files


Sublibrary Files
================
GeomagnetismLibrary.c              WMM Subroutine library, C functions
GeomagnetismHeader.h               WMM Subroutine library, C header file 

Main Programs
===============
wmm_point.c                 Command prompt version for single point computation
wmm_grid.c                  Grid, profile and time series computation, C main function
wmm_file.c                  C program which takes a coordinate file as input with WMM ISO formatted coefficients as input

Data Files
===============
WMM.COF                 WMM Coefficients file


Supporting Documents
=====================
Geomagnetism_Library_Software_Manual.pdf      Technical documents of the WMM Model and softwares


Excecutables
============
wmm_point.exe               Command Prompt program for single point 
wmm_grid.exe                Grid, time series or profile
wmm_file.exe                File processing 


Test Files
===============
WMM2020testvalues.pdf           Test values for WMM2020
WMM2020_TEST_VALUES.txt         Text test values for WMM2020
sample_input_file.txt           Sample input file for program wmm_file.exe 
sample_output_file.txt          Sample output file for program  wmm_file.exe



Compiling with gcc
===================
There is a Makefile in the source directory for building with gcc
gcc inputfile [dependencies] -o outputfile -lm
For example, the wmm_point.c can be compiled as
gcc wmm_point.c GeomagnetismLibrary.c -o wmm_point -lm
Note: The library files GeomagnetismLibrary.c and GeomagnetismHeader.h should reside in the same directory for compiling.

Executing the file processing program
=====================================
The file processing program accepts this syntax:
wmm_file.exe f INPUT_FILE.txt OUTPUT_FILE.txt

Additionally to have uncertainty values appended to the output file add the "e" flag or "Errors".  For example:

wmm_file.exe f e INPUT_FILE.txt OUTPUT_FILE.txt

or

wmm_file.exe f --Errors INPUT_FILE.txt OUTPUT_FILE.txt


Model Software Support
======================

*  National Centers for Environmental Information (NCEI)
*  E/NE42 325 Broadway
*  Boulder, CO 80303 USA
*  Attn: Manoj Nair or Arnaud Chulliat
*  Phone:  (303) 497-4642 or -6522
*  Email:  geomag.models@noaa.gov
For more details about the World Magnetic Model visit 
http://www.ngdc.noaa.gov/geomag/WMM/DoDWMM.shtml
 




       
