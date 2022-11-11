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

	// ���ˊJ�n	���S�ʒu�w��
	void startPlayer(Vec2 centerPos);	// �v���C���[�̃V���b�g
	void startEnemy(Vec2 centerPos);	// �G�̃V���b�g

	// �G�ɂԂ������Ƃ��̏���
	virtual void hit() { m_isExist = false; };

	virtual ColType getColType() const override { return m_myType; }
	virtual ColType getTargetColType() const override { return m_targetType; }

	// �O���t�B�b�N�f�[�^�ݒ�
	void setGraph(int handle) { m_hGraph = handle; }

private:
	// �O���t�B�b�N�n���h��
	int		m_hGraph;
	// �ړ���
	Vec2	m_vec;

	// �G�ɓ�����e���v���C���[�ɓ�����e����ݒ肷��
	ColType	m_myType;
	ColType	m_targetType;
};