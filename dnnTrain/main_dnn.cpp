#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <iostream>

int main(void){

    using ImageF = std::vector<dlib::matrix<unsigned char>>;
    ImageF data, test_data;
    std::vector<unsigned long> labels, test_labels;
    dlib::load_mnist_dataset("./database", data, labels, test_data, test_labels);

    using net_type = dlib::loss_multiclass_log<
                                dlib::fc<10,        
                                dlib::relu<dlib::fc<84,   
                                dlib::relu<dlib::fc<120,  
                                dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<16,5,5,1,1,
                                dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<6,5,5,1,1,
                                dlib::input<dlib::matrix<unsigned char>> 
                                >>>>>>>>>>>>;
    net_type net;

    dlib::dnn_trainer<net_type> trainer(net);
    trainer.set_learning_rate(0.01);
    trainer.set_min_learning_rate(0.00001);
    trainer.set_mini_batch_size(128);
    trainer.be_verbose();
    trainer.set_synchronization_file("mnist_sync", std::chrono::seconds(20));

    trainer.train(data,labels);

    net.clean();
    dlib::serialize("mnist_network.dat") << net;

    std::vector<unsigned long> predicted_labels = net(test_data);
    int num_right = 0;
    int num_wrong = 0;
    // And then let's see if it classified them correctly.
    for (size_t i = 0; i < test_data.size(); ++i)
    {
        if (predicted_labels[i] == test_labels[i])
            ++num_right;
        else
            ++num_wrong;
        
    }

    std::cout << "correct: " << num_right << "\twrong: " << num_wrong << std::endl;

    return 0;
}