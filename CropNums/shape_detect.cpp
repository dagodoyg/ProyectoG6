#include "shape.h"
#include "others.h"
#include <string>
#include <valarray>
#include <cmath>

void statistical_check(const std::map<std::string,double> & params, cv::Mat & S_map, const double & S);

int main(int argc, char** argv){

    std::map<std::string, double> p;
    p["D_SIZE"] = 48;
    p["K_SIZE"] = 3;
    p["SNOISE_LVL"] = 0.67243027;
    p["SNOISE_SIG"] = 0.00182435;
    p["N_SIG"] = 250;

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
    cv::findContours(img_border, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

    std::vector<shape> Figs;

    int idx{0};
    double S{0};
    cv::Mat S_map;
    std::vector<cv::Point> c_ii;

    for (int ii{0}; ii<contours.size(); ii++) {

        c_ii = contours[ii];
        cv::Rect box = cv::boundingRect(c_ii);
        double entropy{0};
        cv::Mat digit = img_border(box);

        //Check primitive conditions
        if (box.width > 1.5*box.height) continue;
        if (box.area() < 0.01*img.cols*img.rows) continue;
        if (hierarchy[ii][3]==1) continue;

        //Resize image
        cv::resize(digit, digit, cv::Size(48,48));
        S = entropy_b32(digit , S_map);

        std::cout << S << std::endl;
        cv::imshow("",digit);
        cv::waitKey(0);
        
    }

    return 0;
}

void statistical_check(const std::map<std::string,double> & params, cv::Mat & S_map, const double & S){
    bool f1 = S < params.at("SNOISE_LVL") - params.at("N_SIG") * params.at("SNOISE_SIG") ;
}