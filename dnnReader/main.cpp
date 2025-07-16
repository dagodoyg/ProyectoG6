#include "functions.h"
#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_io.h>
#include <iostream>

int main(int argc, char** argv){
    std::string ImgPath = argv[1];
    std::vector<Shape> Figs;

    auto fun = [](std::string s){
        if(std::stoi(s)==1) return true;
        if(std::stoi(s)==0) return false;
        std::cerr << "Invalid input in second argument, no dilation by default" << std::endl;
        return false;
    };

    bool dil = fun(argv[2]); //bool for yes or no dilation

    crop_nums<28>(ImgPath, Figs, dil);

    using net_type = dlib::loss_multiclass_log<
                                dlib::fc<10,        
                                dlib::relu<dlib::fc<84,   
                                dlib::relu<dlib::fc<120,  
                                dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<16,5,5,1,1,
                                dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<6,5,5,1,1,
                                dlib::input<dlib::matrix<unsigned char>> 
                                >>>>>>>>>>>>;
    net_type net;

    dlib::deserialize("network.dat") >> net;

    int behind1 = Figs[0].y1;
    int behind2 = Figs[0].y2;
    for (int idx{0}; idx < Figs.size(); idx++){
        dlib::image_window win;
        win.set_image(Figs[idx].digit);
        win.wait_until_closed();

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

    return 0;
}

/* @echo off
g++ -std=c++17 ^
 -IC:/Users/david/opencv_build/install/include ^
 -IC:/Users/david/opencv_build/install/include/opencv2 ^
 -IC:/Users/david/src/dlib/build/install/include ^
 -LC:/Users/david/opencv_build/install/x64/mingw/lib ^
 -LC:/Users/david/src/dlib/build/install/lib ^
 %* ^
 -ldlib -lopencv_world4120 ^
 -luser32 -lgdi32 -limm32 -lole32 -loleaut32 ^
 -luuid -lcomdlg32 -lavifil32 -lwinmm -lcomctl32 -lpthread
 */