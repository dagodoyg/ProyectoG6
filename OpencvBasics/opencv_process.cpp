#include "opencv_process.h"

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat){
    cv::cvtColor(InputMat,OutMat,cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(OutMat,OutMat,cv::Size(21,21),3,0);
    cv::Canny(OutMat, OutMat, 25, 75);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::dilate(OutMat,OutMat,kernel);
}