#include "user_iface.h"
#include "utils.h"
#include <limits>

#ifndef LAYER_PARAMS
#define LAYER_PARAMS

struct ConvLayerParams {
	void *W, *b;
	void *dW, *db;
	cudnnTensorDescriptor_t input_tensor, output_tensor, bias_desc;
	cudnnFilterDescriptor_t filter_desc;
	cudnnConvolutionDescriptor_t conv_desc;
	cudnnConvolutionFwdAlgo_t fwd_algo;
	cudnnConvolutionBwdFilterAlgo_t bwd_filter_algo;
	cudnnConvolutionBwdDataAlgo_t bwd_data_algo;
	int C_in, C_out, filter_h, filter_w;


	int fwd_req_count, fwd_ret_count;
	int bwd_filter_req_count, bwd_filter_ret_count;
	int bwd_data_req_count, bwd_data_ret_count;
	cudnnConvolutionFwdAlgoPerf_t *fwd_perf;
	cudnnConvolutionBwdFilterAlgoPerf_t *bwd_filter_perf;
	cudnnConvolutionBwdDataAlgoPerf_t *bwd_data_perf;

	void initializeValues(cudnnHandle_t cudnn_handle, ConvDescriptor *user_params, cudnnDataType_t data_type, 
							int batch_size, cudnnTensorFormat_t tensor_format, size_t data_type_size, LayerDimension &output_size);

	void allocateSpace(curandGenerator_t curand_gen, cudnnDataType_t data_type, size_t data_type_size, float std_dev, size_t &free_bytes);

	void getWorkspaceSize(size_t &workspace_size, size_t &free_bytes, ConvAlgo conv_algo);
};

struct FCLayerParams {
	void *W, *b;
	void *dW, *db;
	int C_in, C_out;

	void initializeValues(FCDescriptor *user_params, int batch_size, size_t data_type_size, LayerDimension &output_size);
	void allocateSpace(curandGenerator_t curand_gen, cudnnDataType_t data_type, size_t data_type_size, 
						float std_dev, size_t &free_bytes);
};

struct DropoutLayerParams {
	cudnnDropoutDescriptor_t dropout_desc;
	cudnnTensorDescriptor_t input_tensor;
	void *reserved_space;
	void *state;
	size_t reserved_space_size;
	size_t state_size;

	void initializeValues(cudnnHandle_t cudnn_handle, DropoutDescriptor *user_params, cudnnDataType_t data_type, int batch_size,
										 cudnnTensorFormat_t tensor_format, LayerDimension &output_size);

	void allocateSpace(size_t &free_bytes, cudnnHandle_t cudnn_handle, DropoutDescriptor *user_params, long long seed);
};

struct BatchNormLayerParams {
	cudnnTensorDescriptor_t input_tensor;
	cudnnTensorDescriptor_t sbmv_desc;
	void *scale, *bias;
	void *dscale, *dbias;
	void *running_mean, *running_variance;
	void *result_save_mean, *result_save_inv_var;
	double factor, epsilon;
	cudnnBatchNormMode_t mode;
	int h, w, c;
	int sbmv_size;

	void initializeValues(BatchNormDescriptor *user_params, cudnnDataType_t data_type, cudnnTensorFormat_t tensor_format, 
							int batch_size, LayerDimension &output_size);
	void allocateSpace(cudnnDataType_t data_type, size_t data_type_size, size_t &free_bytes);
};

struct PoolingLayerParams {
	cudnnTensorDescriptor_t input_tensor;
	cudnnTensorDescriptor_t output_tensor;

	cudnnPoolingDescriptor_t pool_desc;

	void initializeValues(PoolingDescriptor *user_params, cudnnDataType_t data_type, cudnnTensorFormat_t tensor_format, 
							int batch_size, LayerDimension &output_size);
	void allocateSpace(size_t &free_bytes);
};

struct ActivationLayerParams {
	cudnnActivationDescriptor_t actv_desc;
	cudnnTensorDescriptor_t input_tensor;

	void initializeValues(ActivationDescriptor *user_params, cudnnDataType_t data_type, cudnnTensorFormat_t tensor_format, 
							int batch_size, LayerDimension &output_size);
	void allocateSpace(size_t &free_bytes);
};

struct SoftmaxLayerParams {
	cudnnTensorDescriptor_t input_tensor;
	cudnnSoftmaxAlgorithm_t algo;
	cudnnSoftmaxMode_t mode;
	
	void initializeValues(SoftmaxDescriptor *user_params, cudnnDataType_t data_type, cudnnTensorFormat_t tensor_format, 
							int batch_size, LayerDimension &output_size);
	void allocateSpace(size_t &free_bytes);
};

#endif
