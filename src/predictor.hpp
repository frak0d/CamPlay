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

#include <openpnp-capture.h>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/string_util.h>
#include <tensorflow/lite/model.h>

// Returns the top N confidence values over threshold in the provided vector,
// sorted by confidence in descending order. Taken from tflite Image example.
template <class T>
void get_top_n(T* prediction, int prediction_size, size_t num_results,
               float threshold, std::vector<std::pair<float, int>>* top_results,
               TfLiteType input_type) {
  // Will contain top N results in ascending order.
  std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,
                      std::greater<std::pair<float, int>>>
      top_result_pq;

  const long count = prediction_size;  // NOLINT(runtime/int)
  float value = 0.0;

  for (int i = 0; i < count; ++i) {
    switch (input_type) {
      case kTfLiteFloat32:
        value = prediction[i];
        break;
      case kTfLiteInt8:
        value = (prediction[i] + 128) / 256.0;
        break;
      case kTfLiteUInt8:
        value = prediction[i] / 255.0;
        break;
      default:
        break;
    }
    // Only add it if it beats the threshold and has a chance at being in
    // the top N.
    if (value < threshold) {
      continue;
    }

    top_result_pq.push(std::pair<float, int>(value, i));

    // If at capacity, kick the smallest value out.
    if (top_result_pq.size() > num_results) {
      top_result_pq.pop();
    }
  }

  // Copy to output vector and reverse into descending order.
  while (!top_result_pq.empty()) {
    top_results->push_back(top_result_pq.top());
    top_result_pq.pop();
  }
  std::reverse(top_results->begin(), top_results->end());
}


//template <typename TfDataType>
class Predictor
{
    std::unique_ptr<tflite::FlatBufferModel> model;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::ops::builtin::BuiltinOpResolver resolver;
	
public:
	Predictor(std::string_view model_fname, int thread_num)
{
    // Load Model
    model = tflite::FlatBufferModel::BuildFromFile(model_fname.data());
    
    // Initiate Interpreter
    tflite::InterpreterBuilder(*model.get(), resolver)(&interpreter);

    if (!interpreter)
    	throw std::runtime_error("predictor: failed to initiate interpreter");
    
    if (interpreter->AllocateTensors() != kTfLiteOk)
    	throw std::runtime_error("predictor: failed to allocate tensors");
    
    // Configure the interpreter
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(thread_num);
}

std::tuple<uint8_t, std::string> predictImage()
{
    // Get Input Tensor Dimensions
    int input = interpreter->inputs()[0];
    auto height = interpreter->tensor(input)->dims->data[1];
    auto width = interpreter->tensor(input)->dims->data[2];
    auto channels = interpreter->tensor(input)->dims->data[3];

    // Load Input Image
    //cv::Mat image;

    // Copy image to input tensor
    //cv::resize(frame, image, cv::Size(width, height), cv::INTER_NEAREST);
    //memcpy(interpreter->typed_input_tensor<unsigned char>(0), image.data, image.total() * image.elemSize());

    // Inference
    interpreter->Invoke();
    
    // Get Output
    int output = interpreter->outputs()[0];
    TfLiteIntArray *output_dims = interpreter->tensor(output)->dims;
    auto output_size = output_dims->data[output_dims->size - 1];
    std::vector<std::pair<float, int>> top_results;
    float threshold = 0.01f;

    switch (interpreter->tensor(output)->type)
    {
    case kTfLiteInt32:
        get_top_n<float>(interpreter->typed_output_tensor<float>(0), output_size, 1, threshold, &top_results, kTfLiteFloat32);
        break;
    case kTfLiteUInt8:
        get_top_n<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0), output_size, 1, threshold, &top_results, kTfLiteUInt8);
        break;
    default:
        fprintf(stderr, "cannot handle output type\n");
        exit(-1);
    }
    
    int max_confidence_index;
    float max_confidence = 0;
    std::string max_confidence_name;
    
    for (auto [confidence, index] : top_results)
    {
        if (confidence > max_confidence)
        {
        	max_confidence = confidence;
        	max_confidence_index = index;
        }
    }
    
    return std::make_tuple(max_confidence_index, 
                           max_confidence_name);
}
};
