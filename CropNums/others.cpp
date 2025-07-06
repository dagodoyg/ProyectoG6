#include "others.h"

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat){
    cv::bilateralFilter(InputMat,OutMat,11,17,17);
    cv::cvtColor(OutMat,OutMat,cv::COLOR_RGB2GRAY);
    cv::adaptiveThreshold(OutMat,OutMat,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY_INV,21,10);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(OutMat,OutMat,kernel);
}

double entropy_b32(const cv::Mat & InputMat, cv::Mat & S_map){
    if (InputMat.cols!=48 || InputMat.rows!=48){ std::cerr << "image must be 64x64" << std::endl; return 0;}

    cv::Mat Input_float;
    InputMat.convertTo(Input_float, CV_32F, 1.0 / 255.0);
    
    cv::Mat p0, p1, log_p0, log_p1, S_map_raw;
    cv::blur(Input_float,p1,cv::Size(3,3));

    p0 = 1.0 - p1;

    cv::log(p0, log_p0);
    cv::log(p1, log_p1);
    cv::patchNaNs(log_p0,0);
    cv::patchNaNs(log_p1,0);

    S_map_raw = -p0.mul(log_p0) - p1.mul(log_p1);
    cv::imshow("raw S", S_map_raw);
    cv::waitKey(0);
    S_map = cv::Mat::zeros(cv::Size(16,16),CV_32F);
    double S{0};
    double h{0};

    for (int ii{0}; ii < 16; ii++){
        for (int jj{0}; jj < 16; jj++){
            h = S_map_raw.at<float>(2 + 3*ii, 2 + 3*jj);
            S_map.at<float>(ii,jj) = h;
            S += h;
        }
    }

    return S/(16*16);
}