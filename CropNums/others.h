#include <opencv2/opencv.hpp>
#include <iostream>

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat);
void contour_detector(cv::Mat img_border, std::vector<std::vector<cv::Point>> contours);

template <typename image_t>
void svm_format(cv::Mat & cv_digit, image_t & dlib_digit, const int & D_SIZE){
    cv::resize(cv_digit,cv_digit,cv::Size(D_SIZE,D_SIZE));
    cv::bitwise_not(cv_digit, cv_digit); //Black letter over white background
    cv::transpose(cv_digit,cv_digit);  //Transpose, cv::Mat is row major, dlib::matrix is column major
    cv_digit.convertTo(cv_digit, CV_64F, 1.0/255.0); //Convert to double for dlib

    if (!cv_digit.isContinuous()) cv_digit = cv_digit.clone() ; //Force contiguity

    std::memcpy(dlib_digit.begin(), cv_digit.ptr<double>(), D_SIZE*D_SIZE*sizeof(double)); //Copy mem to dlib_digit
}