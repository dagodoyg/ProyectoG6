#include "functions.h"
#include <dlib/threads.h>
#include <dlib/svm_threaded.h>
#include <dlib/rand.h>

int main(int argc, char** argv){
    std::string ImgPath = argv[1];
    std::vector<Shape> Figs;

    crop_nums<28>(ImgPath, Figs);

    using rbf_kernel = dlib::radial_basis_kernel<ImageF>;
    using ovo_trainer = dlib::one_vs_one_trainer<dlib::any_trainer<ImageF>>;
    dlib::one_vs_one_decision_function<ovo_trainer, dlib::decision_function<rbf_kernel>> df;

    dlib::deserialize("df2.dat") >> df;

    std::cout << Figs.size() << std::endl;
    int x = df(Figs[2].digit); 
    std::cout << "Clase predicha: " << x << std::endl;
    

    return 0;
}