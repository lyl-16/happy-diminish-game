/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#include "StandardMode.h"
#include "cmath"



StandardGame* StandardGame::instance = nullptr;
StandardGame* StandardGame::getInstance()
{
	if (!instance)
	{
		instance = new StandardGame();
	}
	return instance;
}

void StandardGame::destroyInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

StandardGame::StandardGame()
{
	animalAction = AnimalAction::create();
	blockBackground = Sprite::create("blockBackground.png");
	CC_SAFE_RETAIN(animalAction);
	CC_SAFE_RETAIN(blockBackground);
}

StandardGame::~StandardGame()
{
	CC_SAFE_RELEASE(animalAction);
	CC_SAFE_RELEASE(blockBackground);
}

void StandardGame::build()
{
	//构造向量组
	score = 0;
	targetBearNum = 10;
	for (int i = 0; i < size.col; i++)
	{
		vector<BlockElem> tempV;
		for (int j = 0; j < size.row; j++)
		{
			BlockElem tempBE;
			tempBE.sprite = NULL;
			tempBE.location.col = i;
			tempBE.location.row = j;
			tempBE.logicNumber = NO_BLOCK;
			tempV.push_back(tempBE);

			auto block = Sprite::createWithSpriteFrame(blockBackground->getSpriteFrame());
			block->setPosition(startPosition + Vec2(i * block->getContentSize().width, j * block->getContentSize().height));
			pL->addChild(block);
		}
		gameArray.push_back(tempV);
	}
	return;
}

bool StandardGame::init()
{
	//初始化向量组
	srand((unsigned int)(time(0)));
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			while (true)
			{
				//随机生成物块种类
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

int StandardGame::getScore()
{
	return score;
}

int StandardGame::checkLinkedBlocksInOneDirection(const Position* p, const int logicNum, const int direction)
{
	//数一个方向有多少同类型的块相连
	int count = 0;
	if (UP == direction)
	{
		for (int i = p->row + 1; i < size.row; i++)
		{
			if (logicNum == gameArray[p->col][i].logicNumber / 10)
			{
				count++;
			}
			else
			{
				break;
			}
		}
	}
	else if (DOWN == direction)
	{
		for (int i = p->row - 1; i >= 0; i--)
		{
			if (logicNum == gameArray[p->col][i].logicNumber / 10)
			{
				count++;
			}
			else
			{
				break;
			}
		}
	}
	else if (RIGHT == direction)
	{
		for (int i = p->col + 1; i < size.col; i++)
		{
			if (logicNum == gameArray[i][p->row].logicNumber / 10)
			{
				count++;
			}
			else
			{
				break;
			}
		}
	}
	else if (LEFT == direction)
	{
		for (int i = p->col - 1; i >= 0; i--)
		{
			if (logicNum == gameArray[i][p->row].logicNumber / 10)
			{
				count++;
			}
			else
			{
				break;
			}
		}
	}
	return count;
}

int StandardGame::countLinkedBlocks(const Position* p)
{
	//遍历一个块四周看有多少相连
	int countLR = 0;
	int countUD = 0;
	int logicNum = gameArray[p->col][p->row].logicNumber / 10;
	countLR = checkLinkedBlocksInOneDirection(p, logicNum, LEFT) + checkLinkedBlocksInOneDirection(p, logicNum, RIGHT);
	countUD = checkLinkedBlocksInOneDirection(p, logicNum, UP) + checkLinkedBlocksInOneDirection(p, logicNum, DOWN);
	if (countLR < 2 && countUD < 2)
	{
		return (countLR > countUD) ? countLR : countUD;
	}
	else
	{
		if (countLR == 2 && countUD == 2)
		{
			return -5;  //表示L或T型消除
		}
		else
		{
			return (countLR > countUD) ? -countLR : countUD;  //左右返回赋值用于区分连块的位置
		}
	}
}

int StandardGame::getMoveTime()
{
	return moveTime;
}

Position StandardGame::getClickLocation(Vec2 clickBegan)
{
	//获取点击的数组逻辑位置
	Position p;
	p.col = -1;
	p.row = -1;
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			float leftBorder = gameArray[i][j].sprite->getPosition().x - gameArray[i][j].sprite->getContentSize().width / 2;
			float rightBorder = gameArray[i][j].sprite->getPosition().x + gameArray[i][j].sprite->getContentSize().width / 2;
			float downBorder = gameArray[i][j].sprite->getPosition().y - gameArray[i][j].sprite->getContentSize().height / 2;
			float upBorder = gameArray[i][j].sprite->getPosition().y + gameArray[i][j].sprite->getContentSize().height / 2;
			if (clickBegan.x > leftBorder && clickBegan.x<rightBorder && clickBegan.y>downBorder && clickBegan.y < upBorder)
			{
				p.col = i;
				p.row = j;
				break;
			}
		}
	return p;
}

int StandardGame::getMoveDirection(Vec2 clickBegan, Vec2 clickEnded)
{
	//通过点击和松开的坐标确定移动的方向
	float widthDistance = fabs(clickEnded.x - clickBegan.x);
	float heightDistance = fabs(clickEnded.y - clickBegan.y);
	if (widthDistance > heightDistance)
	{
		if (clickEnded.x - clickBegan.x > 0)
		{
			return RIGHT;
		}
		else if (clickEnded.x - clickBegan.x < 0)
		{
			return LEFT;
		}
	}
	else if (widthDistance < heightDistance)
	{
		if (clickEnded.y - clickBegan.y > 0)
		{
			return UP;
		}
		else if (clickEnded.y - clickBegan.y < 0)
		{
			return DOWN;
		}
	}
	return 0;  //表示无法判定移动方向
}

int StandardGame::getTargetNum()
{
	return targetBearNum;
}

void StandardGame::commonClear(const Position p)
{
	//先进行分数清算再进行消除
	if (BEAR_COMMON_BLOCK / 10 == gameArray[p.col][p.row].logicNumber / 10 && targetBearNum > 0)
	{
		targetBearNum--;
	}
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
	//消除一个精灵元素
	gameArray[p.col][p.row].sprite->runAction(animalAction->clearOneTimeAction->clone());
	gameArray[p.col][p.row].sprite = nullptr;
	gameArray[p.col][p.row].logicNumber = NO_BLOCK;
	gameArray[p.col][p.row].bIfMoved = false;
	return;
}

void StandardGame::commonLinkBlocksClear(const Position p, int flag, bool ifClearItself)
{
	//消除相邻块
	Position p1;
	int animalNum = gameArray[p.col][p.row].logicNumber / 10;
	if (ifClearItself)
	{
		//看是否要消除自身
		oneBlockClear(p);
	}
	if (flag > 0 || flag == -5)
	{
		for (int i = p.row + 1; i < size.row; i++)
		{
			if (gameArray[p.col][i].logicNumber / 10 == animalNum)
			{
				p1.col = p.col;
				p1.row = i;
				oneBlockClear(p1);
			}
			else
			{
				break;
			}
		}
		for (int i = p.row - 1; i >= 0; i--)
		{
			if (gameArray[p.col][i].logicNumber / 10 == animalNum)
			{
				p1.col = p.col;
				p1.row = i;
				oneBlockClear(p1);
			}
			else
			{
				break;
			}
		}
	}
	if (flag < 0)
	{
		for (int i = p.col + 1; i < size.col; i++)
		{
			if (gameArray[i][p.row].logicNumber / 10 == animalNum)
			{
				p1.col = i;
				p1.row = p.row;
				oneBlockClear(p1);
			}
			else
			{
				break;
			}
		}
		for (int i = p.col - 1; i >= 0; i--)
		{
			if (gameArray[i][p.row].logicNumber / 10 == animalNum)
			{
				p1.col = i;
				p1.row = p.row;
				oneBlockClear(p1);
			}
			else
			{
				break;
			}
		}
	}
	return;
}

int StandardGame::getTargetScore()
{
	return targetScore;
}

void StandardGame::colClear(const Position p, bool ifClearItself)  //消除一整列
{
	Position p1;
	int animalNum = gameArray[p.col][p.row].logicNumber / 10;
	if (ifClearItself)
	{
		commonClear(p);
	}
	for (int i = 0; i < size.row; i++)
	{
		p1.col = p.col;
		p1.row = i;
		oneBlockClear(p1);
	}
	
	return;
}

void StandardGame::rowClear(const Position p, bool ifClearItself)//消除一整行
{
	Position p1;
	int animalNum = gameArray[p.col][p.row].logicNumber / 10;
	if (ifClearItself)
	{
		commonClear(p);
	}
	for (int i = 0; i < size.col; i++)
	{
		p1.col = i;
		p1.row = p.row;
		oneBlockClear(p1);
	}
	
	return;
}

void StandardGame::boomClear(const Position p, bool ifClearItself)  //爆炸消除效果
{
	Position p1;
	if (ifClearItself)
	{
		commonClear(p);
	}
	int point[12][2] = { (p.col - 2, p.row),(p.col - 1, p.row), (p.col - 1,p.row + 1),(p.col,p.row + 1)
						,(p.col,p.row + 2),(p.col + 1,p.row + 1),(p.col + 1,p.row),(p.col + 2,p.row)
						,(p.col + 1,p.row - 1),(p.col,p.row - 1),(p.col,p.row - 2),(p.col - 1,p.row - 1)
						};//爆炸范围的理论点
	for (int i = 0; i < 12; i++)
	{
		if (point[i][0] >= 0 && point[i][0] < size.col && point[i][1] >= 0 && point[i][1] < size.row)
		{
			p1.col = point[i][0];
			p1.row = point[i][1];
			oneBlockClear(p1);
		}
	}
	return;
}

void StandardGame::twoMagicClear(const Position p1, const Position p2)//魔力鸟双消
{
	commonClear(p1);
	commonClear(p2);
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			oneBlockClear(gameArray[i][j].location);
		}
	drop();
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			oneBlockClear(gameArray[i][j].location);
		}
}

//该函数已经确保不是双魔力鸟消除才调用
//魔力鸟单消
void StandardGame::magicClear(const Position p, int direction)
{
	int logicNum = 0;
	bool bClearAgain = false;
	if (!direction)
	{
		logicNum = 10 * (rand() % 5 + 1);
	}
	else
	{
		if (LEFT == direction)
		{
			logicNum = gameArray[p.col - 1][p.row].logicNumber;
		}
		else if (RIGHT == direction)
		{
			logicNum = gameArray[p.col + 1][p.row].logicNumber;
		}
		if (UP == direction)
		{
			logicNum = gameArray[p.col][p.row + 1].logicNumber;
		}
		if (DOWN == direction)
		{
			logicNum = gameArray[p.col][p.row - 1].logicNumber;
		}
	}
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			if (gameArray[i][j].logicNumber / 10 == logicNum / 10)
			{
				if (COMMON_BLOCK == logicNum % 10)
				{
					oneBlockClear(gameArray[i][j].location);
				}
				else if (UP_AND_DOWN_BLOCK == logicNum % 10 || LEFT_AND_RIGHT_BLOCK == logicNum % 10)
				{
					int randNum = rand() % 2 + 1;
					generateSpecialBlocks(gameArray[i][j].location, randNum);
					bClearAgain = true;
				}
				else if (BRIGHT_BLOCK == logicNum % 10)
				{
					generateSpecialBlocks(gameArray[i][j].location, BRIGHT_BLOCK);
					bClearAgain = true;
				}
			}
		}
	if (bClearAgain)
	{
		for (int i = 0; i < size.col; i++)
			for (int j = 0; j < size.row; j++)
			{
				if (gameArray[i][j].logicNumber / 10 == logicNum / 10)
				{
					oneBlockClear(gameArray[i][j].location);
				}
			}
	}

	return;
}

void StandardGame::oneBlockClear(const Position p, bool ifClearItself)  //根据逻辑值选择消除方式
{
	if (NO_BLOCK == gameArray[p.col][p.row].logicNumber)
	{
		return;
	}
	if (COMMON_BLOCK == gameArray[p.col][p.row].logicNumber % 10)
	{
		commonClear(p);
	}
	else if (UP_AND_DOWN_BLOCK == gameArray[p.col][p.row].logicNumber % 10)
	{
		colClear(p, ifClearItself);
	}
	else if (LEFT_AND_RIGHT_BLOCK == gameArray[p.col][p.row].logicNumber % 10)
	{
		rowClear(p, ifClearItself);
	}
	else if (BRIGHT_BLOCK == gameArray[p.col][p.row].logicNumber % 10)
	{
		boomClear(p, ifClearItself);
	}
	else if (COLOURFUL_BLOCK == gameArray[p.col][p.row].logicNumber)
	{
		magicClear(p);
	}
	return;
}

void StandardGame::clearBlocks()
{
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			int linkBlocks = countLinkedBlocks(&gameArray[i][j].location);
			if (true == gameArray[i][j].bIfMoved)  //只有移动引起的消除才有效
			{
				//分类讨论消除
				if (2 == (int)fabs(linkBlocks))
				{
					commonLinkBlocksClear(gameArray[i][j].location, linkBlocks);
				}
				else if (3 == (int)fabs(linkBlocks))
				{
					if (linkBlocks > 0)
					{
						if (COMMON_BLOCK == gameArray[i][j].logicNumber % 10)
						{
							generateSpecialBlocks(gameArray[i][j].location, UP_AND_DOWN_BLOCK);
							commonLinkBlocksClear(gameArray[i][j].location, linkBlocks, false);
						}
						else if (UP_AND_DOWN_BLOCK == gameArray[i][j].logicNumber % 10)
						{
							colClear(gameArray[i][j].location);
						}
						else if (LEFT_AND_RIGHT_BLOCK == gameArray[i][j].logicNumber % 10)
						{
							rowClear(gameArray[i][j].location, false);
							colClear(gameArray[i][j].location);
						}
						else if (BRIGHT_BLOCK == gameArray[i][j].logicNumber % 10)
						{
							boomClear(gameArray[i][j].location, false);
							colClear(gameArray[i][j].location);
						}
					}
					else
					{
						if (COMMON_BLOCK == gameArray[i][j].logicNumber % 10)
						{
							generateSpecialBlocks(gameArray[i][j].location, LEFT_AND_RIGHT_BLOCK);
							commonLinkBlocksClear(gameArray[i][j].location, linkBlocks, false);
						}
						else if (UP_AND_DOWN_BLOCK == gameArray[i][j].logicNumber % 10)
						{
							colClear(gameArray[i][j].location, false);
							rowClear(gameArray[i][j].location);
						}
						else if (LEFT_AND_RIGHT_BLOCK == gameArray[i][j].logicNumber % 10)
						{
							rowClear(gameArray[i][j].location);
						}
						else if (BRIGHT_BLOCK == gameArray[i][j].logicNumber % 10)
						{
							boomClear(gameArray[i][j].location, false);
							rowClear(gameArray[i][j].location);
						}

					}
				}
				else if (4 == (int)fabs(linkBlocks))
				{
					if (COMMON_BLOCK == gameArray[i][j].logicNumber % 10)
					{
						int tempLogicNum = gameArray[i][j].logicNumber;
						generateSpecialBlocks(gameArray[i][j].location, COLOURFUL_BLOCK);
						gameArray[i][j].logicNumber = tempLogicNum;
						commonLinkBlocksClear(gameArray[i][j].location, linkBlocks, false);
						gameArray[i][j].logicNumber = COLOURFUL_BLOCK;
					}
					else if (UP_AND_DOWN_BLOCK == gameArray[i][j].logicNumber % 10)
					{
						generateSpecialBlocks(gameArray[i][j].location, COLOURFUL_BLOCK);
						colClear(gameArray[i][j].location, false);
						magicClear(gameArray[i][j].location);
					}
					else if (LEFT_AND_RIGHT_BLOCK == gameArray[i][j].logicNumber % 10)
					{
						generateSpecialBlocks(gameArray[i][j].location, COLOURFUL_BLOCK);
						rowClear(gameArray[i][j].location, false);
						magicClear(gameArray[i][j].location);
					}
					else if (BRIGHT_BLOCK == gameArray[i][j].logicNumber % 10)
					{
						generateSpecialBlocks(gameArray[i][j].location, COLOURFUL_BLOCK);
						boomClear(gameArray[i][j].location, false);
						magicClear(gameArray[i][j].location);
					}
				}
				else if (5 == (int)fabs(linkBlocks))
				{
					if (COMMON_BLOCK == gameArray[i][j].logicNumber % 10)
					{
						generateSpecialBlocks(gameArray[i][j].location, BRIGHT_BLOCK);
						commonLinkBlocksClear(gameArray[i][j].location, linkBlocks, false);
					}
					else if (UP_AND_DOWN_BLOCK == gameArray[i][j].logicNumber % 10)
					{
						generateSpecialBlocks(gameArray[i][j].location, BRIGHT_BLOCK);
						commonLinkBlocksClear(gameArray[i][j].location, linkBlocks, false);
						colClear(gameArray[i][j].location, false);
					}
					else if (LEFT_AND_RIGHT_BLOCK == gameArray[i][j].logicNumber % 10)
					{
						generateSpecialBlocks(gameArray[i][j].location, BRIGHT_BLOCK);
						commonLinkBlocksClear(gameArray[i][j].location, linkBlocks, false);
						rowClear(gameArray[i][j].location);
					}
					else if (BRIGHT_BLOCK == gameArray[i][j].logicNumber % 10)
					{
						commonLinkBlocksClear(gameArray[i][j].location, linkBlocks, false);
						boomClear(gameArray[i][j].location);
					}
				}
					
				else
				{
					gameArray[i][j].bIfMoved = false;
				}
			}
		}
	return;
}

void StandardGame::generateSpecialBlocks(const Position p, int type) //生成特殊块
{
	if (UP_AND_DOWN_BLOCK == type)
	{
		gameArray[p.col][p.row].logicNumber = gameArray[p.col][p.row].logicNumber / 10 * 10 + UP_AND_DOWN_BLOCK;
		gameArray[p.col][p.row].sprite->runAction(RepeatForever::create(animalAction->getAnimateUpAndDownByNum(gameArray[p.col][p.row].logicNumber))->clone());
	}
	else if (LEFT_AND_RIGHT_BLOCK == type)
	{
		gameArray[p.col][p.row].logicNumber = gameArray[p.col][p.row].logicNumber / 10 * 10 + LEFT_AND_RIGHT_BLOCK;
		gameArray[p.col][p.row].sprite->runAction(RepeatForever::create(animalAction->getAnimateLeftAndRightByNum(gameArray[p.col][p.row].logicNumber))->clone());
	}
	else if (BRIGHT_BLOCK == type)
	{
		gameArray[p.col][p.row].logicNumber = gameArray[p.col][p.row].logicNumber / 10 * 10 + BRIGHT_BLOCK;
		gameArray[p.col][p.row].sprite->runAction(RepeatForever::create(animalAction->getAnimateBrightByNum(gameArray[p.col][p.row].logicNumber))->clone());
	}
	else if (COLOURFUL_BLOCK == type)
	{
		gameArray[p.col][p.row].logicNumber = COLOURFUL_BLOCK;
		auto colourfulBird = Sprite::create("sprite/bird/bird.png");
		gameArray[p.col][p.row].sprite->setTexture(colourfulBird->getTexture());
	}

	return;
}

bool StandardGame::exchangeTwoBlocks(Vec2 clickBegan, Vec2 clickEnded)  //尝试交换两个块
{
	Position clickBlock = getClickLocation(clickBegan);
	if (clickBlock.col == -1 || clickBlock.row == -1)
	{
		return false;
	}
	int direction = getMoveDirection(clickBegan, clickEnded);
	if (direction)
	{
		if (multipleClear(gameArray[clickBlock.col][clickBlock.row].location, direction))
		{
			return true;
		}
		return exchangeTwoBlocksInOneDirection(gameArray[clickBlock.col][clickBlock.row].location, direction);
	}
	return false; //移动失败
}


bool StandardGame::multipleClear(const Position p1, int direction)  //尝试进行特殊的清除
{
	Position p2;
	bool bCanMove = false;
	int oppositeDirection;
	if (LEFT == direction && p1.col > 0)
	{
		p2.col = p1.col - 1;
		p2.row = p1.row;
		bCanMove = true;
		oppositeDirection = RIGHT;
	}
	else if (RIGHT == direction && p1.col < size.col - 1)
	{
		p2.col = p1.col + 1;
		p2.row = p1.row;
		bCanMove = true;
		oppositeDirection = LEFT;
	}
	else if (UP == direction && p1.row < size.row - 1)
	{
		p2.col = p1.col;
		p2.row = p1.row + 1;
		bCanMove = true;
		oppositeDirection = DOWN;
	}
	else if (DOWN == direction && p1.row > 0)
	{
		p2.col = p1.col;
		p2.row = p1.row - 1;
		bCanMove = true;
		oppositeDirection = UP;
	}
	if (bCanMove)
	{
		int BlockType1 = gameArray[p1.col][p1.row].logicNumber % 10;
		int BlockType2 = gameArray[p2.col][p2.row].logicNumber % 10;
		if ((BlockType1 >= UP_AND_DOWN_BLOCK && BlockType1 <= BRIGHT_BLOCK) && (BlockType2 >= UP_AND_DOWN_BLOCK && BlockType2 <= BRIGHT_BLOCK))
		{
			oneBlockClear(gameArray[p1.col][p1.row].location, false);
			oneBlockClear(gameArray[p2.col][p2.row].location);
			return true;
		}
		else if (COLOURFUL_BLOCK == BlockType1 && COLOURFUL_BLOCK != BlockType2)
		{
			magicClear(gameArray[p1.col][p1.row].location, direction);
			commonClear(p1);
			return true;
		}
		else if (COLOURFUL_BLOCK == BlockType2 && COLOURFUL_BLOCK != BlockType1)
		{
			magicClear(gameArray[p2.col][p2.row].location, oppositeDirection);
			commonClear(p2);
			return true;
		}
		else if (COLOURFUL_BLOCK == BlockType1 && COLOURFUL_BLOCK == BlockType2)
		{
			twoMagicClear(gameArray[p1.col][p1.row].location, gameArray[p2.col][p2.row].location);
			return true;
		}
	}
	return false;
}

void StandardGame::dropOneCol(int col)  //完成一列的掉落
{
	bool isFilled = false;
	while (!isFilled)
	{
		for (int i = 0; i < size.row; i++)
		{
			if (i == size.row - 1)
			{
				if (gameArray[col][i].logicNumber == NO_BLOCK)
				{
					gameArray[col][i].logicNumber = 10 * (rand() % 5 + 1);
					gameArray[col][i].bIfMoved = true;
					gameArray[col][i].createSprite();
					gameArray[col][i].sprite->setPosition(startPosition + Vec2(col * blockBackground->getContentSize().width, i * blockBackground->getContentSize().height + 15));
					pL->addChild(gameArray[col][i].sprite);
					auto moveDown = MoveBy::create(0.5f, Vec2(0, -15));
					gameArray[col][i].sprite->runAction(moveDown);
				}
			}
			else
			{
				if (NO_BLOCK == gameArray[col][i].logicNumber && NO_BLOCK != gameArray[col][i + 1].logicNumber)
				{
					gameArray[col][i + 1].sprite->runAction(animalAction->actionMoveDown->clone());
					exchange(&gameArray[col][i].sprite, &gameArray[col][i + 1].sprite);
					exchange(&gameArray[col][i].logicNumber, &gameArray[col][i + 1].logicNumber);
					gameArray[col][i].bIfMoved = true;
				}
			}
		}
		isFilled = true;  //假设性原则
		for (int i = 0; i < size.row; i++)
		{
			if (gameArray[col][i].logicNumber == NO_BLOCK)
			{
				isFilled = false;
				break;
			}
		}
	}
	return;
}

//往一个方向交换两个块
bool StandardGame::exchangeTwoBlocksInOneDirection(const Position p1, int direction)
{
	Position p2;
	Action* pAction1;
	Action* pAction2;
	bool bCanMove = false;
	if (LEFT == direction && p1.col > 0)
	{
		p2.col = p1.col - 1;
		p2.row = p1.row;
		pAction1 = animalAction->actionMoveLeft->clone();
		pAction2 = animalAction->actionMoveRight->clone();
		bCanMove = true;
	}
	else if (RIGHT == direction && p1.col < size.col - 1)
	{
		p2.col = p1.col + 1;
		p2.row = p1.row;
		pAction1 = animalAction->actionMoveRight->clone();
		pAction2 = animalAction->actionMoveLeft->clone();
		bCanMove = true;
	}
	else if (UP == direction && p1.row < size.row - 1)
	{
		p2.col = p1.col;
		p2.row = p1.row + 1;
		pAction1 = animalAction->actionMoveUp->clone();
		pAction2 = animalAction->actionMoveDown->clone();
		bCanMove = true;
	}
	else if (DOWN == direction && p1.row > 0)
	{
		p2.col = p1.col;
		p2.row = p1.row - 1;
		pAction1 = animalAction->actionMoveDown->clone();
		pAction2 = animalAction->actionMoveUp->clone();
		bCanMove = true;
	}
	if (bCanMove)
	{
		exchange(&gameArray[p1.col][p1.row].logicNumber, &gameArray[p2.col][p2.row].logicNumber);
		if ((int)fabs(countLinkedBlocks(&gameArray[p1.col][p1.row].location)) >= 2 ||
			(int)fabs(countLinkedBlocks(&gameArray[p2.col][p2.row].location)) >= 2)
		{
			gameArray[p1.col][p1.row].sprite->runAction(pAction1);
			gameArray[p2.col][p2.row].sprite->runAction(pAction2);
			exchange(&gameArray[p1.col][p1.row].sprite, &gameArray[p2.col][p2.row].sprite);
			gameArray[p1.col][p1.row].bIfMoved = true;
			gameArray[p2.col][p2.row].bIfMoved = true;
			return true;
		}
		else
		{
			exchange(&gameArray[p1.col][p1.row].logicNumber, &gameArray[p2.col][p2.row].logicNumber);//交换回来
		}
	}
	return false;
}

void StandardGame::drop()
{
	for (int i = 0; i < size.col;i++)
	{
		dropOneCol(i);
	}
	return;
}

bool StandardGame::checkContinue()
{
	bool ifContinue = true;
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			if ((int)fabs(countLinkedBlocks(&gameArray[i][j].location)) > 1)
			{
				ifContinue = false;
				return ifContinue;
			}
		}
	return ifContinue;
}



void StandardGame::gameBegin()
{
	build();
	init();
}

int StandardGame::checkEnded()
{
	if (moveTime > 0)
	{
		if (score >= targetScore && targetBearNum == 0)
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
		if (score >= targetScore && targetBearNum == 0)
		{
			return WIN;
		}
		else
		{
			return LOSE;
		}
	}
	
}


void StandardGame::playGame(Vec2 v1, Vec2 v2)
{
	animalAction->setBlockSize(blockBackground->getContentSize().width);
	animalAction->init();
	for (int i = 0; i < size.col; i++)
		for (int j = 0; j < size.row; j++)
		{
			gameArray[i][j].bIfMoved = false;
		}
	if (exchangeTwoBlocks(v1, v2))
	{
		if (moveTime > 0)
		{
			moveTime--;
		}
	}
	drop();
	while (!checkContinue())
	{
		clearBlocks();
		drop();
	}

}
