#include "vlcPlayerManager.h"
#include <QFile>
#include <QDebug>
#include "QThread"

static void handleEvents(const libvlc_event_t *event, void *userData)
{
	vlcPlayerManager* obj = (vlcPlayerManager*)userData;

	switch (event->type) {
	// 加载完成
	case libvlc_MediaPlayerVout:{
		emit obj->playAllTime(obj->GetTime() / 1000);
		break;
	}
	// media player 位置改变
	case libvlc_MediaPlayerPositionChanged: {
		int time = obj->GetPlayTime();
		emit obj->playCurrentTime(time / 1000);
		break;
	}
	// 播放完成
	case libvlc_MediaPlayerEndReached:{
		obj->GetPlayState();
		obj->PlayEnd();
		break;
	}
	default:
		break;
	}
}

vlcPlayerManager::vlcPlayerManager(QObject *parent)
	: QObject(parent)
{

}

vlcPlayerManager::~vlcPlayerManager()
{
	Release();
}

void vlcPlayerManager::initVlc()
{
	m_pVLC_Player = NULL;
	m_pVLC_Inst = libvlc_new(0, NULL);
}

int vlcPlayerManager::Play(QString filename, void* hwnd)
{
	if (!QFile::exists(filename)) {
		qDebug() << "file is not exist:" << filename;
		return -1;
	}
	if (this->IsPlaying())
		this->Stop();
	libvlc_media_t *media;
	media = libvlc_media_new_path(m_pVLC_Inst, filename.toStdString().data());
	if (media){
		m_pVLC_Player = libvlc_media_player_new_from_media(media);
		if (m_pVLC_Player){
			libvlc_media_release(media);
			m_pVLC_eMg = libvlc_media_player_event_manager(m_pVLC_Player);
			libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerVout, handleEvents, this);
			libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerEndReached, handleEvents, this);
			libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerPositionChanged, handleEvents, this);
			if (hwnd != nullptr)
				libvlc_media_player_set_hwnd(m_pVLC_Player, hwnd);
			this->Play();
		}
	}
	return -1;
}

int vlcPlayerManager::PlayUrl(QString url, void* hwnd)
{
	//if (this->IsPlaying())
		//this->Stop();
	StopPlaying();
	libvlc_media_t *media;
	media = libvlc_media_new_location(m_pVLC_Inst, url.toStdString().data());
	if (media){
		m_pVLC_Player = libvlc_media_player_new_from_media(media);
		if (m_pVLC_Player){
			libvlc_media_parse(media);
			m_pVLC_eMg = libvlc_media_player_event_manager(m_pVLC_Player);
			libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerVout, handleEvents, this);
			libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerEndReached, handleEvents, this);
			libvlc_event_attach(m_pVLC_eMg, libvlc_MediaPlayerPositionChanged, handleEvents, this);
			if (hwnd != nullptr) {
				libvlc_media_player_set_hwnd(m_pVLC_Player, hwnd);
				Play();
			}
		}
	}
	return -1;
}

void vlcPlayerManager::RemoveBlack()
{
	//可以设置视频的比例
	//    "4:3"
	//    "1:1"
	//    "16:10"
	//    "2.21:1"
	//    "2.35:1"
	//    "2.39:1"
	//    "5:4"
	if (!m_pVLC_Player)
		return;

	libvlc_video_set_aspect_ratio(m_pVLC_Player, "16:10");
}

int vlcPlayerManager::Play()
{
	if (m_pVLC_Player)
	{
		return  libvlc_media_player_play(m_pVLC_Player);
	}
	return -1;
}

void vlcPlayerManager::Pause()
{
	if (m_pVLC_Player){
		libvlc_media_player_pause(m_pVLC_Player);
	}
}

void vlcPlayerManager::Stop()
{
	if (m_pVLC_Player){
		if (GetPlayState() != libvlc_Ended) {
			libvlc_media_player_pause(m_pVLC_Player);
			libvlc_media_player_stop(m_pVLC_Player);
		}
		libvlc_media_player_release(m_pVLC_Player);
		m_pVLC_Player = NULL;

	}
}

void vlcPlayerManager::StopPlaying()
{
	if (m_pVLC_Player){
		libvlc_media_player_stop(m_pVLC_Player);
	}
}

void vlcPlayerManager::SetVolume(int nVol)
{
	if (m_pVLC_Player){
		libvlc_audio_set_volume(m_pVLC_Player, nVol);
	}
}

int vlcPlayerManager::GetVolume()
{
	if (m_pVLC_Player){
		return libvlc_audio_get_volume(m_pVLC_Player);
	}
	return 0;
}

void vlcPlayerManager::SeekTo(float nPos)
{
	if (m_pVLC_Player){
		libvlc_media_player_set_position(m_pVLC_Player, nPos);
	}
}

bool vlcPlayerManager::IsOpen()
{
	return NULL != m_pVLC_Player;
}

bool vlcPlayerManager::IsPlaying()
{
	if (m_pVLC_Player){
		return libvlc_media_player_is_playing(m_pVLC_Player);
	}
	return false;
}

bool vlcPlayerManager::IsPause()
{
	bool tem;
	if (m_pVLC_Player){
		int state = libvlc_media_player_get_state(m_pVLC_Player);
		switch (state){
		case libvlc_Paused:
		case libvlc_Stopped:
			tem = true;
			break;
		default:
			break;
		}
	}
	else {
		tem = false;
	}
	return tem;
}

bool vlcPlayerManager::IsEndPlaying()
{
	return m_isEndPlay;
}

int vlcPlayerManager::GetPos()
{
	if (m_pVLC_Player){
		return (int)(100 * libvlc_media_player_get_position(m_pVLC_Player));
	}

	return 0;
}

int64_t vlcPlayerManager::GetTime()
{
	if (m_pVLC_Player){
		return libvlc_media_player_get_length(m_pVLC_Player);
	}

	return 0;
}

int64_t vlcPlayerManager::GetPlayTime()
{
	if (m_pVLC_Player){
		return libvlc_media_player_get_time(m_pVLC_Player);
	}

	return 0;
}

void vlcPlayerManager::SetPlayTime(int64_t time)
{
	if (m_pVLC_Player){
		libvlc_media_player_set_time(m_pVLC_Player, time);
	}
}

int vlcPlayerManager::GetPlayState()
{
	if (m_pVLC_Player) {
		return libvlc_media_player_get_state(m_pVLC_Player);
	}
	return -1;
}

bool vlcPlayerManager::setTrack(int trackIndex)
{
	if (m_pVLC_Player) {
		return  libvlc_audio_set_track(m_pVLC_Player, trackIndex) == 0;
	}
	return false;
}

void vlcPlayerManager::Release()
{
	if (m_pVLC_Player){
		libvlc_media_player_stop(m_pVLC_Player);
		libvlc_media_player_release(m_pVLC_Player);
		m_pVLC_Player = NULL;

	}
	if (m_pVLC_Inst){
		libvlc_release(m_pVLC_Inst);
		m_pVLC_Inst = NULL;
	}
}

void vlcPlayerManager::PlayEnd()
{
	// 播放完成，需要显式调用stop才能正常结束
	m_isEndPlay = true;
	emit playCurrentTime(GetTime() / 1000);
}

void vlcPlayerManager::setOption(libvlc_media_t *m, const QString &option)
{
	if (m != NULL) {
		QByteArray data = option.toUtf8();
		const char *arg = data.constData();
		libvlc_media_add_option(m, arg);
	}
}

