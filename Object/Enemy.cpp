#include "Enemy.h"
#include "../game.h"
#include "DxLib.h"
#include "Pad.h"
#include "Sound.h"

#include "SceneMain.h"

namespace
{
	constexpr int kSize = 32;
	const int kColor = GetColor(255,0,0);

	// 移動情報
	constexpr int kMoveInterval = 30;	// 移動を行う間隔(フレーム)
	constexpr float kMoveSpeed = 8.0f;	// 1回の移動量

	// 弾の生成間隔
	constexpr int kShotInterval = 120;

	// アニメーション情報
	constexpr int kAnimNum = 4;		// アニメのコマ数
	constexpr int kAnimNo[kAnimNum] = { 0, 1, 2, 1 };
	constexpr int kAnimFrame = 8;	// １コマ当たりのフレーム数
}

Enemy::Enemy()
{
	m_hGraph = -1;
	m_animFrameCount = 0;
	m_isWaiting = false;
	m_moveFrameCount = 0;
	m_moveOffsetCount = 0;
	m_moveDir = 0;
	m_shotInterval = 0;
}
Enemy::~Enemy()
{

}

void Enemy::init()
{
	m_isWaiting = true;

	m_moveFrameCount = 0;
	m_moveOffsetCount = 0;
	m_moveDir = 1;
}

void Enemy::end()
{

}

void Enemy::update()
{
	if (!m_isExist)	return;
	// アニメーション
	m_animFrameCount++;
	if (m_animFrameCount >= kAnimNum * kAnimFrame)
	{
		m_animFrameCount = 0;
	}

	// 待機中は移動、ショットを行わない
	if (m_isWaiting)	return;

	// 移動
	m_moveFrameCount++;
	if (m_moveFrameCount >= kMoveInterval)
	{
		m_moveOffsetCount += m_moveDir;
		if (m_moveDir > 0)	// 右方向への移動
		{
			m_pos.x += kMoveSpeed;
			if (m_moveOffsetCount >= 8)
			{
				m_pos.y += kMoveSpeed;
				m_moveDir = -1;
			}
		}
		if (m_moveDir < 0)	// 左方向への移動
		{
			m_pos.x -= kMoveSpeed;
			if (m_moveOffsetCount <= -8)
			{
				m_pos.y += kMoveSpeed;
				m_moveDir = 1;
			}
		}

		m_moveFrameCount = 0;
	}

	// 一定間隔でショットを撃つ
	m_shotInterval--;
	if (m_shotInterval <= 0)
	{
		if (m_pMain->isEnemyShotEnable(getShotStartPos()))
		{
			m_pMain->addEnemyShot(getShotStartPos());
			Sound::play(Sound::SoundId_EnemyShot);
		}
		m_shotInterval = kShotInterval;
	}
}

void Enemy::draw()
{
	if (!m_isExist)	return;

	int animIndex = m_animFrameCount / kAnimFrame;
	int animNo = kAnimNo[animIndex];
	DrawRectGraphF(m_pos.x, m_pos.y,
		animNo * kSize, 0, kSize, kSize,
		m_hGraph, true, false);

	// 当たり判定表示
//	DrawBoxAA(m_pos.x, m_pos.y, m_pos.x + m_colSize.x, m_pos.y + m_colSize.y, kColor, false);
}

void Enemy::setStart(Vec2 pos)
{
	m_isExist = true;
	m_pos = pos;

	m_colSize.x = kSize;
	m_colSize.y = kSize;

	m_pos -= m_colSize / 2;

	m_isWaiting = true;
	m_shotInterval = kShotInterval;
}

void Enemy::beHit()
{
	m_isExist = false;
	Vec2 pos = m_pos;
	pos.x += m_colSize.x / 2;
	pos.y += m_colSize.y / 2;
	m_pMain->createParticle(pos, kColor, 32);

	Sound::play(Sound::SoundId_PlayerExplosion);
}

Vec2 Enemy::getShotStartPos() const
{
	Vec2 result = m_pos;
	result.x += m_colSize.x / 2.0f;
	result.y += m_colSize.y;
	return result;
}