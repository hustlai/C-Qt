#pragma once

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QPen>
#include <QPainter>

class QImageLabel : public QLabel
{
	Q_OBJECT

public:
	QImageLabel(const QString &text, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~QImageLabel();

	void showImage();
	void moveImage();
	void keyPressed(QKeyEvent* event);
	void mousePressed(QMouseEvent* event);
	void mouseMoved(QMouseEvent* event);
	void mouseReleased(QMouseEvent* event);
	void mouseDoubliClick(QMouseEvent* event);
	void wheelRoll(QWheelEvent* event);
	using QLabel::setPixmap;
	void setPixmap(const QPixmap & pixmap);
	void updatePixmap(const QPixmap & pixmap);
	float getScale();
	float getZoomScale();
	void initStartXY();

public slots:

protected:
	void keyPressEvent(QKeyEvent* event);  // listen to key press event
	void mousePressEvent(QMouseEvent* event);  // listen to mouse event
	void mouseMoveEvent(QMouseEvent* event);  // listen to mouse event
	void mouseReleaseEvent(QMouseEvent* event);  // listen to mouse event
	void mouseDoubleClickEvent(QMouseEvent* event);
	void wheelEvent(QWheelEvent* event);
	//void paintEvent(QPaintEvent * paintEvent);

private:
	int imageStartX = 0;
	int imageStartY = 0;
	int mouseLastPosX = 0;
	int mouseLastPosY = 0;
	int mouseNewPosX = 0;
	int mouseNewPosY = 0;
	float newZoomScale = 1.0;
	float lastZoomScale = 1.0;
	float zoomMax = 10.0;
	float zoomMin = 0.02;
	float zoomRatioX = 1.0;
	float zoomRatioY = 1.0;
	bool isImageDisplayed = false;
	bool isMouseLeftPressed = false;
	bool isMouseRightPressed = false;

	QImage image;
};
