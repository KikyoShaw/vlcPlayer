#pragma once

#include <QSlider>
#include <QMouseEvent>
#include <QCoreApplication>

class CustomSlider : public QSlider
{
	Q_OBJECT
public:
	CustomSlider(QWidget *parent = 0);
protected:
	virtual void mousePressEvent(QMouseEvent *event);//重写QSlider的mousePressEvent事件
	virtual void mouseMoveEvent(QMouseEvent *event); //重写QSlider的mouseMoveEvent事件
	virtual void mouseReleaseEvent(QMouseEvent *event);//重写QSlider的mouseReleaseEvent事件

signals:
	void costomSliderClicked();//自定义的鼠标单击信号，用于捕获并处理
	void costomSliderMove();
	void costomSliderRelease();

public:
	int widget_x = 0;
	int widget_y = 0;
};
