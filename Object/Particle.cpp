#include "Particle.h"
#include "game.h"
#include "DxLib.h"

namespace
{
	// サイズ
	constexpr int kSize = 2;
	// 重力	
	constexpr float kGravity = 0.2f;
}

Particle::Particle()
{
	m_color = 0;
}
Particle::~Particle()
{

}

void Particle::init()
{
	
}

void Particle::end()
{

}

void Particle::update()
{
	if (!m_isExist)	return;
	m_pos += m_vec;
	m_vec.y += kGravity;
	if (m_pos.y > Game::kScreenHeight + kSize)
	{
		m_isExist = false;
	}
}

void Particle::draw()
{
	if (!m_isExist)	return;
	DrawBoxAA(m_pos.x, m_pos.y, m_pos.x + kSize, m_pos.y + kSize, m_color, true);
}

void Particle::start(Vec2 pos, int color)
{
	m_isExist = true;
	m_pos = pos;

	m_colSize.x = kSize;
	m_colSize.y = kSize;

	m_vec.x = static_cast<float>(GetRand(10) - 5);
	m_vec.y = static_cast<float>(-GetRand(6));

	m_color = color;
}
