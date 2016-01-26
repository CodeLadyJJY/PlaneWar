#include "Plane.h"
#include "GameDefine.h"
#include "GameScene.h"

USING_NS_CC;

PlaneSprite* PlaneSprite::create()
{
	PlaneSprite* pRet = new PlaneSprite();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool PlaneSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	Sprite* sprite;

	sprite = Sprite::create("JitPlane.png");
	this->addChild(sprite);
	
	m_hitNum = 0;

	return true;
}