#include "Sound.h"
#include "DxLib.h"
#include <vector>

namespace
{
	// サウンドハンドル
	std::vector<int>	m_soundHandle;

	// サウンドファイル名
	const char* const kFileName[Sound::SoundId_Num] =
	{
		// BGM
		"Sound/bgm_main.mp3",
		"Sound/bgm_gameover.mp3",
		// 効果音
		"Sound/playerShot.wav",
		"Sound/enemyShot.wav",
		"Sound/explosion.mp3",
		"Sound/explosion_Long.mp3",
		"Sound/appear.mp3",
	};
}

namespace Sound
{
	void load()
	{
		// サウンドデータの読み込み
		for (auto& fileName : kFileName)
		{
			int handle = LoadSoundMem(fileName);
			m_soundHandle.push_back(handle);
		}
	}
	void unload()
	{
		// サウンドデータの解放
		for (auto& handle : m_soundHandle)
		{
			DeleteSoundMem(handle);
			handle = -1;
		}
	}

	// BGMの再生
	void startBgm(SoundId id, int volume)
	{
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_BACK, true);
		setVolume(id, volume);
	}
	void stopBgm(SoundId id)
	{
		StopSoundMem(m_soundHandle[id]);
	}

	// 効果音の再生
	void play(SoundId id)
	{
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_BACK, true);
	}

	// 音量設定	0~255
	void setVolume(SoundId id, int volume)
	{
		ChangeVolumeSoundMem(volume, m_soundHandle[id]);
	}
}