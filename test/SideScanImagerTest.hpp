#ifndef SIDESCANIMAGERTEST_H
#define SIDESCANIMAGERTEST_H

#include "catch.hpp"
#include <string>
#include "../src/OpenSidescan/sidescan/sidescanimager.h"
#include "../src/OpenSidescan/sidescan/sidescanfile.h"
#include "../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.hpp"


TEST_CASE("Test normal image") {

    std::string sidescanFileName = "../data/wrecks/scotsman1.xtf";

    SidescanImager imager;
    DatagramParser * parser = DatagramParserFactory::build(sidescanFileName,imager);
    parser->parse(sidescanFileName);

    Eigen::Vector3d leverArm(0,0,0);

    SidescanFile * file = imager.generate(sidescanFileName, leverArm);

    REQUIRE(file);

    unsigned int numChannels = 2;

    REQUIRE(file->getImages().size() == numChannels);

    REQUIRE(file->getImages()[0]->getPings().size() > 0);
    REQUIRE(file->getImages()[1]->getPings().size() > 0);

    delete parser;
    delete file;
}

TEST_CASE("Test image with no position data") {

    std::string sidescanFileName = "../data/wrecks/plane1.xtf";

    SidescanImager imager;
    DatagramParser * parser = DatagramParserFactory::build(sidescanFileName,imager);
    parser->parse(sidescanFileName);

    Eigen::Vector3d leverArm(0,0,0);

    SidescanFile * file = imager.generate(sidescanFileName, leverArm);

    REQUIRE(file);

    unsigned int numChannels = 3;
    REQUIRE(file->getImages().size() == numChannels);

    REQUIRE(file->getImages()[0]->getPings().size() > 0);
    REQUIRE(file->getImages()[1]->getPings().size() > 0);
    REQUIRE(file->getImages()[2]->getPings().size() > 0);

    delete parser;
    delete file;
}

TEST_CASE("Test SideScanImager") {
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

            SidescanPing * ping = new SidescanPing();
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

            std::map<std::string,std::string> * properties = new std::map<std::string,std::string>();
            processor.processChannelProperties(0,"",0,properties);
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

    Eigen::Vector3d leverArm(0,0,0);

    std::string f2 = "Don't put no labels on me!";
    SidescanFile * ssFile = imager.generate(f2, leverArm);

    std::vector<SidescanImage*> images = ssFile->getImages();


    SidescanImage* image = images[0];

    SidescanPing* ping = image->getPings()[0];

    double testRoll = 1.5;
    double testPitch = 2.5;
    double testHeading = 45.0;

    double eps = 1e-9;
    REQUIRE(std::abs(ping->getAttitude()->getRoll() - testRoll) < eps);
    REQUIRE(std::abs(ping->getAttitude()->getPitch() - testPitch) < eps);
    REQUIRE(std::abs(ping->getAttitude()->getHeading() - testHeading) < eps);


    double testLatitude = 48.5;
    double testLongitude = -68.5;
    double testHeight = 10.0;

    REQUIRE(std::abs(ping->getPosition()->getLatitude() - testLatitude) < eps);
    REQUIRE(std::abs(ping->getPosition()->getLongitude() - testLongitude) < eps);
    REQUIRE(std::abs(ping->getPosition()->getEllipsoidalHeight() - testHeight) < eps);

    delete parser;
    delete ssFile;
}


#endif // SIDESCANIMAGERTEST_H
