#pragma once

#include <layers_scenes_transitions_nodes\CCLayer.h>
#include <sprite_nodes\ccsprite.h>
#include "cocos2d.h"
#include "LogicFunImple.h"
#include "basefun.h"

USING_NS_CC;

#define ROW 12
#define COL	10
#define PICENUM 10
#define GAMETIME 30
#define HELPNUM  3

class GameLayer:
	public cocos2d::CCLayer
{
public:
	GameLayer(void);
	~GameLayer(void);

	virtual bool init();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	virtual void draw();

	void LoadResource();

	void LoadMusic();

	void LoadMap();

	CCPoint Point2Coordinate(SPoint& point);
	SPoint Coordinate2Point(CCPoint& Coordinate);
	void DealToucheSprite(CCSprite* sprite, SPoint point);
	bool Link2Sprite(CCSprite* sprite1,CCSprite* sprite2,SPoint point1,SPoint point2);
	CCFiniteTimeAction* getmovePath(int Inflectionpointcount,CCPoint stratpoint,CCPoint endpoint,CCPoint Inflectionpoint1,CCPoint Inflectionpoint2,float& livetime);

	void onHelpCallBack(CCObject* pSender);
	void onRefreshLayout(CCObject* pSender);
	void GameSchedule(float t);

	CREATE_FUNC(GameLayer);
private:
	cocos2d::CCSpriteFrameCache* _sourcePngs;
	CCArray* spriteLists;

	CCSprite* selectSprite1;
	CCSprite* selectSprite2;

	SPoint selectPoint1;
	SPoint selectPoint2;
	int selecePointCount;

	CLinkLogic m_LinkLogic;
	CCSize visibleSize;

	int drawResult;
	CCSprite* markSprite1;
	CCSprite* markSprite2;

	CCPoint InflectionpointCCpoint1;
	CCPoint InflectionpointCCpoint2;

	CCPoint m_drawPointstart,m_drawPointend;

	CCProgressTimer *planeHPTimer;

	int m_helpNum;
	int map[ROW][COL];
};