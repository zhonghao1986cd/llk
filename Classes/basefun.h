#ifndef BASE_FUN_H
#define BASE_FUN_H
#include <string.h>
#include <string>

struct SPoint
{
	int row;
	int col;
	SPoint(){memset(this, 0x0, sizeof(SPoint));}
	SPoint(int x,int y){col = x,row = y;}
	SPoint& operator= (const SPoint& other)
	{
		if(this == &other)
			return *this;
		row = other.row;
		col = other.col;
		return *this;
	}

	bool operator!= (const SPoint& other)
	{
		if(row != other.row||col != other.col)
			return true;
		else
			return false;
	}

	bool operator== (const SPoint& other)
	{
		if(row == other.row&&col == other.col)
			return true;
		else
			return false;
	}
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include <iconv\iconv.h>
int GBKToUTF8(std::string &gbkStr,const char* toCode,const char* formCode);
const char* GBKToUTF(std::string &gbkStr);
#endif

#endif