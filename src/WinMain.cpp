#include "DxLib.h"

#include <string>
#include <vector>

using namespace std;

#include "DesktopMascot.h"


DesktopMascot* DesktopMascot::instance = nullptr;


struct Model
{
	int x, y;
	int handle;
	int attach;
	int attachID;
	float playTime;
	float totalTime;

	void SetAnimation(int id)
	{
		if (attachID != id)
		{
			if (attachID != -1)
			{
				MV1DetachAnim(handle, attach);	// アニメーション削除
			}
			attachID = id;
			attach = MV1AttachAnim(handle, id);	// アニメーションの読み込み

			playTime = 0.0f;
			totalTime = MV1GetAttachAnimTotalTime(handle, attach);	// アニメーションの再生時間
		}
		
	}
};


// DxLib のメイン
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DesktopMascot* mascot = DesktopMascot::GetInstance();

	mascot->Create();	//画面初期化


	// DxLib の初期化
	if (DxLib_Init() < 0)
	{
		return -1;
	}

	mascot->CreateMask();	// マスク作成(DxLib_Init() の後に実行)

	SetAlwaysRunFlag(true);	// ウインドウがアクティブではない状態でも処理を続行するか(true:続行)

	// 常に最前面に表示
	SetWindowPos(GetMainWindowHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	Model model;

	// 3Dモデルの読み込み
	model.handle= MV1LoadModel("dat/model/model.mv1");
	model.attachID = -1;
	model.SetAnimation(0);	// 待機モーションを読み込む

	mascot->GetWindowPos(model.x, model.y);


	// カメラの位置など
	SetCameraNearFar(1.0f, 1000.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 12.5f, -22.0f), VGet(0.0f, 12.5f, 0.0f));

	const float pi = 3.141592f; // 円周率π

	bool is_push_key = true;
	
	// メインループ
	for(; ProcessMessage()==0 && ClearDrawScreen()==0;)
	{
		MV1DrawModel(model.handle);	// 3Dモデル描画

		model.playTime += 0.25f;
		if (model.playTime >= model.totalTime)
		{
			// 「待機」「走る」モーションの時
			if (model.attachID <= 1)
			{
				// ループさせる
				model.playTime = 0.0f;
			}
			else
			{
				// ポーズ①～③のアニメーションが終わったら、待機モーションに戻す
				model.SetAnimation(0);
				is_push_key = true;
			}
		}

		MV1SetAttachAnimTime(model.handle, model.attach, model.playTime);	// 再生時間をセット


		if (is_push_key)
		{
			if (CheckHitKey(KEY_INPUT_LEFT))
			{
				model.x -= 3;
				model.SetAnimation(1); // 走るモーションを読み込む
				MV1SetRotationXYZ(model.handle, VGet(0.0f, pi / 2.0f, 0.0f));	// モデルを左向きにする
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT))
			{
				model.x += 3;
				model.SetAnimation(1); // 走るモーションを読み込む
				MV1SetRotationXYZ(model.handle, VGet(0.0f, -pi / 2.0f, 0.0f));	// モデルを右向きにする
			}
			else if (CheckHitKey(KEY_INPUT_UP))
			{
				model.SetAnimation(1); // 走るモーションを読み込む
				MV1SetRotationXYZ(model.handle, VGet(0.0f, pi, 0.0f));	// モデルを後ろ向きにする
			}
			else if (CheckHitKey(KEY_INPUT_DOWN))
			{
				model.SetAnimation(1); // 走るモーションを読み込む
				MV1SetRotationXYZ(model.handle, VGet(0.0f, 0.0f, 0.0f));	// モデルを前向きにする
			}
			else if (CheckHitKey(KEY_INPUT_Z))
			{
				is_push_key = false;
				model.SetAnimation(2); // ポーズ①を読み込む
			}
			else if (CheckHitKey(KEY_INPUT_X))
			{
				is_push_key = false;
				model.SetAnimation(3); // ポーズ②を読み込む
			}
			else if (CheckHitKey(KEY_INPUT_C))
			{
				is_push_key = false;
				model.SetAnimation(4); // ポーズ③を読み込む
			}
			else
			{
				model.SetAnimation(0);	// 待機モーション読み込み
			}
		}

		// ESCキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			// 終了
			break;
		}


		DrawString(0,  0, "→：右", GetColor(255, 255, 255));
		DrawString(0, 20, "←：左", GetColor(255, 255, 255));
		DrawString(0, 40, "↑：後ろ", GetColor(255, 255, 255));
		DrawString(0, 60, "↓：正面", GetColor(255, 255, 255));
		DrawString(0, 90, "Z：ポーズ①", GetColor(255, 255, 255));
		DrawString(0, 110, "X：ポーズ②", GetColor(255, 255, 255));
		DrawString(0, 130, "C：ポーズ③", GetColor(255, 255, 255));
		DrawString(0, 160, "ESC：終了", GetColor(255, 255, 255));

		mascot->Move(model.x, model.y);
		mascot->Update();
	}


	// DxLib の終了
	DxLib_End();

	return 0;
}


