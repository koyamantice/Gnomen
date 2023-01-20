#include "Honey.h"
#include <ActorManager.h>
#include <Easing.h>
#include <ModelManager.h>

void Honey::OnInit() {
	obj->SetScale(base_sca);
	//必要人数
	stock = 0;


	for (int i = 0; i < 2;i++) {
		for (int j = 0; j < 6;j++) {
			Object2d* mission_ = Object2d::Create(ImageManager::kMission_0 + j, { 0,0,0 },
				{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
			mission_->SetIsBillboard(true);
			mission_->Object2dCreate();
			mission_->SetRotation({ 0,0,0 });
			missions[i][j].reset(mission_);
		}
	}
	Object2d* slash_ = Object2d::Create(ImageManager::kMission_s, { 0,0,0 },
		{ 0.2f,0.2f,0.2f }, { 1,1,1,1 });
	slash_->SetIsBillboard(true);
	slash_->Object2dCreate();
	slash_->SetRotation({ 0,0,0 });
	slash.reset(slash_);
	command = WAIT;
}


void Honey::OnUpda() {
	missionUpdate();
	questionUpdate();

	switch (command) {
	case Actor::Phase::APPROCH:
		ApprochUpda();
		break;
	case Actor::Phase::LEAVE:
		LeaveUpda();
		break;
	case Actor::Phase::WAIT:
		WaitUpda();
		break;
	case Actor::Phase::DEAD:
		DeadUpda();
		break;
	case Actor::Phase::UNGUARD:
		IntroOnUpdate(0);
		break;
	default:
		break;
	}
}

void Honey::OnDraw(DirectXCommon* dxCommon) {
	Object3d::PreDraw();

	Object2d::PreDraw();
	if (obj->GetPosition().y >= -0.01f) {
		if (command == LEAVE || command == WAIT) {
			if (stock <= 5) {
				missions[0][stock]->Draw();
			}
			missions[1][5]->Draw();
			slash->Draw();
		}
	}
}

void Honey::OnFinal() {
}

void Honey::OnCollision(const std::string& Tag) {


}

void Honey::missionUpdate() {
	XMFLOAT3 pos = obj->GetPosition();
	for (int i = 0; i < 6; i++) {
		missions[0][i]->Update();
		missions[0][i]->SetPosition({ pos.x,pos.y + 6.5f,pos.z });
		missions[1][i]->Update();
		missions[1][i]->SetPosition({ pos.x,pos.y + 3.5f,pos.z });
	}
	slash->Update();
	slash->SetPosition({ pos.x,pos.y + 5.0f,pos.z });
}

void Honey::questionUpdate() {
}

void Honey::ApprochUpda() {


}

void Honey::LeaveUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	if (frame > 1.0f) {
		frame = 1.0f;
		if (unload_frame>1.0f) {
			frame = 0.0f;
			unload_frame = 0.0f;
			command = APPROCH;
			before_pos = pos;
			stock = 0;
			old_stock = stock;
			ride_num = 0;
			for (int i = 0; i < 5; i++) {
				if (driver[i] != nullptr) {
					driver[i]->Navigation(first_pos);
					driver[i]->SetRotation({ 0,driver[i]->DirRotation(first_pos),0 });
					driver[i] = nullptr;
				}
			}
			return;
		} else {
			unload_frame += 0.01f;
		}
	} else {
		frame += 0.001f;
	}

	after_pos = { 0, 0 ,0 };
	pos.x = Ease(In, Linear, frame, before_pos.x, after_pos.x);
	pos.y = Ease(In, Linear, unload_frame, before_pos.y, after_pos.y);
	pos.z = Ease(In, Linear, frame, before_pos.z, after_pos.z);

	for (int i = 0; i < 5; i++) {
		if (driver[i] == nullptr) { continue; }
		driver[i]->SetPosition({ pos.x + (sinf(((i + 1) * 72) * XM_PI / 180) * 6.0f),0,pos.z + (cosf(((i + 1) * 72) * XM_PI / 180) * 6.0f) });
		driver[i]->SetRotation({0,driver[i]->DirRotation(after_pos),0});
	}
	obj->SetPosition(pos);
}

void Honey::WaitUpda() {
	XMFLOAT3 pos = obj->GetPosition();
	if(pos.y < 0.0f){
		pos.y += 0.02f;
	} else {
		if (stock > 6) { stock = 5; }
		if (collide) {
			driver[ride_num] = ActorManager::GetInstance()->SetActionBullet(obj->GetPosition());
			if (driver[ride_num] != nullptr) {
				driver[ride_num]->SetsPlayActive(true);
				ride_num++;
				stock++;
			}
			collide = false;
		}
		if (stock == 5 && !pause) {
			stock = 5;
			first_pos = pos;
			pos.y = 3.0f;
			before_pos = pos;
			command = LEAVE;
		}
	}
	for (int i = 0; i < 5;i++) {
		if (driver[i] == nullptr) { continue; }
		driver[i]->SetPosition({ pos.x+(sinf(((i+1)*72)*XM_PI/180)*6.0f),0,pos.z + (cosf(((i + 1) * 72) * XM_PI / 180) * 6.0f) });
	}

	obj->SetPosition(pos);
}

void Honey::DeadUpda() {

	if (frame > 1.0f) {
		frame = 0.0f;
		obj->SetScale(base_sca);
		const float rnd_area = 80.0f;
		
		float posX= (float)rand() / RAND_MAX * rnd_area - rnd_area / 2.0f;
		float posZ= (float)rand() / RAND_MAX * rnd_area - rnd_area / 2.0f;
		obj->SetPosition({posX,-5.0f,posZ });
		command = WAIT;
		return;
	} else {
		frame += 0.0016f;
	}

	float scale = Ease(In,Quad,frame, base_sca.x,0);
	obj->SetScale({ scale ,scale ,scale });

}

void Honey::IntroOnUpdate(const float& Timer) {
	//関数が渡す最初の値
	const float start_timer = 0.5f;
	//
	XMFLOAT3 pos = obj->GetPosition();
	//
	float time = Timer - start_timer;
	if (time <= 0.1f) {
		pos.x = Ease(In, Linear, time/ 0.1f, 15,15);
		pos.y = Ease(In, Linear, time/ 0.1f, 15, 0);
		pos.z = Ease(In, Linear, time/ 0.1f, 10, 10);
	} else {
		int a = 0;
		a++;

	}


	obj->SetPosition(pos);

}

