#pragma once
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle() 
	{
		m_hBg = -1;
		m_textBlinkFrame = 0;
	}
	virtual ~SceneTitle() {}


	virtual void init() override;
	virtual void end() override;

	virtual SceneBase* update() override;
	virtual void draw() override;
private:
	// タイトル背景
	int m_hBg;

	// テキスト点滅用フレームカウント
	int m_textBlinkFrame;
};