#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <dlib/image_io.h>
#include <dlib/pixel.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

struct shape {
    int x1, y1, x2, y2;  //Positions of lt and br corners
    dlib::array2d<unsigned char> digit;

    bool operator<(const shape& a) const{
        return x1 < a.x1;
    }

    shape(cv::Rect BOX_, dlib::array2d<unsigned char> & DIGIT_)
        : x1(BOX_.x), y1(BOX_.y),
          x2(x1 + BOX_.width), y2(y1 + BOX_.height)
        {
            dlib::assign_image(digit,DIGIT_);
        }
};
