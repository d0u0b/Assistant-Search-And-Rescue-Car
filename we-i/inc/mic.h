#ifndef _MIC_H_
#define _MIC_H_

#include "hx_drv_tflm.h"
#include "string.h"
#include "cstdlib"
#include "config.h"
// #include "doa_model_data.h"

// #include "tensorflow/lite/micro/micro_error_reporter.h"
// #include "tensorflow/lite/micro/micro_interpreter.h"
// #include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
// #include "tensorflow/lite/schema/schema_generated.h"
// #include "tensorflow/lite/version.h"

#define mic_sample_rate 16000
#define AUD_BLK_100MS_SZ (mic_sample_rate / 10) //100ms
#define QUEUE_LEN 50

typedef struct {
	int16_t left;
	int16_t right;
} META_AUDIO_t;

// namespace
// {
//     tflite::ErrorReporter *error_reporter = nullptr;
//     const tflite::Model *model = nullptr;
//     tflite::MicroInterpreter *interpreter = nullptr;
//     TfLiteTensor *input = nullptr;

//     constexpr int kTensorArenaSize = 136 * 1024;
//     static uint8_t tensor_arena[kTensorArenaSize];
// }

class Mic
{
private:
	hx_drv_mic_data_config_t slt_audio_config;
	int32_t time_prev = 0, time_cur = 0;

public:
	int queue_index;
	META_AUDIO_t *audio_circular_queue;

	Mic(META_AUDIO_t *queue_addr) : audio_circular_queue(queue_addr)
	{
		// audio_circular_queue = (META_AUDIO_t *)malloc(AUD_BLK_100MS_SZ * queue_len * sizeof(META_AUDIO_t));
		if (hx_drv_mic_initial() != HX_DRV_LIB_PASS)
			hx_drv_uart_print("Microphone Initialize Fail\n");
		else
			hx_drv_uart_print("Microphone Initialize Success\n");

		if (hx_drv_mic_on() != HX_DRV_LIB_PASS)
			hx_drv_uart_print("Microphone Enable Fail\n");
		else
			hx_drv_uart_print("Microphone Enable Success\n");
		if (hx_drv_mic_timestamp_get(&time_prev) == HX_DRV_LIB_PASS)
			time_cur = time_prev;
		else
			hx_drv_uart_print("Microphone Timestamp Error\n");

		// static tflite::MicroErrorReporter micro_error_reporter;
		// error_reporter = &micro_error_reporter;

		// model = tflite::GetModel(DOA_float32_tflite_model_data);
		// if (model->version() != TFLITE_SCHEMA_VERSION)
		// {
		//     TF_LITE_REPORT_ERROR(error_reporter,
		//                          "Model provided is schema version %d not equal "
		//                          "to supported version %d.",
		//                          model->version(), TFLITE_SCHEMA_VERSION);
		//     return;
		// }
		// static tflite::MicroMutableOpResolver<6> micro_op_resolver;
		// micro_op_resolver.AddInputLayer();
		// micro_op_resolver.AddMaxPool1D();
		// micro_op_resolver.AddConv1D();
		// micro_op_resolver.AddDropout();
		// micro_op_resolver.AddFlatten();
		// micro_op_resolver.AddSoftmax();

		// static tflite::MicroInterpreter static_interpreter(
		//     model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
		// interpreter = &static_interpreter;

		// // Allocate memory from the tensor_arena for the model's tensors.
		// TfLiteStatus allocate_status = interpreter->AllocateTensors();
		// if (allocate_status != kTfLiteOk)
		// {
		//     TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
		//     return;
		// }

		// // Get information about the memory area to use for the model's input.
		// input = interpreter->input(0);
	}

	void record(void);
};

#endif
