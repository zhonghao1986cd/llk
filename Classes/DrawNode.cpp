#include "DrawNode.h"

DrawNode::DrawNode()
{
	m_inflectionpointCount = 0;
	m_startPoint = ccp(0,0);
	m_endPoint = ccp(0,0);
	m_inflectionpoint1 = ccp(0,0);
	m_inflectionpoint2 = ccp(0,0);
}

DrawNode::DrawNode(int InflectionpointCount,CCPoint startPoint, CCPoint endPoint,CCPoint Inflectionpoint1,CCPoint Inflectionpoint2)
{
	m_inflectionpointCount = InflectionpointCount;
	m_startPoint = startPoint;
	m_endPoint = endPoint;
	m_inflectionpoint1 = Inflectionpoint1;
	m_inflectionpoint2 = Inflectionpoint2;
}

void DrawNode::setDrawPoint(int InflectionpointCount,CCPoint startPoint, CCPoint endPoint,CCPoint Inflectionpoint1,CCPoint Inflectionpoint2)
{
	m_inflectionpointCount = InflectionpointCount;
	m_startPoint = startPoint;
	m_endPoint = endPoint;
	m_inflectionpoint1 = Inflectionpoint1;
	m_inflectionpoint2 = Inflectionpoint2;
}

DrawNode::~DrawNode()
{

}

void DrawNode::draw()
{
	ccDrawColor4B(ccRED.r,ccRED.g,ccRED.b,255);
	glLineWidth(2);
	if(1 == m_inflectionpointCount)
	{	
		ccDrawLine(m_startPoint,m_endPoint);
	}
	else if (2 == m_inflectionpointCount)
	{
		ccDrawLine(m_startPoint,m_inflectionpoint1);
		ccDrawLine(m_inflectionpoint1,m_endPoint);
	}
	else if (3 == m_inflectionpointCount)
	{
		ccDrawLine(m_startPoint,m_inflectionpoint1);
		ccDrawLine(m_inflectionpoint1,m_inflectionpoint2);
		ccDrawLine(m_inflectionpoint2,m_endPoint);
	}
}