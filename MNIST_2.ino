#include <TensorFlowLite.h>

#include <Arduino.h>
#include <ArduinoBLE.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include <tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h>
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
//#include <tensorflow/lite/version.h>
#include "mnist_model.h" // This is the header file containing the model's data (weights and biases)

//variable required by TFLu
//-----------------------------------------------------------------------------

// Model parsed by the TFLu parser
std::unique_ptr<tflite::Model> model;

// The pointer to the interpreter
std::unique_ptr<tflite::MicroInterpreter> interpreter;

tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

// Input and output tensors

TfLiteTensor* tflu_i_tensor = nullptr;
TfLiteTensor* tflu_o_tensor = nullptr;

// Input shape and number of classes for MNIST
// (28 * 28 pixel image and 10 classes, 0 - 9)
const int kImageSize = 28;
const int kNumPixels = kImageSize * kImageSize;
const int kNumClasses = 10;

// Tensor arena size, memory required by interpreter TFLu does not use dynamic
//  allocation. Arena size is determined by model size through experiments.
const int kTensorArenaSize = 20 * 1024;

// allocation of memory dynamcically 
uint8_t* tensor_arena = new uint8_t[kTensorArenaSize];

//-----------------------------------------------------------------------------



// Create a resolver to load the model's operators
static tflite::AllOpsResolver resolver;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial);

  //load the TFLite model from the C-byte array
  model = std::unique_ptr<tflite::Model>(tflite::GetModel(model_tflite));

  // make sure model schema version is compatible 
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
    "Model provided is schema version %d not equal not equal to supported version "
    "  %d. \n", model->version(), TFLITE_SCHEMA_VERSION); 
    exit(1);
  }

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("BLE initialization failed!");
    while (1);
  }
  BLE.setLocalName("MNIST Classifier");


  // Initialize TensorFlow Lite
  interpreter = std::unique_ptr<tflite::MicroInterpreter>(new tflite::MicroInterpreter(model.get(), resolver, tensor_arena, kTensorArenaSize));


  // Allocate memory for the model's input and output tensors
  interpreter->AllocateTensors();

  // Get pointers to the model's input and output tensors
  tflu_i_tensor = interpreter->input(0);
  tflu_o_tensor = interpreter->output(0);

  

    const int IMAGE_SIZE = 28;
    int test_image[IMAGE_SIZE][IMAGE_SIZE];
    int flat_image[IMAGE_SIZE * IMAGE_SIZE];
  
    //test input array size to avoid buffer overflow
    if (sizeof(test_image) != sizeof(int) * IMAGE_SIZE * IMAGE_SIZE) {
      Serial.println("Input image size is not 28x28!");
      exit(1);
    }

    // Set all pixels to 0
    memset(test_image, 0, sizeof(test_image));

    // Draw a vertical line in the center of the image
    int x = IMAGE_SIZE / 2;
    for (int y = 0;y < IMAGE_SIZE; y++) {
        test_image[y][x] = 255;
    }

    // Flatten the image
    for (int i = 0; i < IMAGE_SIZE; i++) {
        for (int j = 0; j < IMAGE_SIZE; j++) {
            flat_image[i * IMAGE_SIZE + j] = test_image[i][j];
        }
    }

  // Copy the test image to the input tensor
  for (int i = 0; i < kNumPixels; i++) {
    tflu_i_tensor->data.f[i] = flat_image[i] / 255.0;
  }


}
void loop(){
  // Run inference on the input tensor
  interpreter->Invoke();
  
  // delete dynamically allocated tensor arena. 
  delete[] tensor_arena;

  // Print the output (the predicted digit)
  float max_prob = 0.0;
  int max_index = 0;
  for (int i = 0; i < kNumClasses; i++) {
    float prob = tflu_o_tensor->data.f[i];
    if (prob > max_prob) {
      max_prob = prob;
      max_index = i;
    }
    Serial.print("Class ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(prob);
  }
  delay(100000000);
}
