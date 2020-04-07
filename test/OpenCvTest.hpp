#ifndef OPENCVTEST_HPP
#define OPENCVTEST_HPP

#include "catch.hpp"
#include "opencv2/opencv.hpp"

TEST_CASE("Test opencv") {
    
    cv::Mat * I = new cv::Mat();
    
    std::cout << "matrix created" << std::endl;
    
    REQUIRE(I);
    
}

#endif /* OPENCVTEST_HPP */

