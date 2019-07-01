#include "CLenet.h"
#include <stdlib.h>

CLenet::CLenet()
{
	testA = 10;
	init_parameter("./../data/params.txt");
	init_resultSpace();
}

CLenet::~CLenet()
{

}

double CLenet::predict(double* img)
{
	// printf("%lf\n", *(img+28*28*3-1));
	// printf("ttttttttttttttttttt:\t%lf\n", m_conv1[0][0][0][0]);
	// printf("aaaaaaaaaaa\n");
	m_conv1Res = operator_conv(24, 24, 20, img, 28, 28, 3, m_conv1, 20, 3, 5, 5, m_bias1, m_conv1Res);
	// printf("bbbbbbbbbbb\n");

	m_pool1Res = operator_pool(2, 2, 2, 2, m_conv1Res, 24, 24, 20, m_pool1Res);

	m_conv2Res = operator_conv(8, 8, 50, m_pool1Res, 12, 12, 20, m_conv2, 50, 20, 5, 5, m_bias2, m_conv2Res);

	m_pool2Res = operator_pool(2, 2, 2, 2, m_conv2Res, 8, 8, 50, m_pool2Res);

	m_fc1Res = operator_fullConnect(m_pool2Res, 4, 4, 50, m_fc1, 500, 800, m_fc1_bias, m_fc1Res);

	m_fc1Res = operator_relu(m_fc1Res, 500);

	m_fc2Res = operator_fullConnect((double***)m_fc1Res, 1, 1, 500, m_fc2, 2, 500, m_fc2_bias, m_fc2Res);

	m_predictRes = operator_predict(m_fc2Res, 2);
}

// double* CLenet::load_image(const char* path, int imm_height, int imm_width, int imm_channels)
// {
// 	float fTemp = 0;
// 	int i = 0, j = 0, k = 0;

// 	FILE* fp_img = fopen(path, "r");

// 	for (i = 0; i < imm_channels; i++)
// 	{
// 		for (j = 0; j < imm_height; j++)
// 		{
// 			for (k = 0; k < imm_width; k++)
// 			{
// 				fscanf(fp_img, "%f", &fTemp);
// 				m_img[i*imm_height*imm_width+j*imm_width+k] = fTemp;
// 			}
// 		}
// 	}

// 	return m_img;
// }

double*** CLenet::operator_conv
(
	//卷积后图像参数(20, 24, 24)--(50, 8, 8)
	int outputImm_height, int outputImm_width, int outputImm_channels,
	//图像参数(3, 28, 28)--(20, 24, 24)
	double* Img, int imm_height, int imm_width, int imm_channels,
	//卷积核参数(20, 3, 5, 5)--(50, 20, 5, 5)
	double**** conv, int convOutput_channels, int convInput_channels, int conv_height, int conv_width,
	//bias参数
	double* bias,
	//卷积结果
	double*** conv_res
)
{
	// printf("ccccccccc");
	float fTemp = 0;
	int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;
	////////////////分通道进行卷积操作
	for (i = 0; i < outputImm_channels; i++)
	{
		/////////////取出bias项
		fTemp = bias[i];
		for (j = 0; j < imm_height - conv_height + 1; j++)
		{
			for (k = 0; k < imm_width - conv_width + 1; k++)
			{
				// printf("aaaaaaaaaaa: %d--%d--%d\n", i, j, k);
				conv_res[i][j][k] = 0;
				// printf("bbbbbbbbbbb: %d--%d--%d\n", i, j, k);
				for (l = 0; l < imm_channels; l++)
				{
					for(m = 0; m < conv_height; m++)
					{
						for(n = 0; n < conv_width; n++)
						{
							// printf("%d--%d--%d\n", l, m, n);
							// printf("%lf\n", conv_res[i][j][k]);
							// printf("%lf\n", Img[l][j+m][k+n]);
							// printf("%lf\n", conv[i][l][m][n]);
							conv_res[i][j][k] += Img[l*imm_height*imm_width+(j+m)*imm_height+(k+n)]*conv[i][l][m][n];
							// conv_res[i][j][k] += Img[l][j+m][k+n]*conv[i][l][m][n];
						}
					}
					///////////////在计算第0通道时加上bias
					if (l == 0)
					{
						conv_res[i][j][k] += fTemp;
					}
				}
			}
		}
	}

	return conv_res;
}

double*** CLenet::operator_conv
(
	//卷积后图像参数(20, 24, 24)--(50, 8, 8)
	int outputImm_height, int outputImm_width, int outputImm_channels,
	//图像参数(3, 28, 28)--(20, 24, 24)
	double*** Img, int imm_height, int imm_width, int imm_channels,
	//卷积核参数(20, 3, 5, 5)--(50, 20, 5, 5)
	double**** conv, int convOutput_channels, int convInput_channels, int conv_height, int conv_width,
	//bias参数
	double* bias,
	//卷积结果
	double*** conv_res
)
{
	// printf("bbbbbbbbb");
	float fTemp = 0;
	int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;
	////////////////分通道进行卷积操作
	for (i = 0; i < outputImm_channels; i++)
	{
		/////////////取出bias项
		fTemp = bias[i];
		for (j = 0; j < imm_height - conv_height + 1; j++)
		{
			for (k = 0; k < imm_width - conv_width + 1; k++)
			{
				conv_res[i][j][k] = 0;
				for (l = 0; l < imm_channels; l++)
				{
					for(m = 0; m < conv_height; m++)
					{
						for(n = 0; n < conv_width; n++)
						{
							conv_res[i][j][k] += Img[l][j+m][k+n]*conv[i][l][m][n];
						}
					}
					///////////////在计算第0通道时加上bias
					if (l == 0)
					{
						conv_res[i][j][k] += fTemp;
					}
				}
			}
		}
	}

	return conv_res;
}

double*** CLenet::operator_pool
(
	///////////////池化参数
	int pool_height, int pool_width, int stride_height, int stride_width,
	///////////////需池化操作的图像参数
	double*** Img, int imm_height, int imm_width, int imm_channels,
	//池化结果
	double*** pool_res
)
{
	int i = 0, j = 0, k = 0, l = 0, m = 0;
	double dMaxTemp = 0;
	for (i = 0; i < imm_channels; i++)
	{
		for (j = 0; j < imm_height; j += stride_height)
		{
			for (k = 0; k < imm_width; k += stride_width)
			{
				dMaxTemp = Img[i][j][k];
				for(l = j; l < j + stride_height; l++)
				{
					for(m = k; m < k + stride_width; m++)
					{
						if(Img[i][l][m] > dMaxTemp)
						{
							dMaxTemp = Img[i][l][m];
						}
					}
				}

				pool_res[i][(int)(j/stride_height)][(int)(k/stride_width)] = dMaxTemp;
			}
		}
	}

	return pool_res;
}

double* CLenet::operator_fullConnect
(
	//////////////////图像数据
	double*** Img, int imm_height, int imm_width, int imm_channels,
	//////////////////全链接参数
	double** matFullConnect, int fc_height, int fc_width,
	///////////////////bias
	double* fc_bias,
	//全连接结果
	double* fc_res
)
{
	if(imm_height == 1 && imm_width == 1)
	{
		double* Imm_tmp = (double*)Img;
		int nCount = 0, i = 0;
		for (nCount = 0; nCount < fc_height; nCount++)
		{
			fc_res[nCount] = 0;
			for (i = 0; i < imm_channels; i++)
			{
				fc_res[nCount] += Imm_tmp[i]*matFullConnect[nCount][i];
			}
			fc_res[nCount] += fc_bias[nCount];
		}
	}
	else
	{
		int nCount = 0, i = 0, j = 0, k = 0;
		for (nCount = 0; nCount < fc_height; nCount++)
		{
			fc_res[nCount] = 0;
			for (i = 0; i < imm_channels; i++)
			{
				for (j = 0; j < imm_height; j++)
				{
					for (k = 0; k < imm_width; k++)
					{
						fc_res[nCount] += Img[i][j][k]*matFullConnect[nCount][i*imm_height*imm_width+j*imm_width+k];
					}
				}
			}
			fc_res[nCount] += fc_bias[nCount];
		//	fc_res->at<float>(i) = (*matFullConnect)(Range(i, i + 1), Range::all()).dot(matTemp) + fc_bias->at<float>(i);
		}
	}

	return fc_res;
}

double* CLenet::operator_relu(double* fc_res, int nLength)
{
	int i = 0;
	for (i = 0; i < nLength; i++)
	{
		if (fc_res[i] < 0)
		{
			fc_res[i] = 0;
		}
	}

	return fc_res;
}

double CLenet::my_exp(double x)
{
	int i,k,m,t;
	int xm=(int)x;
	double sum;
	double e ;
	double ef;
	double z ;
	double sub=x-xm;
	m=1;      //阶乘算法分母
	e=1.0;  //e的xm
	ef=1.0;
	t=10;      //算法精度
	z=1;  //分子初始化
	sum=1;
	if (xm<0)
	{
		xm=(-xm);
		for(k=0;k<xm;k++){ef*=2.718281;}
		e/=ef;
	}
	else
	{
		for(k=0;k<xm;k++)
		{
			e*=2.718281;
		}
	}
	for(i=1;i<t;i++)
	{
		m*=i;
		z*=sub;
		sum+=z/m;
	}
	return sum*e;
}

double* CLenet::operator_predict(double* fc_res, int nLength)
{
	double dTemp = 0;
	int i = 0;
	for (i = 0; i < nLength; i++)
	{
		//cout << (*fc_res).at<float>(i) << endl;
		dTemp += my_exp(fc_res[i]);
	}

	for (i = 0; i < nLength; i++)
	{
		m_predictRes[i] = my_exp(fc_res[i]) / dTemp;
	}

	return m_predictRes;
}

//初始化网络各个层的空间
void CLenet::init_resultSpace()
{
	//初始化m_img
	int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;;
	// m_img = (double*)malloc(sizeof(double)*3*28*28);

	//初始化m_conv1Res
	m_conv1Res = (double***)malloc(sizeof(double**)*20);
	for(i = 0; i < 20; i++)
	{
		m_conv1Res[i] = (double**)malloc(sizeof(double*)*24);
	}
	for(i = 0; i < 20; i++)
	{
		for(j = 0; j < 24; j++)
		{
			m_conv1Res[i][j] = (double*)malloc(sizeof(double)*24);
		}
	}

	//初始化m_pool1Res
	m_pool1Res = (double***)malloc(sizeof(double**)*20);
	for(i = 0; i < 20; i++)
	{
		m_pool1Res[i] = (double**)malloc(sizeof(double*)*12);
	}
	for(i = 0; i < 20; i++)
	{
		for(j = 0; j < 12; j++)
		{
			m_pool1Res[i][j] = (double*)malloc(sizeof(double)*12);
		}
	}

	//初始化m_conv2Res
	m_conv2Res = (double***)malloc(sizeof(double**)*50);
	for(i = 0; i < 50; i++)
	{
		m_conv2Res[i] = (double**)malloc(sizeof(double*)*8);
	}
	for(i = 0; i < 50; i++)
	{
		for(j = 0; j < 8; j++)
		{
			m_conv2Res[i][j] = (double*)malloc(sizeof(double)*8);
		}
	}

	//初始化m_pool2Res
	m_pool2Res = (double***)malloc(sizeof(double**)*50);
	for(i = 0; i < 50; i++)
	{
		m_pool2Res[i] = (double**)malloc(sizeof(double*)*4);
	}
	for(i = 0; i < 50; i++)
	{
		for(j = 0; j < 4; j++)
		{
			m_pool2Res[i][j] = (double*)malloc(sizeof(double)*4);
		}
	}

	//初始化m_fc1Res
	m_fc1Res = (double*)malloc(sizeof(double)*500);

	//初始化m_relu1Res

	//初始化m_fc2Res
	m_fc2Res = (double*)malloc(sizeof(double)*2);

	//初始化m_predictRes
	m_predictRes = (double*)malloc(sizeof(double)*2);
}

void CLenet::init_parameter(const char* params_file)
{
	char cTemp[100] = {0};
	m_fp = fopen(params_file, "r");
	fscanf(m_fp, "%s", cTemp);
	fscanf(m_fp, "%s", cTemp);

	m_conv1 = init_conv(20, 3, 5, 5);
	printf("ddddddddddddddd: %lf\n", m_conv1[0][0][0][0]);
	fscanf(m_fp, "%s", cTemp);

	m_bias1 = init_bias(20);
	fscanf(m_fp, "%s", cTemp);
	fscanf(m_fp, "%s", cTemp);

	m_conv2 = init_conv(50, 20, 5, 5);
	fscanf(m_fp, "%s", cTemp);

	m_bias2 = init_bias(50);
	fscanf(m_fp, "%s", cTemp);
	fscanf(m_fp, "%s", cTemp);

	m_fc1 = init_fullConnect(500, 800);
	fscanf(m_fp, "%s", cTemp);

	m_fc1_bias = init_fcBias(500);
	fscanf(m_fp, "%s", cTemp);
	fscanf(m_fp, "%s", cTemp);

	m_fc2 = init_fullConnect(2, 500);
	fscanf(m_fp, "%s", cTemp);

	m_fc2_bias = init_fcBias(2);
}

double**** CLenet::init_conv
(
	////////图像输出通道数
	int output_channel,
	/////////图像输入通道数
	int input_channel,
	///////////卷积核的宽度
	int kernel_height,
	///////////卷积核的高度
	int kernel_width
)
{
	float fTemp = 0;
	int i = 0, j = 0, k = 0, l = 0;
	double**** conv = (double****)malloc(sizeof(double***)*output_channel);

	for(i = 0; i < output_channel; i++)
	{
		conv[i] = (double***)malloc(sizeof(double**)*input_channel);
	}

	for(i = 0; i < output_channel; i++)
	{
		for(j = 0; j < input_channel; j++)
		{
			conv[i][j] = (double**)malloc(sizeof(double*)*kernel_height);
		}
	}

	for(i = 0; i < output_channel; i++)
	{
		for(j = 0; j < input_channel; j++)
		{
			for(k = 0; k < kernel_height; k++)
			{
				conv[i][j][k] = (double*)malloc(sizeof(double)*kernel_width);
			}
		}
	}

	for(i = 0; i < output_channel; i++)
	{
		for(j = 0; j < input_channel; j++)
		{
			for(k = 0; k < kernel_height; k++)
			{
				for(l = 0; l < kernel_width; l++)
				{
					fscanf(m_fp, "%f", &fTemp);
					conv[i][j][k][l] = fTemp;
				}
			}
		}
	}

	return conv;
}

double* CLenet::init_bias(int output_channel)
{
	float fTemp = 0;
	double* bias = (double*)malloc(sizeof(double)*output_channel);
	int i = 0;
	for (i = 0; i < output_channel; i++)
	{
		fscanf(m_fp, "%f", &fTemp);
		bias[i] = fTemp;
	}

	return bias;
}

double** CLenet::init_fullConnect(int fc_height, int fc_width)
{
	float fTemp = 0;
	double** fc = (double**)malloc(sizeof(double*)*fc_height);
	int i = 0, j = 0;
	for(i = 0; i < fc_height; i++)
	{
		fc[i] = (double*)malloc(sizeof(double)*fc_width);
	}

	for(i = 0; i < fc_height; i++)
	{
		for(j = 0; j < fc_width; j++)
		{
			fscanf(m_fp, "%f", &fTemp);
			fc[i][j] = fTemp;
		}
	}

	return fc;
}

double* CLenet::init_fcBias(int output_channels)
{
	float fTemp = 0;
	double* fc_bias = (double*)malloc(sizeof(double)*output_channels);
	int i = 0;
	for(i = 0; i < output_channels; i++)
	{
		fscanf(m_fp, "%f", &fTemp);
		fc_bias[i] = fTemp;
	}

	return fc_bias;
}