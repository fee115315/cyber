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

	// �����z�u�p�̓G�̐���
	void addStartEnemy(int no);

	// �v���C���[�̃V���b�g����
	void addPlayerShot( Vec2 pos );
	// �G�̃V���b�g����
	void addEnemyShot( Vec2 pos );

	// �G�̃V���b�g�����Ă�����	�ː���ɑ��̓G������ꍇ�͌����Ȃ�
	bool isEnemyShotEnable(Vec2 shotStartPos);

	// �v���C���[���邩	ColType::kPlayer���`�F�b�N����
	bool isExistPlayer() const;
	// �G�̐��擾	ColType::kEnemy���`�F�b�N����
	int getEnemyNum() const;

	// �p�[�e�B�N������
	void createParticle(Vec2 pos, int color, int num);

	// �G�t�F�N�g����
	void createEffect(Vec2 pos, int existFrame);

private:
	enum class Seq
	{
		Seq_Wait,	// �Q�[���J�n�҂�
		Seq_Count,	// �J�E���g�_�E��
		Seq_Game,	// �Q�[���{��
	};

private:
	// �e�V�[�P���X���Ƃ�update����
	SceneBase* updateWait();
	SceneBase* updateCount();
	SceneBase* updateGame();

	// �J�E���g�_�E���\��
	void drawCountDown() const;

	// �G�̔ԍ����珉���ʒu���擾����
	Vec2 getEnemyStartPos(int index);
	// �v���C���[�A�G���̑ҋ@�ݒ�
	void setWaitObject( bool isWait );

	// �V���b�g�����ׂď���	�Q�[���I�����ɍs��
	void eraseShot();

	// �e�V�[�P���X���ʏ���
	void updateBg();	// �w�i�̃X�N���[��

	// list<ObjectBase*>�ւ̊e����
	void updateObject();
	void drawObject();
	void endObject();

private:
	// �O���t�B�b�N�n���h��
	std::vector<int>	m_graphicHandle;

	// �Q�[�����ɓo�ꂷ�镨��
	std::list<ObjectBase*>	m_object;
	// �w�i�̃X�N���[��
	float m_bgScroll;

	// ���݂̃Q�[���V�[�P���X
	Seq	m_seq;
	// �t���[���J�E���g
	int m_seqFrame;
	// �Q�[���I����̃t���[���҂�
	int m_endCount;
};