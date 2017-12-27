#pragma once

#include <opencv2/opencv.hpp>

class Util
{
private:

public:
	static void imShow(cv::String str, cv::Mat img, bool wait = false, int maxW = 960, int maxH = 540);
};