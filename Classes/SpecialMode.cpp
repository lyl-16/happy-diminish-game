#include "SpecialMode.h"

//重写构造和指针初始化函数
SpecialGame* SpecialGame::instance = nullptr;
SpecialGame* SpecialGame::getInstance()
{
	if (!instance)
	{
		instance = new SpecialGame();
	}
	return instance;
}


SpecialGame::SpecialGame()
{
	animalAction = AnimalAction::create();
	blockBackground = Sprite::create("blockBackground.png");
	CC_SAFE_RETAIN(animalAction);
	CC_SAFE_RETAIN(blockBackground);
}


int SpecialGame::getTargetNum()
{
	return snowNum;
}

int SpecialGame::getTargetScore()
{
	return targetscore;
}

//构造块组元素向量的同时将雪块向量组也构建好
void SpecialGame::build()
{
	score = 0;
	snowNum = 0;
	for (int i = 0; i < size.col; i++)
	{
		vector<BlockElem> tempV1;
		vector<Snow>tempV2;
		for (int j = 0; j < size.row; j++)
		{
			BlockElem tempBE;
			tempBE.sprite = NULL;
			tempBE.location.col = i;
			tempBE.location.row = j;
			tempBE.logicNumber = NO_BLOCK;
			tempV1.push_back(tempBE);

			Snow tempS;
			tempS.sprite = NULL;
			tempS.location.col = i;
			tempS.location.row = j;
			tempS.logicNumber = NO_SNOW;
			tempV2.push_back(tempS);

			auto block = Sprite::createWithSpriteFrame(blockBackground->getSpriteFrame());
			block->setPosition(startPosition + Vec2(i * block->getContentSize().width, j * block->getContentSize().height));
			pL->addChild(block);
		}
		gameArray.push_back(tempV1);
		snowArray.push_back(tempV2);
	}
	return;
}

//色块精灵组以及雪块精灵组的初始化
bool SpecialGame::init()
{
	srand((unsigned int)(time(0)));

	//先初始化雪块再初始化精灵，保证正确的遮挡关系
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			if (j < 3)
			{
				snowNum++;
				snowArray[i][j].sprite = Sprite::create("snowBlock.png");
				snowArray[i][j].logicNumber = SNOW;
				snowArray[i][j].sprite->setPosition(startPosition + Vec2(i * blockBackground->getContentSize().width, j * blockBackground->getContentSize().height));
				pL->addChild(snowArray[i][j].sprite);
			}
		}
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			while (true)
			{
				gameArray[i][j].logicNumber = 10 * (rand() % 5 + 1);
				if ((int)fabs(countLinkedBlocks(&gameArray[i][j].location)) < 2)
				{
					break;
				}
			}
			gameArray[i][j].bIfMoved = false;
			gameArray[i][j].createSprite();
			gameArray[i][j].sprite->setPosition(startPosition + Vec2(i * blockBackground->getContentSize().width, j * blockBackground->getContentSize().height));
			pL->addChild(gameArray[i][j].sprite);
		}
	return true;
}

void SpecialGame::commonClear(const Position p)
{
	if (COMMON_BLOCK == gameArray[p.col][p.row].logicNumber % 10)
	{
		score += 10;
	}
	else if (UP_AND_DOWN_BLOCK == gameArray[p.col][p.row].logicNumber % 10 || LEFT_AND_RIGHT_BLOCK == gameArray[p.col][p.row].logicNumber % 10)
	{
		score += 50;
	}
	else if (BRIGHT_BLOCK == gameArray[p.col][p.row].logicNumber % 10)
	{
		score += 100;
	}
	else if (COLOURFUL_BLOCK == gameArray[p.col][p.row].logicNumber)
	{
		score += 200;
	}
	if (SNOW == snowArray[p.col][p.row].logicNumber)
	{
		//加入雪块的消除和统计
		snowNum--;
		snowArray[p.col][p.row].sprite->runAction(animalAction->clearOneTimeAction->clone());
		snowArray[p.col][p.row].sprite = nullptr;
		snowArray[p.col][p.row].logicNumber = NO_SNOW;
	}
	gameArray[p.col][p.row].sprite->runAction(animalAction->clearOneTimeAction->clone());
	gameArray[p.col][p.row].sprite = nullptr;
	gameArray[p.col][p.row].logicNumber = NO_BLOCK;
	gameArray[p.col][p.row].bIfMoved = false;
	return;
}

//新的获胜条件
int SpecialGame::checkEnded()
{
	if (moveTime > 0)
	{
		if (score >= targetscore && snowNum == 0)
		{
			return WIN;
		}
		else
		{
			return CONTINUE;
		}
	}
	else
	{
		if (score >= targetscore && snowNum == 0)
		{
			return WIN;
		}
		else
		{
			return LOSE;
		}
	}
}
