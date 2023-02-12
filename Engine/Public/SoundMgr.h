#pragma once
#include "fmod.h"
#include "fmod.hpp"
#include <io.h>
#include "Engine_Defines.h"
#include "Base.h"


BEGIN(Engine)

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSoundMgr final : public CBase
{
public:
	DECLARE_SINGLETON(CSoundMgr)


public:
	enum CHANNELID { BGM, PLAYER, PLAYER_ATTACK, PLAYER_HIT,MONSTER, MONSTER_FOOT, MONSTER_ATTACK, MONSTER_DEAD, WEAPON, EFFECT, UI, SYSTEM_EFFECT, SYSTEM_EFFECT2, SYSTEM_EFFECT3, UNIQUEOBJ, NPC, TRAP, MAXCHANNEL };
private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	HRESULT Initialize();

public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey, _float fVol);
	void StopSound(CHANNELID eID); 
	void StopAll();

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

private:
	void LoadSoundFile();

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<_tchar*, FMOD::Sound*> m_mapSound;
	FMOD::Sound* m_Sound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD::Channel* m_pChannelArr[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD::System* m_pSystem;

	FMOD_RESULT	m_result;
	_uint		m_Version;
	_bool		m_bPause = false;
	void        *extradriverdata = nullptr;

public:
	virtual void Free();
};

END