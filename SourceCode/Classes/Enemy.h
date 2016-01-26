#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"

class Enemy : public cocos2d::Sprite
{
public:
	int m_x;			//记录飞机横纵坐标
	int m_y;
	int m_hitNum;		//飞机被打中次数
	int m_hitScore;		//打中飞机加的分数
	bool m_isSoonDie;	//飞机是否将要死亡

	void setPositionRC(int row, int col);//设置位置

	static Enemy* create();
	virtual bool init();
};

#endif