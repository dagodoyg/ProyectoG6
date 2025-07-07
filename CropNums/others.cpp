#include "others.h"

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat){
    cv::bilateralFilter(InputMat,OutMat,11,17,17);
    cv::cvtColor(OutMat,OutMat,cv::COLOR_RGB2GRAY);
    cv::threshold(OutMat, OutMat, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
}
