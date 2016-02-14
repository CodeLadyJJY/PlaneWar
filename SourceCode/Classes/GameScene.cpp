#include "GameScene.h"
#include "Plane.h"
#include "Enemy.h"
#include "GameDefine.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

cocos2d::Scene * GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if(!Layer::init())
		return false;

	//背景
	galaxy1 = Sprite::create("galaxy.png");
	galaxy1->setAnchorPoint(Point::ZERO);
	galaxy1->setPosition(Point::ZERO);
	addChild(galaxy1);

	galaxy2 = Sprite::create("galaxy.png");
	galaxy2->setAnchorPoint(Point::ZERO);
	galaxy2->setPosition(Point::ZERO);
	addChild(galaxy2);

	//分数
	m_score = 0;
	auto scoreNum = Label::createWithSystemFont("Score: 0", "Consolas", 30);
	scoreNum->setTag(110);
	scoreNum->setPosition(Point(winSize_width/5, winSize_height*15/16));
	addChild(scoreNum, 1);

	//生命
	for (int i = 0; i < 3; ++i)
	{
		auto heart = Sprite::create("heart.png");
		heart->setPosition(Point(winSize_width * 3 / 5 + i * 32, winSize_height * 15 / 16));
		heart->setTag(200 + i);
		addChild(heart, 1);
	}

	//飞机
	dir = 0;	//默认处于停止状态
	newPlane();

	//触摸侦听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch * t, Event * e){
		m_x = t->getLocation().x;
		m_y = t->getLocation().y;
		return true;
	};

	listener->onTouchMoved = [&](Touch * t, Event * e){
		int px = t->getLocation().x - m_x;
		int py = t->getLocation().y - m_y;

		auto p = this->getChildByTag(120);
		p->runAction(MoveBy::create(0, Point(px, py)));

		m_x = t->getLocation().x;
		m_y = t->getLocation().y;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//键盘事件侦听
	auto listener2 = EventListenerKeyboard::create();

	listener2->onKeyPressed = [&](EventKeyboard::KeyCode k, Event * e) {//判断移动方向并改变方向

		switch (k)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			dir = 1;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			dir = 2;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			dir = 3;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			dir = 4;
			break;
		default:
			dir = 0;
			break;
		}
	};

	listener2->onKeyReleased = [&](EventKeyboard::KeyCode k, Event * e) {
		dir = 0;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);

	//移动背景
	this->schedule(schedule_selector(GameScene::scrollBG), 0.01f);

	//增加子弹、敌机
	this->schedule(schedule_selector(GameScene::newBody), 1);

	//移动飞机、子弹、敌机
	this->schedule(schedule_selector(GameScene::moveBody), 0.02);

	//碰撞检测和游戏逻辑
	this->scheduleUpdate();

	//播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("action_world1.mp3", true);

	return true;
}

void GameScene::scrollBG(float t)
{
	//两个背景循环移动
	galaxy1->setPositionY(galaxy1->getPositionY() - 1.0f);
	galaxy2->setPositionY(galaxy1->getPositionY() + galaxy1->getContentSize().height - 2.0f);
	if (galaxy2->getPositionY() <= 0)
		galaxy1->setPosition(Point::ZERO);
}

void GameScene::newBody(float t)
{
	newBullet();
	newEnemy();
}

void GameScene::moveBody(float t)
{
	movePlane();
	moveBullet();
	moveEnemy();
}

void GameScene::newPlane()
{
	auto plane = PlaneSprite::create();
	plane->setTag(120);
	plane->setPosition(Point(winSize_width / 2, winSize_height / 4));
	addChild(plane);
}

void GameScene::newBullet()
{
	PlaneSprite* plane = (PlaneSprite*)this->getChildByTag(120);
	auto bullet = Sprite::create("bullet_03.png");
	bullet->setPosition(Point(plane->getPosition().x, plane->getPosition().y + plane->getContentSize().height));
	this->addChild(bullet);
	allBullet.pushBack(bullet);
}

void GameScene::newEnemy()
{
	auto enemy = Enemy::create();
	enemy->setPositionRC(rand() % (winSize_width - 30), winSize_height);
	this->addChild(enemy);
	allEnemy.pushBack(enemy);
}

void GameScene::movePlane()
{
	auto p = this->getChildByTag(120);
	
	switch (dir)
	{
	case 1:
		p->runAction(MoveBy::create(0, Point(-5, 0)));
		break;
	case 2:
		p->runAction(MoveBy::create(0, Point(5, 0)));
		break;
	case 3:
		p->runAction(MoveBy::create(0, Point(0, 5)));
		break;
	case 4:
		p->runAction(MoveBy::create(0, Point(0, -5)));
		break;
	}
}

void GameScene::moveBullet()
{
	for (int i = 0; i<allBullet.size(); ++i)
	{
		auto nowBullet = allBullet.at(i);
		nowBullet->setPositionY(nowBullet->getPositionY() + 5);
		if (nowBullet->getPositionY() > winSize_height)
		{
			nowBullet->removeFromParent();
			allBullet.eraseObject(nowBullet);
			i--;
		}
	}
}

void GameScene::moveEnemy()
{
	for (int i = 0; i < allEnemy.size(); ++i)
	{
		auto enemy = allEnemy.at(i);
		enemy->setPositionRC(enemy->m_x, enemy->m_y - 3);
		if (enemy->m_y < 0)
		{
			enemy->removeFromParent();
			allEnemy.eraseObject(enemy);
			i--;
		}
	}
}

//爆炸动画
void GameScene::newBomb(int x, int y, int type)
{
	std::string typestr;

	if (type == 1)
		typestr = "bomb_01.png";
	else
		typestr = "bomb_02.png";

	Vector<SpriteFrame*> allBomb;
	for (int i = 0; i < 5; ++i)
	{
		auto bomb = SpriteFrame::create(typestr, Rect(i * 102, 0, 102, 128));
		allBomb.pushBack(bomb);
	}

	auto ani = Animation::createWithSpriteFrames(allBomb, 0.25);
	auto sprite = Sprite::create();
	auto act = Sequence::create(
		Animate::create(ani),
		CCCallFuncN::create(sprite, callfuncN_selector(GameScene::cleanBomb)),
		nullptr);
	sprite->setPosition(Point(x, y));
	sprite->runAction(act);
	this->addChild(sprite);
}

void GameScene::cleanBomb(Node* pRef)
{
	pRef->removeFromParentAndCleanup(true);
}

void GameScene::update(float t)
{
	PlaneSprite* plane = (PlaneSprite*)this->getChildByTag(120);

	for (int i = 0; i < allEnemy.size(); ++i)
	{
		auto enemy = allEnemy.at(i);
		Rect er(enemy->m_x, enemy->m_y, 64, 64);

		//检测敌机与子弹碰撞
		for (int j = 0; j < allBullet.size(); ++j)
		{
			auto nowBullet = allBullet.at(j);
			Rect br(nowBullet->getPositionX(), nowBullet->getPositionY(), 64, 64);
			if (er.intersectsRect(br))
			{
				if (enemy->m_isSoonDie)
				{
					//加分
					Label* labScore = (Label*)this->getChildByTag(110);
					m_score += enemy->m_hitScore;
					labScore->setString(StringUtils::format("score: %d", m_score));
					//爆炸效果
					newBomb(nowBullet->getPositionX(), nowBullet->getPositionY(), 1);
					//粒子特效
					auto ps = ParticleSystemQuad::create("bomb.plist");
					ps->setPosition(Point(nowBullet->getPositionX(), nowBullet->getPositionY()));
					this->addChild(ps);
					//播放音效
					SimpleAudioEngine::getInstance()->playEffect("explosion3.mp3");
					//子弹消失
					nowBullet->removeFromParent();
					allBullet.eraseObject(nowBullet);
					//敌机消失
					enemy->removeFromParent();
					allEnemy.eraseObject(enemy);
					i--;
					j--;
					break;
				}
				else
				{
					enemy->m_hitNum++;
					//播放音效
					SimpleAudioEngine::getInstance()->playEffect("explosion3.mp3");
					//子弹消失
					nowBullet->removeFromParent();
					allBullet.eraseObject(nowBullet);

					if (enemy->m_hitScore == 200)
					{
						if (enemy->m_hitNum == 1)
							enemy->m_isSoonDie = 1;
					}
					break;
				}
			}
		}

		//检测敌机与飞机碰撞
		Rect pr(plane->getPositionX(), plane->getPositionY(), plane->getContentSize().width, plane->getContentSize().height);
		if (er.intersectsRect(pr))
		{
			auto heart = this->getChildByTag(200 + plane->m_hitNum);
			heart->removeFromParent();
			plane->m_hitNum++;

			//敌机消失
			enemy->removeFromParent();
			allEnemy.eraseObject(enemy);
			//爆炸效果
			newBomb(plane->getPositionX(), plane->getPositionY(), 2);
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect("explosion3.mp3");

			//飞机被击中次数超过3次
			if (plane->m_hitNum >= 3)
			{
				gameOver();
			}
		}
	}
}

void GameScene::gameOver()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	this->unscheduleAllSelectors();
	this->unscheduleUpdate();

	auto scene = GameOver::createScene();
	Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5, scene));
}