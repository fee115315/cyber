#pragma once

#include "ObjectBase.h"

class Enemy : public ObjectBase
{
public:
	Enemy();
	virtual ~Enemy();

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

	// 初期位置の設定
	void setStart(Vec2 pos);
	// グラフィックデータ設定
	void setGraph(int handle) { m_hGraph = handle; }

	// 待機終了(ゲーム開始)
	void setWait(bool isWait) { m_isWaiting = isWait; }

	// プレイヤーのたまにぶつかられた時の処理
	virtual void beHit();

	virtual ColType getColType() const override { return ColType::kEnemy; }
	virtual ColType getTargetColType() const override { return ColType::kPlayer; }


	// 弾発射位置の取得
	Vec2 getShotStartPos() const;

private:
	// グラフィックハンドル
	int		m_hGraph;
	// アニメーション
	int		m_animFrameCount;

	// 演出用	待機(アニメーションはするが移動やショットは撃たない)
	bool	m_isWaiting;

	// 移動情報	インベーダーっぽく動かす
	int		m_moveFrameCount;
	int		m_moveOffsetCount;	// 0 -> 8-> -8 -> 8	みたいに左右に動かす
	int		m_moveDir;

	// ショットの発射間隔
	int m_shotInterval;
};