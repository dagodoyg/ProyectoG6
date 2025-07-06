#include "shape.h"
#include "others.h"
#include <string>
#include <valarray>
#include <cmath>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_io.h>
#include <dlib/pixel.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

int main(int argc, char** argv){

    std::map<std::string, double> p;
    p["D_SIZE"] = 28;
    p["K_SIZE"] = 3;
    p["SNOISE_LVL"] = 0.67243027;
    p["SNOISE_SIG"] = 0.00182435;
    p["N_SIG"] = 100;

    std::string ImgPath = argv[1];
    cv::Mat img = cv::imread(ImgPath);
    cv::Mat img_border;

    if (img.empty()) {
        std::cerr << "Error: no se pudo cargar la imagen 'numeros.jpg'" << std::endl;
        return -1;
    }

    preprocessing(img, img_border);
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_border, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<shape> Figs;

    int idx{0};
    std::vector<cv::Point> c_ii;

    for (int ii{0}; ii<contours.size(); ii++) {

        c_ii = contours[ii];
        cv::Rect box = cv::boundingRect(c_ii);
        cv::Mat cv_digit = img_border(box);

        //Check primitive conditions
        if (box.width > 1.5*box.height) continue;
        if (box.area() < 0.01*img.cols*img.rows) continue;

        //Transform to proper format
        cv::bitwise_not(cv_digit, cv_digit);
        dlib::array2d<unsigned char> dlib_digit;
        dlib::assign_image(dlib_digit, dlib::cv_image<unsigned char>(cv_digit));

        //Store data in struct vector
        Figs.emplace_back(box, dlib_digit);

    }

    //Sort by position in x
    std::sort(Figs.begin(), Figs.end());
    for (auto & c :  Figs){
        std::cout << c.y1 << std::endl;
    }
    

    return 0;
}