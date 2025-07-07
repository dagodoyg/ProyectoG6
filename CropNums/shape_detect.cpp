#include "shape.h"
#include "others.h"
#include <string>
#include <valarray>
#include <cmath>
#include <map>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_io.h>
#include <dlib/pixel.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

int main(int argc, char** argv){

    //Declare parameters
    std::map<std::string, double> p;
    p["D_SIZE"] = 28;
    p["K_SIZE"] = 7;

    //Read image
    std::string ImgPath = argv[1];
    cv::Mat img = cv::imread(ImgPath);
    cv::Mat img_border;

    //Check for empty image input
    if (img.empty()) {
        std::cerr << "Error: no se pudo cargar la imagen 'numeros.jpg'" << std::endl;
        return -1;
    }

    //Preprocessing for border detection
    preprocessing(img, img_border);
    
    //Get contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_border, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    
    std::vector<Shape> Figs;
    int idx{0};

    for (int ii{0}; ii<contours.size(); ii++) {

        //Getting the digit as cv::Mat
        std::vector<cv::Point> c_ii = contours[ii];
        cv::Rect box = cv::boundingRect(c_ii);
        cv::Mat cv_digit = img_border(box);

        //Check primitive conditions
        if (box.width > 1.5*box.height) continue;
        if (box.area() < 0.01*img.cols*img.rows) continue;

        cv::resize(cv_digit,cv_digit,cv::Size(p["D_SIZE"],p["D_SIZE"]));

        //Transform to proper format for dlib
        cv::bitwise_not(cv_digit, cv_digit);
        dlib::array2d<unsigned char> dlib_digit;
        dlib::assign_image(dlib_digit, dlib::cv_image<unsigned char>(cv_digit));

        //Store only valid data as struct vector
        Figs.emplace_back(box, dlib_digit);
    }

    //Sort by position, row major
    std::sort(Figs.begin(), Figs.end());

    return 0;
}