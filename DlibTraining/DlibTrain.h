#include <dlib/matrix.h>
#include <dlib/svm.h>
#include <fstream>
#include <vector>
#include <iostream>

using ImageF = dlib::matrix<double, 28*28, 1>;

uint32_t data_read(std::ifstream & data);
std::vector<ImageF> load_mnist_data(const std::string & filename);
std::vector<double> load_mnist_labels(const std::string & filename);