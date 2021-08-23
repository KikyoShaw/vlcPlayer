#pragma once

#include <QtWidgets/QWidget>
#include "ui_vlcPlayer.h"

class vlcPlayerManager;
class QThread;
class VideoList;
class QMovie;

class vlcPlayer : public QWidget
{
    Q_OBJECT

public:
    vlcPlayer(QWidget *parent = Q_NULLPTR);
	~vlcPlayer();

private:
	void initVlcPlayer();
	void initVideoControls();
	void initVideoList();
	void initMain();
	void initLoadGif();
	void locateWidgets();

private slots:
	void sltMaxOrNormal();
	void sltVideoPlayOrPause(bool state);
	void sltSoundVoiceValue(int value);
	void sltSetPosition(int value);
	void sltPlayVlcByLink();
	void sltPlayVlcByLocal();
	void sltVlcMediaPlayerVount(int duration);
	void sltVlcMediaPlayerTimeChange(int position);
	void sltSendPathToVlc(const QString& path);
	void sltSetRate(float rate);

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void showEvent(QShowEvent *event);
	virtual void moveEvent(QMoveEvent *event);
	virtual void hideEvent(QHideEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::vlcPlayerClass ui;
	bool m_bMove;
	QPoint m_point;
	//����������
	vlcPlayerManager *m_vlcPlayer = nullptr;
	//�������߳�
	QThread *m_thread = nullptr;
	//��¼ý���ļ���ʱ��
	QString m_totalTime;
	//ý���ļ���ʼ��������
	int m_volumn;
	//�Ƿ���ɲ���
	bool m_isFinishPlay;
	//�����б�
	QSharedPointer <VideoList> m_videoList = nullptr;
	//gif
	QMovie *m_loadGif = nullptr;
	//���Ž���
	int m_position;
};
