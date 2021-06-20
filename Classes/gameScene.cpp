/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include "gameScene.h"
#include "PauseScene.h"

Scene* gameScene::createScene()
{
	return gameScene::create();
}

bool gameScene::init()
{
	if(!Scene::create())
	return false;

	//创建背景层
	auto backgroundLayer = Layer::create();
	this->addChild(backgroundLayer);

	//创建背景图片
	auto bg = Sprite::create("GameBackground.png");
	bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	backgroundLayer->addChild(bg);

	//创建记录剩余次数板的精灵
	auto timesBoard = Sprite::create("timesBroad.png");
	timesBoard->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	timesBoard->setPosition(backgroundLayer->getContentSize());
	backgroundLayer->addChild(timesBoard);

	//创建得分版精灵
	auto scoreBoard = Sprite::create("ScoreBoard.png");
	scoreBoard->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	scoreBoard->setPosition(Vec2(0, 800));
	backgroundLayer->addChild(scoreBoard);

	//创建胜利目标版精灵
	auto targetScoreBoard = Sprite::create("targetBoard.png");
	targetScoreBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	targetScoreBoard->setPosition(Vec2(200, backgroundLayer->getContentSize().height));
	backgroundLayer->addChild(targetScoreBoard);

	
	auto targetSpriteBoard = Sprite::create("targetBoard.png");
	targetSpriteBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	targetSpriteBoard->setPosition(Vec2(350, backgroundLayer->getContentSize().height));
	backgroundLayer->addChild(targetSpriteBoard);

	//将游戏层添加到场景中，并初始化游戏类实例
	this->addChild(gameLayer);
	StandardGame::getInstance()->set(6, 6, 20, gameLayer, Vec2(150, 250));
	StandardGame::getInstance()->gameBegin();

	//通过标签控件来显示数据

	//分数标签
	scoreLabel = Label::createWithSystemFont("Score: 0", "微软雅黑", 20);
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(Vec2(60, 770));
	backgroundLayer->addChild(scoreLabel);

	//剩余次数标签
	timeLabel = Label::createWithSystemFont("20", "华文彩云", 35);
	timeLabel->setColor(Color3B::ORANGE);
	timeLabel->setPosition(Vec2(timesBoard->getPosition().x - timesBoard->getContentSize().width / 2 + 20, timesBoard->getPosition().y - timesBoard->getContentSize().height / 2 - 10));
	backgroundLayer->addChild(timeLabel);

	//提示信息标签
	auto infoLabel = Label::createWithSystemFont("Target Score", "微软雅黑", 15);
	infoLabel->setColor(Color3B::WHITE);
	infoLabel->setPosition(Vec2(200, backgroundLayer->getContentSize().height - 70));
	backgroundLayer->addChild(infoLabel);

	//提示目标分值标签
	auto targetScoreLabel = Label::createWithSystemFont(to_string(StandardGame::getInstance()->getTargetScore()), "微软雅黑", 30);
	targetScoreLabel->setColor(Color3B::WHITE);
	targetScoreLabel->setPosition(Vec2(200, backgroundLayer->getContentSize().height - 100));
	backgroundLayer->addChild(targetScoreLabel);

	//提示需要消除的精灵标签
	auto targetSprite = Sprite::create("sprite/bear/bear.png");
	targetSprite->setPosition(Vec2(350, backgroundLayer->getContentSize().height - 80));
	backgroundLayer->addChild(targetSprite);

	targetSpriteLabel = Label::createWithSystemFont("10", "微软雅黑", 20);
	targetSpriteLabel->setColor(Color3B::WHITE);
	targetSpriteLabel->setPosition(Vec2(350, backgroundLayer->getContentSize().height - 110));
	backgroundLayer->addChild(targetSpriteLabel);

	//添加鼠标监听事件
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [this](Touch* touch, Event* event) {
		clickBegan = touch->getLocation();
		return true;
	};
	listener->onTouchEnded = [this](Touch* touch, Event* event) {
		clickEnded = touch->getLocation();
		StandardGame::getInstance()->playGame(clickBegan, clickEnded);
		scoreUpdate(StandardGame::getInstance()->getScore());
		timesUpdate(StandardGame::getInstance()->getMoveTime());
		targetUpdate(StandardGame::getInstance()->getTargetNum());

		int gameCondition = StandardGame::getInstance()->checkEnded();
		if (gameCondition)
		{

			Director::getInstance()->pushScene(PauseScene::createScene(gameCondition));
		}

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//添加公用ui层
	uiSharedLayer::getInstance()->createLayer();
	this->addChild(uiSharedLayer::getInstance());

	//设置暂停按钮
	auto stopButton = Button::create("stopBotton.png", "stopBotton.png");
	stopButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	stopButton->setPosition(Vec2(30, 810));
	this->addChild(stopButton);

	stopButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{

			Director::getInstance()->pushScene(PauseScene::createScene(CONTINUE));
		}
		});

	return true;
}

void gameScene::scoreUpdate(int score)
{
	//使用to_string函数来转换显示数值
	scoreLabel->setString("Score: " + to_string(score));
	return;
}

void gameScene::timesUpdate(int moveTimes)
{
	timeLabel->setString(to_string(moveTimes));
	return;
}

void gameScene::targetUpdate(int leftNum)
{
	targetSpriteLabel->setString(to_string(leftNum));
	return;
}
