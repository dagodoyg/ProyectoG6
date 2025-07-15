#include <opencv2/opencv.hpp>
#include <iostream>
#include <dlib/opencv.h>
#include <algorithm>
#include "shape.h"

template <typename image_t>
void dnn_format(cv::Mat & cv_digit, image_t & dlib_digit, const int & D_SIZE){

    int w = std::max(cv_digit.rows,cv_digit.cols);
    cv::Mat background(w*1.3, w*1.3, CV_8UC1, cv::Scalar(0));
           
    cv::Moments m = cv::moments(cv_digit, true);
    int cx = static_cast<int>(m.m10 / m.m00);
    int cy = static_cast<int>(m.m01 / m.m00);
    int x = std::max(0,background.cols/2 - cx);
    int y = std::max(0,background.rows/2 - cy);

    cv::Rect place(x, y, cv_digit.cols, cv_digit.rows);
    cv_digit.copyTo(background(place));

    cv::resize(background, background, cv::Size(D_SIZE,D_SIZE));
    background.convertTo(background, CV_8UC1);

    //cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));
    //cv::dilate(background,background,kernel);

    dlib::assign_image(dlib_digit, dlib::cv_image<unsigned char>(background));
}

template<int D_SIZE>
void crop_nums(std::string ImgPath, std::vector<Shape> & Figs){
    cv::Mat img = cv::imread(ImgPath);
    cv::Mat img_border;

    //Check for empty image input
    if (img.empty()) {
        std::cerr << "Failed loading: " << ImgPath << std::endl;
        return;
    }

    //Preprocessing for border detection
    cv::cvtColor(img,img_border,cv::COLOR_BGR2GRAY);
    cv::threshold(img_border, img_border, 0, 255, cv::THRESH_BINARY_INV +  cv::THRESH_OTSU);
    
    //Get contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_border, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    for (auto c_ii : contours) {
        cv::Rect box = cv::boundingRect(c_ii);
        cv::Mat cv_digit = img_border(box);

        //Check primitive conditions
        if (box.area() < 0.01*img.cols*img.rows) continue;
        if (box.width > 1.5*box.height) continue;

        //Transform to proper format for dlib mnist dnn
        ImageF dlib_digit;
        dnn_format(cv_digit,dlib_digit,D_SIZE);

        //Store only valid data as struct vector
        Figs.emplace_back(box, dlib_digit);
    }

    //Sort by position, row major
    std::sort(Figs.begin(), Figs.end());

}