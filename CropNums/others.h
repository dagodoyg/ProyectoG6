#include <opencv2/opencv.hpp>
#include <iostream>

double entropy_b32(const cv::Mat & InputMat, cv::Mat & S_map);
void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat);