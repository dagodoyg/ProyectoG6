#include <opencv2/opencv.hpp>
#include <iostream>

int main(void){
    cv::Mat img = cv::imread("numeros.jpg");
    cv::Mat img_rsz;
    cv::Mat img_crop;

    std::cout << img.size() << std::endl;
    cv::resize(img, img_rsz, cv::Size(),0.5,0.5);    
    cv::Rect roi(100, 100,300,250);
    img_crop = img(roi);

    cv::imshow("original", img);
    cv::imshow("resized", img_rsz);
    cv::imshow("cropped", img_crop);

    cv::waitKey(0);
    return 0;
}