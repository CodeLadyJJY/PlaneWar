#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Enemy.h"

class GameScene : public cocos2d::Layer
{
private:
	int m_x;									//记录触摸歧视位置
	int m_y;
	int m_score;								//记录分数
	cocos2d::Vector<Enemy*> allEnemy;			//记录所有敌机
	cocos2d::Vector<cocos2d::Sprite*> allBullet;//记录所有子弹
	int dir;
	cocos2d::Sprite* galaxy1;	//星空背景
	cocos2d::Sprite* galaxy2;

	void scrollBG(float);
	void newBody(float t);
	void moveBody(float t);
	void newPlane();
	void newBullet();
	void newEnemy();
	void movePlane();
	void moveBullet();
	void moveEnemy();
	void newBomb(int x, int y, int type);		//爆炸动画
	void cleanBomb(Node* pRef);
	void update(float t);
	void gameOver();

public:
	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
};

#endif