#pragma once
#include "SceneBase.h"
#include "Vec2.h"
#include <vector>
#include <list>
#include "Player.h"

class SceneMain : public SceneBase
{
public:
	SceneMain()
	{
		m_bgScroll = 0.0f;
		m_seq = Seq::Seq_Wait;
		m_seqFrame = 0;
		m_endCount = 0;
	}
	virtual ~SceneMain() {}


	virtual void init() override;
	virtual void end()  override;

	virtual SceneBase* update() override;
	virtual void draw() override;

	// 初期配置用の敵の生成
	void addStartEnemy(int no);

	// プレイヤーのショット生成
	void addPlayerShot( Vec2 pos );
	// 敵のショット生成
	void addEnemyShot( Vec2 pos );

	// 敵のショット撃っていいか	射線上に他の敵がいる場合は撃たない
	bool isEnemyShotEnable(Vec2 shotStartPos);

	// プレイヤーいるか	ColType::kPlayerをチェックする
	bool isExistPlayer() const;
	// 敵の数取得	ColType::kEnemyをチェックする
	int getEnemyNum() const;

	// パーティクル生成
	void createParticle(Vec2 pos, int color, int num);

	// エフェクト生成
	void createEffect(Vec2 pos, int existFrame);

private:
	enum class Seq
	{
		Seq_Wait,	// ゲーム開始待ち
		Seq_Count,	// カウントダウン
		Seq_Game,	// ゲーム本体
	};

private:
	// 各シーケンスごとのupdate処理
	SceneBase* updateWait();
	SceneBase* updateCount();
	SceneBase* updateGame();

	// カウントダウン表示
	void drawCountDown() const;

	// 敵の番号から初期位置を取得する
	Vec2 getEnemyStartPos(int index);
	// プレイヤー、敵をの待機設定
	void setWaitObject( bool isWait );

	// ショットをすべて消す	ゲーム終了時に行う
	void eraseShot();

	// 各シーケンス共通処理
	void updateBg();	// 背景のスクロール

	// list<ObjectBase*>への各処理
	void updateObject();
	void drawObject();
	void endObject();

private:
	// グラフィックハンドル
	std::vector<int>	m_graphicHandle;

	// ゲーム中に登場する物体
	std::list<ObjectBase*>	m_object;
	// 背景のスクロール
	float m_bgScroll;

	// 現在のゲームシーケンス
	Seq	m_seq;
	// フレームカウント
	int m_seqFrame;
	// ゲーム終了後のフレーム待ち
	int m_endCount;
};