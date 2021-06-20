/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include "StartDemo.h"
#include "MenuDemo.h"

Scene* StartDemo::createScene()
{
	return StartDemo::create();
}

bool StartDemo::init()
{
	if (!Scene::init())
	{
		return false;
	}

	//播放背景音乐
	auto audioEngine = SimpleAudioEngine::getInstance();
	audioEngine->playBackgroundMusic("music.mp3", true);
	
	//创建背景图片精灵
	auto sprite = Sprite::create("StartBackGround.png");
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprite->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2));
	this->addChild(sprite);

	//读取中文配置
	auto configData = FileUtils::getInstance()->getValueMapFromFile("XMLFile.xml");
	for (auto valPair : configData)
	{
		int key = atoi(valPair.first.c_str());
		Value val= valPair.second;

		words.insert(std::pair<int, Value>(key, val));
	}

	//创建进入按钮
	auto button = Button::create("BeginBottonNormal.png", "BeginBottonSelected.png");
	button->setTitleFontSize(32);
	std::string str = words.at(1001).asString();
	button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	button->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 120));
	this->addChild(button);

	//显示进入的提示标签
	auto label = Label::createWithSystemFont(str,"华文琥珀",32);
	label->setPosition(button->getPosition());
	this->addChild(label);

	button->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			auto menuScene = MenuDemo::create();
			Director::getInstance()->replaceScene(menuScene);
		}
		});
	
	//添加公有ui层
	uiSharedLayer::getInstance()->createLayer();
	this->addChild(uiSharedLayer::getInstance());

	return true;
}

