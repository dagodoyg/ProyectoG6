#include "shape.h"
#include "others.h"
#include <string>
#include <cstring>
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
    const int D_SIZE = 28;

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
    contour_detector(img_border, contours);
    
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

        //Transform to proper format for dlib svm
        dlib::matrix<double, D_SIZE*D_SIZE, 1> dlib_digit;
        svm_format(cv_digit,dlib_digit,D_SIZE);

        //Store only valid data as struct vector
        Figs.emplace_back(box, dlib_digit);
    }

    //Sort by position, row major
    std::sort(Figs.begin(), Figs.end());

    return 0;
}