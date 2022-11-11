#include "Player.h"
#include "game.h"
#include "DxLib.h"
#include "Pad.h"
#include "Sound.h"
#include "SceneMain.h"

namespace
{
	// �T�C�Y
	constexpr int kSize = 32;

	// �ړ����x
	constexpr float kSpeed = 4.0f;
	// �e�̔��ˊԊu(�t���[��)
	constexpr int kShotInterval = 16;

	const int kColor = GetColor(255, 255, 255);

	// �A�j���[�V�������
	constexpr int kAnimNum = 2;		// �A�j���̃R�}��
	constexpr int kAnimFrame = 4;	// �P�R�}������̃t���[����
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

	// �v���C���[�A�j���[�V����
	m_animFrameCount++;
	if (m_animFrameCount >= kAnimNum * kAnimFrame)
	{
		m_animFrameCount = 0;
	}

	// �ړ�
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

	// �ҋ@���̓V���b�g���ĂȂ�
	if (m_isWaiting)	return;
	// �V���b�g����
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
	// �����蔻��\��
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