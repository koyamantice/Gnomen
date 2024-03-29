#include"Hornet.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <SourceCode/Common/Easing.h>


void Hornet::OnInitialize() {
	LoadData("Bee");

	collide_size = 2.0f;

	InitCommon(
		ModelManager::GetInstance()->GetFBXModel(ModelManager::kBee),
		baseScale_,
		{ 0, -DEGREE_QUARTER, 0 }
	);

	fbxObject_->PlayAnimation(static_cast<size_t>(Animation_Type::kFlyAnimation));

	before_pos = { 50,20,-20 };

	compornent = new EnemyUI();
	compornent->Initialize();

	ActorManager::GetInstance()->AttachActor("Honey");
	ActorManager::GetInstance()->AttachActor("Honey");
	honey[kLeftHoney] = ActorManager::GetInstance()->SearchActor("Honey");
	honey[kRightHoney] = ActorManager::GetInstance()->SearchActorBack("Honey");

	honey[kLeftHoney]->SetPosition({ 25,0,25 });
	honey[kRightHoney]->SetPosition({ -25,0,-25 });

	player_ = ActorManager::GetInstance()->SearchActor("Player");

}


void Hornet::OnUpdate() {
	//ライフ管理
	LifeCommon();

	//関数ポインタで状態管理
	(this->*phaseFuncTable[static_cast<size_t>(phase_)])();

	ShadowUpdate();
	obj->SetPosition(fbxObject_->GetPosition());
	fbxObject_->Update();
}

void Hornet::OnDraw(DirectXCommon* dxCommon) {
	Object2d::PreDraw();
	//ImGui::SetNextWindowPos(ImVec2(1100, 240));
	//ImGui::SetNextWindowSize(ImVec2(180, 250));

	//ImGui::Begin("enemy");
	//ImGui::Text("phase_:%d",(int)phase_);
	//ImGui::Text("waittimer_:%f", waittimer_);
	//ImGui::Text("hit_once_:%d", hit_once_);
	//ImGui::Text("GetRotation.x:%f", fbxObject_->GetRotation().x);
	//ImGui::Text("GetRotation.y:%f", fbxObject_->GetRotation().y);
	//ImGui::Text("GetRotation.z:%f", fbxObject_->GetRotation().z);

	//ImGui::End();
	shadow_->Draw();
	Object3d::PreDraw();
	fbxObject_->Draw(dxCommon->GetCmdList());
}

void Hornet::OnFinalize() {
}

void Hornet::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		hit_once_ = true;
		switch (phase_) {
		case E_Phase::kPressAttack:
			if (motion_ != E_Motion::kFollowPlayer) {
				player_->SetHitBound(fbxObject_->GetPosition());
			}
			break;
		case E_Phase::kChasePlayer:
			player_->SetHitBound(fbxObject_->GetPosition());
			break;
		case E_Phase::kFeedHoney:

			player_->SetHitBound(fbxObject_->GetPosition());
			break;
		default:
			break;
		}
	}
}


void Hornet::HoneyControl() {
	if (honey[kLeftHoney]->GetCanMove()) {
		honey[kRightHoney]->SetPause(true);
	} else {
		honey[kRightHoney]->SetPause(false);
	}
	if (honey[kRightHoney]->GetCanMove()) {
		honey[kLeftHoney]->SetPause(true);
	} else {
		honey[kLeftHoney]->SetPause(false);
	}
}

void Hornet::IntroOnUpdate(const float timer) {
	XMFLOAT3 pos = fbxObject_->GetPosition();
	XMFLOAT3 rot = fbxObject_->GetRotation();

	if (timer <= 0.5f) {
		after_pos = {0,0,0};

		if ((timer / 0.5f) < 1.0f) {
			pos.x = Ease(Out, Quad, timer / 0.5f, before_pos.x, after_pos.x);
			pos.y = Ease(Out, Quad, timer / 0.5f, before_pos.y, after_pos.y);
			pos.z = Ease(Out, Quad, timer / 0.5f, before_pos.z, after_pos.z);
			rot.y = DirRotation(after_pos);
		} else {
			fbxObject_->PlayAnimation(static_cast<size_t>(Animation_Type::kPosingAnimiation));
			before_pos = after_pos;
		}
	} else {
		if (fbxObject_->GetIsFinish()) {
			fbxObject_->StopAnimation();
			fbxObject_->PlayAnimation(static_cast<size_t>(Animation_Type::kFlyAnimation));
		}
		rot.y = Ease(In,Linear,0.3f,rot.y,DirRotation(player_->GetPosition()));

	}
	fbxObject_->SetPosition(pos);
	fbxObject_->SetRotation(rot);
	fbxObject_->Update();

	shadow_->SetPosition({ fbxObject_->GetPosition().x,0.01f, fbxObject_->GetPosition().z });
	shadow_->Update();
}

void Hornet::StartAction() {
	XMFLOAT3 pos = fbxObject_->GetPosition();

	waittimer_ += 1.0f;
	fbxObject_->SetPosition({ pos.x,sinf(waittimer_* XM_PI / 180) * 2.0f,pos.z });
	
	//蜂蜜がある場合
	if (!honey[kLeftHoney]->GetCanMove()|| 
		!honey[kRightHoney]->GetCanMove()) {
		//どちらに向かうか
		for (int i = 0; i < kHoneyMax; i++) {
			if (!honey[i]->GetCanMove()) {
				honey_approch_ = i;
			}
		}
		//イージング準備
		before_pos = pos;
		after_pos = honey[honey_approch_]->GetPosition();
		//アニメーション変更
		fbxObject_->StopAnimation();
		fbxObject_->PlayAnimation(static_cast<size_t>(Animation_Type::kDownAnimation));

		waittimer_ = 0.0f;
		fade_frame_ = 0.f;
		phase_ = E_Phase::kFeedHoney;
	} else {
		if (waittimer_ > 240) {
			waittimer_ = 0.0f;
			before_pos = pos;
			phase_ = E_Phase::kAttackPredict;
		}
	}

}

void Hornet::AttackPredict() {
	//HP割合によってコマンド変更
	if (hp > max_hp * 0.8f) {
		phase_ = E_Phase::kPressAttack;
		motion_ = E_Motion::kFollowPlayer;
		collide_size = 2.0f;
	} else if ((hp > max_hp * 0.5f)) {
		phase_ = E_Phase::kChasePlayer;
		motion_ = E_Motion::kFirstMoving;
		collide_size = 2.0f;
	} else {
		std::mt19937 mt{ std::random_device{}() };
		std::uniform_int_distribution<int> rnd_phase(1, 100);

		if (rnd_phase(mt) > 50) {
			phase_ = E_Phase::kChasePlayer;
			motion_ = E_Motion::kFirstMoving;
		} else {
			phase_ = E_Phase::kPressAttack;
			motion_ = E_Motion::kFollowPlayer;
		}
		collide_size = 2.0f;
	}
}

void Hornet::PressAttack() {
	XMFLOAT3 player_pos = player_->GetPosition();
	XMFLOAT3 pos = fbxObject_->GetPosition();
	XMFLOAT3 rot = fbxObject_->GetRotation();
	
	waittimer_++;

	switch (motion_) {
	case E_Motion::kFollowPlayer:
		collide_size = 0.0f;
		isUnrivaled = true;
		if (hit_once_) {hit_once_ = false;}

		after_pos = {
		player_pos.x,
		sinf(waittimer_ * XM_PI / DEGREE_HALF) * 2.0f,
		player_pos.z
		};
		pos.x = Ease(In, Quad, 0.4f, pos.x, after_pos.x);
		pos.y = Ease(In, Quad, 0.4f, pos.y, after_pos.y);
		pos.z = Ease(In, Quad, 0.4f, pos.z, after_pos.z);

		if (waittimer_ > 100) {
			waittimer_ = 0;
			collide_size = 2.0f;
			motion_ = E_Motion::kPressBee;
			before_pos = pos;
			fbxObject_->ResetAnimation();
		}

		rot.y = DirRotation(player_pos);
		break;
	case  E_Motion::kPressBee:
		isUnrivaled = false;
		after_pos = {
		pos.x,
		-8,
		pos.z
		};
		if ((float)(waittimer_ / 50) < 1.0f) {
			pos.y = Ease(In, Quad, (float)(waittimer_ / 50), before_pos.y, after_pos.y);
		}
		if (waittimer_ > 120) {
			if (attack_count_ < 3) {
				attack_count_++;
				motion_ = E_Motion::kFollowPlayer;
			} else {
				attack_count_ = 0;
				motion_ = E_Motion::kLeaveHome;
			}
			before_pos = pos;
			waittimer_ = 0;
			fbxObject_->PlayAnimation(static_cast<size_t>(Animation_Type::kFlyAnimation));
			waittimer_ = 0;
		}
		rot.y += 5.0f;
		break;
	case E_Motion::kLeaveHome:
		after_pos = {
		0,
		sinf(waittimer_ * XM_PI / 180) * 2.0f,
		0
		};
		if ((float)(waittimer_ / 120) < 1.0f) {
			pos.x = Ease(In, Quad, (float)(waittimer_ / 120), before_pos.x, after_pos.x);
			pos.y = Ease(In, Quad, (float)(waittimer_ / 120), before_pos.y, after_pos.y);
			pos.z = Ease(In, Quad, (float)(waittimer_ / 120), before_pos.z, after_pos.z);
			rot.y = DirRotation(after_pos);
		} else {
			rot.y = DirRotation(player_pos);
		}
		if (waittimer_ > 150) {
			motion_ = E_Motion::kFollowPlayer;
			phase_ = E_Phase::kStartAction;
			waittimer_ = 0;
			before_pos = pos;
			isUnrivaled = true;
		}
		break;
	default:
		break;
	}
	fbxObject_->SetRotation(rot);
	fbxObject_->SetPosition(pos);

}

void Hornet::ChasePlayer() {
	XMFLOAT3 player_pos = player_->GetPosition();
	XMFLOAT3 pos = fbxObject_->GetPosition();
	XMFLOAT3 rot = fbxObject_->GetRotation();
	waittimer_++;
	switch (motion_) {
	case E_Motion::kFirstMoving:
		if (hit_once_) {hit_once_ = false;}
		after_pos = {
		45,
		0,
		45
		};
		if ((float)(waittimer_ / 130) < 1.0f) {
			pos.x = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.x, after_pos.x);
			pos.y = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.y, after_pos.y);
			pos.z = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.z, after_pos.z);
			rot.y = DirRotation(after_pos);
		}
		if (waittimer_ > 150) {
			motion_ = E_Motion::kSecondMoving;
			before_pos = pos;
			waittimer_ = 0;
		}
		break;
	case E_Motion::kSecondMoving:
		waittimer_++;
		if (hit_once_) { hit_once_ = false; }
		after_pos = {
		45,
		0,
		-45
		};
		if ((float)(waittimer_ / 130) < 1.0f) {
			pos.x = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.x, after_pos.x);
			pos.y = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.y, after_pos.y);
			pos.z = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.z, after_pos.z);
			rot.y = DirRotation(after_pos);
		}
		if (waittimer_ > 190) {
			motion_ = E_Motion::kThirdMoving;
			before_pos = pos;
			waittimer_ = 0;
		}
		break;
	case E_Motion::kThirdMoving:
		after_pos = {
		-45,
		0,
		45
		};
		waittimer_++;
		if ((float)(waittimer_ / 130) < 1.0f) {
			pos.x = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.x, after_pos.x);
			pos.y = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.y, after_pos.y);
			pos.z = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.z, after_pos.z);
			rot.y = DirRotation(after_pos);
		}
		if (waittimer_ > 190) {
			motion_ = E_Motion::kFourthMoving;
			before_pos = pos;
			waittimer_ = 0;
		}
		break;
	case E_Motion::kFourthMoving:
		after_pos = {
		-45,
		0,
		-45
		};
		waittimer_++;
		if ((float)(waittimer_ / 130) < 1.0f) {
			pos.x = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.x, after_pos.x);
			pos.y = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.y, after_pos.y);
			pos.z = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.z, after_pos.z);
			rot.y = DirRotation(after_pos);
		}
		if (waittimer_ > 190) {
			motion_ = E_Motion::kLeaveHome;
			before_pos = pos;
			waittimer_ = 0;
		}
		break;
	case E_Motion::kLeaveHome:
		waittimer_++;
		after_pos = {
		0,
		0,
		0
		};
		waittimer_++;
		if ((float)(waittimer_ / 130) < 1.0f) {
			pos.x = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.x, after_pos.x);
			pos.y = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.y, after_pos.y);
			pos.z = Ease(In, Quad, (float)(waittimer_ / 130), before_pos.z, after_pos.z);
			rot.y = DirRotation(after_pos);
		}
		if (waittimer_ > 190) {
			motion_ = E_Motion::kLeaveHome;
			phase_ = E_Phase::kStartAction;
			waittimer_ = 0;
			before_pos = pos;
		}
		break;

	default:
		break;
	}

	fbxObject_->SetPosition(pos);
	fbxObject_->SetRotation(rot);

}

void Hornet::FeedHoney() {
	if (!honey[honey_approch_]->GetCanMove()) { honey[honey_approch_]->SetCanMove(true); }

	collide_size = 4.0f;
	isUnrivaled = false;

	if (fade_frame_  < 1.0f) {
		fade_frame_  += 0.01f;
		XMFLOAT3 rot{};
		rot.y = DirRotation(after_pos);
		fbxObject_->SetRotation(rot);
	}
	XMFLOAT3 pos{};
	pos.x = Ease(In, Quad, fade_frame_ , before_pos.x, after_pos.x);
	pos.y = Ease(In, Quad, fade_frame_ , 0, -3);
	pos.z = Ease(In, Quad, fade_frame_ , before_pos.z, after_pos.z);
	fbxObject_->SetPosition(pos);

}

void Hornet::SpecialPinch() {
	isUnrivaled = true;
	fbxObject_->StopAnimation();
	fbxObject_->PlayAnimation(static_cast<size_t>(Animation_Type::kFlyAnimation));
	old_phase_ = E_Phase::kStartAction;

}


float Hornet::DirRotation(const XMFLOAT3& target) {
	float itr{};
	XMFLOAT3 pos = fbxObject_->GetPosition();
	XMFLOAT3 position{};

	position.x = (target.x - pos.x);
	position.z = (target.z - pos.z);
	itr = (atan2f(position.x, position.z) * (DEGREE_HALF / XM_PI)) - DEGREE_QUARTER;

	return itr;
}