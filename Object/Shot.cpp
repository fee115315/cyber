#include "Shot.h"
#include "../game.h"
#include "DxLib.h"
#include "Pad.h"

#include "Enemy.h"

namespace
{
	// サイズ
	constexpr int kColSize = 8;		// 当たり判定のサイズ
	constexpr int kGraphSize = 16;	// グラフィックのサイズ
	// 移動速度
	constexpr float kSpeed = -4.0f;
}

namespace
{
	
}

Shot::Shot() :
	m_hGraph(-1),
	m_vec(0.0f, 0.0f),
	m_myType(ColType::kPlayerShot),
	m_targetType(ColType::kEnemy)
{
}

Shot::~Shot()
{

}

void Shot::init()
{
	
}

void Shot::end()
{

}

void Shot::update()
{
	if (!m_isExist)	return;
	m_pos += m_vec;
	if ((m_vec.y < 0) &&
		(m_pos.y < 0.0f - kGraphSize) )
	{
		m_isExist = false;
	}
	if ((m_vec.y > 0) &&
		(m_pos.y > Game::kScreenHeight))
	{
		m_isExist = false;
	}
}

void Shot::draw()
{
	if (!m_isExist)	return;
	// 当たり判定と違うサイズのグラフィック表示
	Vec2 colCenter = m_pos;
	colCenter += m_colSize / 2;
	Vec2 drawPos = colCenter;
	drawPos.x -= kGraphSize / 2;
	drawPos.y -= kGraphSize / 2;
	DrawRectGraphF(drawPos.x, drawPos.y,
		0, 0, kGraphSize, kGraphSize,
		m_hGraph, true, false);

	// 当たり判定の表示
//	DrawBoxAA(getLeft(), getTop(), getRight(), getBottom(), GetColor(0, 0, 255), false);
}

void Shot::startPlayer(Vec2 centerPos)
{
	m_isExist = true;
	m_pos = centerPos;

	m_colSize.x = kColSize;
	m_colSize.y = kColSize;

	m_pos -= m_colSize / 2;

	m_vec.x = 0.0f;
	m_vec.y = kSpeed;

	m_myType = ColType::kPlayerShot;
	m_targetType = ColType::kEnemy;
}

void Shot::startEnemy(Vec2 centerPos)
{
	m_isExist = true;
	m_pos = centerPos;

	m_colSize.x = kColSize;
	m_colSize.y = kColSize;

	m_pos -= m_colSize / 2;

	m_vec.x = 0.0f;
	m_vec.y = -kSpeed;

	m_myType = ColType::kEnemyShot;
	m_targetType = ColType::kPlayer;
}
