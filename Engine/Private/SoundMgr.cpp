#include "fmod_errors.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)
CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr;
}

HRESULT CSoundMgr::Initialize()
{
	//m_result = FMOD_System_Create(&m_pSystem);
	m_result = FMOD::System_Create(&m_pSystem);
	if (m_result != FMOD_OK) {
		MSG_BOX("FMOD Init Failed");
		return E_FAIL;
	}

	//m_result = FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	m_result = m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);
	if (m_result != FMOD_OK) {
		MSG_BOX("FMOD Init Failed");
		return E_FAIL;
	}
	//	m_result = m_pSystem->playSound(sound, 0, false, &Channel);
	LoadSoundFile();

	return S_OK;
}


int CSoundMgr::VolumeUp(CHANNELID eID, _float _vol)
{
	if (m_volume < SOUND_MAX) {
		m_volume += _vol;
	}
	//FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);
	m_pChannelArr[eID]->setVolume(m_volume);

	return 0;
}

int CSoundMgr::VolumeDown(CHANNELID eID, _float _vol)
{
	if (m_volume > SOUND_MIN) {
		m_volume -= _vol;
	}

	//FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);
	m_pChannelArr[eID]->setVolume(m_volume);

	return 0;
}

int CSoundMgr::BGMVolumeUp(_float _vol)
{
	if (m_BGMvolume < SOUND_MAX) {
		m_BGMvolume += _vol;
	}

	//FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);
	m_pChannelArr[BGM]->setVolume(m_BGMvolume);

	return 0;
}

int CSoundMgr::BGMVolumeDown(_float _vol)
{
	if (m_BGMvolume > SOUND_MIN) {
		m_BGMvolume -= _vol;
	}

	//FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);
	m_pChannelArr[BGM]->setVolume(m_BGMvolume);

	return 0;
}

int CSoundMgr::Pause(CHANNELID eID)
{
	m_bPause = !m_bPause;
	//FMOD_Channel_SetPaused((m_pChannelArr[eID]), m_bPause);
	m_pChannelArr[eID]->setPaused(m_bPause);

	return 0;
}




void CSoundMgr::PlaySound(TCHAR * pSoundKey, CHANNELID eID, _float _vol)
{
	map<TCHAR*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	//FMOD_BOOL bPlay = FALSE;
	_bool bPlay = false;

	//if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	if(m_pChannelArr[eID]->isPlaying(&bPlay))
	{
		//FMOD_RESULT eRes = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
		FMOD_RESULT eRes = m_pSystem->playSound(iter->second, 0, false, &m_pChannelArr[eID]);
		if (eRes != FMOD_OK) {
			MSG_BOX("SoundPlay Fail");
			return;
		}
	}
	
	if (_vol >= SOUND_MAX)
		_vol = 1.f;
	else if (_vol <= SOUND_MIN)
		_vol = 0.f;

	//FMOD_Channel_SetVolume(m_pChannelArr[eID], _vol);
	m_pChannelArr[eID]->setVolume(_vol);

	//FMOD_System_Update(m_pSystem);
	m_pSystem->update();
}

void CSoundMgr::PlayBGM(TCHAR * pSoundKey, _float fVol)
{
	map<TCHAR*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	//FMOD_System_PlaySound(m_pSystem,/*iter->second*/FMOD_CHANNEL_FREE,/*nullptr*/iter->second, FALSE, &m_pChannelArr[BGM]);
	m_pSystem->playSound(iter->second, 0, false, &m_pChannelArr[BGM]);
	//FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	m_pChannelArr[BGM]->setMode(FMOD_LOOP_NORMAL);
	//FMOD_Channel_SetVolume(m_pChannelArr[BGM], fVol);
	m_pChannelArr[BGM]->setVolume(fVol);
	//FMOD_System_Update(m_pSystem);
	m_pSystem->update();

}

void CSoundMgr::StopSound(CHANNELID eID)
{
	m_pChannelArr[eID]->stop();
	//FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		m_pChannelArr[i]->stop();
		//FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::LoadSoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddatai64_t fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	long long handle = _findfirsti64("../../Reference/Audio/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../../Reference/Audio/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
				
		strcat_s(szFullPath, fd.name);

		FMOD::Sound* pSound = nullptr;

		//FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);
		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_DEFAULT, 0, &pSound);
		if (eRes == FMOD_OK) {

			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnexti64(handle, &fd);
	}

	//FMOD_System_Update(m_pSystem);
	m_pSystem->update();

	_findclose(handle);

}

void CSoundMgr::Free()
{
	__super::Free();


	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		Mypair.second->release();
	}
	m_mapSound.clear();
	//FMOD_System_Release(m_pSystem);
	m_pSystem->release();
	//FMOD_System_Close(m_pSystem);
	m_pSystem->close();
}
