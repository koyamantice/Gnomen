#pragma once
#include"ActorComponent.h"
#include <Player.h>
#include <SourceCode/Obj/2d/Sprite.h>

class EnemyUI : public  ActorComponent {
public:
	EnemyUI();
	~EnemyUI();
	void OnInitialize() override;	//����������(�p����)
	void OnUpdate() override;	//�X�V����(�p����)
	void OnFinalize() override;	//�I������(�p����)
	void OnDraw() override;		//�`�揈��(�p����)
	void OnAttached() override;	//�Q�[���I�u�W�F�N�g�ɃA�^�b�`���ꂽ�Ƃ��ɌĂ΂��֐�

private:
	std::unique_ptr<Sprite> HpCover;
	std::unique_ptr<Sprite> HpGauge;
	std::unique_ptr<Sprite> UnderGauge;



};