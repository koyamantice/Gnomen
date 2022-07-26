#include"Actor.h"
#include <SourceCode/FrameWork/collision/Collision.h>
#include"ActorComponent.h"
#include <memory>
#include <SourceCode/FrameWork/ActorManager.h>

int Actor::ID = 0;

bool Actor::Collide(Actor* actor) {
	//セットポジションとのコリジョン
	if (Collision::SphereCollision2(obj->GetPosition(), 1.0f, actor->obj->GetPosition(), 1.0f)) {
		if (actor->GetTag() == std::string("enemy")) {//タグcheck
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void Actor::Initialize(Model* model, const std::string& tag, ActorComponent* compornent) {
	id++;
	ID++;
	if (isActive) {
		this->compornent = compornent;
		this->tag = tag;
		Object3d* obj_ = new Object3d();
		obj_->SetModel(model);
		obj_->Initialize();
		obj.reset(obj_);
		//ActorManager::GetInstance()->AttachList(this);
		OnInit();
		if (this->compornent) { this->compornent->Initialize(); }
	}
}

void Actor::Update() {
	if (isActive) {
		if (compornent) { compornent->Update(); }
		obj->Update();
		OnUpda();
	}
}

void Actor::Draw() {
	if (isActive) {
		if (compornent) { compornent->Draw(); }
		Object3d::PreDraw();
		obj->Draw();
		OnDraw();
	}
}

void Actor::Finalize() {
	OnFinal();
}
