#include "Aim.h"
#include "ImageManager.h"
#include "ActorManager.h"
#include <DirectXMath.h>
#include <SourceCode/Common/Easing.h>
using namespace DirectX;

void Aim::Init() {
	Object2d* Lock_ = Object2d::Create(ImageManager::Lock, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	Lock_->Object2dCreate();
	Lock_->SetRotation({ 90,0,0 });
	Lock_->SetColor({ 1.0f,0.2f,0.2f ,0.6f });
	LockOn.reset(Lock_);
	LockOn->SetPosition({ 100,-50,0 });

	Object2d* Whistle_ = Object2d::Create(ImageManager::Lock, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 });
	Whistle_->Object2dCreate();
	Whistle_->SetRotation({ 90,0,0 });
	Whistle_->SetColor({ 1.0f,1.0f,1.0f ,0.5f });
	Whistle.reset(Whistle_);
	Whistle->SetPosition({ 100,-50,0 });


	Object2d* FirstUI_ = Object2d::Create(ImageManager::SlowUI, { 0,0,0 }, { 0.3f,0.3f,0.3f }, { 1,1,1,1 });
	FirstUI_->Object2dCreate();
	FirstUI_->SetRotation({ 0,0,0 });
	FirstUI_->SetIsBillboard(true);
	FirstUI.reset(FirstUI_);
	FirstUI->SetPosition({ 100,-50,0 });

	Object2d* SecondUI_ = Object2d::Create(ImageManager::SetUI, { 0,0,0 }, { 0.3f,0.3f,0.3f }, { 1,1,1,1 });
	SecondUI_->Object2dCreate();
	SecondUI_->SetRotation({ 0,0,0 });
	SecondUI_->SetIsBillboard(true);
	SecondUI[0].reset(SecondUI_);
	SecondUI[0]->SetPosition({ 100,-50,0 });

	Object2d* SecondUI2_ = Object2d::Create(ImageManager::SetUI2, { 0,0,0 }, { 0.3f,0.3f,0.3f }, { 1,1,1,1 });
	SecondUI2_->Object2dCreate();
	SecondUI2_->SetRotation({ 0,0,0 });
	SecondUI2_->SetIsBillboard(true);
	SecondUI[1].reset(SecondUI2_);
	SecondUI[1]->SetPosition({ 100,-50,0 });

	for (int i = 0; i < GuidNum; i++) {
		Object2d* Guid_ = Object2d::Create(ImageManager::Guid, { 0,0,0 }, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
		Guid_->Object2dCreate();
		Guid_->SetRotation({ 90,0,0 });
		Guid_->SetColor({ 1.0f,1.0f,1.0f ,0.6f });
		Guid[i].reset(Guid_);
	}
}

void Aim::Upda(float angle) {
	LockOn->Update();
	Whistle->Update();
	FirstUI->Update();
	SecondUI[0]->Update();
	SecondUI[1]->Update();
	animeframe++;
	if (animeframe > 50) {
		Animation += vel;
		vel *= -1;
		animeframe = 0;
	}
	if (!first) {
		FirstAlpha *= 0.9f;
		FirstUI->SetColor({ 1,1,1,FirstAlpha });
	}
	if (!second) {
		SecondAlpha *= 0.9f;
		SecondUI[0]->SetColor({ 1,1,1,SecondAlpha });
		SecondUI[1]->SetColor({ 1,1,1,SecondAlpha });
	}
	for (int i = 0; i < GuidNum; i++) {
		Guid[i]->Update();
	}
	Move(angle);
	EnemySet();

}

void Aim::Draw() {
	//ImGui::Begin("test");
	//for (int i = 0; i < GuidNum; i++) {
	//	ImGui::SliderFloat("@:@", &GuidPos[i].y, 0, 360);
	//}
	//////ImGui::Unindent();
	//ImGui::End();
	Object2d::PreDraw();
	LockOn->Draw();
	Whistle->Draw();
	FirstUI->Draw();
	if (FirstAlpha < 0.01f) {
		SecondUI[Animation]->Draw();
	}
	for (int i = 0; i < GuidNum; i++) {
		Guid[i]->Draw();
	}
}



void Aim::FirstSet() {
	player = ActorManager::GetInstance()->SearchActor("Player");
	XMFLOAT3 pos = player->GetPosition();
	LockOn->SetPosition({ pos.x,pos.y + 0.1f,pos.z });

}

void Aim::Move(float angle) {
	if (input->TriggerButton(Input::B) || input->TriggerKey(DIK_SPACE)) {
		if (first) {
			first = false;
		}
		player = ActorManager::GetInstance()->SearchActor("Player");
		player->SetStock(player->GetStock() - 1);
		bullet = ActorManager::GetInstance()->SearchWaitBullet();
		if (bullet != nullptr) {
			bullet->SetCommand(Bullet::command::Slow, LockOn->GetPosition());
		}
	}
	XMFLOAT3 Lrot = LockOn->GetRotation();

	Lrot.y += 2.0f;

	LockOn->SetRotation(Lrot);

	if (input->PushButton(Input::A)) {
		if (!first) {
			if (second) {
				second = false;
			}
		}
		if (Area < 8.0f) {
			Area += 0.08f;
		}
		XMFLOAT3 base = LockOn->GetPosition();
		for (int i = 0; i < 1; i++) {
			const float rnd_rad = 360.0f;
			XMFLOAT3 pos{};
			float angle = (float)rand() / RAND_MAX * rnd_rad;
			pos.x = base.x + Area * sinf(angle);
			pos.z = base.z + Area * cosf(angle);
			const float rnd_vel = 0.4f;
			XMFLOAT3 vel{};
			//vel.x = (float)rand() / RAND_MAX * rnd_vel;// -rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel;
			//vel.z = -(float)rand() / RAND_MAX * rnd_vel;// -rnd_vel / 2.0f;
			ParticleManager::GetInstance()->Add(1, 15, pos, vel, XMFLOAT3(), 1.0f, 0.0f);
		}
		ActorManager::GetInstance()->ChangeBulletCommand(base, Area);
		Whistle->SetPosition({ base.x, base.y+(Area/4), base.z });
		Whistle->SetScale({ Area * 0.3f,Area * 0.3f,Area * 0.3f });
		Whistle->SetRotation({90 ,Lrot.y * 2.0f ,Lrot.z * 2.0f });
		Whistle->SetColor({ 1.0f,1.0f,1.0f,1.1f-(Area/8.0f) });

	} else {
		Area = 0.0f;
		Whistle->SetScale({ Area,Area,Area });
	} 

	XMFLOAT3 Lpos = LockOn->GetPosition();
	XMFLOAT3 pos = player->GetPosition();


	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float PI = 3.14159f;
	const float STICK_MAX = 32768.0f;

	if (input->TiltPushStick(Input::L_UP   ,0.1f) ||
		input->TiltPushStick(Input::L_DOWN ,0.1f) ||
		input->TiltPushStick(Input::L_RIGHT,0.1f) ||
		input->TiltPushStick(Input::L_LEFT ,0.1f)) {
		AfterPos = player->GetCameraPos(player->GetRotation().y,10);
	}
	Lpos.x = Ease(In, Quad, 0.95f, Lpos.x, AfterPos.x);
	Lpos.y = Ease(In, Quad, 0.95f, Lpos.y, AfterPos.y);
	Lpos.z = Ease(In, Quad, 0.95f, Lpos.y, AfterPos.z);
	if (Lpos.x > 48.0f) { Lpos.x = 48.0f; }
	if (Lpos.x < -48.0f) { Lpos.x = -48.0f; }
	if (Lpos.z > 48.0f) { Lpos.z = 48.0f; }
	if (Lpos.z < -48.0f) { Lpos.z = -48.0f; }
	LockOn->SetPosition({ Lpos.x,0.01f,Lpos.z });
	FirstUI->SetPosition({ Lpos.x,1.0f * sinf((Lrot.y + 2) * PI / 180.0f) + 3.5f ,Lpos.z });
	SecondUI[0]->SetPosition({ Lpos.x,1.0f * sinf((Lrot.y + 2) * PI / 180.0f) + 3.5f ,Lpos.z });
	SecondUI[1]->SetPosition({ Lpos.x,1.0f * sinf((Lrot.y + 2) * PI / 180.0f) + 3.5f ,Lpos.z });


	for (int i = 0; i < GuidNum; i++) {
		GuidPos[i].x = Ease(InOut, Quad, (i + 1) * 0.1f, pos.x, Lpos.x);
		GuidPos[i].y = 0.1f;
		GuidPos[i].z = Ease(InOut, Quad, (i + 1) * 0.1f, pos.z, Lpos.z);
		Guid[i]->SetPosition(GuidPos[i]);
	}
}

void Aim::EnemySet() {
	if (input->TriggerButton(Input::RT)) {
		Actor* enemy = ActorManager::GetInstance()->SearchActorArea(player->GetPosition());
		//->SearchActor("Enemy");
		XMFLOAT3 base = LockOn->GetPosition();
		base = enemy->GetPosition();
		base.y = 0.18f;
		LockOn->SetPosition(base);
	}
}


DirectX::XMFLOAT3 Aim::MoveVector(XMVECTOR v, float angle) {
	XMMATRIX rot{};
	rot = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot);
	XMFLOAT3 pos = { v.m128_f32[0],v.m128_f32[1] ,v.m128_f32[2] };
	return pos;
}