
#include "parameterscvCreateTrainingSamples.h"


ParameterscvCreateTrainingSamples::ParameterscvCreateTrainingSamples()

    : num( 10 ),
      bgcolor( 0 ),
      bgthreshold( 80 ),
      invert( 0 ),
      maxintensitydev( 40 ),
      maxxangle( 1.1 ),
      maxyangle( 1.1 ),
      maxzangle( 0.5 ),
//      showsamples( 0 ),
      width( 24 ),
      height( 24 ),
      rngseed( 12345 ),

      useOriginalObjectImageWidthAsBasis( true ),
      useOriginalObjectImageHeightAsBasis( true ),

      nbPixelsChangeFromObjectImageWidth( 0 ),
      nbPixelsChangeFromObjectImageHeight( 0 )
{

}
