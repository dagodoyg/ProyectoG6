#include "functions.h"
#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_io.h>
#include <iostream>

int main(int argc, char** argv){
    std::string ImgPath = argv[1];  //Get img path from command line
    std::vector<Shape> Figs;  //Create vector of potential digits

    auto fun = [](std::string s){
        if(std::stoi(s)==1) return true;
        if(std::stoi(s)==0) return false;
        std::cerr << "Invalid input in second argument, no dilation by default" << std::endl;
        return false;
    };  //Dilation from commando line

    bool dil = fun(argv[2]); //bool for yes or no dilation

    crop_nums<28>(ImgPath, Figs, dil);  //Extract digit candidates and store in vector of objects

    using net_type = dlib::loss_multiclass_log< //Multiclass dnn
                                dlib::fc<10, //10 classes (10 neurons in final layer)
                                dlib::relu<dlib::fc<84,   //84 neurons in layer 2
                                dlib::relu<dlib::fc<120,  //120 neurons in layer 1
                                dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<16,5,5,1,1,  //ReLU, second convolution with 16 5x5 filters and 2x2 stride
                                dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<6,5,5,1,1,  //ReLU, first convolution with 6 5x5 filters and 2x2 stride
                                dlib::input<dlib::matrix<unsigned char>>  //Receives grey scale images in uint8
                                >>>>>>>>>>>>;
    net_type net;

    dlib::deserialize("network.dat") >> net;  //Deserialize the trained dnn

    //Print in order
    int behind1 = Figs[0].y1;
    int behind2 = Figs[0].y2;
    for (int idx{0}; idx < Figs.size(); idx++){

        int ahead1 = Figs[idx].y1;
        int ahead2 = Figs[idx].y2;

        if( std::abs(ahead1 - behind1) > 0.2*( Figs[idx].y2 - Figs[idx].y1) &&  std::abs(ahead2 - behind2) > 0.2*( Figs[idx].y2 - Figs[idx].y1)){
            std::cout << std::endl << " " << net(Figs[idx].digit);
        } else {
            std::cout << " " << net(Figs[idx].digit);
        }

        behind1 = ahead1;
        behind2 = ahead2;
    }
    std::cout << std::endl;

    return 0;
}

/*  g++ main.cpp -std=c++17
    -ldlib -lpthread -ljpeg -lpng -lwebp
    -llapack -lblas
    $(pkg-config --cflags --libs opencv4) */