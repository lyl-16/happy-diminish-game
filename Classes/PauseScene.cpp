/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include "PauseScene.h"
#include "MenuDemo.h"

Scene* PauseScene::createScene(int sceneType)
{
	//创建用于返回的ui场景，将背景设为灰色表示暂停或游戏的中断
	Scene* myScene = Scene::create();
	auto background = Sprite::create("GameBackground.png");
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setPosition(Vec2::ZERO);
	background->setColor(Color3B::GRAY);
	myScene->addChild(background);

	//创建返回游戏的按钮
	auto button = Button::create("BeginBottonNormal.png", "BeginBottonSelected.png");
	button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	button->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, 120));
	myScene->addChild(button);

	//根据不同的情况设置不同的表示
	//此场景用于游戏因暂停中断或因胜利或失败中断时的显示
	string str1, str2;

	if (CONTINUE == sceneType)
	{
		str1 = "continue";
		str2 = "STOP";
	}
	else
	{
		str1 = "return";
		if (WIN == sceneType)
		{
			str2 = "YOU WIN";
		}
		else
		{
			str2 = "YOU LOSE";
		}
	}

	//设置按钮的作用提示标签
	auto label = Label::createWithSystemFont(str1, "华文琥珀", 32);
	label->setPosition(button->getPosition());
	myScene->addChild(label);

	//设置信息提示标签
	auto infoLabel = Label::createWithSystemFont(str2, "Arial Black", 50);
	infoLabel->setPosition(button->getPosition() + Vec2(0, 400));
	myScene->addChild(infoLabel);

	button->addTouchEventListener([myScene, sceneType](Ref* sender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			if (CONTINUE == sceneType)
			{
				//返回游戏
				Director::getInstance()->popScene();
			}
			else
			{
				//返回菜单
				float vol = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
				SimpleAudioEngine::getInstance()->stopBackgroundMusic();
				SimpleAudioEngine::getInstance()->playBackgroundMusic("music.mp3", true);
				SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(vol);
				auto meunScene = MenuDemo::create();
				Director::getInstance()->replaceScene(meunScene);
			}
		}
		});

	return myScene;
}


