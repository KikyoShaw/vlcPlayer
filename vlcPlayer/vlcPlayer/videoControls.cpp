#include "videoControls.h"
#include "vsliderwidget.h"

const char* Property_Rate = "RateValue";

VideoControls::VideoControls(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint/* | Qt::Tool*/);
	setAttribute(Qt::WA_TranslucentBackground);

	initVolumeSlider();

	//播放/暂停
	connect(ui.pushButton_play, &QPushButton::clicked, this, &VideoControls::sltVideoPlayOrPause);
	//声音
	connect(ui.pushButton_volum, &QPushButton::clicked, this, &VideoControls::sltVolumControls);
	//进度
	connect(ui.slider_progress, &CustomSlider::costomSliderClicked, this, &VideoControls::sltSliderProgressClicked);
	connect(ui.slider_progress, &CustomSlider::sliderReleased, this, &VideoControls::sltSliderProgressReleased);
	//倍速
	ui.pushButton_rate->setProperty(Property_Rate, 1.0);
	connect(ui.pushButton_rate, &QPushButton::clicked, this, &VideoControls::sltSetVideoRate);
}

VideoControls::~VideoControls()
{
}

void VideoControls::setProgressDuration(qint64 duration)
{
	ui.slider_progress->setMaximum(duration);
}

bool VideoControls::isSliderDown()
{
	return ui.slider_progress->isSliderDown();
}

void VideoControls::setSliderPosition(int position)
{
	ui.slider_progress->setSliderPosition(position);
}

void VideoControls::setProgressText(const QString & text)
{
	ui.label_volumNum->setText(text);
}

void VideoControls::setPlaying(bool state)
{
	//启用播放/暂停按钮，并将其文本设置为“暂停”
	ui.pushButton_play->setChecked(state);
}

void VideoControls::setVoiceValue(int value)
{
	m_volumeSlider->setVoiceValue(value);
}

bool VideoControls::getVolumVisible()
{
	if (m_volumeSlider) {
		return m_volumeSlider->isVisible();
	}
	return false;
}

void VideoControls::initVolumeSlider()
{
	m_volumeSlider = QSharedPointer<VSliderWidget>(new VSliderWidget(this));
	if (m_volumeSlider) {
		m_volumeSlider->installEventFilter(this);
		m_volumeSlider->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
		connect(m_volumeSlider.data(), &VSliderWidget::sigValueChanged, this, &VideoControls::sltSoundVoiceValue);
	}
}

void VideoControls::locateWidgets()
{
	if (m_volumeSlider) {
		int posX = ui.pushButton_volum->x() + 6;
		int posY = -m_volumeSlider->height();
		m_volumeSlider->move(mapToGlobal(QPoint(posX, posY)));
	}
}

void VideoControls::closeWidget()
{
	if (m_volumeSlider) {
		m_volumeSlider->close();
	}
}

void VideoControls::sltSliderProgressClicked()
{
	auto value = ui.slider_progress->value();
	emit sigSetPosition(value);
}

void VideoControls::sltSliderProgressReleased()
{
	auto value = ui.slider_progress->value();
	emit sigSetPosition(value);
}

void VideoControls::sltSoundVoiceValue(int value)
{
	if (0 == value) {
		ui.pushButton_volum->setStyleSheet("border-image: url(:/qrc/qrc/yyf_ico_yl_gb.png);");
	}
	else {
		ui.pushButton_volum->setStyleSheet("border-image: url(:/qrc/qrc/yyf_ico_yl.png);");
	}
	emit sigSoundVoiceValue(value);
}

void VideoControls::sltVolumControls()
{
	if (m_volumeSlider) {
		locateWidgets();
		auto isVisible = m_volumeSlider->isVisible();
		m_volumeSlider->setVisible(!isVisible);
	}
}

void VideoControls::sltSetVideoRate()
{
	auto rate = ui.pushButton_rate->property(Property_Rate).toFloat();
	if (1.0 == rate) {
		rate = 1.5;
	}
	else if (1.5 == rate) {
		rate = 2.0;
	}
	else if (2.0 == rate) {
		rate = 3.0;
	}
	else if (3.0 == rate) {
		rate = 0.5;
	}
	else if (0.5 == rate) {
		rate = 1.0;
	}
	ui.pushButton_rate->setProperty(Property_Rate, rate);
	ui.pushButton_rate->setText(QString("x%1").arg(rate));
	emit sigSetRate(rate);
}

void VideoControls::sltVideoPlayOrPause()
{
	auto isPlay = ui.pushButton_play->isChecked();
	emit sigVideoPlayOrPause(isPlay);
}
