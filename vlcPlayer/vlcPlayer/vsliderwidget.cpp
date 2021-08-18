#include "vsliderwidget.h"
#include <QPalette>

VSliderWidget::VSliderWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//setFocusPolicy(Qt::StrongFocus);
	this->installEventFilter(this);

	ui.widget_tooltip->hide();
	ui.Slider->setMaximum(100);
	this->resize(32, 170);

	connect(ui.Slider, &CustomSlider::costomSliderClicked, this, [=]() {
		ui.widget_tooltip->move(ui.widget_tooltip->x(), ui.Slider->widget_y + 10);
		this->resize(60, 170);
		ui.widget_tooltip->show();
	});
	connect(ui.Slider, &CustomSlider::costomSliderRelease, this, [=]() {
		ui.widget_tooltip->hide();
		this->resize(32, 170);
	});
	connect(ui.Slider, &CustomSlider::costomSliderMove, this, [=]() {
		widget_y = ui.Slider->widget_y;
	});
	connect(ui.Slider, &CustomSlider::sliderMoved, this, [=]() {
		if (ui.Slider->value() > 0 && ui.Slider->value() < 100)
		{
			ui.widget_tooltip->move(ui.widget_tooltip->x(), widget_y + 10);
		}
	});

	connect(ui.Slider, &QSlider::valueChanged, this, &VSliderWidget::sltVoiceValue);

	////静音按钮
	//connect(ui.pushButton, &QPushButton::clicked, this, &VSliderWidget::sltBanSliding);
}

VSliderWidget::~VSliderWidget()
{

}

void VSliderWidget::setVoiceValue(int value)
{
	//滑动条设置同一个值处理
	int num = value > 0 ? value - 1 : value + 1;
	if (m_value == value)
		ui.Slider->setValue(num);
	ui.Slider->setValue(value);
	m_value = value;
}

void VSliderWidget::setMValue(int value)
{
	m_value = value;
}

int VSliderWidget::getVoliceValue()
{
	return m_value;
}

void VSliderWidget::setVoiceValueWhenCurZero(int value)
{
	if (0 == ui.Slider->value())
	{
		ui.Slider->setValue(value);
		m_value = value;
	}
}

void VSliderWidget::sltBanSliding()
{
	if (0 == m_value) {
		ui.Slider->setValue(m_lastValue);
		emit sigValueChanged(m_lastValue);
	}
	else {
		m_lastValue = m_value;
		ui.Slider->setValue(0);
		emit sigValueChanged(0);
	}
}

void VSliderWidget::sltVoiceValue(int value)
{
	m_value = value;
	ui.label_num->setText(QString::number(value));
	emit sigValueChanged(value);
}

void VSliderWidget::wheelEvent(QWheelEvent * event)
{
	int value = m_value;
	if (event->delta() > 0)//上滑
	{
		value += 2;
		if (value > 100)
			value = 100;
	}
	else//下滑
	{
		value -= 2;
		if (value < 0)
			value = 0;
	}

	setVoiceValue(value);
	QWidget::wheelEvent(event);
}

bool VSliderWidget::eventFilter(QObject * obj, QEvent * event)
{
	if (obj != this || event->type() == QEvent::Leave)
	{
		hide();
	}
	return QWidget::eventFilter(obj, event);
}


