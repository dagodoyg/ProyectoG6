#include <opencv2/opencv.hpp>
#include <iostream>
#include "shape.h"

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat);
void contour_detector(const cv::Mat & img_border, std::vector<std::vector<cv::Point>> & contours);

template <typename image_t>
void svm_format(cv::Mat & cv_digit, image_t & dlib_digit, const int & D_SIZE){
    cv::resize(cv_digit,cv_digit,cv::Size(D_SIZE,D_SIZE));
    cv::bitwise_not(cv_digit, cv_digit); //Black letter over white background
    cv::transpose(cv_digit,cv_digit);  //Transpose, cv::Mat is row major, dlib::matrix is column major
    cv_digit.convertTo(cv_digit, CV_64F); //Convert to double for dlib

    if (!cv_digit.isContinuous()) cv_digit = cv_digit.clone() ; //Force contiguity

    std::memcpy(dlib_digit.begin(), cv_digit.ptr<double>(), D_SIZE*D_SIZE*sizeof(double)); //Copy mem to dlib_digit
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
    preprocessing(img, img_border);
    
    //Get contours
    std::vector<std::vector<cv::Point>> contours;
    contour_detector(img_border, contours);
    std::cout << contours.size() << std::endl;
    
    int idx{0};
    for (int ii{0}; ii<contours.size(); ii++) {

        //Getting the digit as cv::Mat
        std::vector<cv::Point> c_ii = contours[ii];
        cv::Rect box = cv::boundingRect(c_ii);
        cv::Mat cv_digit = img_border(box);

        //Check primitive conditions
        if (box.width > 1.5*box.height) continue;
        if (box.area() < 0.01*img.cols*img.rows) continue;

        //Transform to proper format for dlib svm
        dlib::matrix<double, D_SIZE*D_SIZE, 1> dlib_digit;
        svm_format(cv_digit,dlib_digit,D_SIZE);

        //Store only valid data as struct vector
        Figs.emplace_back(box, dlib_digit);
    }

    //Sort by position, row major
    std::sort(Figs.begin(), Figs.end());
    std::cout << Figs.size() << std::endl;

}