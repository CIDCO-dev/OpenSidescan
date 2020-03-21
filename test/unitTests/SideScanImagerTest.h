#ifndef SIDESCANIMAGERTEST_H
#define SIDESCANIMAGERTEST_H

#include "catch.hpp"
#include <string>
#include "../../src/OpenSidescan/sidescanimager.h"


TEST_CASE("Test SideScanImager") {

    int a = 2;




    class TestParser : DatagramParser {
    public:
        TestParser(DatagramEventHandler & processor) : DatagramParser(processor) {};

        void parse(std::string & filename) {

            //ping details
            uint64_t pingTimestamp = 5;
            std::vector<double> samples;
            double sample1 = 1.5;
            double sample2 = 2.5;
            samples.push_back(sample1);
            samples.push_back(sample2);

            SidescanPing * ping;
            ping->setTimestamp(pingTimestamp);
            ping->setChannelNumber(0);
            ping->setSamples(samples);
            processor.processSidescanData(ping);

            //attitude before
            uint64_t attitudeBeforeTimestamp = 4;
            double rollBefore = 1.4;
            double pitchBefore = 2.4;
            double headingBefore = 44.0;
            processor.processAttitude(attitudeBeforeTimestamp, headingBefore, pitchBefore, rollBefore);

            //attitude after
            uint64_t attitudeAfterTimestamp = 6;
            double rollAfter = 1.6;
            double pitchAfter = 2.6;
            double headingAfter = 46.0;
            processor.processAttitude(attitudeAfterTimestamp, headingAfter, pitchAfter, rollAfter);

            //position before
            uint64_t positionBeforeTimestamp = 4;
            double latitudeBefore = 48.4;
            double longitudeBefore = -68.4;
            double heightBefore = 9.0;
            processor.processPosition(positionBeforeTimestamp, longitudeBefore, latitudeBefore, heightBefore);

            //position After
            uint64_t positionAfterTimestamp = 6;
            double latitudeAfter = 48.6;
            double longitudeAfter = -68.6;
            double heightAfter = 11.0;
            processor.processPosition(positionAfterTimestamp, longitudeAfter, latitudeAfter, heightAfter);
        }

        std::string getName(int tag) {
            if(tag == 0) {
                //tag isn't used in this test case
            }
            return "no name";
        }
    };

    SidescanImager imager;
    TestParser * parser = new TestParser(imager);
    std::string f1 = "I am, You are, We remain ... Unidentified!";
    parser->parse(f1);

    std::string f2 = "Don't put no labels on me!";
    SidescanFile * ssFile = imager.generate(f2);

    std::vector<SidescanImage*> images = ssFile->getImages();


    SidescanImage* image = images[0];

    SidescanPing* ping = image->getPings()[0];

    double testRoll = 1.5;

    double eps = 1e-9;
    REQUIRE(std::abs(ping->getAttitude()->getRoll() - testRoll) < eps);
}

#endif // SIDESCANIMAGERTEST_H
