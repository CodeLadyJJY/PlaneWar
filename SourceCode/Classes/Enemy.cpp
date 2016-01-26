#include "Enemy.h"

USING_NS_CC;

Enemy* Enemy::create()
{
	Enemy* pRet = new Enemy();
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

bool Enemy::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	Sprite* sprite;

	int n = rand() % 10;
	if (n >= 2)
	{
		sprite = Sprite::create("PaperPlane_01.png");
		m_hitScore = 100;
		m_isSoonDie = 1;
	}
	else
	{
		sprite = Sprite::create("PaperPlane_02.png");
		m_hitScore = 200;
		m_isSoonDie = 0;
	}	
	
	this->addChild(sprite);

	m_hitNum = 0;

	return true;
}

void Enemy::setPositionRC(int row, int col)
{
	m_x = row;
	m_y = col;
	this->setPosition(Point(row, col));
}