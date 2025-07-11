#include <string>
#include <valarray>
#include <cmath>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <dlib/matrix.h>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>

template <typename image_t>
void svm_format(cv::Mat & cv_digit, image_t & dlib_digit, const int & D_SIZE){
    cv::resize(cv_digit,cv_digit,cv::Size(D_SIZE,D_SIZE));
    cv::bitwise_not(cv_digit, cv_digit); //Black letter over white background
    //cv::transpose(cv_digit,cv_digit);  //Transpose, cv::Mat is row major, dlib::matrix is column major
    cv_digit.convertTo(cv_digit, CV_8UC1); 

    if (!cv_digit.isContinuous()) cv_digit = cv_digit.clone() ; //Force contiguity

    dlib::assign_image(dlib_digit, dlib::cv_image<unsigned char>(cv_digit));
}

int main(void){
    using ImageF = dlib::matrix<unsigned char>;
    int N = 6000;
    int D_SIZE = 28;
    std::vector<cv::Mat> cv_candela(N);
    std::vector<ImageF> dlib_candela(N);
    std::vector<long> candela_labels(N, -1);

    for (int idx{0}; idx < N/2; idx++) {
        cv_candela[idx] = cv::Mat::zeros(D_SIZE, D_SIZE, CV_8UC1);
        cv::randu(cv_candela[idx], 0, 255);
        svm_format(cv_candela[idx],dlib_candela[idx],D_SIZE);
    }
    for (int idx{N/2}; idx < N; idx++) {
        cv_candela[idx] = cv::Mat::zeros(D_SIZE, D_SIZE, CV_8UC1);
        cv::randn(cv_candela[idx], 128, 32);
        svm_format(cv_candela[idx],dlib_candela[idx],D_SIZE);
    }
    
    return 0;
}