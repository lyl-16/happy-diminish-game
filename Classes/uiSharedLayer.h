#pragma once
/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
/*  
创建一个层单例用来专门存放一些每个场景都要出现的控件
*/
#include"cocos2d.h"
#include "..\cocos2d\cocos\editor-support\cocostudio\SimpleAudioEngine.h"
#include<ui\CocosGUI.h>

USING_NS_CC;
using namespace CocosDenshion;
using namespace ui;

class uiSharedLayer : public Layer
{
public:
	static void createLayer();

	virtual bool init();

	//音量滑动条回调函数
	void sliderCallback(Ref* pSender, Slider::EventType type);

	//获取层单例
	static uiSharedLayer* getInstance();

	static void destroyInstance();

	//关闭按钮的回调函数
	void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(uiSharedLayer);

private:

	static uiSharedLayer* instance;

	Slider* musicSlider;
};