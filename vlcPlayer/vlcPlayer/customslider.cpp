#include "customslider.h"
#include <QDebug>

CustomSlider::CustomSlider(QWidget *parent)
	:QSlider(parent)
{

	/*setStyleSheet("QSlider {background:rgba(52,7,91,100%);border-color:rgba(52,7,91,100%)}"
		"QSlider::groove:vertical {background:#cbcbcb;width:6px;border-radius:1px;padding-left:-1px;padding-right:-1px;padding-top:-1px;padding-bottom:-1px;}"
		"QSlider::sub-page:vertical{ background:#cbcbcb;border-radius:2px;}"
		"QSlider::add-page:vertical {  background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,stop : 0 #439cf4, stop:1 #439cf4);background: qlineargradient(x1 : 0, y1 : 0.2, x2 : 1, y2 : 1,stop : 0 #439cf4, stop: 1 #439cf4);width:10px;border-radius:2px;}"
		"QSlider::handle:vertical {margin: -2px - 7px - 2px - 7px;height: 17px;}"
	);

	setRange(0, 100);*/

	//setFocusPolicy(Qt::StrongFocus);

	// 保证点击游标时，不会产生中间值，而是一次性跳跃到点击处
	setPageStep(0);

}

void CustomSlider::mousePressEvent(QMouseEvent *event)
{
	//注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
	QSlider::mousePressEvent(event);
	//获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
	double pos;
	if (this->orientation() == Qt::Horizontal)
	{   //水平Slider
		widget_x = event->pos().x();
		pos = event->pos().x() / (double)width();
		setValue(pos * (maximum() - minimum()) + minimum());
	}
	else if (this->orientation() == Qt::Vertical)
	{   //垂直Slider
		widget_y = event->pos().y();
		pos = event->pos().y() / (double)height();
		setValue(maximum() - pos * (maximum() - minimum()));
	}

	//发送自定义的鼠标单击信号
	emit costomSliderClicked();
}

void CustomSlider::mouseMoveEvent(QMouseEvent * event)
{
	QSlider::mouseMoveEvent(event);
	double pos;
	if (this->orientation() == Qt::Horizontal)
	{   //水平Slider
		widget_x = event->pos().x();
	}
	else if (this->orientation() == Qt::Vertical)
	{   //垂直Slider
		widget_y = event->pos().y();
	}

	emit costomSliderMove();
}

void CustomSlider::mouseReleaseEvent(QMouseEvent * event)
{
	QSlider::mouseReleaseEvent(event);

	emit costomSliderRelease();
}