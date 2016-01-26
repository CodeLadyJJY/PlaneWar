#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"

class Enemy : public cocos2d::Sprite
{
public:
	int m_x;			//��¼�ɻ���������
	int m_y;
	int m_hitNum;		//�ɻ������д���
	int m_hitScore;		//���зɻ��ӵķ���
	bool m_isSoonDie;	//�ɻ��Ƿ�Ҫ����

	void setPositionRC(int row, int col);//����λ��

	static Enemy* create();
	virtual bool init();
};

#endif