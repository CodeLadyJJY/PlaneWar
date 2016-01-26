#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Enemy.h"

class GameScene : public cocos2d::Layer
{
private:
	int m_x;									//��¼��������λ��
	int m_y;
	int m_score;								//��¼����
	cocos2d::Vector<Enemy*> allEnemy;			//��¼���ел�
	cocos2d::Vector<cocos2d::Sprite*> allBullet;//��¼�����ӵ�
	int dir;
	cocos2d::Sprite* galaxy1;	//�ǿձ���
	cocos2d::Sprite* galaxy2;

	void scrollBG(float);
	void newPlane();
	void movePlane(float t);
	void newBullet(float t);
	void moveBullet(float t);
	void newEnemy(float t);
	void moveEnemy(float t);
	void newBomb(int x, int y, int type);		//��ը����
	void cleanBomb(Node* pRef);
	void update(float t);

public:
	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
};

#endif