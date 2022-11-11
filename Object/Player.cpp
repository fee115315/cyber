#include "Player.h"
#include "game.h"
#include "DxLib.h"
#include "Pad.h"
#include "Sound.h"
#include "SceneMain.h"

namespace
{
	// サイズ
	constexpr int kSize = 32;

	// 移動速度
	constexpr float kSpeed = 4.0f;
	// 弾の発射間隔(フレーム)
	constexpr int kShotInterval = 16;

	const int kColor = GetColor(255, 255, 255);

	// アニメーション情報
	constexpr int kAnimNum = 2;		// アニメのコマ数
	constexpr int kAnimFrame = 4;	// １コマ当たりのフレーム数
}

Player::Player()
{
	m_hGraph = -1;
	m_animFrameCount = 0;
	m_isWaiting = false;
	m_shotInterval = 0;
}
Player::~Player()
{

}

void Player::init()
{
	m_isExist = true;

	m_pos.x = Game::kScreenWidth / 2 - kSize / 2;
	m_pos.y = Game::kScreenHeight - 80.0f;

	m_colSize.x = kSize;
	m_colSize.y = kSize;

	m_isWaiting = true;
}

void Player::end()
{

}

void Player::update()
{
	if (!m_isExist)	return;

	// プレイヤーアニメーション
	m_animFrameCount++;
	if (m_animFrameCount >= kAnimNum * kAnimFrame)
	{
		m_animFrameCount = 0;
	}

	// 移動
	if (Pad::isPress(PAD_INPUT_LEFT))
	{
		m_pos.x -= kSpeed;
	}
	if (Pad::isPress(PAD_INPUT_RIGHT))
	{
		m_pos.x += kSpeed;
	}
	if (m_pos.x < 0.0f)
	{
		m_pos.x = 0.0f;
	}
	if (m_pos.x > Game::kScreenWidth - m_colSize.x)
	{
		m_pos.x = Game::kScreenWidth - m_colSize.x;
	}

	// 待機中はショット撃てない
	if (m_isWaiting)	return;
	// ショット生成
	m_shotInterval++;
	if( (Pad::isPress(PAD_INPUT_1)) &&
		(m_shotInterval >= kShotInterval) )
	{
		m_pMain->addPlayerShot(getShotStartPos());
		m_shotInterval = 0;
		Sound::play(Sound::SoundId_PlayerShot);
	}
}

void Player::draw()
{
	if (!m_isExist)	return;
	
	int animNo = m_animFrameCount / kAnimFrame;
	DrawRectGraphF(m_pos.x, m_pos.y,
				animNo * kSize, 96, kSize, kSize,
				m_hGraph, true, false);
	// 当たり判定表示
//	DrawBoxAA(m_pos.x, m_pos.y, m_pos.x + m_colSize.x, m_pos.y + m_colSize.y, kColor, false);
}

void Player::beHit()
{
	m_isExist = false;
	Vec2 pos = m_pos;
	pos.x += m_colSize.x / 2;
	pos.y += m_colSize.y / 2;
	m_pMain->createParticle(pos, kColor, 32);

	Sound::play(Sound::SoundId_PlayerExplosion);
}

Vec2 Player::getShotStartPos() const
{
	Vec2 result = m_pos;
	result.x += m_colSize.x / 2.0f;
	return result;
}