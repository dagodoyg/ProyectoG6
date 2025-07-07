#include "noise_map.h"
#include <string>
#include <valarray>
#include <cmath>

void preprocessing(const cv::Mat & InputMat, cv::Mat & OutMat);
double entropy_b64(const cv::Mat & InputMat, cv::Mat & S_map);
double stddev(const std::valarray<double>& data);
double mean(const std::valarray<double>& data);

int main(void){
    int N = 10000;
    std::vector<noise_map> candela(N);
    std::valarray<double> S(N);

    for (int idx{0}; idx<candela.size(); idx++) {
        cv::randu(candela[idx].N_map, 0, 256);
        cv::threshold(candela[idx].S_map, candela[idx].S_map, 128, 255, cv::THRESH_BINARY);
        S[idx] = entropy_b64( candela[idx].N_map , candela[idx].S_map);
    }

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "mean S_p: " << mean(S) << "\t" << "stddev: " << stddev(S) << std::endl;

    return 0;
}

double entropy_b64(const cv::Mat & InputMat, cv::Mat & S_map){
    if (InputMat.cols!=48 || InputMat.rows!=48){ std::cerr << "image must be 64x64" << std::endl; return 0;}

    cv::Mat Input_float;
    InputMat.convertTo(Input_float, CV_32F, 1.0 / 255.0);
    
    cv::Mat p0, p1, log_p0, log_p1, S_map_raw;
    cv::blur(Input_float,p1,cv::Size(3,3));

    p0 = 1.0 - p1;

    cv::log(p0, log_p0);
    cv::log(p1, log_p1);
    cv::patchNaNs(log_p0,0);
    cv::patchNaNs(log_p1,0);

    S_map_raw = -p0.mul(log_p0) - p1.mul(log_p1);
    S_map = cv::Mat::zeros(cv::Size(16,16),CV_32F);
    double S{0};
    double h{0};

    for (int ii{0}; ii < 16; ii++){
        for (int jj{0}; jj < 16; jj++){
            h = S_map_raw.at<float>(2 + 3*ii, 2 + 3*jj);
            S_map.at<float>(ii,jj) = h;
            S += h;
        }
    }

    return S/(16*16);
}

double mean(const std::valarray<double>& data) {
    return data.sum() / data.size();
}

double stddev(const std::valarray<double>& data) {
    double m = mean(data);
    std::valarray<double> diff = data - m;
    return std::sqrt((diff * diff).sum() / data.size());
}