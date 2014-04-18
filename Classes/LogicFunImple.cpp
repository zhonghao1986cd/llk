#include "LogicFunImple.h"
#include <time.h>
#include <algorithm>
#include <assert.h>

CLinkLogic::CLinkLogic()
	: m_linkMap(NULL)
	, m_row(0)
	, m_column(0)
	, m_picNum(0)
	, m_helpNum(HELP_NUM)
	, m_totalRemainNodeNum(0)
{
	//InitMap();
}

CLinkLogic::~CLinkLogic()
{
	if (m_linkMap)
		delete [] m_linkMap;
}

void CLinkLogic::InitMap()
{
	//memset(m_linkMap, 0x0, sizeof(m_linkMap) / sizeof(int));
}

//传入的行与列已经在首尾加入了'0'这一特殊的行列
bool CLinkLogic::CreateInitLayout(int **maparray, int row, int col, int picNum/* = 16*/)
{
	int data_row = row - 2;
	int data_col = col - 2;
	//assert((data_row * data_col) % picNum == 0);
	assert((data_row * data_col) % 2 == 0);
	//InitMap();

	m_totalRemainNodeNum = data_row * data_col;

	std::vector<int> pic_array(data_row * data_col);         //图片数组
	int pic_pair = (data_row * data_col) / 2 / picNum;       //图片出现的对数（整数部分）
	int resident_pair = (data_row * data_col) / 2 % picNum;  //余数部分

	for (int i = 0; i != pic_pair*2; ++i)
	{
		for(int j = 0; j != picNum; ++j)
		{
			pic_array[i * picNum + j] = j + 1;
		}		
	}
	srand((unsigned)time(NULL));
	random_shuffle(pic_array.begin(), pic_array.begin() + picNum);

	for (int i = 0; i != resident_pair*2; ++i)
	{
		if (i >= resident_pair)
		{
			pic_array[pic_pair * 2 * picNum + i] = pic_array[i-resident_pair];
		}
		else
		{
			pic_array[pic_pair * 2 * picNum + i] = pic_array[i];
		}		
	}

	//srand((unsigned)time(NULL));
	random_shuffle(pic_array.begin(), pic_array.end()); //进行一次随机

	for(int i = 1; i != row - 1; ++i)
	{
		for(int j = 1; j != col - 1; ++j)
		{
			int rand_num = rand()%(pic_array.size());
			*(((int *)maparray) + col * i + j) = pic_array[rand_num];
			pic_array.erase(pic_array.begin() + rand_num);
		}
	}
	//for (int i = 0; i != (row + 2) * (col + 2); ++i)
	//{
	//	int rand_num = rand()%(pic_array.size());
	////	maparray[i/MAX_COL][i%MAX_COL] = pic_array[rand_num];
	//	*(((int *)maparray) + col * (i/MAX_COL) + i%MAX_COL) = pic_array[rand_num];
	//	pic_array.erase(pic_array.begin() + rand_num);
	//}

	if (m_linkMap)
		delete [] m_linkMap;
	m_linkMap = new int[row * col];
	if (!m_linkMap)
		return false;
	memcpy(m_linkMap, maparray, row * col * sizeof(int));
	m_row = row;
	m_column = col;
	m_picNum = picNum;
	return true;
}

//还要加入是否有解的判断，或分开弄
bool CLinkLogic::LinkDirect(SPoint beginpoint, SPoint endpoint, bool flag/* = false*/)
{
	do 
	{
		//两次点击的是同一点
		if (beginpoint.row == endpoint.row && beginpoint.col == endpoint.col)
			break;
		if((*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) == *(m_linkMap + (endpoint.row * m_column) + endpoint.col)) ||  flag) //相连的2幅图片必须相同
		{
			//纵向直连判断
			if(beginpoint.col == endpoint.col)
			{
				int min_row = beginpoint.row <  endpoint.row ? beginpoint.row :  endpoint.row;
				int max_row = beginpoint.row >  endpoint.row ? beginpoint.row :  endpoint.row;

				if (min_row + 1 != max_row)        //考虑相邻的情况
				{
					for(int i = min_row + 1; i < max_row; ++i)
					{
						if(0 != *(m_linkMap + (i * m_column) + beginpoint.col))
							return false;
					}
				}
			}
			//横向直连判断
			else if(beginpoint.row == endpoint.row)
			{
				int min_column = beginpoint.col < endpoint.col ? beginpoint.col : endpoint.col;
				int max_column = beginpoint.col > endpoint.col ? beginpoint.col : endpoint.col;

				if (min_column + 1 != max_column)        //考虑相邻的情况
				{
					for(int i = min_column + 1; i < max_column; ++i)
					{
						if(0 != *(m_linkMap + (beginpoint.row * m_column) + i))
							return false;
					}
				}
			}
			else
				break;

			//if (!flag)
			//{
			//	*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) = 0;
			//	*(m_linkMap + (endpoint.row * m_column) + endpoint.col)     = 0;
			//}
			return true;
		}
	} while (false);

	return false;
}

bool CLinkLogic::LinkOneInDirect(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1)
{
	SPoint inflectionPoint;
	do 
	{
		if(*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) == *(m_linkMap + (endpoint.row * m_column) + endpoint.col)) //相连的2幅图片必须相同
		{
			inflectionPoint.row = beginpoint.row;
			inflectionPoint.col = endpoint.col;
			if ( 0 == *(m_linkMap + inflectionPoint.row * m_column + inflectionPoint.col) )
			{
				if( LinkDirect(inflectionPoint, endpoint, true) && LinkDirect(beginpoint, inflectionPoint, true) )
					break;
			}

			inflectionPoint.row = endpoint.row;
			inflectionPoint.col = beginpoint.col;
			if ( 0 == *(m_linkMap + inflectionPoint.row * m_column + inflectionPoint.col) )
			{
				if( LinkDirect(inflectionPoint, endpoint, true) && LinkDirect(beginpoint, inflectionPoint, true) )
					break;
			}
		}

		return false;

	} while (false);

	Inflectionpoint1 = inflectionPoint;
	//*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) = 0;
	//*(m_linkMap + (endpoint.row * m_column) + endpoint.col)     = 0;
	return true;
}

bool CLinkLogic::LinkTowInDirect(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1, SPoint &Inflectionpoint2)
{
	std::vector<SPoint> pointOneVec;
	std::vector<SPoint> pointTwoVec;
	if(*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) == *(m_linkMap + (endpoint.row * m_column) + endpoint.col)) //相连的2幅图片必须相同
	{
		expandX(beginpoint, endpoint, pointOneVec, pointTwoVec);
		for(std::vector<SPoint>::iterator iter1 = pointOneVec.begin(); iter1 != pointOneVec.end(); ++iter1)
		{
			bool flag = false;
			for (std::vector<SPoint>::iterator iter2 = pointTwoVec.begin(); iter2 != pointTwoVec.end(); ++iter2)
			{
				flag = LinkDirect(*iter1, *iter2, true);
				if (true == flag)
				{
					Inflectionpoint1 = *iter1;
					Inflectionpoint2 = *iter2;
					//*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) = 0;
					//*(m_linkMap + (endpoint.row * m_column) + endpoint.col)     = 0;
					return true;
				}
			}
		}

		pointOneVec.clear();
		pointTwoVec.clear();
		expandY(beginpoint, endpoint, pointOneVec, pointTwoVec);
		for(std::vector<SPoint>::iterator iter1 = pointOneVec.begin(); iter1 != pointOneVec.end(); ++iter1)
		{
			bool flag = false;
			for (std::vector<SPoint>::iterator iter2 = pointTwoVec.begin(); iter2 != pointTwoVec.end(); ++iter2)
			{
				flag = LinkDirect(*iter1, *iter2, true);
				if (true == flag)
				{
					Inflectionpoint1 = *iter1;
					Inflectionpoint2 = *iter2;
					//*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) = 0;
					//*(m_linkMap + (endpoint.row * m_column) + endpoint.col)     = 0;
					return true;
				}
			}
		}
	}
	return false;
}

bool CLinkLogic::expandX(SPoint beginpoint, SPoint endpoint, std::vector<SPoint> &Inflectionpoint1Vec,  std::vector<SPoint> &Inflectionpoint2Vec)
{
	int i = 1;
	bool flag1 = true;
	bool flag2 = true;
	SPoint tmpPoint;
	do 
	{
		if (flag1 && beginpoint.col - i >= 0 && (0 == *(m_linkMap + (beginpoint.row * m_column) + beginpoint.col - i)))
		{
			tmpPoint.row = beginpoint.row;
			tmpPoint.col = beginpoint.col - i;
			Inflectionpoint1Vec.push_back(tmpPoint);
		}
		else
			flag1 = false;

		if (flag2 && beginpoint.col + i < m_column && (0 == *(m_linkMap + (beginpoint.row * m_column) + beginpoint.col + i)))
		{
			tmpPoint.row = beginpoint.row;
			tmpPoint.col = beginpoint.col + i;
			Inflectionpoint1Vec.push_back(tmpPoint);
		}
		else
			flag2 = false;
		++i;

	} while (flag1 || flag2);


	i = 1;
	flag1 = true;
	flag2 = true;
	do 
	{
		if (flag1 && endpoint.col - i >= 0 && (0 == *(m_linkMap + (endpoint.row * m_column) + endpoint.col - i)))
		{
			tmpPoint.row = endpoint.row;
			tmpPoint.col = endpoint.col - i;
			Inflectionpoint2Vec.push_back(tmpPoint);
		}
		else
			flag1 = false;

		if (flag2 && endpoint.col + i < m_column && (0 == *(m_linkMap + (endpoint.row * m_column) + endpoint.col + i)))
		{
			tmpPoint.row = endpoint.row;
			tmpPoint.col = endpoint.col + i;
			Inflectionpoint2Vec.push_back(tmpPoint);
		}
		else
			flag2 = false;
		++i;

	} while (flag1 || flag2);

	return true;
}

bool CLinkLogic::expandY(SPoint beginpoint, SPoint endpoint, std::vector<SPoint> &Inflectionpoint1Vec,  std::vector<SPoint> &Inflectionpoint2Vec)
{
	int i = 1;
	bool flag1 = true;
	bool flag2 = true;
	SPoint tmpPoint;
	do 
	{
		if (flag1 && beginpoint.row - i >= 0 && (0 == *(m_linkMap + ((beginpoint.row - i)* m_column) + beginpoint.col)))
		{
			tmpPoint.row = beginpoint.row - i;
			tmpPoint.col = beginpoint.col;
			Inflectionpoint1Vec.push_back(tmpPoint);
		}
		else
			flag1 = false;

		if (flag2 && beginpoint.row + i < m_row && (0 == *(m_linkMap + ((beginpoint.row + i) * m_column) + beginpoint.col)))
		{
			tmpPoint.row = beginpoint.row +i;
			tmpPoint.col = beginpoint.col;
			Inflectionpoint1Vec.push_back(tmpPoint);
		}
		else
			flag2 = false;
		++i;

	} while (flag1 || flag2);


	i = 1;
	flag1 = true;
	flag2 = true;
	do 
	{
		if (flag1 && endpoint.row - i >= 0 && (0 == *(m_linkMap + ((endpoint.row - i) * m_column) + endpoint.col)))
		{
			tmpPoint.row = endpoint.row - i;
			tmpPoint.col = endpoint.col;
			Inflectionpoint2Vec.push_back(tmpPoint);
		}
		else
			flag1 = false;

		if (flag2 && endpoint.row + i < m_row && (0 == *(m_linkMap + ((endpoint.row + i) * m_column) + endpoint.col)))
		{
			tmpPoint.row = endpoint.row + i;
			tmpPoint.col = endpoint.col;
			Inflectionpoint2Vec.push_back(tmpPoint);
		}
		else
			flag2 = false;
		++i;

	} while (flag1 || flag2);

	return true;
}

unsigned CLinkLogic::LinkJudge(SPoint beginpoint, SPoint endpoint, SPoint &Inflectionpoint1, SPoint &Inflectionpoint2)
{
	if(LinkDirect(beginpoint, endpoint))
	{
		*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) = 0;
		*(m_linkMap + (endpoint.row * m_column) + endpoint.col)     = 0;
		m_totalRemainNodeNum -= 2;
		return 1;
	}
	else if(LinkOneInDirect(beginpoint, endpoint, Inflectionpoint1))
	{
		*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) = 0;
		*(m_linkMap + (endpoint.row * m_column) + endpoint.col)     = 0;
		m_totalRemainNodeNum -= 2;
		return 2;
	}
	else if(LinkTowInDirect(beginpoint, endpoint, Inflectionpoint1, Inflectionpoint2))
	{
		*(m_linkMap + (beginpoint.row * m_column) + beginpoint.col) = 0;
		*(m_linkMap + (endpoint.row * m_column) + endpoint.col)     = 0;
		m_totalRemainNodeNum -= 2;
		return 3;
	}
	else
		return 0;
}


bool CLinkLogic::SolutionOfJudge()
{
	if(!m_totalRemainNodeNum)
		return true;
	SPoint p1, p2, Inflectionpoint1, Inflectionpoint2;

	//逐行扫描
	for (int i = 1; i != m_row - 1; ++i)
	{
		for (int j = 1; j != m_column - 1; ++j)
		{
			if ( *(m_linkMap + i*m_column + j) != 0 ) //找到第一个扫描点
			{
				for (int x = i; x != m_row -1 ; ++x)  //x为当前扫描的行
				{
					for (int y = (x == i ? (j + 1) : 1); y != m_column - 1; ++y)
					{
						if ( *(m_linkMap + x*m_column + y) == *(m_linkMap + i*m_column + j) )
						{
							p1.row = i;
							p1.col = j;
							p2.row = x;
							p2.col = y;
							if (LinkDirect(p1, p2) || LinkOneInDirect(p1, p2, Inflectionpoint1) || LinkTowInDirect(p1, p2, Inflectionpoint1, Inflectionpoint2))
							{
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

void CLinkLogic::Refresh(int **maparray)
{
	srand((unsigned)time(NULL));
	for (int i = 1; i != m_row - 1; ++i)
	{
		for (int j = 1; j != m_column - 1; ++j)
		{
			if ( *(m_linkMap + i*m_column + j) != 0 )
			{	
				int tempRow = rand() % (m_row - 2) + 1;
				int tempCol = rand() % (m_column - 2) + 1;
				if (tempRow == i && tempCol == j)
				{
					continue;
				}
				//随机交换
				int temp = *(m_linkMap + i*m_column + j);
                *(m_linkMap + i*m_column + j) = *(m_linkMap + tempRow * m_column + tempCol);
				*(m_linkMap + tempRow * m_column + tempCol) = temp;
			}
		}
	}

	if ( !SolutionOfJudge() )
	{
		Refresh(maparray);
	}

	memset(maparray, 0, sizeof(int) * m_row * m_column);
	memcpy(maparray, m_linkMap, sizeof(int) * m_row * m_column);

	return;
}

int CLinkLogic::Help(SPoint &beginpoint, SPoint &endpoint/*, SPoint &Inflectionpoint1, SPoint &Inflectionpoint2*/)
{
	//if (0 == m_helpNum) //返回0表示帮助次数用完
	//{
	//	return 0;
	//}

	//逐行扫描
	for (int i = 1; i != m_row - 1; ++i)
	{
		for (int j = 1; j != m_column - 1; ++j)
		{
			if ( *(m_linkMap + i*m_column + j) != 0 ) //找到第一个扫描点
			{
				for (int x = i; x != m_row -1 ; ++x)  //x为当前扫描的行
				{
					for (int y = (x == i ? (j + 1) : 1); y != m_column - 1; ++y)
					{
						if ( *(m_linkMap + x*m_column + y) == *(m_linkMap + i*m_column + j) )
						{
							beginpoint.row = i;
							beginpoint.col = j;
							endpoint.row = x;
							endpoint.col = y;

							SPoint Inflectionpoint1, Inflectionpoint2;
							if (LinkDirect(beginpoint, endpoint) || LinkOneInDirect(beginpoint, endpoint, Inflectionpoint1) || LinkTowInDirect(beginpoint, endpoint, Inflectionpoint1, Inflectionpoint2))
							{
								return 0;
							}
						}
					}
				}
			}
		}
	}
	return -1;
}