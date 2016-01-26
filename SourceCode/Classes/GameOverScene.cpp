#include "GameOverScene.h"
#include "GameScene.h"
#include "GameDefine.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOver::create();
	scene->addChild(layer);
	return scene;
}

bool GameOver::init()
{
	if(!Layer::init())
		return false;

	//显示GameOver
	TTFConfig config("fonts/Marker Felt.ttf", winSize_width/8);

	auto label = Label::createWithTTF(config, "GAME OVER");
	label->setPosition(Point(winSize_width/2, winSize_height/2));
	this->addChild(label);

	//添加返回按钮
	auto menuItemBack = MenuItemFont::create("New Game", CC_CALLBACK_1(GameOver::menuCallBack, this));
	auto menu = Menu::create(menuItemBack, NULL);

	menu->setPosition(Point::ZERO);
	menuItemBack->setPosition(Point(winSize_width*5/6, winSize_height/8));

	this->addChild(menu);

	return true;
}

void GameOver::menuCallBack(Ref* obj)
{
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}