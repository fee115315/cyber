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

	// �ړ����
	constexpr int kMoveInterval = 30;	// �ړ����s���Ԋu(�t���[��)
	constexpr float kMoveSpeed = 8.0f;	// 1��̈ړ���

	// �e�̐����Ԋu
	constexpr int kShotInterval = 120;

	// �A�j���[�V�������
	constexpr int kAnimNum = 4;		// �A�j���̃R�}��
	constexpr int kAnimNo[kAnimNum] = { 0, 1, 2, 1 };
	constexpr int kAnimFrame = 8;	// �P�R�}������̃t���[����
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
	// �A�j���[�V����
	m_animFrameCount++;
	if (m_animFrameCount >= kAnimNum * kAnimFrame)
	{
		m_animFrameCount = 0;
	}

	// �ҋ@���͈ړ��A�V���b�g���s��Ȃ�
	if (m_isWaiting)	return;

	// �ړ�
	m_moveFrameCount++;
	if (m_moveFrameCount >= kMoveInterval)
	{
		m_moveOffsetCount += m_moveDir;
		if (m_moveDir > 0)	// �E�����ւ̈ړ�
		{
			m_pos.x += kMoveSpeed;
			if (m_moveOffsetCount >= 8)
			{
				m_pos.y += kMoveSpeed;
				m_moveDir = -1;
			}
		}
		if (m_moveDir < 0)	// �������ւ̈ړ�
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

	// ���Ԋu�ŃV���b�g������
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

	// �����蔻��\��
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