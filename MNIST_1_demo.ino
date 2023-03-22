// add the model to the sketch
#include "model.h"

// Include header files needed for TFLu
#include <TensorFlowLite.h>
// To load DNN operators required for running the ml model
#include <tensorflow/lite/micro/all_ops_resolver.h>
// To output the debug information returned by the TFLu runtime
#include <tensorflow/lite/micro/micro_error_reporter.h>
// To load and execute the ML model
#include <tensorflow/lite/micro/micro_interpreter.h>
// For the schema of the TFLite FlatBuffer format
#include <tensorflow/lite/schema/schema_generated.h>
//For the versioning of the TFLite schema
#include <tensorflow/lite/version.h>


//variable required by TFLu
//-----------------------------------------------------------------------------

// Model parsed by the TFLu parser
const tflite::Model* tflu_model = nullptr;

// The pointer to the interpreter
tflite::MicroInterpreter* tflu_interpreter = nunllptr; 
tflite::MicroErrorReporter tflu_error;
tflite::ErrorReporter* error_reporter = &tflu_error;

// Input and output tensors

TfLiteTensor* tflu_i_tensor = nullptr;
TfLiteTensor* tflu_o_tensor = nullptr;

// Input shape and number of classes for MNIST
// (28 * 28 pixel image and 10 classes, 0 - 9)
const int input_tensor_size = 28 * 28;
cont int num_classes = 10;

// Tensor arena size, memory required by interpreter TFLu does not use dynamic
//  allocation. Arena size is determined by model size through experiments.
constexpr int tensor_arena_size = 4 * 1024;

// allocation of memory 
unit8_t tensor_arena[tensor_arena_size];

// SETUP
//-----------------------------------------------------------------------------

void setup() {
//load the TFLite model from the C-byte array
tflu_model = tflite::GetModel(model_tflite);

// make sure model schema version is compatible (from tflite website)
if (model->version() != TFLITE_SCHEMA_VERSION) {
  TF_LITE_REPORT_ERROR(error_reporter,
  "Model provided is schema version %d not equal not equal to supported version "
  "  %d. \n", model->version(), TFLITE_SCHEMA_VERSION);
  )  
}

// define allopsresolver object (operation implementation)
tflite::AllOpsResolver tflu_ops_resolver;

// build an interpreter to run the model with
tflu_interpreter = new tflite::MicroInterpreter(tflu_model, tflu_ops_resolver, tensor_arena, tensor_arena_size, error_reporter);

// allocate the memory required for the model and get 
// the memeory pointer of the i/o tensors
tflu_interpreter->AllocateTensors();
// provides a pointer to the models input tensor by 
// calling input(0), 0 represents the first input tensor
tflu_i_tensor = tflu_interpreter->input(0);
tflu_o_tensor = tflu_interpreter->output(0);




}

void loop() {

  
}
