#pragma once


// デスクトップマスコット関係のクラス
class DesktopMascot
{
	private:
		static DesktopMascot* instance;

		int initScreenH;	// 初期のスクリーンサイズ

		int softImage;

		int windowPosX;
		int windowPosY;

		int screenW;
		int screenH;

		static const int WindowW = 480;
		static const int WindowH = 360;

		// コンストラクタ
		DesktopMascot()
		{
			
		}

	public:
		// インスタンスの生成
		static DesktopMascot* GetInstance()
		{
			if(instance == nullptr)
			{
				instance = new DesktopMascot;
			}
			return instance;
		}

		// デスクトップマスコットデータ作成
		void Create()
		{

			this->GetScreenSize();

			this->initScreenH = this->screenH;


			this->windowPosX = (this->screenW - this->WindowW) / 2;
			this->windowPosY = this->screenH - this->WindowH;
			SetWindowInitPosition(this->windowPosX, this->windowPosY);	// ウィンドウを表示する座標の指定


			ChangeWindowMode(true);		// ウインドウモードで起動
			SetGraphMode(this->WindowW, this->WindowH, 32);		// 描画スクリーンのサイズを指定


			SetWindowStyleMode(2);	// ウィンドウのスタイルを枠無し

			SetUseBackBufferTransColorFlag(true);	// ウィンドウを透明	

			SetUseUpdateLayerdWindowFlag(true);	// true で UpdateLayerdWindowForSoftImage を使用する
		}

		// マスク作成(DxLib_Init() の後に実行)
		void CreateMask()
		{
			int mask = MakeScreen(WindowW, WindowH, true);	// 描画対象にできるアルファチャンネル付き画面を作成

			this->softImage = MakeARGB8ColorSoftImage(this->WindowW, this->WindowH);	// 画面取り込み用のソフトウエア用画像を作成
		
			SetUsePremulAlphaConvertLoad(false);	

			SetDrawScreen(mask);					// 描画先を描画対象にできるアルファチャンネル付き画面にする
		}


		// ループ時の処理
		void Update()
		{	
			// 指定領域のみ
			int old_scr_h = this->screenH;

			this->GetScreenSize();	// 現在のウィンドウサイズ(縦)を取得して、前の縦のサイズと比較

			// タスクバーを動かしたときに、ウィンドウの表示位置を変更する
			if (old_scr_h != this->screenH)
			{
				this->Move(this->windowPosX, this->windowPosY);
			}


		
			GetDrawScreenSoftImage(0, 0, this->WindowW, this->WindowH, this->softImage);	// 描画先の画像をソフトイメージに取得する
				
			UpdateLayerdWindowForPremultipliedAlphaSoftImage(this->softImage);	// 取り込んだソフトイメージを使用して透過ウインドウの状態を更新する	
		}


		// 指定されたウィンドウの位置を変更
		void Move(int x, int y)
		{
			int h = this->screenH - this->initScreenH;	// 「初期のスクリーンの縦のサイズ」と「タスクバー上げ下げした後」の差
			this->windowPosX = x;
			this->windowPosY = y;
			MoveWindow(GetMainWindowHandle(), x, y + h, this->WindowW, this->WindowH, true);
		}

		// 現在のウィンドウの位置を取得
		void GetWindowPos(int &x, int &y)
		{
			x = this->windowPosX;
			y = this->windowPosY;
		}


	private:
		// スクリーンの大きさを取得
		void GetScreenSize()
		{
			RECT rc;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0); // デスクトップ作業領域の矩形を取得する。

			this->screenW = rc.right;
			this->screenH = rc.bottom;
		}

};