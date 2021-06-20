/*
作者：赖羿龙
学号：2051970
邮箱：3188001246@qq.com
完成日期：2021年6月20日
*/
#pragma once
#include"AnimalAction.h"

//基础关卡类
//虚函数多便于其他关卡的继承
class StandardGame 
{
private:
	const int targetScore = 700;

protected:

	//类指针
	static StandardGame* instance;

	//向量组大小
	Position size;

	//所属层指针
	Layer* pL;

	vector<vector<BlockElem> > gameArray;//直接用精灵组向量

	//背景图块指针
	Sprite* blockBackground; 

	//开始位置，确定游戏图像的整体位置
	Vec2 startPosition;

	//所需动作的实例
	AnimalAction* animalAction;

	int score;

	int moveTime;

	int targetBearNum;

	//交换两个任意数据类型变量的函数
	template<typename T>
	void exchange(T* pT1, T* pT2);

public:

	static StandardGame* getInstance();
	static void destroyInstance();
	StandardGame();
	~StandardGame();

	//初始化设置函数
	void set(int col, int row, int time, Layer* layer, Vec2 leftBottomPosition)
	{
		size.col = col;
		size.row = row;
		moveTime = time;
		pL = layer;
		startPosition = leftBottomPosition;
	};

	//获取当前分数
	int getScore();

	//获取剩余移动次数
	int getMoveTime();

	//获取剩余目标数
	virtual int getTargetNum();

	//获取目标分数
	virtual int getTargetScore();

	//构建游戏的精灵组向量
	virtual void build();

	//将向量初始化
	virtual bool init();

	int checkLinkedBlocksInOneDirection(const Position* p, const int logicNum, const int direction);  //判断物块沿某方向有多少个相同的函数

	int countLinkedBlocks(const Position* p);  //确定相连数和类型

	virtual Position getClickLocation(Vec2 clickBegan);  //获取点击的数组位置

	virtual int getMoveDirection(Vec2 clickBegan, Vec2 clickEnded);  //获取移动方向

	//完全消除
	virtual void commonClear(const Position p);

	//消除相连的块
	virtual void commonLinkBlocksClear(const Position p, int flag, bool ifClearItself = true);

	//消除整列
	virtual void colClear(const Position p, bool ifClearItself = true);

	//消除整行
	virtual void rowClear(const Position p, bool ifClearItself = true);

	//爆炸消除
	virtual void boomClear(const Position p, bool ifClearItself = true);

	//双魔力鸟消除
	virtual void twoMagicClear(const Position p1, const Position p2);

	virtual void magicClear(const Position p, int direction = 0);//触发魔力鸟

	//单个块消除
	virtual void oneBlockClear(const Position p, bool ifClearItself = true);

	//遍历全局进行消除
	virtual void clearBlocks();

	//生成新的特殊块
	virtual void generateSpecialBlocks(const Position p, int type);

	//特效叠加消除
	virtual bool multipleClear(const Position p1, int direction);

	//往一个方向交换两个块
	virtual bool exchangeTwoBlocksInOneDirection(const Position p1, int direction);

	//通过获取点击事件进行块的移动
	virtual bool exchangeTwoBlocks(Vec2 clickBegan, Vec2 clickEnded);

	virtual void dropOneCol(int col);  //掉落一列

	virtual void drop();  //全部掉落

	virtual bool checkContinue(); //检查是否掉落并消除完毕

	//游戏开始
	void gameBegin();
	
	//游戏是否结束检测
	virtual int checkEnded();

	//游戏入口
	virtual void playGame(Vec2 v1, Vec2 v2);

};

template<typename T>
inline void StandardGame::exchange(T* pT1, T* pT2)
{
	T temp;
	temp = *pT1;
	*pT1 = *pT2;
	*pT2 = temp;
	return;
}
