#ifndef PARAMETERSCVCREATETRAININGSAMPLES
#define PARAMETERSCVCREATETRAININGSAMPLES





class ParameterscvCreateTrainingSamples
{


public:
    ParameterscvCreateTrainingSamples();

    int num;                // Number of positive samples to generate per object image
    int bgcolor;
    int bgthreshold;
    int invert;             // Default value of 0 for don't invert, 1 for invert, CV_RANDOM_INVERT for invert randomly
    int maxintensitydev;
    double maxxangle;       // Radians
    double maxyangle;       // Radians
    double maxzangle;       // Radians
//    int showsamples;
    int width;
    int height;
    int rngseed;

    bool useOriginalObjectImageWidthAsBasis; 		// Default value of true
    bool useOriginalObjectImageHeightAsBasis;		// Default value of true

    int nbPixelsChangeFromObjectImageWidth;     // Used when useOriginalObjectImageWidthAsBasis == true
    int nbPixelsChangeFromObjectImageHeight;    // Used when useOriginalObjectImageWidthAsBasis == true

};




#endif // PARAMETERSCVCREATETRAININGSAMPLES
