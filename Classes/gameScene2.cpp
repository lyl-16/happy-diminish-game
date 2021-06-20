/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include "gameScene2.h"
#include "PauseScene.h"

Scene* gameScene2::createScene()
{
	return gameScene2::create();
}

bool gameScene2::init()
{

	//逻辑与第一关场景完全相同，参考第一关场景注释
	if (!Scene::create())
		return false;


	auto backgroundLayer = Layer::create();
	this->addChild(backgroundLayer);

	auto bg = Sprite::create("GameBackground.png");
	bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	backgroundLayer->addChild(bg);

	auto timesBoard = Sprite::create("timesBroad.png");
	timesBoard->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	timesBoard->setPosition(backgroundLayer->getContentSize());
	backgroundLayer->addChild(timesBoard);

	auto scoreBoard = Sprite::create("ScoreBoard.png");
	scoreBoard->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	scoreBoard->setPosition(Vec2(0, 800));
	backgroundLayer->addChild(scoreBoard);

	auto targetScoreBoard = Sprite::create("targetBoard.png");
	targetScoreBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	targetScoreBoard->setPosition(Vec2(200, backgroundLayer->getContentSize().height));
	backgroundLayer->addChild(targetScoreBoard);

	auto targetSpriteBoard = Sprite::create("targetBoard.png");
	targetSpriteBoard->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	targetSpriteBoard->setPosition(Vec2(350, backgroundLayer->getContentSize().height));
	backgroundLayer->addChild(targetSpriteBoard);


	this->addChild(gameLayer);
	SpecialGame::getInstance()->set(6, 6, 30, gameLayer, Vec2(150, 250));
	SpecialGame::getInstance()->gameBegin();

	scoreLabel = Label::createWithSystemFont("Score: 0", "微软雅黑", 20);
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(Vec2(60, 770));
	backgroundLayer->addChild(scoreLabel);

	timeLabel = Label::createWithSystemFont("30", "华文彩云", 35);
	timeLabel->setColor(Color3B::ORANGE);
	timeLabel->setPosition(Vec2(timesBoard->getPosition().x - timesBoard->getContentSize().width / 2 + 20, timesBoard->getPosition().y - timesBoard->getContentSize().height / 2 - 10));
	backgroundLayer->addChild(timeLabel);

	auto infoLabel = Label::createWithSystemFont("Target Score", "微软雅黑", 15);
	infoLabel->setColor(Color3B::WHITE);
	infoLabel->setPosition(Vec2(200, backgroundLayer->getContentSize().height - 70));
	backgroundLayer->addChild(infoLabel);

	auto targetScoreLabel = Label::createWithSystemFont(to_string(SpecialGame::getInstance()->getTargetScore()), "微软雅黑", 30);
	targetScoreLabel->setColor(Color3B::WHITE);
	targetScoreLabel->setPosition(Vec2(200, backgroundLayer->getContentSize().height - 100));
	backgroundLayer->addChild(targetScoreLabel);

	auto targetSprite = Sprite::create("snowBlock.png");
	targetSprite->setPosition(Vec2(350, backgroundLayer->getContentSize().height - 80));
	backgroundLayer->addChild(targetSprite);

	targetSpriteLabel = Label::createWithSystemFont("18", "微软雅黑", 20);
	targetSpriteLabel->setColor(Color3B::WHITE);
	targetSpriteLabel->setPosition(Vec2(350, backgroundLayer->getContentSize().height - 110));
	backgroundLayer->addChild(targetSpriteLabel);


	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [this](Touch* touch, Event* event) {
		clickBegan = touch->getLocation();
		return true;
	};
	listener->onTouchEnded = [this](Touch* touch, Event* event) {
		clickEnded = touch->getLocation();
		SpecialGame::getInstance()->playGame(clickBegan, clickEnded);
		scoreUpdate(SpecialGame::getInstance()->getScore());
		timesUpdate(SpecialGame::getInstance()->getMoveTime());
		targetUpdate(SpecialGame::getInstance()->getTargetNum());

		int gameCondition = SpecialGame::getInstance()->checkEnded();
		if (gameCondition)
		{

			Director::getInstance()->pushScene(PauseScene::createScene(gameCondition));
		}

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	uiSharedLayer::getInstance()->createLayer();
	this->addChild(uiSharedLayer::getInstance());

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

void gameScene2::scoreUpdate(int score)
{
	scoreLabel->setString("Score: " + to_string(score));
	return;
}

void gameScene2::timesUpdate(int moveTimes)
{
	timeLabel->setString(to_string(moveTimes));
	return;
}

void gameScene2::targetUpdate(int leftNum)
{
	targetSpriteLabel->setString(to_string(leftNum));
	return;
}
