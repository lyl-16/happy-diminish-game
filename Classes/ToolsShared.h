#pragma once
/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
/*
本头文件用于存放不同cpp种需要使用到的宏定义和结构体
*/
#include"cocos2d.h"
#include"uiSharedLayer.h"

//定义上下左右的逻辑值
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

//定义普通宠物精灵的逻辑值
#define NO_BLOCK 0
#define BEAR_COMMON_BLOCK 10
#define CHICK_COMMON_BLOCK 20
#define FOX_COMMON_BLOCK 30
#define FROG_COMMON_BLOCK 40
#define HORSE_COMMON_BLOCK 50

#define BEAR_UP_AND_DOWN_BLOCK 11
#define BEAR_LEFT_AND_RIGHT_BLOCK 12
#define BEAR_BRIGHT_BLOCK 13

#define CHICK_UP_AND_DOWN_BLOCK 21
#define CHICK_LEFT_AND_RIGHT_BLOCK 22
#define CHICK_BRIGHT_BLOCK 23

#define FOX_UP_AND_DOWN_BLOCK 31
#define FOX_LEFT_AND_RIGHT_BLOCK 32
#define FOX_BRIGHT_BLOCK 33

#define FROG_UP_AND_DOWN_BLOCK 41
#define FROG_LEFT_AND_RIGHT_BLOCK 42
#define FROG_BRIGHT_BLOCK 43

#define HORSE_UP_AND_DOWN_BLOCK 51
#define HORSE_LEFT_AND_RIGHT_BLOCK 52
#define HORSE_BRIGHT_BLOCK 53

//定义特殊块的区别值
#define COMMON_BLOCK 0
#define UP_AND_DOWN_BLOCK 1
#define LEFT_AND_RIGHT_BLOCK 2
#define BRIGHT_BLOCK 3
#define COLOURFUL_BLOCK 8

//定义输赢和继续游戏的逻辑值
#define CONTINUE 0
#define WIN 1
#define LOSE 2

//定义有无雪块的逻辑值
#define NO_SNOW 0
#define SNOW 1

USING_NS_CC;
using namespace std;


struct Position
{
	int row;
	int col;
};

struct BlockElem
{
	Sprite* sprite;
	Position location;
	int logicNumber;
	bool bIfMoved;//用来记录是否发生了移动
	
	void createSprite()
	{
		if (BEAR_COMMON_BLOCK == logicNumber)
		{
			sprite = Sprite::create("sprite/bear/bear.png");
		}
		else if (CHICK_COMMON_BLOCK == logicNumber)
		{
			sprite = Sprite::create("sprite/chick/chick.png");
		}
		else if (FOX_COMMON_BLOCK == logicNumber)
		{
			sprite = Sprite::create("sprite/fox/fox.png");
		}
		else if (FROG_COMMON_BLOCK == logicNumber)
		{
			sprite = Sprite::create("sprite/frog/frog.png");
		}
		else if (HORSE_COMMON_BLOCK == logicNumber)
		{
			sprite = Sprite::create("sprite/horse/horse.png");
		}
		return;
	}
};