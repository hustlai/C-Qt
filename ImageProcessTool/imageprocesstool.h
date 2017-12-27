#pragma once

#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QMouseEvent>
#include "ui_imageprocesstool.h"
#include <opencv2/opencv.hpp>

#include "qimagelabel.h"

class ImageProcessTool : public QMainWindow
{
	Q_OBJECT

public:
	ImageProcessTool(QWidget *parent = Q_NULLPTR);

	void updatePreviousMat();
	void displayMat(cv::Mat imgMat, QImageLabel *qLabel);
	cv::Size getScaledSize(cv::Size imgSize, QSize qSize);
	cv::Size getScaledSize(cv::Mat src, QSize qSize);
	QImage convertMatToQImage(cv::Mat src);

	void showBlurDialog();
	void gaussionBlur(int kernelSize);
	void showColorMatchDialog();
	void showEnlargeDialog();
	void showMorphologyDialog();
	void showRemoveBlocksDialog(int type);
	void showEdgeDetectDialog(int type);
	void removeBlocks(int type, float minV, float maxV, float minHeight = 0, float maxHeight = 0);
	void edgeDetect(int type, float param1 = 0, float param2 = 0, float param3 = 0);
	void colorMatchByHSV(int minH, int maxH, int minS, int maxS, int minV, int maxV);
	void enlarge(int top, int bottom, int left, int right, int r, int g, int b);
	void morphology(int type, int kernelWidth, int kernelHeight);

	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:
	Ui::ImageProcessToolClass ui;
	void createView();
	void createActions();
	void createMenus();
	void createToolBar();

	QAction* openAction;
	QAction* saveAction;
	QAction* exitAction;
	QAction* grayAction;
	QAction* binAction;
	QAction* reverseAction;
	QAction* blurAction;
	QAction* colorMatchAction;
	QAction* enlargeAction;
	QAction* morphologyAction;
	QAction* removeBlocksSizeAction;
	QAction* removeBlocksRatioAction;
	QAction* removeBlocksAreaAction;
	QAction* edgeSobelAction;
	QAction* edgeCannyAction;
	QAction* edgeLaplacianAction;
	QAction* showSourceImageAction;
	QAction* backwardAction;
	QAction* forwardAction;
	QAction* scaleAction;

	QMenu *fileMenu;
	QMenu *processMenu;

	QImageLabel *imgLabel;
	QLabel *coordsLabel;
	QLabel *sizeLabel;
	QLabel *statusLabel;

	cv::Mat srcMat;
	cv::Mat preMat;
	cv::Mat curMat;
	cv::Mat nexMat;

	double ratio;
	bool enableScale;
	bool isMouseLeftPressed;
	bool isMouseRightPressed;
	int mouseLastPosX;
	int mouseLastPosY;
	int mouseNewPosX;
	int mouseNewPosY;

private slots:
	bool open();
	bool save();
	bool gray();
	bool binarization();
	bool reverse();
	bool blur();
	bool colorMatch();
	bool enlarge();
	bool morphology();
	bool removeBlocksSize();
	bool removeBlocksRatio();
	bool removeBlocksArea();
	bool edgeDetectSobel();
	bool edgeDetectCanny();
	bool edgeDetectLaplacian();
	bool showSourceImage();
	bool backward();
	bool forward();
	bool scale();
};
