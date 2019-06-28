#ifndef __CLENET_H__
#define __CLENET_H__

#include <stdio.h>

class CLenet
{
public:
	CLenet();
	~CLenet();

public:
	double predict(double*** img);

private:
	void init_parameter(const char* params_file);
	void init_resultSpace();

	double**** init_conv
	(
		////////图像输出通道数
		int output_channel,
		/////////图像输入通道数
		int input_channel,
		///////////卷积核的宽度
		int kernel_height,
		///////////卷积核的高度
		int kernel_width
	);

	double* init_bias(int output_channel);

	double** init_fullConnect(int fc_height, int fc_width);

	double* init_fcBias(int output_channels);

	double*** load_image(const char* path, int img_height, int img_width, int img_channels);

	double*** operator_conv
	(
		//卷积后图像参数(20, 24, 24)--(50, 8, 8)
		int outputImg_height, int outputImg_width, int outputImg_channels,
		//图像参数(3, 28, 28)--(20, 24, 24)
		double*** Img, int img_height, int img_width, int img_channels,
		//卷积核参数(20, 3, 5, 5)--(50, 20, 5, 5)
		double**** conv, int convOutput_channels, int convInput_channels, int conv_height, int conv_width,
		//bias参数
		double* bias
	);

	double*** operator_pool
	(
		///////////////池化参数
		int pool_height, int pool_width, int stride_height, int stride_width,
		///////////////需池化操作的图像参数
		double*** Img, int img_height, int img_width, int img_channels
	);

	double* operator_fullConnect
	(
		//////////////////图像数据
		double*** Img, int img_height, int img_width, int img_channels,
		//////////////////全链接参数
		double** matFullConnect, int fc_height, int fc_width,
		///////////////////bias
		double* fc_bias
	);

	double* operator_relu(double* fc_res, int nLength);

	double* operator_predict(double* fc_res, int nLength);

	double my_exp(double x);

private:
	FILE* m_fp;

private:
	double**** m_conv1;
	double* m_bias1;
	double**** m_conv2;
	double* m_bias2;
	double** m_fc1;
	double* m_fc1_bias;
	double** m_fc2;
	double* m_fc2_bias;

	//conv1空间指针
	double*** m_conv1Res;
	//pool1空间指针
	double*** m_pool1Res;
	//conv2空间指针
	double*** m_conv2Res;
	//pool2空间指针
	double*** m_pool2Res;
	//fc1空间指针
	double* m_fc1Res;
	//relu1空间指针
	//double* g_relu1Res;
	//fc2空间指针
	double* m_fc2Res;
	//predict空间指针
	double* m_predictRes;
};

#endif