#pragma once


// �f�X�N�g�b�v�}�X�R�b�g�֌W�̃N���X
class DesktopMascot
{
	private:
		static DesktopMascot* instance;

		int initScreenH;	// �����̃X�N���[���T�C�Y

		int softImage;

		int windowPosX;
		int windowPosY;

		int screenW;
		int screenH;

		static const int WindowW = 480;
		static const int WindowH = 360;

		// �R���X�g���N�^
		DesktopMascot()
		{
			
		}

	public:
		// �C���X�^���X�̐���
		static DesktopMascot* GetInstance()
		{
			if(instance == nullptr)
			{
				instance = new DesktopMascot;
			}
			return instance;
		}

		// �f�X�N�g�b�v�}�X�R�b�g�f�[�^�쐬
		void Create()
		{

			this->GetScreenSize();

			this->initScreenH = this->screenH;


			this->windowPosX = (this->screenW - this->WindowW) / 2;
			this->windowPosY = this->screenH - this->WindowH;
			SetWindowInitPosition(this->windowPosX, this->windowPosY);	// �E�B���h�E��\��������W�̎w��


			ChangeWindowMode(true);		// �E�C���h�E���[�h�ŋN��
			SetGraphMode(this->WindowW, this->WindowH, 32);		// �`��X�N���[���̃T�C�Y���w��


			SetWindowStyleMode(2);	// �E�B���h�E�̃X�^�C����g����

			SetUseBackBufferTransColorFlag(true);	// �E�B���h�E�𓧖�	

			SetUseUpdateLayerdWindowFlag(true);	// true �� UpdateLayerdWindowForSoftImage ���g�p����
		}

		// �}�X�N�쐬(DxLib_Init() �̌�Ɏ��s)
		void CreateMask()
		{
			int mask = MakeScreen(WindowW, WindowH, true);	// �`��Ώۂɂł���A���t�@�`�����l���t����ʂ��쐬

			this->softImage = MakeARGB8ColorSoftImage(this->WindowW, this->WindowH);	// ��ʎ�荞�ݗp�̃\�t�g�E�G�A�p�摜���쐬
		
			SetUsePremulAlphaConvertLoad(false);	

			SetDrawScreen(mask);					// �`����`��Ώۂɂł���A���t�@�`�����l���t����ʂɂ���
		}


		// ���[�v���̏���
		void Update()
		{	
			// �w��̈�̂�
			int old_scr_h = this->screenH;

			this->GetScreenSize();	// ���݂̃E�B���h�E�T�C�Y(�c)���擾���āA�O�̏c�̃T�C�Y�Ɣ�r

			// �^�X�N�o�[�𓮂������Ƃ��ɁA�E�B���h�E�̕\���ʒu��ύX����
			if (old_scr_h != this->screenH)
			{
				this->Move(this->windowPosX, this->windowPosY);
			}


		
			GetDrawScreenSoftImage(0, 0, this->WindowW, this->WindowH, this->softImage);	// �`���̉摜���\�t�g�C���[�W�Ɏ擾����
				
			UpdateLayerdWindowForPremultipliedAlphaSoftImage(this->softImage);	// ��荞�񂾃\�t�g�C���[�W���g�p���ē��߃E�C���h�E�̏�Ԃ��X�V����	
		}


		// �w�肳�ꂽ�E�B���h�E�̈ʒu��ύX
		void Move(int x, int y)
		{
			int h = this->screenH - this->initScreenH;	// �u�����̃X�N���[���̏c�̃T�C�Y�v�Ɓu�^�X�N�o�[�グ����������v�̍�
			this->windowPosX = x;
			this->windowPosY = y;
			MoveWindow(GetMainWindowHandle(), x, y + h, this->WindowW, this->WindowH, true);
		}

		// ���݂̃E�B���h�E�̈ʒu���擾
		void GetWindowPos(int &x, int &y)
		{
			x = this->windowPosX;
			y = this->windowPosY;
		}


	private:
		// �X�N���[���̑傫�����擾
		void GetScreenSize()
		{
			RECT rc;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0); // �f�X�N�g�b�v��Ɨ̈�̋�`���擾����B

			this->screenW = rc.right;
			this->screenH = rc.bottom;
		}

};