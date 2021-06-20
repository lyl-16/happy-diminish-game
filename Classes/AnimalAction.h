/* 
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#pragma once
#include"GameSprite.h"
#include "ToolsShared.h"
using namespace std;

class AnimalAction : public GameAction  //继承动作的集合类GameAction
{
private:
	//方框大小(边长)
	float blockSize;

	//用于初始化动作的函数
	void createClearOneTimeAction();
	Animate* createAnimateUpAndDown(string name);
	Animate* createAnimateLeftAndRight(string name);
	Animate* createAnimateBright(string name);
	void createActionMoveLeft();
	void createActionMoveRight();
	void createActionMoveUp();
	void createActionMoveDown();


public:

	CREATE_FUNC(AnimalAction);

	virtual bool init();

	//特效动物的动作
	Animate* bearAnimateUAD;
	Animate* chickAnimateUAD;
	Animate* foxAnimateUAD;
	Animate* frogAnimateUAD;
	Animate* horseAnimateUAD;

	Animate* bearAnimateLAR;
	Animate* chickAnimateLAR;
	Animate* foxAnimateLAR;
	Animate* frogAnimateLAR;
	Animate* horseAnimateLAR;

	Animate* bearAnimateBright;
	Animate* chickAnimateBright;
	Animate* foxAnimateBright;
	Animate* frogAnimateBright;
	Animate* horseAnimateBright;

	//移动的动作
	Action* actionMoveLeft;
	Action* actionMoveRight;
	Action* actionMoveUp;
	Action* actionMoveDown;

	//通过逻辑值得到动作的方法
	Animate* getAnimateUpAndDownByNum(int logicNum);
	Animate* getAnimateLeftAndRightByNum(int logicNum);
	Animate* getAnimateBrightByNum(int logicNum);

	void setBlockSize(float sideLength);

};