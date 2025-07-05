#include "opencv_process.h"

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat){
    cv::cvtColor(InputMat,OutMat,cv::COLOR_BGR2GRAY);
    cv::adaptiveThreshold(OutMat,OutMat,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY_INV,21,10);
    //cv::GaussianBlur(OutMat,OutMat,cv::Size(21,21),3,0);
    //cv::Canny(OutMat, OutMat, 25, 75);

}