#include "DlibTrain.h"
#include <dlib/gui_widgets.h>
#include <dlib/threads.h>
#include <dlib/svm_threaded.h>
#include <dlib/rand.h>

int main(void){
    std::cout << "Reading data base\n";
    std::vector<ImageF> data = load_mnist_data("./database/train-images.idx3-ubyte");
    std::vector<double> labels = load_mnist_labels("./database/train-labels.idx1-ubyte");
    dlib::randomize_samples(data,labels);

    std::vector<ImageF> data_reduced(data.begin(), data.end());
    std::vector<double> labels_reduced(labels.begin(), labels.end());

    using rbf_kernel = dlib::radial_basis_kernel<ImageF>;

    using svm_trainer_type = dlib::svm_nu_trainer<rbf_kernel>;
    svm_trainer_type svm_trainer;

    using ovo_trainer = dlib::one_vs_one_trainer<dlib::any_trainer<ImageF>>;
    ovo_trainer trainer;
    trainer.set_trainer(svm_trainer);
    trainer.set_num_threads(16);

    std::cout << "Training...\n";
    dlib::one_vs_one_decision_function<ovo_trainer> df = trainer.train(data_reduced, labels_reduced);
    std::cout << "Testing...\n";
    std::cout << "predicted label: "<< df(data_reduced[0])  << ", true label: "<< labels_reduced[0] << std::endl;
    std::cout << "predicted label: "<< df(data_reduced[90]) << ", true label: "<< labels_reduced[90] << std::endl;

    std::cout << "Saving...\n";
    dlib::one_vs_one_decision_function<ovo_trainer, dlib::decision_function<rbf_kernel>> df2;
    df2 = df;
    dlib::serialize("df.dat") << df2;
    
    dlib::one_vs_one_decision_function<ovo_trainer>::binary_function_table functs;
    functs = df.get_binary_decision_functions();
    std::cout << "number of binary decision functions in df: " << functs.size() << std::endl;
    return 0;
}