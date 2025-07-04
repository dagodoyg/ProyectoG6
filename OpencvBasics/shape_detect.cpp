#include "opencv_process.h"

int main(void){
    cv::Mat img = cv::imread("numeros.jpg");
    cv::Mat img_border;

    preprocessing(img, img_border);

    cv::imshow("Numeros", img);
    cv::imshow("Canny", img_border);
    cv::waitKey(0);

    return 0;
}