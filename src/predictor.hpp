#include "fdeep/tensor.hpp"
#include "fdeep/tensor_pos.hpp"
#include "fdeep/tensor_shape.hpp"
#include <queue>
#include <string>
#include <vector>
#include <utility>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <string_view>

#include <iostream>

#include <fdeep/fdeep.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

void square_crop_resize(int size, const cv::Mat& img_in, cv::Mat& img_out)
{
    if (img_in.rows > img_in.cols)
    {
        auto crop_region_ref = img_in(cv::Rect{0, (img_in.rows-img_in.cols)/2, img_in.cols, img_in.cols});
        cv::resize(crop_region_ref, img_out, cv::Size(size, size), cv::INTER_NEAREST);
    }
    else if (img_in.rows < img_in.cols)
    {
        auto crop_region_ref = img_in(cv::Rect{(img_in.cols-img_in.rows)/2, 0, img_in.rows, img_in.rows});
        cv::resize(crop_region_ref, img_out, cv::Size(size, size), cv::INTER_NEAREST);
    }
    else // img_in is already square
    {
        cv::resize(img_in, img_out, cv::Size(size, size), cv::INTER_NEAREST);
    }
}

class Predictor
{
    cv::Mat image;
    cv::VideoCapture camera;
    fdeep::model model;
	
public:
	Predictor(const std::string& model_fname) : model{fdeep::load_model(model_fname)}
    {
        const auto& shape = model.get_input_shapes();
        std::cout << shape.size() << ' ' << shape[0].rank() <<std::endl;
        
        // prefer lower resolution, if available
        camera.set(cv::CAP_PROP_FRAME_WIDTH, 320);
        camera.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
        
        if (!camera.open(0))
            throw std::runtime_error("predictor: failed to open camera");
    }

    auto predictImage()
    {
        // Get Input Tensor Dimensions
        auto size = model.get_input_shapes();
        cv::Mat raw_image;
        camera >> raw_image;
        
        if (raw_image.empty())
            throw std::runtime_error("predictor: failed to capture image");

        square_crop_resize(224, raw_image, image); assert(image.isContinuous());
        auto image_tensor = fdeep::tensor_from_bytes(image.data, image.rows, image.cols, image.channels(), -1.0f, +1.0f);
        return model.predict_class_with_confidence({image_tensor});
    }
};
