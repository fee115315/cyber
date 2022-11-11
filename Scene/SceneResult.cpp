#include "DxLib.h"
#include "game.h"
#include "SceneResult.h"
#include "SceneTitle.h"
#include "Pad.h"


namespace
{
	// �\�����镶����
	const char* const kSuccessText = "��퐬��";
	const char* const kFailText = "��편�s";
	const char* const kScoreText = "���j��:%d";
	const char* const kGuideText = "1�{�^���������Ă�������";
	// ������_��
	constexpr int kTextDispFrame = 45;
	constexpr int kTextHideFrame = 15;
	// �����F
	const int kTextColorSuccess = GetColor(0, 0, 0);
	const int kTextColorFail = GetColor(255,255,255);
}

void SceneResult::init()
{
	m_textBlinkFrame = 0;
	SetFontSize(16);

	m_hBgSuccess = LoadGraph("Data/successBg.jpg");
	m_hBgFail = LoadGraph("Data/failBg.jpg");
}

void SceneResult::end()
{
	DeleteGraph(m_hBgSuccess);
	DeleteGraph(m_hBgFail);
}

SceneBase* SceneResult::update()
{
	if (isFading())
	{
		bool isOut = isFadingOut();
		SceneBase::updateFade();
		// �t�F�[�h�A�E�g�I�����ɃV�[���؂�ւ�
		if (!isFading() && isOut)
		{
			return (new SceneTitle);
		}
	}
	// �e�L�X�g�̓_��
	m_textBlinkFrame++;
	if (m_textBlinkFrame >= kTextDispFrame + kTextHideFrame)
	{
		m_textBlinkFrame = 0;
	}
	if (!isFading())
	{
		if (Pad::isTrigger(PAD_INPUT_1))
		{
			// �t�F�[�h�A�E�g�J�n
			startFadeOut();
		}
	}
	return this;
}

void SceneResult::draw()
{
	int textColor = 0;

	if (m_isSuccess)
	{
		textColor = kTextColorSuccess;

		DrawGraph(0, 0, m_hBgSuccess, false);

		int width = GetDrawStringWidth(kSuccessText, static_cast<int>(strlen(kSuccessText)));
		DrawString(Game::kScreenWidth / 2 - width / 2, 180, kSuccessText, textColor);
	}
	else
	{
		textColor = kTextColorFail;

		DrawGraph(0, 0, m_hBgFail, false);

		int width = GetDrawStringWidth(kFailText, static_cast<int>(strlen(kFailText)));
		DrawString(Game::kScreenWidth / 2 - width / 2, 180, kFailText, textColor);

		DrawFormatString(Game::kScreenWidth / 2 - width / 2, 196, textColor, kScoreText, m_destroyNum);
	}

	if (m_textBlinkFrame < kTextDispFrame)
	{
		int width = GetDrawStringWidth(kGuideText, static_cast<int>(strlen(kGuideText)));
		DrawString(Game::kScreenWidth / 2 - width / 2, 300, kGuideText, textColor);
	}

	SceneBase::drawFade();
}

void SceneResult::setResult(bool isWin, int destroyNum)
{
	m_isSuccess = isWin;
	m_destroyNum = destroyNum;
}