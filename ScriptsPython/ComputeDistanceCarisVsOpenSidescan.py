#!/usr/bin/python3

import pprint

from math import pi, cos
from Haversine import computeDistanceGreatCircleHaversine

import csv

class LongitudeLatitude :

    def __init__( self, longitude, latitude ) :

        self.longitude = longitude
        self.latitude = latitude

    # End of def

# End of class


fileCaris = '../test/data/wrecks/scotsman5info/ScotsmanLongitudeLatitudeCaris.csv'

tableCaris = []
dictionaryCaris = {}

with open( fileCaris, 'r' ) as fp :
        
    fileReader = csv.reader( fp , delimiter = ',')
    
    for line in fileReader :

        # Don't put the first line of the file in the list and dictionary
        if line[ 0 ] != 'name' : 

            tableCaris.append( [ line[ 0 ], float( line[ 1 ] ), float( line[ 2 ] ) ] )

            dictionaryCaris[ line[ 0 ] ] = LongitudeLatitude( float( line[ 1 ] ), float( line[ 2 ] ) )
        # End of if

    # End of for

# End of with

pp = pprint.PrettyPrinter(indent=4)

# pp.pprint( tableCaris )
# pp.pprint( dictionaryCaris )


fileOpenSidescan = '../test/data/wrecks/scotsman5info/ScotsmanLongitudeLatitudeOpenSidescan2020-03-18.csv'

tableOpenSidescan = []
dictionaryOpenSidescan = {}

with open( fileOpenSidescan, 'r' ) as fp :
        
    fileReader = csv.reader( fp , delimiter = ',')
    
    for line in fileReader :

        if line[ 0 ] != 'name' : 

            tableOpenSidescan.append( [ line[ 0 ], float( line[ 2 ] ), float( line[ 3 ] ) ] )   

            dictionaryOpenSidescan[ line[ 0 ] ] = LongitudeLatitude( float( line[ 2 ] ), float( line[ 3 ] ) )
        # End of if        

    # End of for

# End of with

# pp.pprint( tableOpenSidescan )
# pp.pprint( dictionaryOpenSidescan )

# Test what happen when key not in dictionary
# print( dictionaryCaris.get( 'JUNK' ).longitude )


allValues = []
greatCircleDistance = []
distanceDifferenceInLatitude = []
distanceDifferenceInLongitude = []

r = 6371000 # Earth radius

for lineCaris in tableCaris :

    # Find this object name in dictionaryOpenSidescan

    longitudeLatitudeOpenSidescan = dictionaryOpenSidescan.get( lineCaris[ 0 ] )

    if longitudeLatitudeOpenSidescan != None :

        # print( lineCaris[ 0 ], "\n",
        #     "Caris:        ", "longitude: ", lineCaris[ 1 ], 
        #         ", latitude: ", lineCaris[ 2 ], "\n"
        #     "OpenSidescan: ",  "longitude: ", longitudeLatitudeOpenSidescan.longitude, 
        #         ", latitude: ", longitudeLatitudeOpenSidescan.latitude, sep='')

        allValues.append( [ lineCaris[ 0 ], lineCaris[ 1 ], lineCaris[ 2 ], 
                    longitudeLatitudeOpenSidescan.longitude, longitudeLatitudeOpenSidescan.latitude ] )

        greatCircleDistance.append( [ lineCaris[ 0 ], 
                    computeDistanceGreatCircleHaversine( 
                        lineCaris[ 1 ], lineCaris[ 2 ], 
                        longitudeLatitudeOpenSidescan.longitude, longitudeLatitudeOpenSidescan.latitude ) ] )
                        
        distanceDifferenceInLatitude.append( [ lineCaris[ 0 ], 
                    2 * pi * r / 360 * abs( lineCaris[ 2 ] -longitudeLatitudeOpenSidescan.latitude ) ] )

        distanceDifferenceInLongitude.append( [ lineCaris[ 0 ], 
                    2 * pi * r / 360 * cos( lineCaris[ 2 ]* pi / 180 ) 
                                            * abs( lineCaris[ 1 ] -longitudeLatitudeOpenSidescan.longitude ) ] )
    # End of if

# End of for line in tableCaris


print( "\n\nObject name, Caris longitude-latitude, OpenSidescan longitude-latitude")
pp.pprint( allValues )

print( "\n\n-----> ASSUMING earth is a sphere of radius = ", r/1000, " km", sep='')


print( "\nDistance on a great circle between points in Caris vs. OpenSidescan, in meters:")
pp.pprint( greatCircleDistance )

print( "\nDistance for the latitude difference, in meters:")
pp.pprint( distanceDifferenceInLatitude )

print( "\nDistance for the longitude difference (at the latitude in Caris), in meters:")
pp.pprint( distanceDifferenceInLongitude )



# Scotsman
# http://patrimoine-culturel.gouv.qc.ca/rpcq/detail.do?methode=consulter&id=210726&type=bien#.XnJUhqYpAUE
# "D'une longueur de près de 25 mètres et d'une largeur de 7 mètres,"

longitudeLatitudeCarisW = dictionaryCaris.get( 'W' )
longitudeLatitudeCarisY = dictionaryCaris.get( 'Y' )

print( "\nDistance on a great circle between W and Y points, Caris:        ", 
            computeDistanceGreatCircleHaversine( 
                    longitudeLatitudeCarisW.longitude, longitudeLatitudeCarisW.latitude, 
                    longitudeLatitudeCarisY.longitude, longitudeLatitudeCarisY.latitude ) , sep='' )

longitudeLatitudeOpenSideScanW = dictionaryOpenSidescan.get( 'W' )
longitudeLatitudeOpenSideScanY = dictionaryOpenSidescan.get( 'Y' )

print( "\nDistance on a great circle between W and Y points, OpenSidescan: ", 
            computeDistanceGreatCircleHaversine( 
                longitudeLatitudeOpenSideScanW.longitude, longitudeLatitudeOpenSideScanW.latitude, 
                longitudeLatitudeOpenSideScanY.longitude, longitudeLatitudeOpenSideScanY.latitude ) , sep='' )




# # Tests of computation of the distance for a degree of latitude
# print( "\nDistance for one degree of latitude, assuming earth is a sphere of radius = ", r/1000, " km:\n",
#                 2 * pi * r / 360, " meters", sep='' )

# # Tests of computation of the distance for a degree of longitude
# # To take into account longitude lines getting closer as going toward the poles
# # Based on area element when computing surface integral of a sphere, 
# # http://math.mit.edu/~jorloff/suppnotes/suppnotes02/v9.pdf

# # To compare values:
# # https://www.thoughtco.com/degree-of-latitude-and-longitude-distance-4070616
# # 111.321 kilometers at equator
# # 85 kilometers at 40 degrees north or south

# lat = 0
# print( "\nDistance for one degree of longitude, assuming earth is a sphere of radius = ", r/1000, " km:\n",
#     "at latitude of ", lat, " degrees: ",
#                 2 * pi * r / 360 * cos( lat * pi / 180 ), " meters", sep='' )

# lat = 40
# print( "\nDistance for one degree of longitude, assuming earth is a sphere of radius = ", r/1000, " km:\n",
#     "at latitude of ", lat, " degrees: ",
#                 2 * pi * r / 360 * cos( lat * pi / 180 ), " meters", sep='' )

# lat = 90
# print( "\nDistance for one degree of longitude, assuming earth is a sphere of radius = ", r/1000, " km:\n",
#     "at latitude of ", lat, " degrees: ",
#                 2 * pi * r / 360 * cos( lat * pi / 180 ), " meters", sep='' )




