#include "Effect.h"
#include "game.h"
#include "DxLib.h"

namespace
{
	// アニメーション情報
	constexpr int kAnimNum = 3;		// アニメのコマ数
	constexpr int kAnimFrame = 4;	// １コマ当たりのフレーム数
}

Effect::Effect()
{
	m_hGraph = -1;
	m_animFrameCount = 0;
	m_existFrame = 0;
}
Effect::~Effect()
{

}

void Effect::init()
{
	
}

void Effect::end()
{

}

void Effect::update()
{
	if (!m_isExist)	return;

	// 終了確認
	m_existFrame--;
	if (m_existFrame <= 0)
	{
		m_isExist = false;
	}
	m_animFrameCount++;
	if (m_animFrameCount >= kAnimFrame * kAnimNum)
	{
		m_animFrameCount = 0;
	}
}

void Effect::draw()
{
	if (!m_isExist)	return;

	int animNo = m_animFrameCount / kAnimFrame;
	DrawRectGraphF(m_pos.x, m_pos.y,
		animNo * 64, 112*2, 64, 112,
		m_hGraph, true, false);

	// test
//	DrawBoxAA(m_pos.x, m_pos.y, m_pos.x + 32, m_pos.y + 32, GetColor(255, 255, 255), true);
}

void Effect::start(Vec2 pos, int existFrame)
{
	m_isExist = true;
	m_pos = pos;
	m_existFrame = existFrame;
}
