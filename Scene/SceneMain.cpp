#include "DxLib.h"
#include <cassert>
#include "game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneResult.h"
#include "Pad.h"
#include "Sound.h"

#include "Enemy.h"
#include "Shot.h"
#include "Particle.h"
#include "Effect.h"

namespace
{
	// 使用するグラフィックファイルリスト
	typedef enum GraphicFileData
	{
		kGraphicFileData_Player,		// プレイヤー
		kGraphicFileData_Enemy,			// 敵
		kGraphicFileData_PlayerShot,	// プレイヤーの撃つ弾
		kGraphicFileData_EnemyShot,		// 敵の撃つ弾

		kGraphicFileData_AppearEffect,	// 登場演出
		kGraphicFileData_Bg,			// スクロールする背景の雲

		kGraphicFileData_Num
	}GraphicFileData;
	// グラフィックファイル名
	const char* const kGraphicFileNameList[kGraphicFileData_Num] =
	{
		"Data/player.png",
		"Data/enemy00.png",
		"Data/playerShot.png",
		"Data/enemyShot.png",
		"Data/appearEffect.png",
		"Data/bg.png",
	};

	// 背景色
	const int kBgColor = GetColor(160, 216, 239);

	// プレイヤー死亡or敵全滅後、タイトルに戻るまでのフレーム数
	constexpr int kGameEndWaitFrame = 150;
	constexpr int kGameEndFadeOutStartFrame = 120;
	// 敵の数
	constexpr int kEnemyHNum = 7;	// 横方向の敵の数
	constexpr int kEnemyVNum = 3;	// 縦方向の敵の数
	constexpr int kEnemyNum = kEnemyHNum * kEnemyVNum;

	// カウントダウン関連
	constexpr int kCountDownWaitSec = 3;	// 待ち時間(秒)
	constexpr int kCountDownWaitFrame = kCountDownWaitSec*60;	// 待ち時間(フレーム)
}

// ===============================================

void SceneMain::init()
{
	// グラフィックデータの読み込み
	for (auto& fileName : kGraphicFileNameList)
	{
		int handle = LoadGraph(fileName);
		m_graphicHandle.push_back(handle);
	}
	{
		Player* pPlayer = new Player;
		pPlayer->init();
		pPlayer->setMain(this);
		pPlayer->setGraph(m_graphicHandle[kGraphicFileData_Player]);
		m_object.push_back(pPlayer);
	}

	m_bgScroll = 0.0f;

	m_seq = Seq::Seq_Wait;
	m_seqFrame = 0;
	m_endCount = 0;
}

void SceneMain::end()
{
	endObject();
	// グラフィックデータの解放
	for (auto& handle : m_graphicHandle)
	{
		DeleteGraph(handle);
		handle = -1;
	}
	m_graphicHandle.clear();
}

SceneBase* SceneMain::update()
{
	switch (m_seq)
	{
	case Seq::Seq_Wait:
		return updateWait();
	case Seq::Seq_Count:
		return updateCount();
	case Seq::Seq_Game:
		return updateGame();
	}
	assert(false);
	return nullptr;
}

void SceneMain::draw()
{
	// 背景色
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBgColor, true);

	// 背景
	DrawGraphF(0, m_bgScroll, m_graphicHandle[kGraphicFileData_Bg], true);
	DrawGraphF(0, m_bgScroll - Game::kScreenHeight , m_graphicHandle[kGraphicFileData_Bg], true);
	// ゲームオブジェクト
	drawObject();

	// カウントダウン
	if( m_seq == Seq::Seq_Count )
	{
		drawCountDown();
	}

	// デバッグ表示
	SetFontSize(16);
	DrawFormatString(0, 16, GetColor(255, 255, 255), "敵の数:%d", getEnemyNum());
	DrawFormatString(0, 32, GetColor(255, 255, 255), "オブジェクトの数:%d", m_object.size());

	SceneBase::drawFade();
}

void SceneMain::addStartEnemy(int no)
{
	Enemy* pEnemy = new Enemy;

	pEnemy->init();
	pEnemy->setMain(this);
	pEnemy->setGraph(m_graphicHandle[kGraphicFileData_Enemy]);
	Vec2 pos = getEnemyStartPos(no);
	pEnemy->setStart(pos);
	m_object.push_back(pEnemy);
	// 登場演出
	{
		pos.x -= 32.0f;
		pos.y -= 82.0f;
		createEffect(pos, 60);
	}
	Sound::play(Sound::SoundId_EnemyAppear);
}

void SceneMain::addPlayerShot(Vec2 pos)
{
	Shot* pShot = new Shot;
	pShot->init();
	pShot->setMain(this);
	pShot->setGraph(m_graphicHandle[kGraphicFileData_PlayerShot]);
	pShot->startPlayer(pos);
	m_object.push_back(pShot);
}

void SceneMain::addEnemyShot(Vec2 pos)
{
	Shot* pShot = new Shot;
	pShot->init();
	pShot->setMain(this);
	pShot->setGraph(m_graphicHandle[kGraphicFileData_EnemyShot]);
	pShot->startEnemy(pos);
	m_object.push_back(pShot);
}

bool SceneMain::isEnemyShotEnable(Vec2 shotStartPos)
{
	for (const auto& pObj : m_object)
	{
		if (pObj->getColType() == ObjectBase::ColType::kEnemy)
		{
			// 発射位置よりも上にいる敵は気にしない
			if (shotStartPos.y > pObj->getTop())	continue;
			// 左右位置がずれている敵は気にしない
			if (shotStartPos.x > pObj->getRight())	continue;
			if (shotStartPos.x < pObj->getLeft())	continue;
			// それ以外は弾の射線上にいるので撃たない
			return false;
		}
	}
	return true;
}

bool SceneMain::isExistPlayer() const
{
	for (const auto& pObj : m_object)
	{
		if (pObj->getColType() == ObjectBase::ColType::kPlayer)
		{
			return true;
		}
	}
	return false;
}

int SceneMain::getEnemyNum() const
{
	int result = 0;
	for (const auto& pObj : m_object)
	{
		if (pObj->getColType() == ObjectBase::ColType::kEnemy)
		{
			result++;
		}
	}
	return result;
}

void SceneMain::createParticle(Vec2 pos, int color, int num)
{
	for (int i = 0; i < num; i++)
	{
		Particle* pPart = new Particle;
		pPart->init();
		pPart->setMain(this);
		pPart->start(pos, color);
		m_object.push_back(pPart);
	}
}

void SceneMain::createEffect(Vec2 pos, int existFrame)
{
	Effect* pEff = new Effect;
	pEff->init();
	pEff->setMain(this);
	pEff->setGraph(m_graphicHandle[kGraphicFileData_AppearEffect]);
	pEff->start(pos, 60);
	m_object.push_back(pEff);
}

SceneBase* SceneMain::updateWait()
{
	SceneBase::updateFade();
	if (isFading())	return this;	// フェードイン、アウト中は動かない
	// 背景のスクロール
	updateBg();
	// 各オブジェクトの処理
	updateObject();

	// 敵の登場処理
	m_seqFrame++;

	// 敵登場開始まで待つ
	if (m_seqFrame > 60)
	{
		if (!(m_seqFrame % 8))
		{
			if (getEnemyNum() < kEnemyNum)
			{
				addStartEnemy(getEnemyNum());
			}
		}
	}

	if ((m_seqFrame >= 300) &&
		(getEnemyNum() >= kEnemyNum))
	{
		m_seq = Seq::Seq_Count;
		m_seqFrame = 0;
	}
	return this;
}

SceneBase* SceneMain::updateCount()
{
	SceneBase::updateFade();
	if (isFading())	return this;	// フェードイン、アウト中は動かない
	// 背景のスクロール
	updateBg();
	// 各オブジェクトの処理
	updateObject();

	if (!(m_seqFrame % 60))
	{
		Sound::play(Sound::SoundId_PlayerExplosion);
	}

	m_seqFrame++;
	if (m_seqFrame >= kCountDownWaitFrame)
	{
		setWaitObject(false);	// 待機状態解除
		m_seq = Seq::Seq_Game;
		m_seqFrame = 0;
		m_endCount = 0;

		Sound::play(Sound::SoundId_PlayerExplosionLong);
		Sound::startBgm(Sound::SoundId_BgmMain);
	}
	return this;
}

SceneBase* SceneMain::updateGame()
{
	SceneBase::updateFade();
	if (isFading())	return this;	// フェードイン、アウト中は動かない
	// 背景のスクロール
	updateBg();
	// 各オブジェクトの処理
	updateObject();

	// 当たり判定
	for (auto& hitObj : m_object)
	{
		for (auto& beHitObj : m_object)
		{
			if (hitObj->isCol(*beHitObj))
			{
				hitObj->hit();
				beHitObj->beHit();
			}
		}
	}

	// 敵が全滅orプレイヤーが死んだらタイトルに戻る
	if (!isExistPlayer() || (getEnemyNum() == 0))
	{
		if (m_endCount == 0)
		{
			setWaitObject(true);	// 待機状態に
			eraseShot();	// ショットすべて消す
		}

		int volume = 255;
		volume -= m_endCount * 4;
		if (volume < 0)	volume = 0;
		Sound::setVolume(Sound::SoundId_BgmMain, volume);

		m_endCount++;
		if ((!isFading()) && (m_endCount >= kGameEndFadeOutStartFrame))
		{
			startFadeOut();
		}
		if (m_endCount >= kGameEndWaitFrame)
		{
			// 結果表示画面へ	ゲーム結果もここで渡しておく
			SceneResult* pResult = new SceneResult;
			pResult->setResult(isExistPlayer(), kEnemyNum - getEnemyNum());
			Sound::stopBgm(Sound::SoundId_BgmMain);
			return pResult;
		}
	}
	return this;
}

void SceneMain::drawCountDown() const
{
	int fonstSize = 128;

	SetFontSize(fonstSize);
	int dispNo = (kCountDownWaitFrame - m_seqFrame) / 60 + 1;
	if (dispNo <= 0)	dispNo = 1;
	if (dispNo > 3)		dispNo = 3;
	int width = GetDrawFormatStringWidth("%d", dispNo);
	int dispX = Game::kScreenWidth / 2 - width / 2;
	int dispY = Game::kScreenHeight / 2 - fonstSize / 2;
	dispX += GetRand(8) - 4;
	dispY += GetRand(8) - 4;

	DrawFormatString(dispX, dispY, GetColor(0, 0, 0), "%d", dispNo);
}

Vec2 SceneMain::getEnemyStartPos(int index)
{
	Vec2 pos;
	pos.x = static_cast<float>((index % kEnemyHNum) * 64 + 128);
	pos.y = static_cast<float>((index / kEnemyHNum) * 48 + 60);
	return pos;
}

void SceneMain::setWaitObject(bool isWait)
{
	for (const auto& pObj : m_object)
	{
		if (pObj->getColType() == ObjectBase::ColType::kEnemy)
		{
			dynamic_cast<Enemy*>(pObj)->setWait(isWait);
		}
		else if (pObj->getColType() == ObjectBase::ColType::kPlayer)
		{
			dynamic_cast<Player*>(pObj)->setWait(isWait);
		}
	}
}

void SceneMain::eraseShot()
{
	std::list<ObjectBase*>::iterator it = m_object.begin();
	while (it != m_object.end())
	{
		auto pObj = (*it);
		pObj->update();
		if( (pObj->getColType() == ObjectBase::ColType::kPlayerShot) ||
			(pObj->getColType() == ObjectBase::ColType::kEnemyShot) )
		{
			Vec2 pos = pObj->getPos() + pObj->getColSize() / 2;
			createParticle(pos, GetColor(255,0,0), 32);

			delete pObj;
			it = m_object.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void SceneMain::updateBg()
{
	m_bgScroll += 1.0f;
	if (m_bgScroll > Game::kScreenHeight)
	{
		m_bgScroll -= Game::kScreenHeight;
	}
}

void SceneMain::updateObject()
{
	std::list<ObjectBase*>::iterator it = m_object.begin();
	while (it != m_object.end())
	{
		auto pObj = (*it);
		pObj->update();
		if (!pObj->isExist())
		{
			delete pObj;
			it = m_object.erase(it);
		}
		else
		{
			it++;
		}
	}
}
void SceneMain::drawObject()
{
	for (auto& pObj : m_object)
	{
		pObj->draw();
	}
}
void SceneMain::endObject()
{
	for (auto& pObj : m_object)
	{
		pObj->end();
		delete pObj;
	}
	m_object.clear();
}