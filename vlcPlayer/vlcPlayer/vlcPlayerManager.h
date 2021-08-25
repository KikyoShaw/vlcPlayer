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
	//����б�
	void addPlayList(const QStringList& pathList, void* hwnd = nullptr);
	//�����б���
	void PlayList(const QString& filename);
	void PlayList(int index);
	// ���ű����ļ�
	void Play(const QString& filename, void* hwnd = nullptr);
	// ���������ļ�
	void PlayUrl(const QString& url, void* hwnd = nullptr);


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
	//ֹͣ�б���
	void StopPlayList();

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
	//�б��Ƿ��ڲ���
	bool IsListPlaying();
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
	//��ȡ�б���״̬
	int GetPlayListState();
	//��������
	bool setTrack(int trackIndex);
	//���ñ���
	void setRate(float rate);
	//����
	void Release();
	// �������
	void PlayEnd();

	//�����б���ģʽ
	void setListPlayModel(int state);

	//�����������
	void initInst();

signals:
	void loadFinish();
	void playToralTimeAudio(int duration);
	void playToralTime(int duration);
	void playCurrentTime(int position);

private:
	//VLCʵ��
	libvlc_instance_t     *m_pVLC_Inst = nullptr;
	//VLC������
	libvlc_media_player_t *m_pVLC_Player = nullptr;
	//VLC�����б�
	libvlc_media_list_player_t *m_pVLC_PlayerList = nullptr;
	//VLC�¼�
	libvlc_event_manager_t *m_pVLC_eMg = nullptr;

	// �Ƿ񲥷����
	bool m_isEndPlay = false;

	// ��������(ȫ��)
	void setOption(libvlc_media_t *m, const QString &option);
};
