#include "DlibTrain.h"


uint32_t data_read(std::ifstream & data){
    std::vector<uint8_t> bytes(4);
    data.read(reinterpret_cast<char*>(bytes.data()), 4);
    return (bytes[0]<<24) | (bytes[1]<<16) | (bytes[2]<<8) | bytes[3];
}

std::vector<ImageF> load_mnist_data(const std::string & FilePath) {
    std::ifstream file(FilePath, std::ios::binary);
    if (!file) std::cerr << "Failed loading: " << FilePath;

    uint32_t aux = data_read(file);
    uint32_t img_idx = data_read(file);
    uint32_t rows = data_read(file);
    uint32_t cols = data_read(file);

    std::vector<ImageF> images(img_idx);
    for (uint32_t ii = 0; ii < img_idx; ii++) {

        ImageF img(rows, cols);

        for (uint32_t c{0}; c < cols; c++) {
            for (uint32_t r{0}; r < rows; r++) {
                unsigned char px;
                file.read((char*) & px, 1);
                img(c + r*cols, 1) = px;
            }
        }

        images[ii] = img;

    }

    return images;
}

std::vector<double> load_mnist_labels(const std::string & filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Failed loading: " + filename);

    uint32_t aux = data_read(file);
    uint32_t labels_idx = data_read(file);

    std::vector<double> labels(labels_idx);
    for (uint32_t ii{0}; ii < labels_idx; ii++) {
        unsigned char label;
        file.read((char*)&label, 1);
        labels[ii] = static_cast<double>(label);
    }

    return labels;
}