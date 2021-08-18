#pragma once

#include <QWidget>
#include "ui_videoControls.h"

class VSliderWidget;

class VideoControls : public QWidget
{
	Q_OBJECT
public:
	VideoControls(QWidget *parent = Q_NULLPTR);
	~VideoControls();

	//进度条
	void setProgressDuration(qint64 duration);
	bool isSliderDown();
	void setSliderPosition(int position);
	//进度时间
	void setProgressText(const QString& text);
	//播放状态
	void setPlaying(bool state);
	//设置声音
	void setVoiceValue(int value);
	//获取声音窗口出现情况
	bool getVolumVisible();
	//窗口管理
	void locateWidgets();
	void closeWidget();

private:
	void initVolumeSlider();

private slots:
	void sltVideoPlayOrPause();
	void sltSliderProgressClicked();
	void sltSliderProgressReleased();
	void sltSoundVoiceValue(int value);
	void sltVolumControls();

signals:
	void sigVideoPlayOrPause(bool state);
	void sigSetPosition(int value);
	void sigSoundVoiceValue(int value);

private:
	Ui::controls ui;
	//声音进度条
	QSharedPointer <VSliderWidget> m_volumeSlider = nullptr;
};