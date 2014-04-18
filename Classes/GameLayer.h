#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include <layers_scenes_transitions_nodes\CCLayer.h>
#include <sprite_nodes\ccsprite.h>
#include "cocos2d.h"
#include "LogicFunImple.h"
#include "basefun.h"
#include "DrawNode.h"
#include "SimpleAudioEngine.h"

#include <map>

USING_NS_CC;

#define ROW 12
#define COL	10
#define PICENUM 10
#define HELPNUM  3
#define GAMETIME 30  //”Œœ∑ ±º‰30s
#define GAMESCORETAG 1000

class GameLayer:
	public cocos2d::CCLayer
{
public:
	GameLayer(void);
	~GameLayer(void);

	virtual bool init();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();



	CCPoint Point2Coordinate(SPoint& point);
	SPoint Coordinate2Point(CCPoint& Coordinate);
	void DealToucheSprite(CCSprite* sprite, SPoint point);
	bool Link2Sprite(CCSprite* sprite1,CCSprite* sprite2,SPoint point1,SPoint point2);

	void  GameSchedule(float t);

	void AddProgressTime();


	CREATE_FUNC(GameLayer);
private:

	//load resource funcation
	void LoadBackground();
	void LoadResource();
	void LoadMap();
	void LoadProgressBar();
	void LoadButton();
	void reLoadMap(float dt);


	//menu backcall funcation
	void onMenu(CCObject* pSender);
	void onSuspend();
	void onResume(cocos2d::CCNode *pNode);
	void onHelpCallBack();
	void onRefreshLayout();
	void onBacktoMainMenu();

	//score record
	void ScoreCalcu();
	void ScoreShow();

	//exit fundation
	void SaveScore();


private:
	cocos2d::CCSpriteFrameCache* _sourcePngs;
	cocos2d::CCSpriteFrameCache* _sourcePngs4X6;
	cocos2d::CCSpriteFrameCache* _sourcePngs6X8;
	cocos2d::CCSpriteFrameCache* _sourcePngs8X8;
 	cocos2d::CCSpriteFrameCache* _sourcePngs8X10;
	CCArray* spriteLists;

	DrawNode* m_drawNode;

	CCSprite* selectSprite1;
	CCSprite* selectSprite2;

	SPoint selectPoint1;
	SPoint selectPoint2;
	int m_SelecePointCount;

	CLinkLogic m_LinkLogic;
	CCSize m_VisibleSize;

	int drawResult;
	CCSprite* markSprite1;
	CCSprite* markSprite2;

	CCPoint InflectionpointCCpoint1;
	CCPoint InflectionpointCCpoint2;


	CCPoint m_LastHelpPoint1,m_LastHelpPoint2;
	CCProgressTimer *planeHPTimer;
	CCLabelTTF *helpFont;

	
	int m_curRetainSec;

	bool m_bIsOpenAudio;
	bool m_bIsSuspend;

	int m_helpNum;
	int* map;
	int m_comingCount;
	int m_totalRemainNodeNum;

	int m_row;
	int m_col;
	int m_piceNum;

	unsigned int m_GameLevelSet;
	unsigned int m_gameScoreRecode;
};
#endif