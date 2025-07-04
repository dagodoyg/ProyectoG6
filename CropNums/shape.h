#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

struct shape{
    std::vector<cv::Point> border;
    cv::Rect box;
    cv::Mat digit;

    shape(const std::vector<cv::Point> & contour, const cv::Mat & img)
        :   border(contour),
            box(cv::boundingRect(contour)),
            digit(img(box))
            {}
};