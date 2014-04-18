#ifndef _DRAWNODE_H_
#define _DRAWNODE_H_

#include "cocos2d.h" 
#include <base_nodes/CCNode.h>

USING_NS_CC;

class DrawNode:
	public cocos2d::CCNode
{
public:
	DrawNode();
	DrawNode(int InflectionpointCount,CCPoint startPoint, CCPoint endPoint,CCPoint Inflectionpoint1 = ccp(0,0),CCPoint Inflectionpoint2 = ccp(0,0));
	~DrawNode();

	CREATE_FUNC(DrawNode);

	virtual void draw();

	void setDrawPoint(int InflectionpointCount,CCPoint startPoint, CCPoint endPoint,CCPoint Inflectionpoint1 = ccp(0,0),CCPoint Inflectionpoint2 = ccp(0,0));

private:
	CCPoint m_startPoint;
	CCPoint m_endPoint;
	CCPoint m_inflectionpoint1;
	CCPoint m_inflectionpoint2;

	int m_inflectionpointCount;
	
};

#endif
