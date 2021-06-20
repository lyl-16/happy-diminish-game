/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include "MenuDemo.h"
#include"gameScene.h"
#include "gameScene2.h"



void MenuDemo::enterGameOneCallback(Ref* pSender)
{
	//播放关卡音乐
	float vol = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("gameMusic.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(vol);
	//替换成相应关卡场景
	auto gameScene = gameScene::create();
	Director::getInstance()->replaceScene(gameScene);
}

void MenuDemo::enterGameTwoCallback(Ref* pSender)
{
	float vol = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("gameMusic.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(vol);
	auto gameScene = gameScene2::create();
	Director::getInstance()->replaceScene(gameScene);
}

bool MenuDemo::init()
{
	if(!Scene::init())
	return false;

	//添加天空背景层
	auto skygroundLayer = Layer::create();
	skygroundLayer->setColor(Color3B::WHITE);
	this->addChild(skygroundLayer);

	//添加天空背景图片到层中
	auto sprite1 = Sprite::create("skyBackground.png");
	sprite1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	sprite1->setPosition(Vec2::ZERO);
	skygroundLayer->addChild(sprite1);
	
	//设置背景层
	backgroundLayer = Layer::create();
	backgroundLayer->setColor(Color3B::WHITE);
	this->addChild(backgroundLayer);

	//添加树干图
	sprite = Sprite::create("MenuBackground.png");
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	sprite->setPosition(Vec2::ZERO);
	backgroundLayer->addChild(sprite);

	//创建两个关卡的按钮通道
	auto entryItem1 = MenuItemImage::create(
		"entryNormal.png",
		"entry.png",
		CC_CALLBACK_1(MenuDemo::enterGameOneCallback, this));

	auto entryItem2 = MenuItemImage::create(
		"entryNormal.png",
		"entry.png",
		CC_CALLBACK_1(MenuDemo::enterGameTwoCallback, this));

	entryItem1->setPosition(Vec2(230, 415));
	entryItem2->setPosition(Vec2(360, 470));

	//用两个按钮创建菜单
	auto menu = Menu::create(entryItem1, entryItem2, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	uiSharedLayer::getInstance()->createLayer();
	this->addChild(uiSharedLayer::getInstance());
	
	return true;
}


