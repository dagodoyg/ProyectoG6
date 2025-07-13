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

    int behind = Figs[0].y1;
    std::cout << std::endl;
    for (int idx{0}; idx < Figs.size(); idx++){

        int ahead = Figs[idx].y1;

        if( std::abs(ahead - behind) > 0.2*( Figs[idx].y2 - Figs[idx].y1) ){
            std::cout << std::endl << " " << net(Figs[idx].digit);
        } else {
            std::cout << " " << net(Figs[idx].digit);
        }

        behind = ahead;
    }

    return 0;
}