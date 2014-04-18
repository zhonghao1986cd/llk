#ifndef LOGIC_FUN_IMPLE_H
#define LOGIC_FUN_IMPLE_H

#include "basefun.h"
#include <vector>
#define MAX_ROW 10
#define MAX_COL 8
#define HELP_NUM 30

class CLinkLogic
{
public:
	CLinkLogic();
	~CLinkLogic();
	//完成初始布局
	bool CreateInitLayout(int **maparray, int row, int col, int picNum = 16);
	
	//设置初始布局
	//void SetInitLayout(int **a, int row, int col);
	
	//判断链接
	unsigned LinkJudge(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1, SPoint &Inflectionpoint2);

	
	//判断有无解
	bool SolutionOfJudge();
	
	//完成后，进入下一关
	//退出此布局时
private:
	//{用于判读链接的三种情况
	bool LinkDirect(SPoint beginpoint, SPoint endpoint, bool flag = false);
	bool LinkOneInDirect(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1);
	bool LinkTowInDirect(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1, SPoint &Inflectionpoint2);
	bool expandX(SPoint beginpoint, SPoint endpoint, std::vector<SPoint> &Inflectionpoint1Vec,  std::vector<SPoint> &Inflectionpoint2Vec);
	bool expandY(SPoint beginpoint, SPoint endpoint, std::vector<SPoint> &Inflectionpoint1Vec,  std::vector<SPoint> &Inflectionpoint2Vec);
	//}


	//初始化数组
	void InitMap(); 
public:
	//刷新布局
	void Refresh(int **maparray);

	//帮助找到一个解,返回值为0，1，2，3，-1
	int Help(SPoint &beginpoint, SPoint &endpoint/*, SPoint &Inflectionpoint1, SPoint &Inflectionpoint2*/);
private:
	int *m_linkMap;  /*[MAX_ROW+2][MAX_COL+2]*/
	int m_row;
	int m_column;
	int m_picNum;
	int m_helpNum;
	int m_totalRemainNodeNum;
};

#endif