#pragma once
/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include"StandardMode.h"

struct Snow
{
	Sprite* sprite;
	Position location;
	int logicNumber;
};

//第二关继承第一关，并重新某些函数
class SpecialGame : public StandardGame
{
private:
	static SpecialGame* instance;

	//目标分数
	const int targetscore = 2000;

	//还需要消除的雪块数量
	int snowNum;

	vector<vector<Snow> > snowArray;//直接用精灵组向量

public:
	static SpecialGame* getInstance();
	SpecialGame();

	//需要重写的函数
	int getTargetNum();

	int getTargetScore();

	void build();

	bool init();

	void commonClear(const Position p);

	int checkEnded();
};