#pragma once
#include <map>
#include <string>
#include "Sprite.h"
#include "Texture.h"
class ImageManager  final {
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager & r) = default;
	ImageManager& operator= (const ImageManager & r) = default;

public:

	enum texName {
		Test,
		DebugBack,
		Black,
		Title,
		Clear,
		UISheet,
		TitleBack,
		Button,
		DebugMove,
		TitleMove,
		Bar,
		ZBack,
		Option,
		Vignette,
		PlayerCover,
		CharaCover,
		Num,
		numBack,
		slash,
		SceneCover,
	};
	enum tex2dName {
		Lock,
		Battle,
		Fire,
		InOut,
		Guid,
		Shadow,
	};
public:
	static ImageManager* GetIns();		//�擾�p

	void Load2D();
	void LoadTex2D();
	void Finalize();
};

