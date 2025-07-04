#include "opencv_process.h"

int main(void){
    cv::Mat img = cv::imread("numeros.jpg");
    cv::Mat img_border, img_contour;

    preprocessing(img, img_border);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_border, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (auto& c : contours) {
    cv::Rect bbox = cv::boundingRect(c);
    if (bbox.height > 30 && bbox.width > 10) {
        cv::Mat digit = img_border(bbox);
        cv::resize(digit, digit, cv::Size(480, 28));
        cv::imshow("contour", digit);
        cv::waitKey(0);
     }
    }   


    cv::imshow("Numeros", img);
    cv::imshow("Canny", img_border);
    cv::waitKey(0);

    return 0;
}