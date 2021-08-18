#pragma once

#include <QObject>
#include "vlc/vlc.h"

class vlcPlayerManager : public QObject
{
	Q_OBJECT
public:
	vlcPlayerManager(QObject *parent = Q_NULLPTR);
	~vlcPlayerManager();

	// ��ʼ��vlcģ��
	void initVlc();
	// ���ű����ļ�
	int Play(QString filename, void* hwnd = nullptr);
	// ���������ļ�
	int PlayUrl(QString url, void* hwnd = nullptr);
	//ȥ�ڱ�
	void RemoveBlack();
	// ����
	int Play();
	// ��ͣ
	void Pause();
	// ֹͣ
	void Stop();
	// ֹͣ����
	void StopPlaying();
	// ��������ΪnVol
	void SetVolume(int nVol);
	//��ȡ����ֵ
	int GetVolume();
	// ����ָ��λ��nPos
	void SeekTo(float nPos);
	// �ļ��Ƿ��
	bool IsOpen();
	// �ļ��Ƿ����ڲ���
	bool IsPlaying();
	// �Ƿ���ͣ
	bool IsPause();
	// �Ƿ񲥷����
	bool IsEndPlaying();
	// ��ȡ�ļ���ǰ���ŵ�λ��
	int GetPos();
	//��ȡ��ʱ��
	int64_t GetTime();
	// ��ȡ����ʱ��
	int64_t GetPlayTime();
	// ���ò���ʱ��
	void SetPlayTime(int64_t time);
	//��ȡ����״̬
	int GetPlayState();
	//��������
	bool setTrack(int trackIndex);
	//����
	void Release();
	// �������
	void PlayEnd();

signals:
	void loadFinish();
	void totalTime(int sec);
	void playAllTime(int sec);
	void playCurrentTime(int sec);

private:
	libvlc_instance_t     *m_pVLC_Inst;                      // VLCʵ��
	libvlc_media_player_t *m_pVLC_Player;            // VLC������
	libvlc_event_manager_t *m_pVLC_eMg;           //VLC�¼�

	// �Ƿ񲥷����
	bool m_isEndPlay = false;

	// ��������
	void setOption(libvlc_media_t *m, const QString &option);
};
