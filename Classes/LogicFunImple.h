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
	//��ɳ�ʼ����
	bool CreateInitLayout(int **maparray, int row, int col, int picNum = 16);
	
	//���ó�ʼ����
	//void SetInitLayout(int **a, int row, int col);
	
	//�ж�����
	unsigned LinkJudge(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1, SPoint &Inflectionpoint2);

	
	//�ж����޽�
	bool SolutionOfJudge();
	
	//��ɺ󣬽�����һ��
	//�˳��˲���ʱ
private:
	//{�����ж����ӵ��������
	bool LinkDirect(SPoint beginpoint, SPoint endpoint, bool flag = false);
	bool LinkOneInDirect(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1);
	bool LinkTowInDirect(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1, SPoint &Inflectionpoint2);
	bool expandX(SPoint beginpoint, SPoint endpoint, std::vector<SPoint> &Inflectionpoint1Vec,  std::vector<SPoint> &Inflectionpoint2Vec);
	bool expandY(SPoint beginpoint, SPoint endpoint, std::vector<SPoint> &Inflectionpoint1Vec,  std::vector<SPoint> &Inflectionpoint2Vec);
	//}


	//��ʼ������
	void InitMap(); 
public:
	//ˢ�²���
	void Refresh(int **maparray);

	//�����ҵ�һ����,����ֵΪ0��1��2��3��-1
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