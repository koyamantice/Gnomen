#include "SceneFactory.h"
#include "TitleScene.h"
#include "DebugScene.h"
#include "FirstStage.h"
#include "SecondStage.h"
#include "MSecondStage.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンの作成
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	if (sceneName == "DEBUG") {
		newScene = new DebugScene();
	}
	if (sceneName == "FIRSTSTAGE") {
		newScene = new FirstStage();
	}
	if (sceneName == "MSECOND") {
		newScene = new MSecondStage();
	}
	if (sceneName=="SECONDSTAGE") {
		newScene = new SecondStage();
	}
	return newScene;
}
