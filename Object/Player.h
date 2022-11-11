#pragma once

#include "ObjectBase.h"

class Player : public ObjectBase
{
public:
	Player();
	virtual ~Player();

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

	virtual ColType getColType() const override { return ColType::kPlayer; }
	virtual ColType getTargetColType() const override { return ColType::kNone; }

	// グラフィックデータ設定
	void setGraph(int handle) { m_hGraph = handle; }
	// 待機終了(ゲーム開始)
	void setWait(bool isWait) { m_isWaiting = isWait; }
	// 敵の弾が当たったときの処理
	virtual void beHit() override;
	// 弾発射位置の取得
	Vec2 getShotStartPos() const;

private:
	// グラフィックハンドル
	int		m_hGraph;
	// アニメーション
	int		m_animFrameCount;

	// 演出用	待機(ショット撃てない)
	bool	m_isWaiting;

	// ショットの発射間隔
	int		m_shotInterval;
};