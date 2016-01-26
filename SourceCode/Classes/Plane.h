#ifndef _PLANE_H_
#define _PLANE_H_

#include "cocos2d.h"

class PlaneSprite : public cocos2d::Sprite
{
public:
	int m_hitNum;//飞机被打中次数

	static PlaneSprite* create();
	virtual bool init();
};

#endif