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

	// �O���t�B�b�N�f�[�^�ݒ�
	void setGraph(int handle) { m_hGraph = handle; }
	// �ҋ@�I��(�Q�[���J�n)
	void setWait(bool isWait) { m_isWaiting = isWait; }
	// �G�̒e�����������Ƃ��̏���
	virtual void beHit() override;
	// �e���ˈʒu�̎擾
	Vec2 getShotStartPos() const;

private:
	// �O���t�B�b�N�n���h��
	int		m_hGraph;
	// �A�j���[�V����
	int		m_animFrameCount;

	// ���o�p	�ҋ@(�V���b�g���ĂȂ�)
	bool	m_isWaiting;

	// �V���b�g�̔��ˊԊu
	int		m_shotInterval;
};