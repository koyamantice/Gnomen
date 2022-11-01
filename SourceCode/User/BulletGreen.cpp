#include"BulletGreen.h"
#include "Easing.h"
#include"ActorManager.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ImageManager.h"
#include"ModelManager.h"
using namespace DirectX;



BulletGreen::BulletGreen() {
}

void BulletGreen::OnInit() {
	ID = ActorManager::GetInstance()->SearchNum("BulletGreen");
	command = Wait;
	player = ActorManager::GetInstance()->SearchActor("Player");
	enemy = ActorManager::GetInstance()->SearchActorBack("Enemy");
	landing = player->GetLockPos();
	Texture* Lock_ = Texture::Create(ImageManager::Battle, { fbxObj->GetPosition().x,fbxObj->GetPosition().y + 1.0f,fbxObj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Lock_->SetIsBillboard(true);
	Lock_->TextureCreate();
	Lock_->SetRotation({ 0,0,0 });
	Status.reset(Lock_);

	Texture* Explo_ = Texture::Create(ImageManager::Fire, { fbxObj->GetPosition().x,fbxObj->GetPosition().y + 1.0f,fbxObj->GetPosition().z
		}, { 0.1f,0.1f,0.1f }, { 1,1,1,1 });
	Explo_->SetIsBillboard(true);
	Explo_->TextureCreate();
	Explo_->SetRotation({ 0,0,0 });
	Explo.reset(Explo_);

	WaitBullet();
}

void BulletGreen::OnUpda() {
	if (DeadFlag) {
		Dead();
		return;
	}
	switch (command) {
	case Wait:
		WaitUpda();
		break;
	case Attack:
		if (enemy->GetIsActive()) {
			if (knockBacking) {
				KnockBack();
			} else {
				AttackUpda();
			}
		}
		break;
	case Slow:
		SlowUpda();
		break;
	default:
		assert(0);
		break;
	}
	if (burning) {
		Explo->Update();
		BurnOut();
	}
	Status->Update();
	Status->SetPosition({ fbxObj->GetPosition().x,fbxObj->GetPosition().y + 2.5f,fbxObj->GetPosition().z });
}
void BulletGreen::Follow2Enemy() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	XMFLOAT3 rot = fbxObj->GetRotation();
	XMFLOAT3 position{};
	position.x = (enemy->GetPosition().x - pos.x);
	position.z = (enemy->GetPosition().z - pos.z);
	rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 180; //- 90;// *(XM_PI / 180.0f);
	vel_follow.x = sin(-atan2f(position.x, position.z)) * 0.3f;
	vel_follow.y = cos(-atan2f(position.x, position.z)) * 0.3f;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	fbxObj->SetPosition(pos);
	fbxObj->SetRotation(rot);
}

void BulletGreen::Follow2Player() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	XMFLOAT3 rot = fbxObj->GetRotation();
	float vel = (int)(rand() % 10 + 1) * 0.03f;
	XMFLOAT3 position{};
	position.x = (player->GetPosition().x - pos.x);
	position.z = (player->GetPosition().z - pos.z);
	rot.y = (atan2f(position.x, position.z) * (180.0f / XM_PI)) - 180; //- 90;// *(XM_PI / 180.0f);
	vel_follow.x = sinf(-atan2f(position.x, position.z)) * vel;
	vel_follow.y = cosf(-atan2f(position.x, position.z)) * vel;
	pos.x -= vel_follow.x;
	pos.z += vel_follow.y;
	//fbxObj->SetPosition(pos);
	fbxObj->SetRotation(rot);

}

void BulletGreen::WaitBullet() {

	fbxObj->SetPosition({ ((int)ID % 10) * 3.0f, 0, ((int)ID / 10) * 5.0f });
}

void BulletGreen::KnockBack() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	rebound.x = sin(atan2f(distance.x, distance.z)) * 0.5f;
	rebound.z = cos(atan2f(distance.x, distance.z)) * 0.5f;

	if (damageframe >= 1.0f) {
		damageframe = 0.0f;
		knockBacking = false;
		fall = 0.4f;
		pos.y = 0.0f;
		fbxObj->SetPosition(pos);
		return;
	} else {
		damageframe += 0.05f;
	}

	rebound = {
	Ease(InOut,Quad,damageframe,rebound.x,0),
	0,
	Ease(InOut,Quad,damageframe,rebound.z,0)
	};
	pos.y += fall; //+
	fall -= 0.16f;//
	if (pos.y < 0.0f) {
		pos.y = 0;
	}
if (pos.x <= 100.0f && pos.x >= -100.0f) {
	pos.x -= rebound.x;
}
if (pos.z <= 100.0f && pos.z >= -100.0f) {
	pos.z -= rebound.z;
}

fbxObj->SetPosition(pos);
}

void BulletGreen::DamageInit() {
	if (!knockBacking) {
		enemy->SetHp(enemy->GetHp() - 1);
		burning = true;
		XMFLOAT3 pos = fbxObj->GetPosition();
		XMFLOAT3 pos2 = enemy->GetPosition();
		if (pos2.x < pos.x) {
			isLeft = true;
		}
		fall = 1.0f;
		exploPos = pos;
		distance = { pos2.x - pos.x,0,pos2.z - pos.z };
		knockBacking = true;
	}
}


void BulletGreen::Dead() {
	isRemove = true;
}

void BulletGreen::BurnOut() {
	XMFLOAT3 pos = fbxObj->GetPosition();
	if (effectRate < 1.0f) {
		effectRate += 0.08f;
	} else {
		effectRate = 0;
		Explo->SetScale({ 0,0,0 });
		Explo->SetPosition({ pos.x,-100,pos.z });
		burning = false;
	}
	scale = Ease(In, Quad, effectRate, 0.5f, 1.0f);
	Explo->SetScale({ scale,scale,scale });
	Explo->SetPosition(exploPos);
}

void BulletGreen::OnDraw(DirectXCommon* dxCommon) {
	if (enemy == NULL) { return; }
	if (enemy->GetIsActive()) {
		if (command == Wait) { return; }
		Texture::PreDraw();
		if (Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
			Status->Draw();
		}
		if (burning) {
			Explo->Draw();
		}
	}
}
void BulletGreen::OnFinal() {
}

void BulletGreen::OnCollision(const std::string& Tag) {
	if (Tag == "Player") {
		switch (command) {
		case Wait:

			break;
		case Attack:
			player->SetStock(player->GetStock() + 1);
			command = Wait;
			break;
		case Slow:

			break;

		default:
			assert(0);
			break;
		}
	}

	if (Tag == "Enemy") {
		int a = 0;
		switch (command) {
		case Wait:
			break;
		case Attack:
			switch (enemy->GetCommand()) {
			case Actor::Phase::WAIT:
				DamageInit();
				break;
			case Actor::Phase::ATTACK:
				break;
			case Actor::Phase::LEAVE:
				DamageInit();
				break;
			default:
				break;
			}

			break;
		case Slow:

			break;

		default:
			assert(0);
			break;
		}

	}




}

void BulletGreen::WaitUpda() {
	throwReady = true;
	XMFLOAT3 pos = fbxObj->GetPosition();
	if (pos.y > 0) {
		pos.y -= 0.3f;
	} else {
		pos.y = 0;
	}
	if (pos.x > 100) { pos.x = 100; }
	if (pos.z > 100) { pos.z = 100; }
	if (pos.x < -100) { pos.x = -100; }
	if (pos.z < -100) { pos.z = -100; }
	vel = 0.8f;
	frame = 0.0f;

	fbxObj->SetPosition(pos);

	if (!Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 3.0f, player->GetPosition().x, player->GetPosition().z, 1.0f)) {
		Follow2Player();
		//WaitBullet();
	}
}

void BulletGreen::SlowUpda() {
	if (throwReady) {
		XMFLOAT3 pos = fbxObj->GetPosition();
		XMFLOAT3 rot = fbxObj->GetRotation();
		pos.x = Ease(InOut, Quad, frame, pos.x, AftaerPos.x);
		pos.y += vel; //+
		vel -= 0.05f;//
		if (frame< 0.7f) {
			rot.x = Ease(In, Quad, frame+0.3f, 0, -360);
		}
		if (pos.y < 0.0f) {
			pos.y = 0;
		}
		pos.z = Ease(InOut, Quad, frame, pos.z, AftaerPos.z);
		if (frame < 1.0f) {
			frame += 0.02f;
		} else {
			frame = 1.0f;
			pos.y = 0.0f;
			rot.x = 0;
			vel = 0.8f;
			throwReady = false;
		}
		fbxObj->SetPosition(pos);
		fbxObj->SetRotation(rot);

	} else {
		frame = 0.0f;
		vel = 0.8f;
		fbxObj->SetRotation({0,fbxObj->GetRotation().y,0});
		command = Attack;
	}

}

void BulletGreen::AttackUpda() {
	if (!knockBacking) {
		if (Collision::CircleCollision(fbxObj->GetPosition().x, fbxObj->GetPosition().z, 15.0f, enemy->GetPosition().x, enemy->GetPosition().z, 1.0f)) {
			Follow2Enemy();
		}
	}


}
