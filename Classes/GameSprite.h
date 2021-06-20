/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#pragma once
#include"cocos2d.h"
USING_NS_CC;
using namespace std;

//这个游戏动作类用来作为一个抽象基类，用于定义某种特殊块的动作，其中消除一次是这些块共有的特征
//后面没时间写太多特殊块，所以这个类没什么用
class GameAction : public Ref  //继承ref便于引擎自行进行内存管理
{
protected:
	//使用图集创建动画的函数
	Animation* createAnimation(const string& plist, float delayPerUnit);
public:
	Action* clearOneTimeAction;

	virtual void createClearOneTimeAction() = 0;
};

