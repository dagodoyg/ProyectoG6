#include <opencv2/opencv.hpp>


int main(void){
    cv::Mat img = cv::imread("numeros.jpg");
    cv::Mat img_gray;
    cv::Mat img_blur;
    cv::Mat img_canny;
    cv::Mat img_dil;
    cv::Mat img_ero;

    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(img_gray, img_blur, cv::Size(7,7),5,0);
    cv::Canny(img_blur, img_canny, 50, 150);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));
    cv::dilate(img_canny, img_dil, kernel);
    cv::erode(img_canny, img_ero, kernel);

    imshow("canny", img_canny);
    imshow("dilate", img_dil);
    imshow("erode", img_ero);

    cv::waitKey(0);
    return 0;
}