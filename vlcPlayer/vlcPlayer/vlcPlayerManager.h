#pragma once

#include <QObject>
#include "vlc/vlc.h"

class vlcPlayerManager : public QObject
{
	Q_OBJECT
public:
	vlcPlayerManager(QObject *parent = Q_NULLPTR);
	~vlcPlayerManager();

	// 初始化vlc模块
	void initVlc();
	// 播放本地文件
	int Play(QString filename, void* hwnd = nullptr);
	// 播放在线文件
	int PlayUrl(QString url, void* hwnd = nullptr);
	//去黑边
	void RemoveBlack();
	// 播放
	int Play();
	// 暂停
	void Pause();
	// 停止
	void Stop();
	// 停止播放
	void StopPlaying();
	// 音量设置为nVol
	void SetVolume(int nVol);
	//获取音量值
	int GetVolume();
	// 跳到指定位置nPos
	void SeekTo(float nPos);
	// 文件是否打开
	bool IsOpen();
	// 文件是否正在播放
	bool IsPlaying();
	// 是否暂停
	bool IsPause();
	// 是否播放完成
	bool IsEndPlaying();
	// 获取文件当前播放的位置
	int GetPos();
	//获取总时长
	int64_t GetTime();
	// 获取播放时间
	int64_t GetPlayTime();
	// 设置播放时间
	void SetPlayTime(int64_t time);
	//获取播放状态
	int GetPlayState();
	//设置音轨
	bool setTrack(int trackIndex);
	//销毁
	void Release();
	// 播放完成
	void PlayEnd();

signals:
	void loadFinish();
	void totalTime(int sec);
	void playAllTime(int sec);
	void playCurrentTime(int sec);

private:
	libvlc_instance_t     *m_pVLC_Inst;                      // VLC实例
	libvlc_media_player_t *m_pVLC_Player;            // VLC播放器
	libvlc_event_manager_t *m_pVLC_eMg;           //VLC事件

	// 是否播放完成
	bool m_isEndPlay = false;

	// 配置属性
	void setOption(libvlc_media_t *m, const QString &option);
};
