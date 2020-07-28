/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   DependencyTest.hpp
 * Author: Jordan McManus
 */

#ifndef DEPENDENCYTEST_HPP
#define DEPENDENCYTEST_HPP

#include "catch.hpp"
#include "opencv2/opencv.hpp"
#include <Eigen>

TEST_CASE("Test Eigen") {
    Eigen::Vector3d v(1.0, 2.0, 3.0);
    
    double eps = 1e-9;
    REQUIRE(std::abs(v(0) - 1.0) < eps);
    REQUIRE(std::abs(v(1) - 2.0) < eps);
    REQUIRE(std::abs(v(2) - 3.0) < eps);
    
}

TEST_CASE("Test opencv") {
    
    cv::Mat * I = new cv::Mat();
    
    REQUIRE(I);
    
}

#endif /* DEPENDENCYTEST_HPP */

