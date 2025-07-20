#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/matrix.h>
#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <iostream>
#include <vector>

int main(void){

    using ImageF = std::vector<dlib::matrix<unsigned char>>;  //Declare image vector type, uint8 one channel
    ImageF data, test_data;  //Declare data vectors
    std::vector<unsigned long> labels, test_labels;   //Declase label vectors
    dlib::load_mnist_dataset("./database", data, labels, test_data, test_labels);  //load MNIST data set

    using net_type = dlib::loss_multiclass_log< //Multiclass dnn
                                dlib::fc<10, //10 classes (10 neurons in final layer)
                                dlib::relu<dlib::fc<84,   //84 neurons in layer 2
                                dlib::relu<dlib::fc<120,  //120 neurons in layer 1
                                dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<16,5,5,1,1,  //ReLU, second convolution with 16 5x5 filters and 2x2 stride
                                dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<6,5,5,1,1,  //ReLU, first convolution with 6 5x5 filters and 2x2 stride
                                dlib::input<dlib::matrix<unsigned char>>  //Receives grey scale images in uint8
                                >>>>>>>>>>>>;
    net_type net;

    dlib::dnn_trainer<net_type> trainer(net);  //Create traner
    trainer.set_learning_rate(0.01);
    trainer.set_min_learning_rate(0.00001);
    trainer.set_mini_batch_size(128);
    trainer.be_verbose();  //Turn on detailed output while training dnn
    trainer.set_synchronization_file("mnist_sync", std::chrono::seconds(20));  //Set sync file for recovery if interrupted
    trainer.train(data,labels);  //Train model

    net.clean();  //Wipe training info from output for serializarion
    dlib::serialize("../dnnTrain/network.dat") << net;  //Serialize for storage

    //Check accuracy with test dataset
    std::vector<unsigned long> predicted_labels = net(test_data);
    int right = 0;
    int wrong = 0;

    for (int ii{0}; ii < test_data.size(); ii++)
    {
        if (predicted_labels[ii] == test_labels[ii])
            right++;
        else
            wrong++;
    }

    std::cout << "right: " << right << "\twrong: " << wrong << "\taccuracy: " << static_cast<double>(right)/(test_data.size()) <<std::endl;

    return 0;
}

