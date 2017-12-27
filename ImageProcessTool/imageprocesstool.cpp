#include "imageprocesstool.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <qgroupbox.h>
#include <qdebug.h>
#include <qstatusbar.h>
#include <qfiledialog.h>
#include <QtWidgets\qdialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtoolbar.h>
#include <qcombobox.h>
#include <exception>
#include "util.h"
#include "proc.h"


ImageProcessTool::ImageProcessTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.mainToolBar->setHidden(true);

	createView();
	createActions();
	createMenus();
	createToolBar();
}

void ImageProcessTool::createView()
{
	this->setFixedSize(800, 600);

	QVBoxLayout *imgLayout = new QVBoxLayout(this);
	QGroupBox *imgGroupBox = new QGroupBox(tr("Image"));
	imgLabel = new QImageLabel(tr(""));
	//imgLabel->setFixedSize(800, 600);
	imgLabel->setAlignment(Qt::AlignCenter);
	imgLabel->setMouseTracking(true);
	imgLayout->addWidget(imgLabel);
	imgGroupBox->setLayout(imgLayout);

	coordsLabel = new QLabel(tr("Coordinate"));
	coordsLabel->setFixedWidth(140);
	coordsLabel->setAlignment(Qt::AlignLeft);
	coordsLabel->setIndent(10);
	sizeLabel = new QLabel(tr("Image size"));
	sizeLabel->setFixedWidth(200);
	sizeLabel->setAlignment(Qt::AlignLeft);
	sizeLabel->setIndent(10);
	statusLabel = new QLabel(tr(""));
	statusLabel->setFixedWidth(450);
	statusLabel->setAlignment(Qt::AlignLeft);
	statusLabel->setIndent(10);
	ui.statusBar->addWidget(coordsLabel);
	ui.statusBar->addWidget(sizeLabel);
	ui.statusBar->addWidget(statusLabel);
	ui.statusBar->setSizeGripEnabled(false);//去掉状态栏右下角的三角
	//ui.statusBar->setStyleSheet(QString("background-color: rgb(252, 252, 252);"));

	QWidget *centralWidget = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(imgLabel);
	centralWidget->setLayout(layout);
	setCentralWidget(centralWidget);

	this->setMouseTracking(true);
	centralWidget->setMouseTracking(true);
	imgGroupBox->setMouseTracking(true);
}

void ImageProcessTool::createActions()
{
	openAction = new QAction(tr("&Open"), this);
	openAction->setStatusTip(tr("Open an image"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setStatusTip(tr("Save the image"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

	exitAction = new QAction(tr("&Exit"), this);
	exitAction->setStatusTip(tr("Exit"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	grayAction = new QAction(tr("&Gray"), this);
	grayAction->setStatusTip(tr("Gray processing"));
	connect(grayAction, SIGNAL(triggered()), this, SLOT(gray()));

	binAction = new QAction(tr("&Binarization"), this);
	binAction->setStatusTip(tr("Binarization"));
	connect(binAction, SIGNAL(triggered()), this, SLOT(binarization()));

	reverseAction = new QAction(tr("&Reverse"), this);
	reverseAction->setStatusTip(tr("Reverse"));
	connect(reverseAction, SIGNAL(triggered()), this, SLOT(reverse()));

	blurAction = new QAction(tr("&Blur"), this);
	blurAction->setStatusTip(tr("Blur"));
	connect(blurAction, SIGNAL(triggered()), this, SLOT(blur()));

	colorMatchAction = new QAction(tr("&Color match"), this);
	colorMatchAction->setStatusTip(tr("Color match"));
	connect(colorMatchAction, SIGNAL(triggered()), this, SLOT(colorMatch()));

	enlargeAction = new QAction(tr("&Enlarge"), this);
	enlargeAction->setStatusTip(tr("Enlarge"));
	connect(enlargeAction, SIGNAL(triggered()), this, SLOT(enlarge()));

	morphologyAction = new QAction(tr("&Morphology"), this);
	morphologyAction->setStatusTip(tr("Morphology"));
	connect(morphologyAction, SIGNAL(triggered()), this, SLOT(morphology()));

	removeBlocksSizeAction = new QAction(tr("&Size"), this);
	removeBlocksSizeAction->setStatusTip(tr("Remove blocks by size"));
	connect(removeBlocksSizeAction, SIGNAL(triggered()), this, SLOT(removeBlocksSize()));

	removeBlocksRatioAction = new QAction(tr("&Ratio"), this);
	removeBlocksRatioAction->setStatusTip(tr("Remove blocks by ratio"));
	connect(removeBlocksRatioAction, SIGNAL(triggered()), this, SLOT(removeBlocksRatio()));

	removeBlocksAreaAction = new QAction(tr("&Area"), this);
	removeBlocksAreaAction->setStatusTip(tr("Remove blocks by area"));
	connect(removeBlocksAreaAction, SIGNAL(triggered()), this, SLOT(removeBlocksArea()));

	edgeSobelAction = new QAction(tr("&Sobel"), this);
	edgeSobelAction->setStatusTip(tr("Sobel edge detection"));
	connect(edgeSobelAction, SIGNAL(triggered()), this, SLOT(edgeDetectSobel()));

	edgeCannyAction = new QAction(tr("&Canny"), this);
	edgeCannyAction->setStatusTip(tr("Canny edge detection"));
	connect(edgeCannyAction, SIGNAL(triggered()), this, SLOT(edgeDetectCanny()));

	edgeLaplacianAction = new QAction(tr("&Laplacian"), this);
	edgeLaplacianAction->setStatusTip(tr("Laplacian edge detection"));
	connect(edgeLaplacianAction, SIGNAL(triggered()), this, SLOT(edgeDetectLaplacian()));

	showSourceImageAction = new QAction(tr("&Show source image"), this);
	showSourceImageAction->setStatusTip(tr("Show source image"));
	showSourceImageAction->setIcon(QIcon(":/ImageProcessTool/Resources/img/display.png"));
	connect(showSourceImageAction, SIGNAL(triggered()), this, SLOT(showSourceImage()));

	backwardAction = new QAction(tr("&Backward"),this);
	backwardAction->setStatusTip(tr("Backward"));
	backwardAction->setIcon(QIcon(":/ImageProcessTool/Resources/img/backward.png"));
	connect(backwardAction, SIGNAL(triggered()), this, SLOT(backward()));

	forwardAction = new QAction(tr("&Forward"), this);
	forwardAction->setStatusTip(tr("Forward"));
	forwardAction->setIcon(QIcon(":/ImageProcessTool/Resources/img/forward.png"));
	connect(forwardAction, SIGNAL(triggered()), this, SLOT(forward()));

	scaleAction = new QAction(tr("&Scale"), this);
	scaleAction->setStatusTip(tr("Scale"));
	scaleAction->setIcon(QIcon(":/ImageProcessTool/Resources/img/scale.png"));
	connect(scaleAction, SIGNAL(triggered()), this, SLOT(scale()));
}

void ImageProcessTool::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	processMenu = menuBar()->addMenu(tr("&Process"));
	processMenu->addAction(grayAction);
	processMenu->addAction(binAction);
	processMenu->addAction(reverseAction);
	processMenu->addAction(blurAction);
	processMenu->addAction(colorMatchAction);
	processMenu->addAction(enlargeAction);
	processMenu->addAction(morphologyAction);

	QMenu* removeBlocksMenu = processMenu->addMenu(tr("&Remove blocks"));
	removeBlocksMenu->addAction(removeBlocksSizeAction);
	removeBlocksMenu->addAction(removeBlocksRatioAction);
	removeBlocksMenu->addAction(removeBlocksAreaAction);

	QMenu* edgeDetectMenu = processMenu->addMenu(tr("&Edge detect"));
	edgeDetectMenu->addAction(edgeSobelAction);
	edgeDetectMenu->addAction(edgeCannyAction);
	edgeDetectMenu->addAction(edgeLaplacianAction);
}

void ImageProcessTool::createToolBar()
{
	//QToolBar* processToolBar = new QToolBar(this);
	QToolBar* processToolBar = addToolBar(tr("&Process"));

	processToolBar->addAction(backwardAction);
	processToolBar->addAction(forwardAction);
	processToolBar->addAction(showSourceImageAction);
	processToolBar->addAction(scaleAction);
}

bool ImageProcessTool::open()
{
	bool flag = true;

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
		"", tr("Image Files (*.png *.jpg *.bmp)"));
	qDebug() << "filenames:" << fileName;
	if (!fileName.isEmpty())
	{
		srcMat = cv::imread(fileName.toLatin1().data());
		cv::Mat src(srcMat);
		if (src.data)
		{
			/*cv::imshow("tykgyu", src);//直接显示
			waitKey(0);*/
			displayMat(src, imgLabel);
			preMat = src.clone();
			curMat = src.clone();
			sizeLabel->setText("Width: " + QString::number(src.cols) + "\tHeight: " + QString::number(src.rows));
		}

		/*test();*/
	}

	return flag;
}

bool ImageProcessTool::save()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Open Config"),
		"",
		tr("Config Files (*.jpg)"));

	if (!fileName.isNull())
	{
		//fileName是文件名
		cv::imwrite(fileName.toStdString(), curMat);
		statusLabel->setText(tr("Saved successfully!"));
	}
	else {
		//点的是取消
	}

	return true;
}

bool ImageProcessTool::gray()
{
	if (curMat.data && curMat.channels() >= 3)
	{
		updatePreviousMat();
		cv::Mat tmpMat;
		cv::cvtColor(curMat, tmpMat, CV_BGR2GRAY);
		cv::cvtColor(tmpMat, curMat, CV_GRAY2BGR);
		displayMat(curMat, imgLabel);
	}

	return true;
}

bool ImageProcessTool::binarization()
{
	cv::Mat gray;
	cv::Mat bin;

	if (curMat.data)
	{
		updatePreviousMat();

		//if (curMat.channels() >= 3)
		//{
		//	cv::cvtColor(curMat, gray, CV_BGR2GRAY);
		//}
		//else
		//{
		//	curMat.copyTo(gray);
		//}

		cv::cvtColor(curMat, gray, CV_BGR2GRAY);

		cv::threshold(gray, bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		cv::cvtColor(bin, curMat, CV_GRAY2BGR);

		displayMat(curMat, imgLabel);
	}

	return true;
}

bool ImageProcessTool::reverse()
{
	if (curMat.data)
	{
		updatePreviousMat();

		Proc::reverse(curMat);
		displayMat(curMat, imgLabel); 
	}

	return true;
}

bool ImageProcessTool::blur()
{
	if (curMat.data)
	{
		showBlurDialog();
	}

	return true;
}

void ImageProcessTool::showBlurDialog()
{
	QDialog* dialog = new QDialog(this);
	dialog->setFixedSize(200, 100);
	dialog->setWindowTitle("Gaussion blur");

	QLabel* kernelLabel = new QLabel(dialog);
	kernelLabel->setText("Kernel size");
	QLineEdit* kernelLineEdit = new QLineEdit(dialog);

	QPushButton* cancelButton = new QPushButton("Cancel", dialog);
	QPushButton* okButton = new QPushButton("OK", dialog);
	connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
	connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));

	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	QHBoxLayout* hLayout1 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout2 = new QHBoxLayout(dialog);
	hLayout1->addWidget(kernelLabel);
	hLayout1->addWidget(kernelLineEdit);
	hLayout2->addWidget(cancelButton);
	hLayout2->addWidget(okButton);
	mainLayout->addLayout(hLayout1);
	mainLayout->addLayout(hLayout2);

	dialog->setLayout(mainLayout);
	dialog->show();

	if (dialog->exec() == QDialog::Accepted)
	{
		QString tmpStr = kernelLineEdit->text();

		if (!tmpStr.isEmpty())
		{
			int kerSize = 3;
			bool ok;
			kerSize = tmpStr.toInt(&ok, 10);
			if (ok && ((kerSize % 2) == 1))
			{
				gaussionBlur(kerSize);
				statusLabel->setText("Blur successfully!");
			}
			else
			{
				statusLabel->setText("Blur failed!");
			}
		}
		else
		{
			statusLabel->setText("Blur failed!");
		}
	}
	else
	{
		//blurDialog->close();
	}
}

bool ImageProcessTool::colorMatch()
{
	if (curMat.data)
	{
		showColorMatchDialog();
	}

	return false;
}

bool ImageProcessTool::enlarge()
{
	if (curMat.data)
	{
		showEnlargeDialog();
	}

	return false;
}

void ImageProcessTool::showEnlargeDialog()
{
	QDialog* dialog = new QDialog(this);
	dialog->setFixedSize(200, 140);
	dialog->setWindowTitle("Enlarge");

	int fixedWidth[2];
	fixedWidth[0] = 30;
	fixedWidth[1] = 40;

	QLabel* locLabel[4];
	QLineEdit* locLineEdit[4];
	for (int i = 0; i < 4; i++)
	{
		locLabel[i] = new QLabel(dialog);
		locLabel[i]->setFixedWidth(fixedWidth[i % 2]);
		locLineEdit[i] = new QLineEdit(dialog);
		locLineEdit[i]->setAlignment(Qt::AlignCenter);
		locLineEdit[i]->setText("1");
	}
	locLabel[0]->setText("Top:");
	locLabel[1]->setText("Bottom:");
	locLabel[2]->setText("Left:");
	locLabel[3]->setText("Right:");

	QLabel* colorLabel[3];
	QLineEdit* colorLineEdit[3];
	for (int i = 0; i < 3; i++)
	{
		colorLabel[i] = new QLabel(dialog);
		colorLineEdit[i] = new QLineEdit(dialog);
		colorLineEdit[i]->setAlignment(Qt::AlignCenter);
		colorLineEdit[i]->setText("0");
	}
	colorLabel[0]->setText("R:");
	colorLabel[1]->setText("G:");
	colorLabel[2]->setText("B:");

	QPushButton* cancelButton = new QPushButton("Cancel", dialog);
	QPushButton* okButton = new QPushButton("OK", dialog);
	connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
	connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));

	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	QHBoxLayout* hLayout[4];
	for (int i = 0; i < 4; i++)
	{
		hLayout[i] = new QHBoxLayout(dialog);
	}
	hLayout[0]->addWidget(locLabel[0]);
	hLayout[0]->addWidget(locLineEdit[0]);
	hLayout[0]->addWidget(locLabel[1]);
	hLayout[0]->addWidget(locLineEdit[1]);
	hLayout[1]->addWidget(locLabel[2]);
	hLayout[1]->addWidget(locLineEdit[2]);
	hLayout[1]->addWidget(locLabel[3]);
	hLayout[1]->addWidget(locLineEdit[3]);
	hLayout[2]->addWidget(colorLabel[0]);
	hLayout[2]->addWidget(colorLineEdit[0]);
	hLayout[2]->addWidget(colorLabel[1]);
	hLayout[2]->addWidget(colorLineEdit[1]);
	hLayout[2]->addWidget(colorLabel[2]);
	hLayout[2]->addWidget(colorLineEdit[2]);
	hLayout[3]->addWidget(cancelButton);
	hLayout[3]->addWidget(okButton);
	for (int i = 0; i < 4; i++)
	{
		mainLayout->addLayout(hLayout[i]);
	}

	dialog->setLayout(mainLayout);
	dialog->show();

	if (dialog->exec() == QDialog::Accepted)
	{
		QString locTmpStr[4];
		QString colorTmpStr[3];
		bool locStrEmpty = false;
		bool colorStrEmpty = false;
		for (int i = 0; i < 4; i++)
		{
			locTmpStr[i] = locLineEdit[i]->text();
			locStrEmpty |= locTmpStr[i].isEmpty();
		}
		for (int i = 0; i < 3; i++)
		{
			colorTmpStr[i] = colorLineEdit[i]->text();
			colorStrEmpty |= colorTmpStr[i].isEmpty();
		}

		if (!locStrEmpty && !colorStrEmpty)
		{
			int pixel[4], color[3];
			bool locOk, colorOk, ok = true;
			for (int i = 0; i < 4; i++)
			{
				pixel[i] = locTmpStr[i].toInt(&locOk, 10);
				ok &= locOk;
				if (ok && pixel[i] < 0)
				{
					ok = false;
					return;
				}
			}
			for (int i = 0; i < 3; i++)
			{
				color[i] = colorTmpStr[i].toInt(&colorOk, 10);
				ok &= colorOk;
				if (ok && (color[i] > 255 || color[i] < 0))
				{
					ok = false;
					return;
				}
			}

			if (ok)
			{
				enlarge(pixel[0], pixel[1], pixel[2], pixel[3], color[0], color[1], color[2]);
				statusLabel->setText("Blur successfully!");
			}
			else
			{
				statusLabel->setText("Blur failed!");
			}
		}
		else
		{
			statusLabel->setText("Blur failed!");
		}
	}
	else
	{
		//blurDialog->close();
	}
}

void ImageProcessTool::enlarge(int top, int bottom, int left, int right, int r, int g, int b)
{
	updatePreviousMat();
	int newW = curMat.cols + left + right;
	int newH = curMat.rows + top + bottom;
	cv::resize(curMat, curMat, cv::Size(newW, newH));
	for (int i = 0; i < newH; i++)
	{
		for (int j = 0; j < newW; j++)
		{
			if (i < top || i > preMat.rows + top - 1 || j < left || j > preMat.cols + left - 1)
			{
				curMat.at<cv::Vec3b>(i, j)[0] = b;
				curMat.at<cv::Vec3b>(i, j)[1] = g;
				curMat.at<cv::Vec3b>(i, j)[2] = r;
				//curMat.at<cv::Vec3b>(j, i)[0] = b;
				//curMat.at<cv::Vec3b>(j, i)[1] = g;
				//curMat.at<cv::Vec3b>(j, i)[2] = r;
			}
			else
			{
				for (int k = 0; k < 3; k++)
				{
					curMat.at<cv::Vec3b>(i, j)[k] = preMat.at<cv::Vec3b>(i - top, j - left)[k];
					//curMat.at<cv::Vec3b>(i, j)[k] = preMat.at<cv::Vec3b>(j - left, i - top)[k];
				}
			}
		}
	}

	displayMat(curMat, imgLabel);
}

bool ImageProcessTool::morphology()
{
	if (curMat.data)
	{
		showMorphologyDialog();
	}

	return false;
}

void ImageProcessTool::showMorphologyDialog()
{
	QDialog* dialog = new QDialog(this);
	dialog->setFixedSize(200, 140);
	//dialog->setWindowTitle("Gaussion blur");

	QLabel* operLabel = new QLabel(dialog);
	operLabel->setText("Operation: ");
	operLabel->setFixedWidth(85);
	QComboBox* operComboBox = new QComboBox(dialog);
	operComboBox->insertItem(0, "Erode");
	operComboBox->insertItem(1, "Dilate");
	operComboBox->insertItem(2, "Open");
	operComboBox->insertItem(3, "Close");
	operComboBox->insertItem(4, "Gradient");
	operComboBox->insertItem(5, "Top Hat");
	operComboBox->insertItem(6, "Black Hat");

	QLabel* kernelWidthLabel = new QLabel(dialog);
	kernelWidthLabel->setText("Kernel width: ");
	kernelWidthLabel->setFixedWidth(85);
	QLineEdit* kernelWidthLineEdit = new QLineEdit(dialog);
	kernelWidthLineEdit->setText("15");
	QLabel* kernelHeightLabel = new QLabel(dialog);
	kernelHeightLabel->setText("Kernel height: ");
	kernelHeightLabel->setFixedWidth(85);
	QLineEdit* kernelHeightLineEdit = new QLineEdit(dialog);
	kernelHeightLineEdit->setText("15");

	QPushButton* cancelButton = new QPushButton("Cancel", dialog);
	QPushButton* okButton = new QPushButton("OK", dialog);
	connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
	connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));

	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	QHBoxLayout* hLayout1 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout2 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout3 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout4 = new QHBoxLayout(dialog);
	hLayout1->addWidget(operLabel);
	hLayout1->addWidget(operComboBox);
	hLayout2->addWidget(kernelWidthLabel);
	hLayout2->addWidget(kernelWidthLineEdit);
	hLayout3->addWidget(kernelHeightLabel);
	hLayout3->addWidget(kernelHeightLineEdit);
	hLayout4->addWidget(cancelButton);
	hLayout4->addWidget(okButton);
	mainLayout->addLayout(hLayout1);
	mainLayout->addLayout(hLayout2);
	mainLayout->addLayout(hLayout3);
	mainLayout->addLayout(hLayout4);

	dialog->setLayout(mainLayout);
	dialog->show();

	if (dialog->exec() == QDialog::Accepted)
	{
		QString tmpStr[2];
		tmpStr[0] = kernelWidthLineEdit->text();
		tmpStr[1] = kernelHeightLineEdit->text();

		if (!tmpStr[0].isEmpty() && !tmpStr[1].isEmpty())
		{
			int kerSize[2] = { 3, 3 };
			int index = operComboBox->currentIndex();
			bool ok[2];
			kerSize[0] = tmpStr[0].toInt(&ok[0], 10);
			kerSize[1] = tmpStr[1].toInt(&ok[1], 10);
			if (ok[0] && ok[1])
			{
				morphology(index, kerSize[0], kerSize[1]);
				statusLabel->setText("Morphology operation complete!");
			}
			else
			{
				statusLabel->setText("Morphology operation failed!");
			}
		}
		else
		{
			statusLabel->setText("Morphology operation failed!");
		}
	}
	else
	{
		//blurDialog->close();
	}
}

void ImageProcessTool::morphology(int type, int kernelWidth, int kernelHeight)
{
	updatePreviousMat();

	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelWidth, kernelHeight));

	switch (type)
	{
	case 0:
		cv::erode(curMat, curMat, element);
		break;
	case 1:
		cv::dilate(curMat, curMat, element);
		break;
	case 2:
		cv::morphologyEx(curMat, curMat, cv::MORPH_OPEN, element);
		break;
	case 3:
		cv::morphologyEx(curMat, curMat, cv::MORPH_CLOSE, element);
		break;
	case 4:
		cv::morphologyEx(curMat, curMat, cv::MORPH_GRADIENT, element);
		break;
	case 5:
		cv::morphologyEx(curMat, curMat, cv::MORPH_TOPHAT, element);
		break;
	case 6:
		cv::morphologyEx(curMat, curMat, cv::MORPH_BLACKHAT, element);
		break;
	default:
		break;
	}

	displayMat(curMat, imgLabel);
}

bool ImageProcessTool::removeBlocksSize()
{
	if (curMat.data)
	{
		showRemoveBlocksDialog(0);
	}
	return true;
}

bool ImageProcessTool::removeBlocksRatio()
{
	if (curMat.data)
	{
		showRemoveBlocksDialog(1);
	}
	return true;
}

bool ImageProcessTool::removeBlocksArea()
{
	if (curMat.data)
	{
		showRemoveBlocksDialog(2);
	}
	return false;
}

bool ImageProcessTool::edgeDetectSobel()
{
	if (curMat.data)
	{
		showEdgeDetectDialog(0);
	}

	return false;
}

bool ImageProcessTool::edgeDetectCanny()
{
	if (curMat.data)
	{
		showEdgeDetectDialog(1);
	}

	return false;
}

bool ImageProcessTool::edgeDetectLaplacian()
{
	if (curMat.data)
	{
		edgeDetect(2);
		statusLabel->setText("Edge detection complete!");
	}

	return false;
}

void ImageProcessTool::showEdgeDetectDialog(int type)
{
	QString str[3] = { "Sobel", "Canny", "Laplacian" };
	QDialog* dialog = new QDialog(this);
	dialog->setFixedSize(200, 120);
	dialog->setWindowTitle(str[type] + " edge detect");
	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	QHBoxLayout* hLayout1 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout2 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout3;
	QLabel* paramLabel1 = new QLabel(dialog);
	QLabel* paramLabel2 = new QLabel(dialog);
	QLabel* aperSizeLabel;
	QLineEdit* paramLineEdit1 = new QLineEdit(dialog);
	QLineEdit* paramLineEdit2 = new QLineEdit(dialog);
	QLineEdit* aperSizeLineEdit;

	if (0 == type)
	{
		paramLabel1->setText("dx: ");
		paramLabel2->setText("dy: ");
		paramLineEdit1->setText("1");
		paramLineEdit2->setText("1");
		hLayout1->addWidget(paramLabel1);
		hLayout1->addWidget(paramLineEdit1);
		hLayout2->addWidget(paramLabel2);
		hLayout2->addWidget(paramLineEdit2);
		mainLayout->addLayout(hLayout1);
		mainLayout->addLayout(hLayout2);
	}
	else
	{
		hLayout3 = new QHBoxLayout(dialog);
		aperSizeLabel = new QLabel(dialog);
		aperSizeLineEdit = new QLineEdit(dialog);
		paramLabel1->setText("Threshold 1:   ");
		paramLabel2->setText("Threshold 2:   ");
		aperSizeLabel->setText("Aperture size: ");
		paramLineEdit1->setText("50");
		paramLineEdit2->setText("150");
		aperSizeLineEdit->setText("3");
		hLayout1->addWidget(paramLabel1);
		hLayout1->addWidget(paramLineEdit1);
		hLayout2->addWidget(paramLabel2);
		hLayout2->addWidget(paramLineEdit2);
		hLayout3->addWidget(aperSizeLabel);
		hLayout3->addWidget(aperSizeLineEdit);
		mainLayout->addLayout(hLayout1);
		mainLayout->addLayout(hLayout2);
		mainLayout->addLayout(hLayout3);
	}

	QHBoxLayout* hLayout4 = new QHBoxLayout(dialog);
	QPushButton* cancelButton = new QPushButton("Cancel", dialog);
	QPushButton* okButton = new QPushButton("OK", dialog);
	connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
	connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));
	hLayout4->addWidget(cancelButton);
	hLayout4->addWidget(okButton);
	mainLayout->addLayout(hLayout4);

	dialog->setLayout(mainLayout);
	dialog->show();

	if (dialog->exec() == QDialog::Accepted)
	{
		QString str[3];
		float val[3];
		str[0] = paramLineEdit1->text();
		str[1] = paramLineEdit2->text();
		if (!str[0].isEmpty() && !str[1].isEmpty())
		{
			bool ok[3];
			val[0] = str[0].toFloat(&ok[0]);
			val[1] = str[1].toFloat(&ok[1]);

			switch (type)
			{
			case 0:
				if (ok[0] && ok[1])
				{
					edgeDetect(0, val[0], val[1]);
					statusLabel->setText("Edge detection complete!");
				}
				else
				{
					statusLabel->setText("Edge detection failed!");
				}
				break;
			case 1:
				str[2] = aperSizeLineEdit->text();
				val[2] = str[2].toFloat(&ok[2]);
				if (ok[0] && ok[1] && ok[2])
				{
					edgeDetect(1, val[0], val[1], val[2]);
					statusLabel->setText("Edge detection complete!");
				}
				else
				{
					statusLabel->setText("Edge detection failed!");
				}
				break;
			}
		}
		else
		{
			statusLabel->setText("Edge detection failed!");
		}
	}
	else
	{
		//blurDialog->close();
	}
}

void ImageProcessTool::edgeDetect(int type, float param1, float param2, float param3)
{
	updatePreviousMat();

	cv::Mat gray;
	cv::cvtColor(curMat, gray, CV_BGR2GRAY);
	if (0 == type)
	{
		cv::Sobel(gray, gray, gray.depth(), param1, param2);
	}
	else if (1 == type)
	{
		cv::Canny(gray, gray, param1, param2, param3);
	}
	else if (2 == type)
	{
		cv::Laplacian(gray, gray, gray.depth());
	}
	cv::cvtColor(gray, curMat, CV_GRAY2BGR);
	displayMat(curMat, imgLabel);
}

bool ImageProcessTool::showSourceImage()
{
	if (srcMat.data)
	{
		Util::imShow("Source image", srcMat, true, 800, 600);
	}

	return true;
}

bool ImageProcessTool::backward()
{
	if (preMat.data)
	{
		curMat.copyTo(nexMat);
		preMat.copyTo(curMat);
		preMat.release();
		displayMat(curMat, imgLabel);
	}

	return true;
}

bool ImageProcessTool::forward()
{
	if (nexMat.data)
	{
		updatePreviousMat();
		nexMat.copyTo(curMat);
		nexMat.release();
		displayMat(curMat, imgLabel);
	}

	return true;
}

bool ImageProcessTool::scale()
{
	return false;
}

void ImageProcessTool::gaussionBlur(int kernelSize)
{
	cv::GaussianBlur(curMat, curMat, cv::Size(kernelSize, kernelSize), 0, 0, cv::BORDER_DEFAULT);
	displayMat(curMat, imgLabel);
}

void ImageProcessTool::showRemoveBlocksDialog(int type)
{
	QString str[3] = { "size", "ratio", "area" };
	QDialog* dialog = new QDialog(this);
	dialog->setFixedSize(200, 120);
	dialog->setWindowTitle("Remove blocks by " + str[type]);

	QLabel* minVLabel = new QLabel(dialog);
	QLabel* maxVLabel = new QLabel(dialog);
	QLabel* minHeightLabel;
	QLabel* maxHeightLabel;
	QLineEdit* minVLineEdit = new QLineEdit(dialog);
	QLineEdit* maxVLineEdit = new QLineEdit(dialog);
	QLineEdit* minHeightLineEdit;
	QLineEdit* maxHeightLineEdit;

	QPushButton* cancelButton = new QPushButton("Cancel", dialog);
	QPushButton* okButton = new QPushButton("OK", dialog);
	connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
	connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));

	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	QHBoxLayout* hLayout1 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout2 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout3 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout4 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout5 = new QHBoxLayout(dialog);
	hLayout1->addWidget(minVLabel);
	hLayout1->addWidget(minVLineEdit);
	hLayout2->addWidget(maxVLabel);
	hLayout2->addWidget(maxVLineEdit);
	hLayout5->addWidget(cancelButton);
	hLayout5->addWidget(okButton);
	mainLayout->addLayout(hLayout1);
	mainLayout->addLayout(hLayout2);
	switch (type)
	{
	case 0:
		dialog->setFixedSize(200, 200);
		minHeightLabel = new QLabel(dialog);
		maxHeightLabel = new QLabel(dialog); 
		minHeightLineEdit = new QLineEdit(dialog);
		maxHeightLineEdit = new QLineEdit(dialog);
		minVLabel->setText(tr("Minimum width"));
		maxVLabel->setText(tr("Maximum width"));
		minHeightLabel->setText(tr("Minimum height"));
		maxHeightLabel->setText(tr("Maximum height"));
		minVLineEdit->setText(tr("20"));
		maxVLineEdit->setText(tr("10000"));
		minHeightLineEdit->setText(tr("20"));
		maxHeightLineEdit->setText(tr("10000"));
		hLayout3->addWidget(minHeightLabel);
		hLayout3->addWidget(minHeightLineEdit);
		hLayout4->addWidget(maxHeightLabel);
		hLayout4->addWidget(maxHeightLineEdit);
		mainLayout->addLayout(hLayout3);
		mainLayout->addLayout(hLayout4);
		break;
	case 1:
		minVLabel->setText(tr("Minimum ratio"));
		maxVLabel->setText(tr("Maximum ratio"));
		minVLineEdit->setText(tr("0.1"));
		maxVLineEdit->setText(tr("10"));
		break;
	case 2:
		minVLabel->setText(tr("Minimum area"));
		maxVLabel->setText(tr("Maximum area"));
		minVLineEdit->setText(tr("100"));
		maxVLineEdit->setText(tr("10000000"));
		break;
	}
	mainLayout->addLayout(hLayout5);

	dialog->setLayout(mainLayout);
	dialog->show();

	if (dialog->exec() == QDialog::Accepted)
	{
		QString str[4];
		float val[4];
		str[0] = minVLineEdit->text();
		str[1] = maxVLineEdit->text();
		if (!str[0].isEmpty() && !str[1].isEmpty())
		{
			bool ok[4];
			val[0] = str[0].toFloat(&ok[0]);
			val[1] = str[1].toFloat(&ok[1]);

			switch (type)
			{
			case 0:
				str[2] = minHeightLineEdit->text();
				str[3] = maxHeightLineEdit->text();
				val[2] = str[2].toFloat(&ok[2]);
				val[3] = str[3].toFloat(&ok[3]);
				if (ok[0] && ok[1] && ok[2] && ok[3])
				{
					removeBlocks(0, val[0], val[1], val[2], val[3]);
					statusLabel->setText("Blocks remove complete!");
				}
				else
				{
					statusLabel->setText("Blocks remove failed!");
				}
				break;
			case 1:
				if (ok[0] && ok[1])
				{
					removeBlocks(1, val[0], val[1]);
					statusLabel->setText("Blocks remove complete!");
				}
				else
				{
					statusLabel->setText("Blocks remove failed!");
				}
				break;
			case 2:
				if (ok[0] && ok[1])
				{
					removeBlocks(2, val[0], val[1]);
					statusLabel->setText("Blocks remove complete!");
				}
				else
				{
					statusLabel->setText("Blocks remove failed!");
				}
				break;
			}
		}
		else
		{
			statusLabel->setText("Blocks remove failed!");
		}
	}
	else
	{
		//blurDialog->close();
	}
}

void ImageProcessTool::showColorMatchDialog()
{
	QDialog* dialog = new QDialog(this);
	dialog->setFixedSize(200, 140);
	dialog->setWindowTitle("Color match");

	QLabel* minHLabel = new QLabel(dialog);
	QLabel* maxHLabel = new QLabel(dialog);
	QLabel* minSLabel = new QLabel(dialog);
	QLabel* maxSLabel = new QLabel(dialog);
	QLabel* minVLabel = new QLabel(dialog);
	QLabel* maxVLabel = new QLabel(dialog);
	minHLabel->setText("Min H: ");
	maxHLabel->setText("Max H: ");
	minSLabel->setText("Min S: ");
	maxSLabel->setText("Max S: ");
	minVLabel->setText("Min V: ");
	maxVLabel->setText("Max V: ");

	QLineEdit* minHLineEdit = new QLineEdit(dialog);
	QLineEdit* maxHLineEdit = new QLineEdit(dialog);
	QLineEdit* minSLineEdit = new QLineEdit(dialog);
	QLineEdit* maxSLineEdit = new QLineEdit(dialog);
	QLineEdit* minVLineEdit = new QLineEdit(dialog);
	QLineEdit* maxVLineEdit = new QLineEdit(dialog);
	minHLineEdit->setAlignment(Qt::AlignCenter);
	maxHLineEdit->setAlignment(Qt::AlignCenter);
	minSLineEdit->setAlignment(Qt::AlignCenter);
	maxSLineEdit->setAlignment(Qt::AlignCenter);
	minVLineEdit->setAlignment(Qt::AlignCenter);
	maxVLineEdit->setAlignment(Qt::AlignCenter);
	minHLineEdit->setText("70");
	maxHLineEdit->setText("120");
	minSLineEdit->setText("30");
	maxSLineEdit->setText("255");
	minVLineEdit->setText("30");
	maxVLineEdit->setText("255");

	QPushButton* cancelButton = new QPushButton("Cancel", dialog);
	QPushButton* okButton = new QPushButton("OK", dialog);
	connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
	connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));

	QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
	QHBoxLayout* hLayout1 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout2 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout3 = new QHBoxLayout(dialog);
	QHBoxLayout* hLayout4 = new QHBoxLayout(dialog);
	hLayout1->addWidget(minHLabel);
	hLayout1->addWidget(minHLineEdit);
	hLayout1->addWidget(maxHLabel);
	hLayout1->addWidget(maxHLineEdit);
	hLayout2->addWidget(minSLabel);
	hLayout2->addWidget(minSLineEdit);
	hLayout2->addWidget(maxSLabel);
	hLayout2->addWidget(maxSLineEdit);
	hLayout3->addWidget(minVLabel);
	hLayout3->addWidget(minVLineEdit);
	hLayout3->addWidget(maxVLabel);
	hLayout3->addWidget(maxVLineEdit);
	hLayout4->addWidget(cancelButton);
	hLayout4->addWidget(okButton);
	mainLayout->addLayout(hLayout1);
	mainLayout->addLayout(hLayout2);
	mainLayout->addLayout(hLayout3);
	mainLayout->addLayout(hLayout4);

	dialog->setLayout(mainLayout);
	dialog->show();

	if (dialog->exec() == QDialog::Accepted)
	{
		QString str[6];
		float val[6];
		str[0] = minHLineEdit->text();
		str[1] = maxHLineEdit->text();
		str[2] = minSLineEdit->text();
		str[3] = maxSLineEdit->text();
		str[4] = minVLineEdit->text();
		str[5] = maxVLineEdit->text();
		if (!str[0].isEmpty() && !str[1].isEmpty())
		{
			bool ok[6];
			bool okAll = true;
			for (int i = 0; i < 6; i++)
			{
				val[i] = str[i].toFloat(&ok[i]);
				okAll = okAll && ok[i];
			}
			if (okAll)
			{
				colorMatchByHSV(val[0], val[1], val[2], val[3], val[4], val[5]);
				
				statusLabel->setText("Color match complete!");
			}
		}
		else
		{
			statusLabel->setText("Color match failed!");
		}
	}
	else
	{
		//blurDialog->close();
	}
}

void ImageProcessTool::removeBlocks(int type, float minV, float maxV, float minHeight, float maxHeight)
{
	updatePreviousMat();

	cv::Mat tmpMat;
	cv::cvtColor(curMat, tmpMat, CV_BGR2GRAY);
	switch (type)
	{
	case 0:
		Proc::removeBlocksBySize(tmpMat, minV, maxV, minHeight, maxHeight);
		break;
	case 1:
		Proc::removeBlocksByRatio(tmpMat, minV, maxV);
		break;
	case 2:
		Proc::removeBlocksByArea(tmpMat, minV, maxV);
		break;
	}
	cv::cvtColor(tmpMat, curMat, CV_GRAY2BGR);
	displayMat(curMat, imgLabel);
}

void ImageProcessTool::colorMatchByHSV(int minH, int maxH, int minS, int maxS, int minV, int maxV)
{
	if (curMat.channels() > 2)
	{
		updatePreviousMat();

		cv::Mat color = Proc::colorMatch(curMat, minH, maxH, minS, maxS, minV, maxV);
		float scale = 1 / imgLabel->getScale();
		//float scale = imgLabel->getZoomScale() / imgLabel->getScale();
			//* ((curMat.rows * imgLabel->height() > curMat.cols * imgLabel->width())
			//	? (1.0 * curMat.rows / imgLabel->width())
			//	: (1.0 * curMat.cols / imgLabel->height()));
		for (int i = 0; i < curMat.rows; i++)
		{
			for (int j = 0; j < curMat.cols; j++)
			{
				if (color.at<uchar>(i, j) < 100)
				{
					int gapI = (int)(i / scale) % 20;
					int gapJ = (int)(j / scale) % 20;
					if ((gapI > 9) ^ (gapJ > 9))
					{
						for (int k = 0; k < 3; k++)
						{
							curMat.at<cv::Vec3b>(i, j)[k] = 255;
						}
					}
					else
					{
						for (int k = 0; k < 3; k++)
						{
							curMat.at<cv::Vec3b>(i, j)[k] = 204;
						}
					}
				}
			}
		}

		displayMat(curMat, imgLabel);
	}
}

void ImageProcessTool::updatePreviousMat()
{
	curMat.copyTo(preMat);
}

void ImageProcessTool::displayMat(cv::Mat imgMat, QImageLabel * qLabel)
{
	QImage qImage;
	//QSize qtS = qLabel->size();
	//cv::Size cvS = getScaledSize(imgMat, qtS);
	//ratio = (double)cvS.width / imgMat.cols;
	//cv::resize(imgMat, imgMat, cvS);
	qImage = convertMatToQImage(imgMat);
	qLabel->setPixmap(QPixmap::fromImage(qImage));//显示
}

cv::Size ImageProcessTool::getScaledSize(cv::Size imgSize, QSize qSize)
{
	cv::Size scaledSize;
	if (imgSize.height > qSize.height() || imgSize.width > qSize.width())
	{
		if (imgSize.height*qSize.width() > imgSize.width*qSize.height())
		{
			scaledSize.height = qSize.height();
			scaledSize.width = qSize.height()*imgSize.width / imgSize.height;
		}
		else
		{
			scaledSize.width = qSize.width();
			scaledSize.height = qSize.width()*imgSize.height / imgSize.width;
		}
	}
	else
	{
		scaledSize.height = imgSize.height;
		scaledSize.width = imgSize.width;
	}

	return scaledSize;
}

cv::Size ImageProcessTool::getScaledSize(cv::Mat src, QSize qSize)
{
	cv::Size scaledSize;
	scaledSize.height = src.rows;
	scaledSize.width = src.cols;
	if (src.rows > qSize.height() || src.cols > qSize.width())
	{
		scaledSize = getScaledSize(scaledSize, qSize);
	}

	return scaledSize;
}

QImage ImageProcessTool::convertMatToQImage(cv::Mat src)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1  
	if (src.type() == CV_8UC1)
	{
		QImage image(src.cols, src.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat  
		uchar *pSrc = src.data;
		for (int row = 0; row < src.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, src.cols);
			pSrc += src.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if (src.type() == CV_8UC3)
	{
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)src.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, src.cols, src.rows, src.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (src.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)src.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}

void ImageProcessTool::mousePressEvent(QMouseEvent * event)
{
	QString str = "(" + QString::number(event->x()) + "," + QString::number(event->y()) + ")";
	// ui->lineEdit->setText(tr("鼠标按下")+str);
	if (event->button() == Qt::LeftButton)
	{
		isMouseLeftPressed = true;
		mouseLastPosX = event->localPos().x();
		mouseLastPosY = event->localPos().y();
	}
	else if (event->button() == Qt::RightButton)
	{
		isMouseRightPressed = true;
	}
	else if (event->button() == Qt::MidButton)
	{
		
	}
}

void ImageProcessTool::mouseMoveEvent(QMouseEvent* event)
{
	if (!isMouseLeftPressed && !isMouseRightPressed)
	{
		int x = (event->pos().x() - (800 - srcMat.cols*ratio + 1) / 2) / ratio;
		int y = (event->pos().y() - (600 - srcMat.rows*ratio + 1) / 2) / ratio;
		if (x >= 0 && x < srcMat.cols && y >= 0 && y < srcMat.rows)
		{
			coordsLabel->setText("X: " + QString::number(x, 10) + "\t  Y: " + QString::number(y, 10));
		}
	}
	else
	{

	}
}

void ImageProcessTool::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		isMouseLeftPressed = false;
	}
	else if (event->button() == Qt::RightButton)
	{
		isMouseRightPressed = false;
	}
	else if (event->button() == Qt::MidButton)
	{
		
	}
}