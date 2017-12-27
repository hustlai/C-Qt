#include "util.h"

void Util::imShow(cv::String str, cv::Mat img, bool wait, int maxW, int maxH)
{
	//const int mImgMaxHeight = 270;
	//const int mImgMaxWidth = 480;

	cv::Mat mat;
	int newRows = img.rows;
	int newCols = img.cols;

	if (img.rows > maxH || img.cols > maxW)
	{

		if (img.rows > img.cols)
		{
			newRows = maxH;
			newCols = img.cols * newRows / img.rows;
		}
		else
		{
			newCols = maxW;
			newRows = img.rows * newCols / img.cols;
		}
	}

	cv::resize(img, mat, cv::Size(newCols, newRows), 0, 0);
	imshow(str, mat);

	if (wait)
	{
		cv::waitKey(0);
	}
}