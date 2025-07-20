#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <dlib/image_io.h>
#include <dlib/pixel.h>
#include <dlib/image_processing.h>

using ImageF = dlib::matrix<unsigned char>;

struct Shape {
    int x1, y1, x2, y2;  //Positions of lt and br corners
    ImageF digit;

    bool operator<(const Shape & a) const{  //overwrite the operator for sorting
        if ( std::abs(y1 - a.y1) < 0.2*( y2 - y1) ) return x1 < a.x1;
        return y1 < a.y1;
    }

    Shape(cv::Rect BOX_, ImageF & DIGIT_)  //constructor
        : x1(BOX_.x), y1(BOX_.y),
          x2(x1 + BOX_.width), y2(y1 + BOX_.height),
          digit(DIGIT_)
        {}
};
