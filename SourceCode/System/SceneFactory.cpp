#include "SceneFactory.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "DebugScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンの作成
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	if (sceneName == "PLAY") {
		newScene = new PlayScene();
	}
	if (sceneName == "DEBUG") {
		newScene = new DebugScene();
	}
	return newScene;
}
