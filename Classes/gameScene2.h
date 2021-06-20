#pragma once
#include"cocos2d.h"
#include"SpecialMode.h"


USING_NS_CC;

class gameScene2 : public Scene
{
public:
	Scene* createScene();

	virtual bool init();

	void scoreUpdate(int score);

	void timesUpdate(int moveTimes);

	void targetUpdate(int leftNum);

	CREATE_FUNC(gameScene2);

private:
	Layer* gameLayer = Layer::create();

	Vec2 clickBegan;

	Vec2 clickEnded;

	Label* scoreLabel;

	Label* timeLabel;

	Label* targetSpriteLabel;

};
