#include "functions.h"
#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_io.h>
#include <iostream>

int main(int argc, char** argv){
    std::string ImgPath = argv[1];
    std::vector<Shape> Figs;

    crop_nums<28>(ImgPath, Figs);

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

    for (int idx{0}; idx < Figs.size(); idx++){
        dlib::image_window win;
        win.set_image(Figs[idx].digit);
        win.wait_until_closed();
        std::cout << net(Figs[idx].digit) << std::endl; 
    }

    return 0;
}