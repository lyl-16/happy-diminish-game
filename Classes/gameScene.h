#pragma once
/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include"cocos2d.h"
#include"StandardMode.h"


USING_NS_CC;

class gameScene : public Scene
{
public:
	Scene* createScene();

	virtual bool init();

	//更新分数显示
	void scoreUpdate(int score);

	//更新剩余次数显示
	void timesUpdate(int moveTimes);

	//更新距离胜利目标次数显示
	void targetUpdate(int leftNum);

	CREATE_FUNC(gameScene);

private:
	Layer* gameLayer = Layer::create();

	Vec2 clickBegan;

	Vec2 clickEnded;

	Label* scoreLabel;

	Label* timeLabel;

	Label* targetSpriteLabel;

};
