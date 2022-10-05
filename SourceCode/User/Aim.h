#pragma once
#include <SourceCode/Obj/2d/Texture.h>
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Texture.h>
#include<DirectXMath.h>
#include"Input.h"
#include"Actor.h"

class Player;

class Aim {
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	void Init();
	void Upda();
	void Draw();
	void FirstSet();
private:
	std::unique_ptr<Texture> LockOn{};
	Input* input = Input::GetInstance();

	void Move();
	void EnemySet();

	float angle = 0;
	float distance = 5.0f;
	Actor* player = nullptr;
	Actor* bullet = nullptr;
};