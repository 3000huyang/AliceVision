#include "image_opencv.h"

#include <opencv2/highgui.hpp>

INSIGHT_NAME_SPACE

/*
depth����ȣ���ÿһ�����ص�λ��(bits)��
��opencv��Mat.depth()�еõ�����һ�� 0 �C 6 �����֣��ֱ����ͬ��λ����
enum { CV_8U=0, CV_8S=1, CV_16U=2, CV_16S=3, CV_32S=4, CV_32F=5, CV_64F=6 }; 
�ɼ� 0��1������8λ�� 2��3������16λ��4��5����32λ��6����64λ��
*/
IMAGEIO_EXPORT bool readJPGImage(std::string &jpg_file, std::vector<uint8_t> &buffer, int &w, int &h, int &channels, int &depth)
{
	cv::Mat mat = cv::imread(jpg_file);
	if (mat.empty()){
		return false;
	}
	channels = mat.channels();
	h = mat.rows;
	w = mat.cols;
	depth = mat.depth();
	int n = 1;
	if (depth == 0 || depth == 1) n = 1;
	else if (depth == 2 || depth == 3) n = 2;
	else if (depth == 4 || depth == 5) n = 4;
	else if (depth == 6) n = 8;
	buffer.resize(h * w * channels * n);
	memcpy(buffer.data(), mat.data, buffer.size());
	return true;
}

INSIGHT_NAME_SPACE_END