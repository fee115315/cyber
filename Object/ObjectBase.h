#pragma once

#include "Vec2.h"

class SceneMain;

// 矩形当たり判定を持つゲームオブジェクト基底クラス
class ObjectBase
{
public:
	// 物体の分類
	enum class ColType
	{
		kNone,
		kPlayer,
		kEnemy,
		kPlayerShot,
		kEnemyShot,
	};

public:
	ObjectBase();
	virtual ~ObjectBase() {}

	virtual void init() = 0;
	virtual void end() = 0;

	virtual void update() = 0;
	virtual void draw() = 0;

	void setMain(SceneMain* pMain) { m_pMain = pMain; }

	void setExist(bool isExist) { m_isExist = isExist; }
	bool isExist() const { return m_isExist; }

	// 他の物体との当たり判定
	bool isCol(ObjectBase& target);

	// ぶつかったときのぶつかる側の処理	弾→敵の場合の弾
	virtual void hit() {};
	// ぶつかったときのぶつかられる側の処理	弾→敵の場合の敵
	virtual void beHit() {};

	// 情報取得
	Vec2 getPos() const { return m_pos; }
	Vec2 getColSize() const { return m_colSize; }

	// 当たり判定に使用する情報
	virtual float getLeft() const { return m_pos.x; }
	virtual float getRight() const { return m_pos.x + m_colSize.x; }
	virtual float getTop() const { return m_pos.y; }
	virtual float getBottom() const { return m_pos.y + m_colSize.y; }
	// 自身の当たり判定情報
	virtual ColType getColType() const { return ColType::kNone; }
	// 自身が何にぶつかるか	自分の弾→敵にぶつかる
	virtual ColType getTargetColType() const { return ColType::kNone; }

protected:
	SceneMain* m_pMain;

	// 存在するか
	bool m_isExist;

	// 左上座標
	Vec2	m_pos;
	// 当たり判定サイズ(矩形)
	Vec2	m_colSize;
};