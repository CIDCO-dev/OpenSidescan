#!/usr/bin/python3

from math import pi, cos, sin, asin, sqrt

# https://en.wikipedia.org/wiki/Haversine_formula
def computeDistanceGreatCircleHaversine( longitude1, latitude1, longitude2, latitude2 ) :

    # From https://en.wikipedia.org/wiki/Haversine_formula

    # What is inside the square root is called "h" on Wikipedia
    # From https://en.wikipedia.org/wiki/Haversine_formula
    # "When using these formulae, one must ensure that h does not exceed 1 due to a 
    # floating point error (d is only real for h from 0 to 1). h only approaches 1 for 
    # antipodal points (on opposite sides of the sphere)—in this region, relatively 
    # large numerical errors tend to arise in the formula when finite precision is used. 
    # Because d is then large (approaching πR, half the circumference) a small error is 
    # often not a major concern in this unusual case (although there are other great-circle 
    # distance formulas that avoid this problem).

    longitude1  *= pi / 180.0
    latitude1  *= pi / 180.0
    longitude2  *= pi / 180.0
    latitude2  *= pi / 180.0


    sin2LatDiffOver2 = ( sin( ( latitude2 - latitude1 ) / 2 ) ) ** 2

    sin2LongDiffOver2 =  ( sin( ( longitude2 - longitude1 ) / 2 ) ) ** 2

    insideSqrt = sin2LatDiffOver2 + cos( latitude1 ) * cos( latitude2 ) * sin2LongDiffOver2

    # print( "   insideSqrt: ", insideSqrt, sep='' )

    if insideSqrt > 1.0 :
        print( "\n\n--- computeDistanceGreatCircleHaversine(): the calculation inside the square root is ", 
            insideSqrt, '.\n',
            "This must be a floating point error as it should be between 0 and 1 inclusively.\n\n" )

    r = 6371000
    # r = 6378000

    return 2 * r * asin( sqrt( insideSqrt ) )

# End of def

# https://en.wikipedia.org/wiki/Great-circle_distance
# From chord length
def computeDistanceGreatCircleFromChordLength( longitude1, latitude1, longitude2, latitude2 ) :

    longitude1  *= pi / 180.0
    latitude1  *= pi / 180.0
    longitude2  *= pi / 180.0
    latitude2  *= pi / 180.0

    dx = cos( latitude2 ) * cos( longitude2 ) - cos( latitude1 ) * cos( longitude1 )
    dy = cos( latitude2 ) * sin( longitude2 ) - cos( latitude1 ) * sin( longitude1 )
    dz = sin( latitude2 ) - sin( latitude1 )

    # Chord length
    C = sqrt( dx**2 + dy**2 + dz**2 )

    # Central angle
    centralAngle = 2 * asin( C / 2 )

    r = 6371000
    # r = 6378000

    return centralAngle * r


# End of def

