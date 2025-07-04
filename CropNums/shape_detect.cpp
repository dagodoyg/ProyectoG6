#include "shape.h"
#include <string>

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat);

int main(void){
    cv::Mat img = cv::imread("numeros.jpg");
    cv::Mat img_border;

    preprocessing(img, img_border);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_border, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<shape> Figs;
    int idx{0};
    for (const auto & c_ii : contours) {
        cv::Rect box = cv::boundingRect(c_ii);
        if (box.height > (0.05*img.rows) && box.width > (0.05*img.cols)){
            Figs.emplace_back(c_ii,img_border);
            cv::imshow(std::to_string(idx), Figs[idx].digit);
            cv::waitKey(0);
            idx++;
        }
    }

    return 0;
}

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat){
    cv::cvtColor(InputMat,OutMat,cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(OutMat,OutMat,cv::Size(21,21),3,0);
    cv::Canny(OutMat, OutMat, 25, 75);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::dilate(OutMat,OutMat,kernel);
}
