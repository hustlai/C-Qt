#pragma once

#include <opencv2/opencv.hpp>

#ifndef PLATE_SIZE
#define PLATE_SIZE
	const int WIDTH = 204;
	const int HEIGHT = 54;
	const int TYPE = CV_8UC3;
#endif // !PLATE_SIZE

class Proc
{
private:

public:
	static std::vector<cv::Rect_<float>> getPlateRects(cv::Mat m, int morW, int morH);
	static std::vector<cv::Mat> getPlates(const cv::Mat src, cv::Mat m, int morW, int morH);
	static bool verifySize(cv::RotatedRect& rr);
	static bool calcSafeRect(const cv::RotatedRect& rr, const cv::Mat& m, cv::Rect_<float>& br);
	//static bool removeLargeSmallBlocks(cv::Mat& m, int minSize = 0, int maxSize = 6000000, float minRatio = 0.0, float maxRatio = 5000.0);
	//static bool removeUselessBlocks(cv::Mat& m, int minW = 0, int maxW = 2000, int minH = 0, int maxH = 4000);

	static bool removeBlocksBySize(cv::Mat& m, int minW = 0, int maxW = 4000, int minH = 0, int maxH = 2000);
	static bool removeBlocksByArea(cv::Mat& m, int minA = 0, int maxA = 10000);
	static bool removeBlocksByRatio(cv::Mat& m, float minR = 0.0, float maxR = 2000.0);
	static bool deskew(const cv::Mat& src, const cv::Mat& m, std::vector<cv::RotatedRect>& rr, std::vector<cv::Mat>& dst);
	static bool rotate(cv::Mat& in, cv::Mat& out, const cv::Size rect_size, const cv::Point2f center, const double angle);
	static bool affine(const cv::Mat& in, cv::Mat& out, const double slope);
	static float getInclinedAngle(cv::Mat m);
	static cv::Mat clearMaoDing(cv::Mat src);
	static cv::Mat colorMatch(cv::Mat m, int minH = 0, int maxH = 180, int minS = 10, int maxS = 255, int minV = 10, int maxV = 255);
	static cv::Mat sobelEdgeDetect(cv::Mat m, int blurSize, int sobelDDepth, int dx, int dy, int kSize = 3);
	static cv::Mat morphClose(cv::Mat m, int morW, int morH);
	static std::vector<cv::RotatedRect> getRotatedRects(cv::Mat m);
	static std::vector<cv::Rect> getRects(cv::Mat m);
	static std::vector<cv::Vec2f> getHoughLines(cv::Mat m, int dx, int dy);
	static std::vector<cv::Vec2f> getProperLines(cv::Mat m, int dx, int dy);
	static void drawLines(cv::Mat& m, std::vector<cv::Vec2f> lines);
	static void drawRects(cv::Mat& m, std::vector<cv::RotatedRect> rrv);
	static void drawRects(cv::Mat& m, std::vector<cv::Rect> rv);
	static cv::Mat getRectTarget(cv::Mat m, int dx, int dy);
	static cv::Mat getRectTarget(cv::Mat m, std::vector<cv::Vec2f> lines);
	static std::vector<cv::Point> getROIPoints(cv::Mat m, std::vector<cv::Vec2f> lines);
	static std::vector<cv::Mat> getTemplate(std::string filePath, int count);
	static std::vector<cv::Rect> getBoundingRects(std::vector<cv::RotatedRect> rrv);
	static void reverse(cv::Mat& m);
	static int predict(std::vector<cv::Mat> tmpl, cv::Mat m);

	// not for project
	static cv::Mat laiEdge(cv::Mat src);
};