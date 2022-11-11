#pragma once

#include "ObjectBase.h"
class Enemy;

class Shot : public ObjectBase
{
public:
	Shot();
	virtual ~Shot();

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

	// 発射開始	中心位置指定
	void startPlayer(Vec2 centerPos);	// プレイヤーのショット
	void startEnemy(Vec2 centerPos);	// 敵のショット

	// 敵にぶつかったときの処理
	virtual void hit() { m_isExist = false; };

	virtual ColType getColType() const override { return m_myType; }
	virtual ColType getTargetColType() const override { return m_targetType; }

	// グラフィックデータ設定
	void setGraph(int handle) { m_hGraph = handle; }

private:
	// グラフィックハンドル
	int		m_hGraph;
	// 移動量
	Vec2	m_vec;

	// 敵に当たる弾かプレイヤーに当たる弾かを設定する
	ColType	m_myType;
	ColType	m_targetType;
};