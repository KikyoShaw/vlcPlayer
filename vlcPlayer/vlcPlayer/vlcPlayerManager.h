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
	//设置倍速
	void setRate(float rate);
	//销毁
	void Release();
	// 播放完成
	void PlayEnd();

	//解决卡顿问题
	void initInst();

signals:
	void loadFinish();
	void playToralTimeAudio(int duration);
	void playToralTime(int duration);
	void playCurrentTime(int position);

private:
	//VLC实例
	libvlc_instance_t     *m_pVLC_Inst;
	//VLC播放器
	libvlc_media_player_t *m_pVLC_Player;
	//VLC事件
	libvlc_event_manager_t *m_pVLC_eMg;

	// 是否播放完成
	bool m_isEndPlay = false;

	// 配置属性(全屏)
	void setOption(libvlc_media_t *m, const QString &option);
};
