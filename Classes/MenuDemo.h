#pragma once
/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include"cocos2d.h"
#include"uiSharedLayer.h"

USING_NS_CC;


class MenuDemo : public Scene
{
public:
	CREATE_FUNC(MenuDemo);

	//进入第一关按钮的回调函数
	void enterGameOneCallback(Ref* pSender);

	//进入第二关按钮的回调函数
	void enterGameTwoCallback(Ref* pSender);

	virtual bool init();

private:
	Vec2 position;

	Sprite* sprite;

	Layer* backgroundLayer;

	Vec2 visibleSize = Director::getInstance()->getVisibleSize();

};