#pragma once

#include "ObjectBase.h"

// その場で表示するグラフィック演出
class Effect : public ObjectBase
{
public:
	Effect();
	virtual ~Effect();

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

	void start(Vec2 pos, int existFrame);

	// グラフィックデータ設定
	void setGraph(int handle) { m_hGraph = handle; }

private:
	// グラフィックハンドル
	int		m_hGraph;
	// アニメーション
	int		m_animFrameCount;
	// 存在するフレーム数
	int		m_existFrame;
};
