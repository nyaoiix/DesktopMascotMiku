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
				MV1DetachAnim(handle, attach);	// �A�j���[�V�����폜
			}
			attachID = id;
			attach = MV1AttachAnim(handle, id);	// �A�j���[�V�����̓ǂݍ���

			playTime = 0.0f;
			totalTime = MV1GetAttachAnimTotalTime(handle, attach);	// �A�j���[�V�����̍Đ�����
		}
		
	}
};


// DxLib �̃��C��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DesktopMascot* mascot = DesktopMascot::GetInstance();

	mascot->Create();	//��ʏ�����


	// DxLib �̏�����
	if (DxLib_Init() < 0)
	{
		return -1;
	}

	mascot->CreateMask();	// �}�X�N�쐬(DxLib_Init() �̌�Ɏ��s)

	SetAlwaysRunFlag(true);	// �E�C���h�E���A�N�e�B�u�ł͂Ȃ���Ԃł������𑱍s���邩(true:���s)

	// ��ɍőO�ʂɕ\��
	SetWindowPos(GetMainWindowHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	Model model;

	// 3D���f���̓ǂݍ���
	model.handle= MV1LoadModel("dat/model/model.mv1");
	model.attachID = -1;
	model.SetAnimation(0);	// �ҋ@���[�V������ǂݍ���

	mascot->GetWindowPos(model.x, model.y);


	// �J�����̈ʒu�Ȃ�
	SetCameraNearFar(1.0f, 1000.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 12.5f, -22.0f), VGet(0.0f, 12.5f, 0.0f));

	const float pi = 3.141592f; // �~������

	bool is_push_key = true;
	
	// ���C�����[�v
	for(; ProcessMessage()==0 && ClearDrawScreen()==0;)
	{
		MV1DrawModel(model.handle);	// 3D���f���`��

		model.playTime += 0.25f;
		if (model.playTime >= model.totalTime)
		{
			// �u�ҋ@�v�u����v���[�V�����̎�
			if (model.attachID <= 1)
			{
				// ���[�v������
				model.playTime = 0.0f;
			}
			else
			{
				// �|�[�Y�@�`�B�̃A�j���[�V�������I�������A�ҋ@���[�V�����ɖ߂�
				model.SetAnimation(0);
				is_push_key = true;
			}
		}

		MV1SetAttachAnimTime(model.handle, model.attach, model.playTime);	// �Đ����Ԃ��Z�b�g


		if (is_push_key)
		{
			if (CheckHitKey(KEY_INPUT_LEFT))
			{
				model.x -= 3;
				model.SetAnimation(1); // ���郂�[�V������ǂݍ���
				MV1SetRotationXYZ(model.handle, VGet(0.0f, pi / 2.0f, 0.0f));	// ���f�����������ɂ���
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT))
			{
				model.x += 3;
				model.SetAnimation(1); // ���郂�[�V������ǂݍ���
				MV1SetRotationXYZ(model.handle, VGet(0.0f, -pi / 2.0f, 0.0f));	// ���f�����E�����ɂ���
			}
			else if (CheckHitKey(KEY_INPUT_UP))
			{
				model.SetAnimation(1); // ���郂�[�V������ǂݍ���
				MV1SetRotationXYZ(model.handle, VGet(0.0f, pi, 0.0f));	// ���f�����������ɂ���
			}
			else if (CheckHitKey(KEY_INPUT_DOWN))
			{
				model.SetAnimation(1); // ���郂�[�V������ǂݍ���
				MV1SetRotationXYZ(model.handle, VGet(0.0f, 0.0f, 0.0f));	// ���f����O�����ɂ���
			}
			else if (CheckHitKey(KEY_INPUT_Z))
			{
				is_push_key = false;
				model.SetAnimation(2); // �|�[�Y�@��ǂݍ���
			}
			else if (CheckHitKey(KEY_INPUT_X))
			{
				is_push_key = false;
				model.SetAnimation(3); // �|�[�Y�A��ǂݍ���
			}
			else if (CheckHitKey(KEY_INPUT_C))
			{
				is_push_key = false;
				model.SetAnimation(4); // �|�[�Y�B��ǂݍ���
			}
			else
			{
				model.SetAnimation(0);	// �ҋ@���[�V�����ǂݍ���
			}
		}

		// ESC�L�[�ŏI��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			// �I��
			break;
		}


		DrawString(0,  0, "���F�E", GetColor(255, 255, 255));
		DrawString(0, 20, "���F��", GetColor(255, 255, 255));
		DrawString(0, 40, "���F���", GetColor(255, 255, 255));
		DrawString(0, 60, "���F����", GetColor(255, 255, 255));
		DrawString(0, 90, "Z�F�|�[�Y�@", GetColor(255, 255, 255));
		DrawString(0, 110, "X�F�|�[�Y�A", GetColor(255, 255, 255));
		DrawString(0, 130, "C�F�|�[�Y�B", GetColor(255, 255, 255));
		DrawString(0, 160, "ESC�F�I��", GetColor(255, 255, 255));

		mascot->Move(model.x, model.y);
		mascot->Update();
	}


	// DxLib �̏I��
	DxLib_End();

	return 0;
}


