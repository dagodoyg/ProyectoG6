#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

struct noise_map{
    cv::Mat N_map;
    cv::Mat S_map;

    noise_map() : N_map(48, 48, CV_8UC1) {}
};