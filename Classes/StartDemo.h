#pragma once
/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include<cocos2d.h>
#include"uiSharedLayer.h"
USING_NS_CC;

#include<map>



class StartDemo : public Scene
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(StartDemo);
private:
	std::map<int, Value> words;


};