#ifndef PROJECTTEST_H
#define PROJECTTEST_H

#include "catch.hpp"
#include "../../src/OpenSidescan/project.h"

TEST_CASE("Test Project") {

    Project * project = new Project();

    Eigen::Vector3d leverArm;
    leverArm << 1.0, 2.0, 3.0;

    project->setAntenna2TowPointLeverArm(leverArm);

    double eps = 1e-9;
    REQUIRE(std::abs(project->getAntenna2TowPointLeverArm()[0] - leverArm[0]) < eps);
    REQUIRE(std::abs(project->getAntenna2TowPointLeverArm()[1] - leverArm[1]) < eps);
    REQUIRE(std::abs(project->getAntenna2TowPointLeverArm()[2] - leverArm[2]) < eps);

}

#endif // PROJECTTEST_H
