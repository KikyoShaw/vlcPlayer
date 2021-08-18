#pragma once

#ifndef VSLIDERWIDGET_H
#define VSLIDERWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QTimer>
#include "ui_vsliderwidget.h"
#include "customslider.h"

class VSliderWidget : public QWidget
{
	Q_OBJECT

public:
	VSliderWidget(QWidget *parent = Q_NULLPTR);
	~VSliderWidget();

public:
	void setVoiceValue(int value);
	void setMValue(int value);
	int getVoliceValue();
	void setVoiceValueWhenCurZero(int value);

private slots:
	void sltVoiceValue(int value);
	void sltBanSliding();

signals:
	void sigValueChanged(int value);

protected:
	virtual void wheelEvent(QWheelEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);

private:
	Ui::VSliderWidget ui;
	int widget_y = 0;
	int m_value = 0;
	QTimer m_timer;
	int num_timer = 0;
	int m_lastValue = 0; //上一次值
};

#endif // VSLIDERWIDGET_H
