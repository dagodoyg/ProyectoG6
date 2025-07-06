#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

struct shape{
    cv::Rect box;
    cv::Mat digit;
    cv::Mat S_map;
    double S;

    shape(cv::Rect box_, cv::Mat digit_, cv::Mat S_map_, double S_)
        :
            box(box_),
            digit(digit_),
            S_map(S_map_),
            S(S_)
            {}
};