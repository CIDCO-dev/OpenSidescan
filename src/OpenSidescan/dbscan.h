#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

/*
 * The MIT License (MIT)

 * Copyright (c) 2018 glm,swyoon

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

const int NOISE = -2;
const int NOT_CLASSIFIED = -1;

/**
 * @brief The DBSCAN class
 */

//This uses a multiplexed version of the KEyPoint class where the octave member is used for pointCount

class DBSCAN {
public:
    //int n
    int minPts;
    double eps;
    std::vector<cv::KeyPoint> & points;
    int size;
    std::vector<std::vector<int> > adjPoints;
    std::vector<bool> visited;
    std::vector<std::vector<int> > cluster;
    int clusterIdx;

    DBSCAN(double eps, int minPts, std::vector<cv::KeyPoint> & points) : points(points) {
        //this->n = n;
        this->eps = eps;
        this->minPts = minPts;
        this->size = (int)points.size();
        adjPoints.resize(size);
        this->clusterIdx=-1;
    }

    void run () {
        checkNearPoints();

        for(int i=0;i<size;i++) {
            if(points[i].class_id != NOT_CLASSIFIED) continue;

            if(isCoreObject(i)) {
                dfs(i, ++clusterIdx);
            } else {
                points[i].class_id = NOISE;
            }
        }

        cluster.resize(clusterIdx+1);
        for(int i=0;i<size;i++) {
            if(points[i].class_id != NOISE) {
                cluster[points[i].class_id].push_back(i);
            }
        }
    }

    void dfs (int now, int c) {
        points[now].class_id = c;
        if(!isCoreObject(now)) return;

        for(auto&next:adjPoints[now]) {
            if(points[next].class_id != NOT_CLASSIFIED) continue;
            dfs(next, c);
        }
    }

    void checkNearPoints() {
        for(int i=0;i<size;i++) {
            for(int j=0;j<size;j++) {
                if(i==j) continue;
                if(getDistance(points[i],points[j]) <= eps) {
                    points[i].octave++;
                    adjPoints[i].push_back(j);
                }
            }
        }
    }
    // is idx'th point core object?
    bool isCoreObject(int idx) {
        return points[idx].octave >= minPts;
    }

    std::vector<std::vector<int> > getCluster() {
        return cluster;
    }

    double getDistance(cv::KeyPoint & p1,cv::KeyPoint & p2){
        return sqrt(pow(p1.pt.x-p2.pt.x,2) + pow(p1.pt.y-p2.pt.y,2));
    }
};



#endif // DBSCAN_H
