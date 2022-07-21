#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Texture.h>


class Crystal :public Actor {
	enum class Phase {
		Approch,
		Leave,
		Wait,
	};
public:
	Crystal() {};
	//void LoadData();
	//void UpdateCommand();
	//void DebugUpdate();
private:
	Input* input = Input::GetInstance();
	std::stringstream parameterCommands;


	void OnInit()override;
	void OnUpda()override;
	void OnDraw()override;
	void OnFinal()override;

	//�t�F�[�Y�܂Ƃ�
	//void PhaseMove();
	//void ApprochUpdate();
	//void LeaveUpdate();
	//void WaitUpdate();
	////
	//void LifeCommon();

	float waitTimer = 0;
	float vel = 0;
};