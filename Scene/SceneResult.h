#pragma once
#include "SceneBase.h"

class SceneResult : public SceneBase
{
public:
	SceneResult()
	{
		m_hBgSuccess = -1;
		m_hBgFail = -1;
		m_isSuccess = false;
		m_destroyNum = 0;
		m_textBlinkFrame = 0;
	}
	virtual ~SceneResult() {}


	virtual void init() override;
	virtual void end() override;

	virtual SceneBase* update() override;
	virtual void draw() override;

	void setResult(bool isWin, int destroyNum);

private:
	// タイトル背景
	int m_hBgSuccess;
	int m_hBgFail;

	// ゲーム結果
	bool m_isSuccess;
	int m_destroyNum;

	// テキスト点滅用フレームカウント
	int m_textBlinkFrame;
};