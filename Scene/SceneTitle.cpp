#include "DxLib.h"
#include "game.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "Pad.h"


namespace
{
	// 表示する文字列
	const char* const kTitleText = "インベーダー風ゲーム";
	const char* const kGuideText = "1ボタンを押してください";
	// 文字列点滅
	constexpr int kTextDispFrame = 45;
	constexpr int kTextHideFrame = 15;

	// 文字色
	const int kFontColor = GetColor(0, 0, 0);
}

void SceneTitle::init()
{
	m_textBlinkFrame = 0;
	SetFontSize(32);

	m_hBg = LoadGraph("Data/titleBg.jpg");
}

void SceneTitle::end()
{
	DeleteGraph(m_hBg);
	SetFontSize(16);
}

SceneBase* SceneTitle::update()
{
	if(isFading())
	{
		bool isOut = isFadingOut();
		SceneBase::updateFade();
		// フェードアウト終了時にシーン切り替え
		if (!isFading() && isOut)
		{
			return (new SceneMain);
		}
	}
	// テキストの点滅
	m_textBlinkFrame++;
	if (m_textBlinkFrame >= kTextDispFrame + kTextHideFrame)
	{
		m_textBlinkFrame = 0;
	}
	if (!isFading())
	{
		if (Pad::isTrigger(PAD_INPUT_1))
		{
			// フェードアウト開始
			startFadeOut();
		}
	}
	return this;
}

void SceneTitle::draw()
{
	DrawGraph(0,0,m_hBg, false);

	int width = GetDrawStringWidth(kTitleText, static_cast<int>(strlen(kTitleText)));
	DrawString(Game::kScreenWidth/2 - width/2, 160, kTitleText,kFontColor);

	if (m_textBlinkFrame < kTextDispFrame)
	{
		width = GetDrawStringWidth(kGuideText, static_cast<int>(strlen(kGuideText)));
		DrawString(Game::kScreenWidth / 2 - width / 2, 280, kGuideText, kFontColor);
	}

	SceneBase::drawFade();
}
