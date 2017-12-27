#include "qimagelabel.h"

QImageLabel::QImageLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
	: QLabel(parent)
{
	//imageStartX = 0;
	//imageStartY = 0;
	//mouseLastPosX = 0;
	//mouseLastPosY = 0;
	//mouseNewPosX = 0;
	//mouseNewPosY = 0;
	//zoomScale = 1.0;
	//zoomMax = 10.0;
	//zoomMin = 0.1;
	//zoomRatioX = 0.0;
	//zoomRatioY = 0.0;
	//isImageDisplayed = 0.0;

	//this->setStyleSheet("background-color:white");
	this->setAttribute(Qt::WA_TranslucentBackground, false);
}

QImageLabel::~QImageLabel()
{
}

void QImageLabel::keyPressed(QKeyEvent * event)
{

}

void QImageLabel::mousePressed(QMouseEvent * event)
{
	if (isImageDisplayed)
	{
		// select edge point
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
	}
}

void QImageLabel::mouseMoved(QMouseEvent * event)
{
	if (isImageDisplayed) {
		if (isMouseLeftPressed) {
			mouseNewPosX = event->localPos().x();
			mouseNewPosY = event->localPos().y();
			moveImage();
		}
	}
}

void QImageLabel::mouseReleased(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		isMouseLeftPressed = false;
	}
	else if (event->button() == Qt::RightButton) {
		isMouseRightPressed = false;
	}
}

void QImageLabel::mouseDoubliClick(QMouseEvent * event)
{
}

void QImageLabel::wheelRoll(QWheelEvent * event)
{
	// get the delta
	// float deltaDeg = event->delta()*1.0 / 640;
	float deltaDeg = event->delta()*1.0 / 1500;
	lastZoomScale = newZoomScale;

	// case1 : front image opened
	if (isImageDisplayed) {

		// zoom in and zoom out
		// update scale ratio
		if (newZoomScale + deltaDeg > zoomMax || newZoomScale + deltaDeg < zoomMin) {
			return;
		}
		newZoomScale += deltaDeg;
		int width = image.width();
		zoomRatioX = image.width()*1.0 / (newZoomScale*this->width());
		zoomRatioY = image.height()*1.0 / (newZoomScale*this->height());
		// imageStartX = (image.width() * newZoomScale - event->pos().x()) / 2 - 1;
		// imageStartY = (image.height() * newZoomScale - event->pos().y()) / 2 - 1;
		imageStartX = (imageStartX + event->pos().x()) * newZoomScale / lastZoomScale - event->pos().x();
		imageStartY = (imageStartY + event->pos().y()) * newZoomScale / lastZoomScale - event->pos().y();
		if (imageStartX < 0)
		{
			imageStartX = 0;
		}
		if (imageStartY < 0)
		{
			imageStartY = 0;
		}

		// show image
		showImage();
	}
}

void QImageLabel::keyPressEvent(QKeyEvent * event)
{
	emit keyPressed(event);
}

void QImageLabel::mousePressEvent(QMouseEvent * event)
{
	emit mousePressed(event);
}

void QImageLabel::mouseMoveEvent(QMouseEvent * event)
{
	emit mouseMoved(event);
}

void QImageLabel::mouseReleaseEvent(QMouseEvent * event)
{
	emit mouseReleased(event);
}

void QImageLabel::mouseDoubleClickEvent(QMouseEvent * event)
{
	emit mouseDoubliClick(event);
}

void QImageLabel::wheelEvent(QWheelEvent * event)
{
	emit wheelRoll(event);
}

void QImageLabel::setPixmap(const QPixmap & pixmap)
{
	isImageDisplayed = true;

	image = pixmap.toImage().copy();
	
	//zoomScale = getScale();
	initStartXY();
	showImage();
	//if (image != NULL)
	//{
	//	delete image;
	//}
	//else
	//{
	//	image = new QImage();
	//}
	//QImage* newImg = &(pixmap.toImage());

	//QLabel::setPixmap(pixmap);
}

void QImageLabel::updatePixmap(const QPixmap & pixmap)
{
	QLabel::setPixmap(pixmap);
}

float QImageLabel::getScale()
{
	float scale;

	if (image.width() > this->width() || image.height() > this->height())
	{
		if (image.width() * this->height() > image.height() * this->width())
		{
			scale = this->width() * 1.0 / image.width();
		}
		else
		{
			scale = this->height() * 1.0 / image.height();
		}
	}
	else
	{
		scale = 1.0;
	}

	return scale;
}

float QImageLabel::getZoomScale()
{
	return newZoomScale;
}

void QImageLabel::initStartXY()
{
	float scale = getScale();
	int scaledWidth = image.width() * scale;
	int scaledHeight = image.height() * scale;
	imageStartX = 0;
	imageStartY = 0;
	if (scaledWidth < this->width())
	{
		imageStartX = (scaledWidth - this->width()) / 2 - 1;
	}
	if (scaledHeight < this->height())
	{
		imageStartY = (scaledHeight - this->height()) / 2 - 1;
	}

	newZoomScale = scale;
}

//void QImageLabel::paintEvent(QPaintEvent * paintEvent)
//{
//	QPainter painter(this);
//	QPen pen = painter.pen();
//	pen.setWidth(20);//改变线宽
//	pen.setColor(Qt::red);
//	painter.setPen(pen);
//	painter.drawPoint(mousePos_X, mousePos_Y);
//
//	QImage img(":/images/tz.jpg");
//	QPoint pt(mousePos_X, mousePos_Y);
//	painter.drawImage(pt, img);
//
//	painter.end();
//}

void QImageLabel::showImage()
{
	// test if qimage_front_image is not opened
	if (image.isNull()) {
		return;
	}

	// show image 
	QImage qImageTmp = image.copy().scaled(QSize(image.width()*newZoomScale, image.height()*newZoomScale), Qt::IgnoreAspectRatio);

	// judge if the start_x or start_y is legal
	if (imageStartX + this->width() > image.width()*newZoomScale) {
		imageStartX = (image.width()*newZoomScale - this->width()) / 2;
	}
	if (imageStartY + this->height() > image.height()*newZoomScale) {
		imageStartY = (image.height()*newZoomScale - this->height()) / 2;
	}

	QImage qImageScaled = QImage(this->width(), this->height(), QImage::Format_ARGB32);
	//QPainter painter(&qImageScaled);
	//painter.setCompositionMode(QPainter::CompositionMode_Source);
	//painter.fillRect(qImageScaled.rect(), Qt::red);
	//painter.setBackground(Qt::transparent);
	//painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	//painter.end();
	//qImageScaled.convertToFormat(QImage::Format_ARGB32);

	int scaledWidth = image.width()*newZoomScale;
	int scaledHeight = image.height()*newZoomScale;
	for (int i = 0; i < this->width(); i++)
	{
		for (int j = 0; j < this->height(); j++)
		{
			int x = i + imageStartX;
			int y = j + imageStartY;
			if (x >= 0 && y >= 0 && x < scaledWidth && y < scaledHeight)
			{
				qImageScaled.setPixelColor(i, j, qImageTmp.pixelColor(x, y));
			}
		}
	}

	//qImageScaled = qImageTmp.scaled(QSize(image.width()*newZoomScale, image.height()*newZoomScale), Qt::IgnoreAspectRatio).copy(imageStartX, imageStartY, this->width(), this->height());
	//
	//QColor imgBack(0, 0, 0);
	//QColor imgBackA(240, 240, 240, 0);
	//int lb = -imageStartX;
	//int rb = this->width() + imageStartX - 3;
	//int tb = -imageStartY;
	//int bb = this->height() + imageStartY - 3;
	//for (int i = 0; i< qImageScaled.width(); ++i)
	//{
	//	for (int j = 0; j<qImageScaled.height(); ++j)
	//	{
	//		//如果身份证背景色等于 Color(254,254,254,255),则设置为透明色 Color(254,254,254,0)

	//		/*if (qImageScaled.pixelColor(i, j) == imgBack)
	//		{
	//			qImageScaled.setPixelColor(i, j, imgBackA);
	//		}*/

	//		if (i < lb || i > rb || j < tb || j > bb)
	//		{
	//			qImageScaled.setPixelColor(i, j, imgBackA);
	//		}
	//	}
	//}
	////qImageScaled->setColor(QPalette::Background, Qt::transparent);
	this->updatePixmap(QPixmap::fromImage(qImageScaled));
}

void QImageLabel::moveImage()
{
	// compute the delta
	int deltaX = -1 * (mouseNewPosX - mouseLastPosX);
	int deltaY = -1 * (mouseNewPosY - mouseLastPosY);

	int dist = deltaX*deltaX + deltaY*deltaY;
	if (dist < 10) {
		return;
	}

	// start x exceeds the left edge
	if (imageStartX + deltaX < 0) {
		imageStartX = 0;
	}
	else if (imageStartX + deltaX + this->width() > int(newZoomScale*image.width() - 1)) {
		//start x exceeds the right edge
		imageStartX = int(newZoomScale*image.width() - this->width()) - 1;
	}
	else {
		imageStartX += deltaX;
	}

	// start y exceeds the top edge
	if (imageStartY + deltaY < 0) {
		imageStartY = 0;
	}
	else if (imageStartY + deltaY + this->height() > int(newZoomScale*image.height() - 1)) {
		//start y exceeds the bottom edge
		imageStartY = int(newZoomScale*image.height() - this->height()) - 1;
	}
	else {
		imageStartY += deltaY;
	}

	// update the mouse last position 
	mouseLastPosX = mouseNewPosX;
	mouseLastPosY = mouseNewPosY;

	// show image 
	showImage();
}