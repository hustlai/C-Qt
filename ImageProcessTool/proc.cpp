#include "proc.h"
#include "qdebug.h"

std::vector<cv::Rect_<float>> Proc::getPlateRects(cv::Mat m, int morW, int morH)
{
	//cv::Mat src_threshold;
	std::vector<cv::Rect_<float>> plateRectVec;
	cv::Mat mClone = m.clone();
	cv::Mat mMorph;
	cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(morW, morH));
	morphologyEx(mClone, mMorph, cv::MORPH_CLOSE, element);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mMorph,
		contours,               // a vector of contours
		CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE);  // all pixels of each contours

	std::vector<std::vector<cv::Point>>::iterator itc = contours.begin();
	std::vector<cv::RotatedRect> rrs;

	while (itc != contours.end()) {
		cv::RotatedRect mr = minAreaRect(cv::Mat(*itc));

		//first_rects.push_back(mr);

		if (verifySize(mr)) {
			rrs.push_back(mr);

			float area = mr.size.height * mr.size.width;
			float r = (float)mr.size.width / (float)mr.size.height;
			if (r < 1) r = (float)mr.size.height / (float)mr.size.width;
		}

		++itc;
	}

	for (size_t i = 0; i < rrs.size(); i++) {
		cv::RotatedRect roi_rect = rrs[i];

		cv::Rect_<float> safeBoundRect;
		if (!calcSafeRect(roi_rect, m, safeBoundRect)) continue;

		plateRectVec.push_back(safeBoundRect);
	}

	return plateRectVec;
}

std::vector<cv::Mat> Proc::getPlates(const cv::Mat src, cv::Mat m, int morW, int morH)
{
	std::vector<cv::Mat> plateMatVec;
	cv::Mat mClone = m.clone();
	cv::Mat mMorph;
	cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(morW, morH));
	morphologyEx(mClone, mMorph, cv::MORPH_CLOSE, element);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mMorph,
		contours,               // a vector of contours
		CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE);  // all pixels of each contours

	std::vector<std::vector<cv::Point>>::iterator itc = contours.begin();
	std::vector<cv::RotatedRect> rrs;

	while (itc != contours.end()) {
		cv::RotatedRect mr = minAreaRect(cv::Mat(*itc));

		//first_rects.push_back(mr);

		if (verifySize(mr)) {
			rrs.push_back(mr);

			float area = mr.size.height * mr.size.width;
			float r = (float)mr.size.width / (float)mr.size.height;
			if (r < 1) r = (float)mr.size.height / (float)mr.size.width;
		}

		++itc;
	}

	return plateMatVec;
}

bool Proc::verifySize(cv::RotatedRect& rr)
{
	const int mMaxArea = 500000;
	const int mMinArea = 5000;
	const int mMaxRatio = 7;
	const int mMinRatio = 0;
	const int mMaxAngle = 60;

	float ratio = (float)rr.size.width / (float)rr.size.height;
	float angle = rr.angle;
	int area = rr.size.width * rr.size.height;

	cv::Size size = rr.size;
	if (ratio < 1) {
		angle = 90 + angle;
		ratio = 1 / ratio;
		cv::swap(rr.size.width, rr.size.height);
		rr.angle = angle;
	}

	if ((area < mMinArea || area > mMaxArea) || (ratio < mMinRatio || ratio > mMaxRatio) || (angle + mMaxAngle < 0 || angle - mMaxAngle > 0))
		return false;
	else
		return true;
}

bool Proc::calcSafeRect(const cv::RotatedRect & rr, const cv::Mat & m, cv::Rect_<float>& br)
{
	cv::Rect_<float> boudRect = rr.boundingRect();

	float tl_x = boudRect.x > 0 ? boudRect.x : 0;
	float tl_y = boudRect.y > 0 ? boudRect.y : 0;

	float br_x = boudRect.x + boudRect.width < m.cols
		? boudRect.x + boudRect.width - 1
		: m.cols - 1;
	float br_y = boudRect.y + boudRect.height < m.rows
		? boudRect.y + boudRect.height - 1
		: m.rows - 1;

	float roi_width = br_x - tl_x;
	float roi_height = br_y - tl_y;

	if (roi_width <= 0 || roi_height <= 0) return false;

	//  a new rect not out the range of mat

	br = cv::Rect_<float>(tl_x, tl_y, roi_width, roi_height);

	return true;
}

//bool Proc::removeLargeSmallBlocks(cv::Mat & m, int minSize, int maxSize, float minRatio, float maxRatio)
//{
//	cv::Mat srcCopy = m.clone();
//	std::vector<std::vector<cv::Point>> contours;
//	findContours(srcCopy,
//		contours,               // a vector of contours
//		CV_RETR_EXTERNAL,
//		CV_CHAIN_APPROX_NONE);
//	std::vector<std::vector<cv::Point>>::iterator iter;
//	for (iter = contours.begin(); iter != contours.end(); )
//	{
//		if ((*iter).size() > maxSize || (*iter).size() < minSize)
//		{
//			drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), 2);
//			drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
//			iter = contours.erase(iter);
//		}
//		else
//		{
//			iter++;
//		}
//	}
//	
//	if (minRatio != 0 || maxRatio != 5000)
//	{
//		for (iter = contours.begin(); iter != contours.end(); )
//		{
//			cv::RotatedRect mr = minAreaRect(cv::Mat(*iter));
//			int w = mr.size.width;
//			int h = mr.size.height;
//			float angle = mr.angle;
//			if (angle > 45 | angle < -45)
//			{
//				mr.size.width = h;
//				mr.size.height = w;
//				w = mr.size.width;
//				h = mr.size.height;
//				angle += 90;
//				mr.angle = angle;
//			}
//			if (w < h * minRatio || w > h * maxRatio)
//			{
//				drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), 2);
//				drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
//				iter = contours.erase(iter);
//			}
//			else
//			{
//				qDebug() << "angle: " << angle << "\n";
//				iter++; 
//			}
//		}
//	}
//	return true;
//}

//bool Proc::removeUselessBlocks(cv::Mat & m, int minW, int maxW, int minH, int maxH)
//{
//	cv::Mat srcCopy = m.clone();
//	std::vector<std::vector<cv::Point>> contours;
//	findContours(srcCopy,
//		contours,               // a vector of contours
//		CV_RETR_EXTERNAL,
//		CV_CHAIN_APPROX_NONE);
//	std::vector<std::vector<cv::Point>>::iterator iter;
//	for (iter = contours.begin(); iter != contours.end(); )
//	{
//		cv::RotatedRect rr = cv::minAreaRect(*iter);
//		if (rr.angle < -45 || rr.angle > 45)
//		{
//			cv::swap(rr.size.width, rr.size.height);
//			rr.angle = rr.angle + 90;
//		}
//		if (rr.size.width > maxW || rr.size.width < minW || rr.size.height > maxH || rr.size.height < minH)
//		{
//			drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), 2);
//			drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
//			iter = contours.erase(iter);
//		}
//		else
//		{
//			iter++;
//		}
//	}
//
//	return true;
//}

bool Proc::removeBlocksBySize(cv::Mat & m, int minW, int maxW, int minH, int maxH)
{
	cv::Mat srcCopy = m.clone();
	std::vector<std::vector<cv::Point>> contours;
	findContours(srcCopy,
		contours,               // a vector of contours
		CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE);
	std::vector<std::vector<cv::Point>>::iterator iter;
	for (iter = contours.begin(); iter != contours.end(); )
	{
		cv::RotatedRect rr = cv::minAreaRect(*iter);
		if (rr.angle < -45 || rr.angle > 45)
		{
			cv::swap(rr.size.width, rr.size.height);
			rr.angle = rr.angle + 90;
		}
		if (rr.size.width > maxW || rr.size.width < minW || rr.size.height > maxH || rr.size.height < minH)
		{
			drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), 2);
			drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
			iter = contours.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	return true;
}

bool Proc::removeBlocksByArea(cv::Mat & m, int minA, int maxA)
{
	cv::Mat srcCopy = m.clone();
	std::vector<std::vector<cv::Point>> contours;
	findContours(srcCopy,
		contours,               // a vector of contours
		CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE);
	std::vector<std::vector<cv::Point>>::iterator iter;
	for (iter = contours.begin(); iter != contours.end(); )
	{
		if ((*iter).size() > maxA || (*iter).size() < minA)
		{
			drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), 2);
			drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
			iter = contours.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}

bool Proc::removeBlocksByRatio(cv::Mat & m, float minR, float maxR)
{
	cv::Mat srcCopy = m.clone();
	std::vector<std::vector<cv::Point>> contours;
	findContours(srcCopy,
		contours,               // a vector of contours
		CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE);
	std::vector<std::vector<cv::Point>>::iterator iter;

	if (minR != 0 || maxR != 2000)
	{
		for (iter = contours.begin(); iter != contours.end(); )
		{
			cv::RotatedRect mr = minAreaRect(cv::Mat(*iter));
			int w = mr.size.width;
			int h = mr.size.height;
			float angle = mr.angle;
			if (angle > 45 | angle < -45)
			{
				mr.size.width = h;
				mr.size.height = w;
				w = mr.size.width;
				h = mr.size.height;
				angle += 90;
				mr.angle = angle;
			}
			if (w < h * minR || w > h * maxR)
			{
				drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), 2);
				drawContours(m, contours, iter - contours.begin(), cv::Scalar(0), CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
				iter = contours.erase(iter);
			}
			else
			{
				qDebug() << "angle: " << angle << "\n";
				iter++;
			}
		}
	}
	return true;
}

bool Proc::deskew(const cv::Mat & src, const cv::Mat & m, std::vector<cv::RotatedRect>& rr, std::vector<cv::Mat>& dst)
{
	//cv::Mat mat_debug;
	//src.copyTo(mat_debug);

	for (size_t i = 0; i < rr.size(); i++)
	{
		cv::RotatedRect rri = rr[i];

		if (verifySize(rri))
		{
			cv::Rect_<float> safeBoundRect;
			bool isFormRect = calcSafeRect(rri, src, safeBoundRect);
			if (!isFormRect) continue;

			cv::Mat boundMat = src(safeBoundRect);
			//cv::Mat bound_mat_b = src_b(safeBoundRect);

			//if (0) {
			//	showImg("bound_mat_b", src);
			//	cv::waitKey(0);
			//	showImg("bound_mat_b", src_b);
			//	cv::waitKey(0);
			//	showImg("bound_mat_b", bound_mat);
			//	cv::waitKey(0);
			//	showImg("bound_mat_b", bound_mat_b);
			//	cv::waitKey(0);
			//	cv::destroyWindow("bound_mat_b");
			//}

			cv::Point2f center = rri.center - safeBoundRect.tl();

			cv::Mat deskewMat;

			if (false)
			{
				deskewMat = boundMat;
			}
			else {
				cv::Mat rotatedMat;
				//cv::Mat rotated_mat_b;

				if (!rotate(boundMat, rotatedMat, rri.size, center, rri.angle))
					continue;

				//if (!rotation(bound_mat_b, rotated_mat_b, roi_rect_size, roi_ref_center, roi_angle))
				//	continue;

				// we need affine for rotatioed image
				double slope = tan(rri.angle * 3.14159265 / 180.0);
				affine(rotatedMat, deskewMat, slope);

				//if (0)
				//{
				//	showImg("after rotation", deskew_mat);
				//	cv::waitKey(0);
				//}
				// //////////////////////////////////////////////////////////////////////////////////
			}

			cv::Mat plateMat;
			plateMat.create(HEIGHT, WIDTH, TYPE);

			cv::Mat no_md_mat = clearMaoDing(deskewMat);
			//if (0)
			//{
			//	showImg("no maoding", no_md_mat);
			//	cv::waitKey(0);
			//}

			cv::Mat grayMat;
			cv::Mat no_md_deskew_mat;
			cv::cvtColor(no_md_mat, grayMat, CV_BGR2GRAY);
			//if (0)
			//{
			//	showImg("gray_mat", grayMat);
			//	cv::waitKey(0);
			//}
			float angle = getInclinedAngle(grayMat);
			float slope = tan(angle);
			//qDebug() << "angle" << angle;
			affine(no_md_mat, no_md_deskew_mat, slope);

			if (no_md_deskew_mat.rows > 100)
			{
				if (no_md_deskew_mat.cols >= WIDTH || no_md_deskew_mat.rows >= HEIGHT)
					cv::resize(no_md_deskew_mat, plateMat, plateMat.size(), 0, 0, cv::INTER_AREA);
				else
					cv::resize(no_md_deskew_mat, plateMat, plateMat.size(), 0, 0, cv::INTER_CUBIC);

				dst.push_back(plateMat);

				//if (0)
				//{
				//	showImg("candidate plate", plate_mat);
				//	cv::waitKey(0);
				//	cv::destroyAllWindows();
				//}
			}
		}
	}

	return true;
}

bool Proc::rotate(cv::Mat & in, cv::Mat & out, const cv::Size rect_size, const cv::Point2f center, const double angle)
{
	cv::Mat in_large;
	//in_large.create(int(in.rows * 1.5), int(in.cols * 1.5), in.type());
	in_large = cv::Mat::zeros(cv::Size(int(in.rows*1.5), int(in.cols*1.5)), in.type());

	float x = in_large.cols / 2 - center.x > 0 ? in_large.cols / 2 - center.x : 0;
	float y = in_large.rows / 2 - center.y > 0 ? in_large.rows / 2 - center.y : 0;

	float width = x + in.cols < in_large.cols ? in.cols : in_large.cols - x;
	float height = y + in.rows < in_large.rows ? in.rows : in_large.rows - y;

	/*assert(width == in.cols);
	assert(height == in.rows);*/

	if (width != in.cols || height != in.rows) return false;

	cv::Mat imageRoi = in_large(cv::Rect_<float>(x, y, width, height));
	addWeighted(imageRoi, 0, in, 1, 0, imageRoi);

	cv::Point2f center_diff(in.cols / 2.f, in.rows / 2.f);
	cv::Point2f new_center(in_large.cols / 2.f, in_large.rows / 2.f);

	cv::Mat rot_mat = getRotationMatrix2D(new_center, angle, 1);

	/*imshow("in_copy", in_large);
	waitKey(0);*/

	cv::Mat mat_rotated;
	warpAffine(in_large, mat_rotated, rot_mat, cv::Size(in_large.cols, in_large.rows),
		CV_INTER_CUBIC);

	/*imshow("mat_rotated", mat_rotated);
	waitKey(0);*/

	cv::Mat img_crop;
	getRectSubPix(mat_rotated, cv::Size(rect_size.width, rect_size.height),
		new_center, img_crop);

	out = img_crop;

	if (0) {
		imshow("out", out);
		cv::waitKey(0);
		cv::destroyWindow("out");
	}

	/*imshow("img_crop", img_crop);
	waitKey(0);*/

	return true;
}

bool Proc::affine(const cv::Mat & in, cv::Mat & out, const double slope)
{
	cv::Point2f dstTri[3];
	cv::Point2f plTri[3];

	float height = (float)in.rows;
	float width = (float)in.cols;
	float xiff = (float)abs(slope) * height;

	if (slope > 0) {

		// right, new position is xiff/2

		plTri[0] = cv::Point2f(0, 0);
		plTri[1] = cv::Point2f(width - xiff - 1, 0);
		plTri[2] = cv::Point2f(0 + xiff, height - 1);

		dstTri[0] = cv::Point2f(xiff / 2, 0);
		dstTri[1] = cv::Point2f(width - 1 - xiff / 2, 0);
		dstTri[2] = cv::Point2f(xiff / 2, height - 1);
	}
	else {

		// left, new position is -xiff/2

		plTri[0] = cv::Point2f(0 + xiff, 0);
		plTri[1] = cv::Point2f(width - 1, 0);
		plTri[2] = cv::Point2f(0, height - 1);

		dstTri[0] = cv::Point2f(xiff / 2, 0);
		dstTri[1] = cv::Point2f(width - 1 - xiff + xiff / 2, 0);
		dstTri[2] = cv::Point2f(xiff / 2, height - 1);
	}

	cv::Mat warp_mat = getAffineTransform(plTri, dstTri);

	cv::Mat affine_mat;
	affine_mat.create((int)height, (int)width, TYPE);

	if (in.rows > HEIGHT || in.cols > WIDTH)
		warpAffine(in, affine_mat, warp_mat, affine_mat.size(), CV_INTER_AREA);
	else
		warpAffine(in, affine_mat, warp_mat, affine_mat.size(), CV_INTER_CUBIC);

	out = affine_mat;

	return true;
}

float Proc::getInclinedAngle(cv::Mat m)
{
	cv::Mat tmpMat;
	cv::threshold(m, tmpMat, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);

	int maxGap = 0;
	float bestAngle = 0;
	int count = 0;
	int rows = tmpMat.rows;
	int cols = tmpMat.cols;
	bool flag = true;
	for (int a = -27; a < 28; a++)
	{
		float aa = a * 3.141592653 / 180;
		count = 0;
		for (int i = cols / 5 - tan(aa)*rows; i < cols * 4 / 5 - tan(aa)*rows; i++)
		{
			flag = true;
			for (int j = 0; j < rows; j++)
			{
				if (tmpMat.at<uchar>(j, (int)(i + j*tan(aa))) != 0)
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				count++;
			}
		}

		if (count > maxGap)
		{
			maxGap = count;
			bestAngle = aa;
		}
	}

	return bestAngle;
}

cv::Mat Proc::clearMaoDing(cv::Mat src)
{
	cv::Mat ret;
	cv::Mat grayMat;
	cv::Mat binMat;
	cvtColor(src, grayMat, CV_RGB2GRAY);
	threshold(grayMat, binMat, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

	const int x = 14;
	int upper = 0, bottom = src.rows;
	cv::Mat jump = cv::Mat::zeros(1, binMat.rows, CV_32F);
	for (int i = 0; i < binMat.rows; i++) {
		int jumpCount = 0;
		for (int j = 0; j < binMat.cols - 1; j++) {
			if (binMat.at<uchar>(i, j) != binMat.at<uchar>(i, j + 1)) jumpCount++;
		}

		jump.at<float>(i) = (float)jumpCount;
	}

	for (int i = binMat.rows / 2; i >= 0; i--) {
		if (jump.at<float>(i) < x) {
			upper = i;
			break;
		}
	}

	for (int i = binMat.rows / 2; i < binMat.rows; i++)
	{
		if (jump.at<float>(i) < x) {
			bottom = i + 1;
			break;
		}
	}

	ret = src(cv::Range(upper, bottom), cv::Range(0, src.cols));

	return ret;
}

cv::Mat Proc::colorMatch(cv::Mat m, int minH, int maxH, int minS, int maxS, int minV, int maxV)
{
	cv::Mat hsv, dst;
	cv::cvtColor(m, hsv, CV_BGR2HSV);

	std::vector<cv::Mat> hsvSplit;
	hsvSplit.resize(3);
	split(hsv, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, hsv);

	float diffH = float((maxH - minH) / 2);
	float avgH = minH + diffH;

	// consider multi channel image
	int ch = hsv.channels();
	int nRows = hsv.rows;
	int nCols = hsv.cols * ch;
	if (hsv.isContinuous()) {
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i) {
		p = hsv.ptr<uchar>(i);
		for (j = 0; j < nCols; j += 3) {
			int H = int(p[j]);      // 0-180
			int S = int(p[j + 1]);  // 0-255
			int V = int(p[j + 2]);  // 0-255

			if (maxH > 180)
			{
				if (((H >= minH && H <= 180) || (H >= 0 && H <= maxH - 180)) && (S >= minS && S <= maxS) && (V >= minV && V <= maxV))
				{
					p[j] = 0;
					p[j + 1] = 0;
					p[j + 2] = 255;
				}
				else {
					p[j] = 0;
					p[j + 1] = 0;
					p[j + 2] = 0;
				}
			}
			else
			{
				if ((H >= minH && H <= maxH) && (S >= minS && S <= maxS) && (V >= minV && V <= maxV)) {
					p[j] = 0;
					p[j + 1] = 0;
					p[j + 2] = 255;
				}
				else {
					p[j] = 0;
					p[j + 1] = 0;
					p[j + 2] = 0;
				}
			}
		}
	}

	// get the final binary
	std::vector<cv::Mat> hsvSplitDone;
	hsvSplitDone.resize(3);
	split(hsv, hsvSplitDone);
	dst = hsvSplitDone[2];

	return dst;
}

cv::Mat Proc::sobelEdgeDetect(cv::Mat m, int blurSize, int sobelDDepth, int dx, int dy, int kSize)
{
	cv::Mat blur, gray, sobel, sobelMat, bin;

	if (blurSize != 0)
	{
		cv::GaussianBlur(m, blur, cv::Size(blurSize, blurSize), 0, 0, cv::BORDER_DEFAULT);
	}
	else
	{
		blur = m.clone();
	}

	if (blur.channels() == 3)
	{
		cv::cvtColor(blur, gray, CV_BGR2GRAY);
	}
	else if(blur.channels() == 1)
	{
		gray = blur;
	}
	cv::Sobel(gray, sobel, sobelDDepth, dx, dy, kSize);
	cv::convertScaleAbs(sobel, sobelMat);
	cv::threshold(sobelMat, bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	return bin;
}

cv::Mat Proc::morphClose(cv::Mat m, int morW, int morH)
{
	cv::Mat close;
	cv::Mat elem = getStructuringElement(cv::MORPH_RECT, cv::Size(morW, morH));
	morphologyEx(m, close, cv::MORPH_CLOSE, elem);

	return close;
}

std::vector<cv::RotatedRect> Proc::getRotatedRects(cv::Mat m)
{
	std::vector<cv::RotatedRect> rrv;
	cv::Mat mClone = m.clone();
	std::vector<std::vector<cv::Point>> contours;
	findContours(mClone,
		contours,               // a vector of contours
		CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE);  // all pixels of each contours
	std::vector<std::vector<cv::Point>>::iterator iter;
	for (iter = contours.begin(); iter != contours.end(); iter++)
	{
		rrv.push_back(cv::minAreaRect(*iter));
	}
	
	return rrv;
}

std::vector<cv::Rect> Proc::getRects(cv::Mat m)
{
	std::vector<cv::Rect> rv;
	cv::Mat mClone = m.clone();
	std::vector<std::vector<cv::Point>> contours;
	findContours(mClone,
		contours,               // a vector of contours
		CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE);  // all pixels of each contours
	std::vector<std::vector<cv::Point>>::iterator iter;
	for (iter = contours.begin(); iter != contours.end(); iter++)
	{
		rv.push_back(cv::boundingRect(*iter));
	}

	return rv;
}

std::vector<cv::Vec2f> Proc::getHoughLines(cv::Mat m, int dx, int dy)
{
	std::vector<cv::Vec2f> lines;
	cv::Mat sobel = Proc::sobelEdgeDetect(m, 1, 3, dx, dy);
	cv::HoughLines(sobel, lines, 1, CV_PI / 180, 500, 0, 0);
	//cv::HoughLinesP(sobel, lines, 1, CV_PI / 180, 50, 50, 10);
	return lines;
}

/**
 * get the two lines that best fit the region of interest 
 */
std::vector<cv::Vec2f> Proc::getProperLines(cv::Mat m, int dx, int dy)
{
	int l1 = 0, l2 = 1;
	float pii = 1.570796;

	std::vector<cv::Vec2f> houghLines;
	cv::Mat sobel = Proc::sobelEdgeDetect(m, 1, 3, dx, dy);
	cv::HoughLines(sobel, houghLines, 1, CV_PI / 180, 400, 0, 0);
	//cv::HoughLinesP(sobel, houghLines, 1, CV_PI / 180, 50, 50, 10);

	std::vector<cv::Vec2f> properLines;
	if (houghLines.size() <= 2)
	{
		return houghLines;
	}
	else
	{
		float minSlopeDiff = 10;
		float minLineGap = 10;
		//for (int i = 0; i < houghLines.size() - 1; i++)
		//{
		//	for (int j = i + 1; j < houghLines.size(); j++)
		//	{
		//		//qDebug() << "line 1: " << lines[i][1] << "\tline 2: " << lines[j][1];
		//		if ((abs(abs(houghLines[i][1]) - abs(houghLines[j][1])) < minSlopeDiff)
		//			&& (abs(abs(houghLines[i][0] - pii) - abs(houghLines[j][0] - pii)) > minLineGap))
		//		{
		//			minSlopeDiff = abs(houghLines[i][1] - houghLines[j][1]);
		//			l1 = i;
		//			l2 = j;
		//		}
		//	}
		//}
		//properLines.push_back(houghLines[l1]);
		//properLines.push_back(houghLines[l2]);

		std::vector<cv::Vec2f>::iterator iter1, iter2;
		for (iter1 = houghLines.begin(); iter1 != houghLines.end(); iter1++)
		{
			int count = 1;
			for (iter2 = iter1+1; iter2 != houghLines.end(); )
			{
				if (abs((abs((*iter1)[1]) - abs((*iter2)[1]))) < minSlopeDiff
					&& abs((abs((*iter1)[0] - pii) - abs((*iter2)[0] - pii))) < minLineGap)
				{
					(*iter1)[0] += (*iter2)[0];
					(*iter1)[1] += (*iter2)[1];
					count++;

					iter2 = houghLines.erase(iter2);
				}
				else
				{
					iter2++;
				}
			}

			(*iter1)[0] /= count;
			(*iter1)[1] /= count;
		}
		
		properLines.push_back(houghLines[0]);
		properLines.push_back(houghLines[1]);
	}

	return properLines;
}

void Proc::drawLines(cv::Mat & m, std::vector<cv::Vec2f> lines)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		cv::Point pt1, pt2;

		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));//cvRound，函数的一种，对一个double型的数进行四舍五入  
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(m, pt1, pt2, cv::Scalar(55, 100, 195), 1, CV_AA);
	}
}

void Proc::drawRects(cv::Mat & m, std::vector<cv::RotatedRect> rrv)
{
	for (int i = 0; i < rrv.size(); i++)
	{
		cv::Point2f P[4];
		rrv[i].points(P);
		for (int j = 0; j < 4; j++)
		{
			line(m, P[j], P[(j + 1) % 4], cv::Scalar(0, 255, 0), 1);
		}
	}
}

void Proc::drawRects(cv::Mat & m, std::vector<cv::Rect> rv)
{
	for (int i = 0; i < rv.size(); i++)
	{
		cv::rectangle(m, rv[i], cv::Scalar(0, 255, 0), 1);
	}
}

cv::Mat Proc::getRectTarget(cv::Mat m, int dx, int dy)
{
	int l1 = 0, l2 = 1;
	float pii = 1.570796;
	cv::Mat mClone = m.clone();
	std::vector<cv::Vec2f> lines = getHoughLines(m, dx, dy);
	if (lines.size() < 2)
	{
		return mClone;
    }
    else if (lines.size() > 2)
	{
		float minSlopeDiff = 10;
		float minLineGap = 10;
		for (int i = 0; i < lines.size() - 1; i++)
		{
			for (int j = i + 1; j < lines.size(); j++)
			{
				//qDebug() << "line 1: " << lines[i][1] << "\tline 2: " << lines[j][1];
				if (abs((abs(lines[i][1] - pii) - abs(lines[j][1] - pii)) < minSlopeDiff) && (abs(abs(lines[i][0] - pii) - abs(lines[j][0] - pii)) > minLineGap))
				{
					minSlopeDiff = abs(lines[i][1] - lines[j][1]);
					l1 = i;
					l2 = j;
				}
			}
		}	
	}

	double cos1, sin1, cos2, sin2, rho1, rho2;

	if (abs(lines[l1][0]) > abs(lines[l2][0]))
	{
		cos1 = cos(lines[l2][1]);
		sin1 = sin(lines[l2][1]);
		cos2 = cos(lines[l1][1]);
		sin2 = sin(lines[l1][1]);
		rho1 = lines[l2][0];
		rho2 = lines[l1][0];
	}
	else
	{
		cos1 = cos(lines[l1][1]);
		sin1 = sin(lines[l1][1]);
		cos2 = cos(lines[l2][1]);
		sin2 = sin(lines[l2][1]);
		rho1 = lines[l1][0];
		rho2 = lines[l2][0];
	}

	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			if ((abs(i*sin1 + j*cos1) < abs(rho1)) || (abs(i*sin2 + j*cos2) > abs(rho2)))
			{
				mClone.at<uchar>(i, j) = 0;
			}
		}
	}

	return mClone;
}

cv::Mat Proc::getRectTarget(cv::Mat m, std::vector<cv::Vec2f> lines)
{
	cv::Mat mClone = m.clone();
	double cos1, sin1, cos2, sin2, rho1, rho2;

	if (abs(lines[0][0]) > abs(lines[1][0]))
	{
		cos1 = cos(lines[1][1]);
		sin1 = sin(lines[1][1]);
		cos2 = cos(lines[0][1]);
		sin2 = sin(lines[0][1]);
		rho1 = lines[1][0];
		rho2 = lines[0][0];
	}
	else
	{
		cos1 = cos(lines[0][1]);
		sin1 = sin(lines[0][1]);
		cos2 = cos(lines[1][1]);
		sin2 = sin(lines[1][1]);
		rho1 = lines[0][0];
		rho2 = lines[1][0];
	}

	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			if ((abs(i*sin1 + j*cos1) < abs(rho1)) || (abs(i*sin2 + j*cos2) > abs(rho2)))
			{
				mClone.at<uchar>(i, j) = 0;
			}
		}
	}

	return mClone;
}

std::vector<cv::Point> Proc::getROIPoints(cv::Mat m, std::vector<cv::Vec2f> lines)
{
	std::vector<cv::Point> points;

	double cos1;
	double sin1;
	double cos2;
	double sin2;
	double rho1;
	double rho2;

	if (abs(lines[0][0]) > abs(lines[1][0]))
	{
		rho1 = abs(lines[1][0]);
		rho2 = abs(lines[0][0]);
		cos1 = cos(lines[1][1]);
		sin1 = sin(lines[1][1]);
		cos2 = cos(lines[0][1]);
		sin1 = sin(lines[0][1]);
	}
	else
	{
		rho1 = abs(lines[0][0]);
		rho2 = abs(lines[1][0]);
		cos1 = cos(lines[0][1]);
		sin1 = sin(lines[0][1]);
		cos2 = cos(lines[1][1]);
		sin2 = sin(lines[1][1]);
	}
	
	int bestRow = -1;
	int leftCol = -1; 
	int rightCol = -1;
	for (int i = 10; i < m.rows; i++)
	{
		leftCol = -1;
		rightCol = -1;
		int countWhite = 0, countAll = 0;
		for (int j = 0; j < m.cols; j++)
		{
			if ((abs(i*sin1 + j*cos1) > rho1) && (abs(i*sin2 + j*cos2) < rho2))
			{
				countAll++;
				int xx = m.at<uchar>(i, j);
				if (m.at<uchar>(i, j) > 100)
				{
					if (bestRow < 0)
					{
						bestRow = i;
					}
					countWhite++;
				}
				if (leftCol < 0)
				{
					leftCol = j;
				}
				rightCol = j;
			}
		}
		if (countWhite > 0.95 * countAll)
		{
			points.push_back(cv::Point(leftCol, bestRow));
			points.push_back(cv::Point(rightCol, bestRow));
			break;
		}
	}

	bestRow = -1;
	for (int i = m.rows - 10; i >= 0; i--)
	{
		int countWhite = 0, countAll = 0;
		leftCol = -1;
		rightCol = -1;
		for (int j = 0; j < m.cols; j++)
		{
			if ((abs(i*sin1 + j*cos1) > rho1) && (abs(i*sin2 + j*cos2) < rho2))
			{
				countAll++;
				if (m.at<uchar>(i, j) > 100)
				{
					bestRow = i;
					countWhite++;
				}
				if (leftCol < 0)
				{
					leftCol = j;
				}
				rightCol = j;
			}
		}
		if (countWhite > 0.5 * countAll)
		{
			points.push_back(cv::Point(leftCol, bestRow));
			points.push_back(cv::Point(rightCol, bestRow));
			break;
		}
	}

	return points;
}

std::vector<cv::Mat> Proc::getTemplate(std::string filePath, int count)
{
	std::vector<cv::Mat> tmplMatVec;
	for (int i = 0; i < count; i++)
	{
		cv::String str = std::to_string(i);
		str = filePath + "\\" + str + ".jpg";
		cv::Mat c = imread(str, CV_8UC1);

		tmplMatVec.push_back(c);
	}

	return tmplMatVec;
}

std::vector<cv::Rect> Proc::getBoundingRects(std::vector<cv::RotatedRect> rrv)
{
	std::vector<cv::Rect> rv;
	for (int i = 0; i < rrv.size(); i++)
	{
		rv.push_back(rrv[i].boundingRect());
	}

	return rv;
}

void Proc::reverse(cv::Mat & m)
{
	const int ch = m.channels();
	switch (ch)
	{
	case 1:
	{
		cv::MatIterator_<uchar> it, end;
		for (it = m.begin<uchar>(), end = m.end<uchar>(); it != end; ++it)
			*it = 255 - *it;
		break;
	}
	case 3:
	{
		cv::MatIterator_<cv::Vec3b> it, end;
		for (it = m.begin<cv::Vec3b>(), end = m.end<cv::Vec3b>(); it != end; ++it)
		{
			(*it)[0] = 255 - (*it)[0];
			(*it)[1] = 255 - (*it)[1];
			(*it)[2] = 255 - (*it)[2];
		}
	}
	}
}

int Proc::predict(std::vector<cv::Mat> tmpl, cv::Mat m)
{
	int index = 0;
	int score = 100000000;
	cv::Mat m1 = m.clone();    // the mat to be judged
							   //sc.convertTo(m1, CV_32FC1);
	for (int i = 0; i < tmpl.size(); i++)
	{
		cv::Mat c = tmpl[i].clone();
		
		cv::Mat m2 = c;    // the mat of module

		int fs = 0;
		for (int m = 0; m < m1.rows; m++)
		{
			for (int n = 0; n < m1.cols; n++)
			{
				int f1 = m1.at<uchar>(m, n);
				int f2, ff = 0;

				for (int mm = 0; mm < m1.rows; mm++)
				{
					for (int nn = 0; nn < m1.cols; nn++)
					{
						f2 = m2.at<uchar>(mm, nn);
						int den = (m - mm) * (m - mm) + (n - nn) * (n - nn) + 1;
						if (den < 255)
							ff += abs(f1 - f2) / den;
					}
				}
				fs += ff;
			}
		}

		if (fs < score)
		{
			score = fs;
			index = i;
		}
	}

	return index;
}

cv::Mat Proc::laiEdge(cv::Mat src)
{
	cv::Mat ret = src.clone();
	cv::Mat ret_cpy = src.clone();
	int width = src.cols;
	int height = src.rows;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
			{
				ret.at<uchar>(i, j) = 0;
				ret_cpy.at<uchar>(i, j) = 0;
			}
			else
			{
				ret.at<uchar>(i, j) = abs(src.at<uchar>(i - 1, j - 1) * 1 - src.at<uchar>(i - 1, j + 1) * 1
					+ src.at<uchar>(i + 1, j - 1) * 1 - src.at<uchar>(i + 1, j + 1) * 1
					+ src.at<uchar>(i, j - 1) * 2 - src.at<uchar>(i, j + 1) * 2) / 4;

				//ret_cpy.at<uchar>(i, j) = abs(src.at<uchar>(i - 1, j - 1) * 1 + src.at<uchar>(i - 1, j) * 2
				//	+ src.at<uchar>(i + 1, j + 1) * 1 - src.at<uchar>(i + 1, j - 1) * 1
				//	+ src.at<uchar>(i + 1, j) * 2 - src.at<uchar>(i + 1, j + 1) * 1) / 4;

				//if (ret_cpy.at<uchar>(i, j) / 10 > ret.at<uchar>(i, j))
				//{
				//	ret.at<uchar>(i, j) = 0;
				//}

				//if (ret.at<uchar>(i, j) > 255)
				//	ret.at<uchar>(i, j) = 255;
			}
		}
	}

	return ret;
}