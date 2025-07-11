#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <dlib/image_io.h>
#include <dlib/pixel.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

struct Shape {
    int x1, y1, x2, y2;  //Positions of lt and br corners
    dlib::matrix<double, 784, 1> digit;

    bool operator<(const Shape & a) const{  //overwrite the operator for sorting
        if ( std::abs(y1 - a.y1) < 0.1*( y2 - y1) ) return x1 < a.x1;
        return y1 < a.y1;
    }

    Shape(cv::Rect BOX_, dlib::matrix<double, 784, 1> & DIGIT_)  //constructor
        : x1(BOX_.x), y1(BOX_.y),
          x2(x1 + BOX_.width), y2(y1 + BOX_.height)
        {
            dlib::assign_image(digit,DIGIT_);
        }
};
