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

	// �����ʒu�̐ݒ�
	void setStart(Vec2 pos);
	// �O���t�B�b�N�f�[�^�ݒ�
	void setGraph(int handle) { m_hGraph = handle; }

	// �ҋ@�I��(�Q�[���J�n)
	void setWait(bool isWait) { m_isWaiting = isWait; }

	// �v���C���[�̂��܂ɂԂ���ꂽ���̏���
	virtual void beHit();

	virtual ColType getColType() const override { return ColType::kEnemy; }
	virtual ColType getTargetColType() const override { return ColType::kPlayer; }


	// �e���ˈʒu�̎擾
	Vec2 getShotStartPos() const;

private:
	// �O���t�B�b�N�n���h��
	int		m_hGraph;
	// �A�j���[�V����
	int		m_animFrameCount;

	// ���o�p	�ҋ@(�A�j���[�V�����͂��邪�ړ���V���b�g�͌����Ȃ�)
	bool	m_isWaiting;

	// �ړ����	�C���x�[�_�[���ۂ�������
	int		m_moveFrameCount;
	int		m_moveOffsetCount;	// 0 -> 8-> -8 -> 8	�݂����ɍ��E�ɓ�����
	int		m_moveDir;

	// �V���b�g�̔��ˊԊu
	int m_shotInterval;
};