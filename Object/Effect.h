#pragma once

#include "ObjectBase.h"

// ���̏�ŕ\������O���t�B�b�N���o
class Effect : public ObjectBase
{
public:
	Effect();
	virtual ~Effect();

	virtual void init() override;
	virtual void end() override;

	virtual void update() override;
	virtual void draw() override;

	void start(Vec2 pos, int existFrame);

	// �O���t�B�b�N�f�[�^�ݒ�
	void setGraph(int handle) { m_hGraph = handle; }

private:
	// �O���t�B�b�N�n���h��
	int		m_hGraph;
	// �A�j���[�V����
	int		m_animFrameCount;
	// ���݂���t���[����
	int		m_existFrame;
};
