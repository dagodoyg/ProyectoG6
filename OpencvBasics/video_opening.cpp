#include <opencv2/opencv.hpp>

int main(void){
    cv::VideoCapture CvCapture("video.mp4");
    cv::Mat frame;

    cv::namedWindow("video",1);
    for( ; ; ){
        CvCapture >> frame;
        if(frame.empty()) break;
        cv::imshow("video",frame);
        cv::waitKey(20);
    }
    cv::waitKey(0);
    return 0;
}