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
	// �g�p����O���t�B�b�N�t�@�C�����X�g
	typedef enum GraphicFileData
	{
		kGraphicFileData_Player,		// �v���C���[
		kGraphicFileData_Enemy,			// �G
		kGraphicFileData_PlayerShot,	// �v���C���[�̌��e
		kGraphicFileData_EnemyShot,		// �G�̌��e

		kGraphicFileData_AppearEffect,	// �o�ꉉ�o
		kGraphicFileData_Bg,			// �X�N���[������w�i�̉_

		kGraphicFileData_Num
	}GraphicFileData;
	// �O���t�B�b�N�t�@�C����
	const char* const kGraphicFileNameList[kGraphicFileData_Num] =
	{
		"Data/player.png",
		"Data/enemy00.png",
		"Data/playerShot.png",
		"Data/enemyShot.png",
		"Data/appearEffect.png",
		"Data/bg.png",
	};

	// �w�i�F
	const int kBgColor = GetColor(160, 216, 239);

	// �v���C���[���Sor�G�S�Ō�A�^�C�g���ɖ߂�܂ł̃t���[����
	constexpr int kGameEndWaitFrame = 150;
	constexpr int kGameEndFadeOutStartFrame = 120;
	// �G�̐�
	constexpr int kEnemyHNum = 7;	// �������̓G�̐�
	constexpr int kEnemyVNum = 3;	// �c�����̓G�̐�
	constexpr int kEnemyNum = kEnemyHNum * kEnemyVNum;

	// �J�E���g�_�E���֘A
	constexpr int kCountDownWaitSec = 3;	// �҂�����(�b)
	constexpr int kCountDownWaitFrame = kCountDownWaitSec*60;	// �҂�����(�t���[��)
}

// ===============================================

void SceneMain::init()
{
	// �O���t�B�b�N�f�[�^�̓ǂݍ���
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
	// �O���t�B�b�N�f�[�^�̉��
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
	// �w�i�F
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBgColor, true);

	// �w�i
	DrawGraphF(0, m_bgScroll, m_graphicHandle[kGraphicFileData_Bg], true);
	DrawGraphF(0, m_bgScroll - Game::kScreenHeight , m_graphicHandle[kGraphicFileData_Bg], true);
	// �Q�[���I�u�W�F�N�g
	drawObject();

	// �J�E���g�_�E��
	if( m_seq == Seq::Seq_Count )
	{
		drawCountDown();
	}

	// �f�o�b�O�\��
	SetFontSize(16);
	DrawFormatString(0, 16, GetColor(255, 255, 255), "�G�̐�:%d", getEnemyNum());
	DrawFormatString(0, 32, GetColor(255, 255, 255), "�I�u�W�F�N�g�̐�:%d", m_object.size());

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
	// �o�ꉉ�o
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
			// ���ˈʒu������ɂ���G�͋C�ɂ��Ȃ�
			if (shotStartPos.y > pObj->getTop())	continue;
			// ���E�ʒu������Ă���G�͋C�ɂ��Ȃ�
			if (shotStartPos.x > pObj->getRight())	continue;
			if (shotStartPos.x < pObj->getLeft())	continue;
			// ����ȊO�͒e�̎ː���ɂ���̂Ō����Ȃ�
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
	if (isFading())	return this;	// �t�F�[�h�C���A�A�E�g���͓����Ȃ�
	// �w�i�̃X�N���[��
	updateBg();
	// �e�I�u�W�F�N�g�̏���
	updateObject();

	// �G�̓o�ꏈ��
	m_seqFrame++;

	// �G�o��J�n�܂ő҂�
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
	if (isFading())	return this;	// �t�F�[�h�C���A�A�E�g���͓����Ȃ�
	// �w�i�̃X�N���[��
	updateBg();
	// �e�I�u�W�F�N�g�̏���
	updateObject();

	if (!(m_seqFrame % 60))
	{
		Sound::play(Sound::SoundId_PlayerExplosion);
	}

	m_seqFrame++;
	if (m_seqFrame >= kCountDownWaitFrame)
	{
		setWaitObject(false);	// �ҋ@��ԉ���
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
	if (isFading())	return this;	// �t�F�[�h�C���A�A�E�g���͓����Ȃ�
	// �w�i�̃X�N���[��
	updateBg();
	// �e�I�u�W�F�N�g�̏���
	updateObject();

	// �����蔻��
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

	// �G���S��or�v���C���[�����񂾂�^�C�g���ɖ߂�
	if (!isExistPlayer() || (getEnemyNum() == 0))
	{
		if (m_endCount == 0)
		{
			setWaitObject(true);	// �ҋ@��Ԃ�
			eraseShot();	// �V���b�g���ׂď���
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
			// ���ʕ\����ʂ�	�Q�[�����ʂ������œn���Ă���
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