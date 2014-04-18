#include "GameLayer.h"
#include <sprite_nodes\CCSpriteBatchNode.h>
#include <sprite_nodes\CCSpriteFrameCache.h>
#include "GameStartInterfaceSce.h"
#include "PoptipLayer.h"
#include "PopupLayer.h"
#include "GameScene.h"

#define DELAYTIME 0.2

USING_NS_CC;
using namespace std;

GameLayer::GameLayer()
	: m_bIsOpenAudio(true)
	, planeHPTimer(NULL)
	, helpFont(NULL)
	, m_helpNum(HELPNUM)
	, m_comingCount(0)
	, m_SelecePointCount(0)
	, m_curRetainSec(GAMETIME)
	, m_GameLevelSet(1)
	, m_gameScoreRecode(0)
	,m_row(8)
	,m_col(6)
	,m_piceNum(8)
	,m_totalRemainNodeNum(24)
	,m_bIsSuspend(false)
{
	spriteLists = CCArray::create();
	m_VisibleSize = CCDirector::sharedDirector()->getVisibleSize();

	m_LastHelpPoint1 = ccp(0, 0);
	m_LastHelpPoint2 = ccp(0, 0);
	map = new int[m_col*m_row];
	memset(map,0,sizeof(int)*m_col*m_row);
}

GameLayer::~GameLayer()
{
	SaveScore();
}

bool GameLayer::init()
{
	if (!CCLayer::init())
		return false;

	if(!spriteLists)
		spriteLists = CCArray::create();

	m_SelecePointCount = 0;

	markSprite1 = CCSprite::create("bj.png");
	markSprite1->setVisible(false);
	markSprite1->retain();
	markSprite1->setOpacity(200);
	this->addChild(markSprite1);
	markSprite2 = CCSprite::create("bj.png");
	markSprite2->setVisible(false);
	markSprite2->setOpacity(200);
	markSprite2->retain();
	this->addChild(markSprite2);

	m_drawNode = DrawNode::create();
	m_drawNode->setVisible(false);
	this->addChild(m_drawNode,4);

	LoadBackground();
	LoadResource();
	LoadMap();
	//load progress bar
	LoadProgressBar();
	//load refresh、helper and search button
	LoadButton();
	//set touch
	setTouchEnabled(true);
	//set audio
	m_bIsOpenAudio = CCUserDefault::sharedUserDefault()->getBoolForKey("audio", true);
	//set score
	ScoreShow();
	return true;
}
void GameLayer::LoadResource()
{
	//_sourcePngs4X6 = CCSpriteFrameCache::sharedSpriteFrameCache();
	//_sourcePngs4X6->addSpriteFramesWithFile("resourcefile4X6.plist","resourcefile4X6.png");

	//_sourcePngs6X8 = CCSpriteFrameCache::sharedSpriteFrameCache();
	//_sourcePngs6X8->addSpriteFramesWithFile("resourcefile6X8.plist","resourcefile6X8.png");

	//_sourcePngs8X8 = CCSpriteFrameCache::sharedSpriteFrameCache();
	//_sourcePngs8X8->addSpriteFramesWithFile("resourcefile8X8.plist","resourcefile8X8.png");

	//_sourcePngs8X10 = CCSpriteFrameCache::sharedSpriteFrameCache();
	//_sourcePngs8X10->addSpriteFramesWithFile("resourcefile8X10.plist","resourcefile8X10.png");
}


void GameLayer::LoadBackground()
{
	CCSprite* _backGroundSprite = CCSprite::create("wall.png");
	_backGroundSprite->setPosition(ccp(m_VisibleSize.width/2,m_VisibleSize.height/2));
	this->addChild(_backGroundSprite);
}


void GameLayer::LoadProgressBar()
{
	CCSprite *progress_bk = CCSprite::create("progress.png");
	progress_bk->setAnchorPoint(ccp(0, 0));  
	progress_bk->setPosition(ccp(200, 900));    
	this->addChild(progress_bk);

	planeHPTimer = CCProgressTimer::create(CCSprite::create("progress_bk.png"));
	planeHPTimer->setType(kCCProgressTimerTypeBar);
	planeHPTimer->setAnchorPoint(ccp(0, 0));
	planeHPTimer->setMidpoint(ccp(1,0));
	planeHPTimer->setBarChangeRate(ccp(1,0));
	planeHPTimer->setPercentage(0);
	planeHPTimer->setPosition(ccp(200, 900));
	this->addChild(planeHPTimer);  

	this->schedule(SEL_SCHEDULE(&GameLayer::GameSchedule), 0);
}

void GameLayer::LoadButton()
{
	CCMenu* pMenu = CCMenu::create();
	pMenu->setAnchorPoint(ccp(0,0));
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//界面刷新控件
	CCMenuItemImage *pRefreshItem = CCMenuItemImage::create(
		"refresh.png",
		"refresh2.png",
		this,
		menu_selector(GameLayer::onMenu));
	pRefreshItem->setScale(2.0);
	pRefreshItem->setPosition(ccp(visibleSize.width/2-100,150));
	pRefreshItem->setTag(0);
	pMenu->addChild(pRefreshItem);



	//帮助查找一个解
	CCMenuItemImage *pHelpItem = CCMenuItemImage::create(
		"search.png",
		"search2.png",
		this,
		menu_selector(GameLayer::onMenu));
	pHelpItem->setScale(2.0);
	pHelpItem->setPosition(ccp(visibleSize.width/2+100,150));
	pHelpItem->setTag(1);
	pMenu->addChild(pHelpItem);


	//显示帮助次数
	helpFont = CCLabelTTF::create("3", "Arial", 20);
	helpFont->setAnchorPoint(ccp(0,0));
	helpFont->setPosition(ccp(410, 180));
	ccColor3B color = ccc3(255,0,0);
	helpFont->setColor(color);
	GameLayer::addChild(helpFont, 1);

	//返回主菜单

	CCMenuItemImage *pBackItem = CCMenuItemImage::create(
		"back.png",
		"back.png",
		this,
		menu_selector(GameLayer::onMenu));
	pBackItem->setScale(2.0);
	pBackItem->setPosition(ccp(origin.x + 50, origin.y + visibleSize.height - 50));
	pBackItem->setTag(2);
	pMenu->addChild(pBackItem);

	CCMenuItemImage *pSuspendItem = CCMenuItemImage::create(
		"suspend.png",
		"suspend.png",
		this,
		menu_selector(GameLayer::onMenu));
	pSuspendItem->setScale(2.0);
	pSuspendItem->setPosition(ccp(origin.x + visibleSize.width - 50, origin.y + visibleSize.height - 50));
	pSuspendItem->setTag(3);
	pMenu->addChild(pSuspendItem);
}

void GameLayer::LoadMap()
{
	if (map)
	{	 
		delete [] map;
	}
	for (int i = 0; i != spriteLists->count(); ++i)
	{
		this->removeChild( (CCSprite*)spriteLists->objectAtIndex(i) );
	}
	spriteLists->removeAllObjects();
	
	if(_sourcePngs)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
		_sourcePngs = NULL;
	}

	if(1 == m_GameLevelSet)
	{
		m_col = 6;
		m_row = 8;
		m_piceNum = 8;
		map = new int[m_row*m_col];
		_sourcePngs4X6 = CCSpriteFrameCache::sharedSpriteFrameCache();
		_sourcePngs4X6->addSpriteFramesWithFile("resourcefile4X6.plist","resourcefile4X6.png");
		_sourcePngs	= _sourcePngs4X6;
	}
	else if (2 == m_GameLevelSet)
	{
		m_col = 8;
		m_row = 10;
		m_piceNum = 8;
		map = new int[m_row*m_col];
		_sourcePngs6X8 = CCSpriteFrameCache::sharedSpriteFrameCache();
		_sourcePngs6X8->addSpriteFramesWithFile("resourcefile6X8.plist","resourcefile6X8.png");
		_sourcePngs	= _sourcePngs6X8;
	}
	else if (3 == m_GameLevelSet)
	{
		m_col = 8;
		m_row = 10;
		m_piceNum = 12;
		map = new int[m_row*m_col];
		_sourcePngs6X8 = CCSpriteFrameCache::sharedSpriteFrameCache();
		_sourcePngs6X8->addSpriteFramesWithFile("resourcefile6X8.plist","resourcefile6X8.png");
		_sourcePngs	= _sourcePngs6X8;
	}
	else if ((3<m_GameLevelSet)&&(m_GameLevelSet<=5))
	{
		m_col = 10;
		m_row = 10;
		m_piceNum = 12;
		map = new int[m_row*m_col];
		_sourcePngs8X8 = CCSpriteFrameCache::sharedSpriteFrameCache();
		_sourcePngs8X8->addSpriteFramesWithFile("resourcefile8X8.plist","resourcefile8X8.png");
		_sourcePngs	= _sourcePngs8X8;
	} 
	else if ((5<m_GameLevelSet)&&(m_GameLevelSet<=7))
	{
		m_col = 10;
		m_row = 10;
		m_piceNum = 16;
		map = new int[m_row*m_col];
		_sourcePngs8X8 = CCSpriteFrameCache::sharedSpriteFrameCache();
		_sourcePngs8X8->addSpriteFramesWithFile("resourcefile8X8.plist","resourcefile8X8.png");
		_sourcePngs	= _sourcePngs8X8;
	} 
	else
	{
		m_col = 10;
		m_row = 12;
		m_piceNum = 16;
		map = new int[m_row*m_col];
		_sourcePngs8X10 = CCSpriteFrameCache::sharedSpriteFrameCache();
		_sourcePngs8X10->addSpriteFramesWithFile("resourcefile8X10.plist","resourcefile8X10.png");
		_sourcePngs	= _sourcePngs8X10;
	}
	m_totalRemainNodeNum = (m_row-2) * (m_col-2);

	memset(map,0,sizeof(int)*m_col*m_row);
	if (m_LinkLogic.CreateInitLayout((int**)map,m_row,m_col,m_piceNum))
	{
		for (int i=1;i<m_row-1;++i)
		{
			for(int j=1;j<m_col-1;++j)
			{
				CCSprite* sprite =  CCSprite::createWithSpriteFrame(_sourcePngs->spriteFrameByName(CCString::createWithFormat("%d.png",(*(map + i*m_col + j)))->getCString()));
				SPoint blockPosition= SPoint(j,i);
				CCPoint blockCoordinate = ccp(0,0);
				blockCoordinate = Point2Coordinate(blockPosition);
				if (blockCoordinate.x !=0&& blockCoordinate.y !=0)
				{
					sprite->setPosition(blockCoordinate);
					//sprite->setOpacity(00);
					this->addChild(sprite,2);
					spriteLists->addObject(sprite);
				}
			}
		}
	}
	spriteLists->retain();
}

CCPoint GameLayer::Point2Coordinate(SPoint& point)
{
	CCPoint _result = ccp(0,0);
	if(1 == m_GameLevelSet)
	{
	_result = ccp((30+45+90*point.col),(m_VisibleSize.height-130-40-80*point.row));
	}
	else if ((1<m_GameLevelSet)&&(m_GameLevelSet<=3))
	{
	_result = ccp((60+30+60*point.col),(m_VisibleSize.height-140-30-60*point.row));
	}
	else if ((3<m_GameLevelSet)&&(m_GameLevelSet<=7))
	{
	_result = ccp((75+22.5+45*point.col),(m_VisibleSize.height-140-30-60*point.row));
	} 
	else
	{
	_result = ccp((75+22.5+45*point.col),(m_VisibleSize.height-146-24-48*point.row));
	}

	return _result;
}

SPoint GameLayer::Coordinate2Point(CCPoint& Coordinate)
{
	SPoint _result = SPoint(0,0);

	if(1 == m_GameLevelSet)
	{
		_result = SPoint((Coordinate.x-30)/90,(m_VisibleSize.height-Coordinate.y-130)/80);
	}
	else if ((1<m_GameLevelSet)&&(m_GameLevelSet<=3))
	{
		_result = SPoint((Coordinate.x-60)/60,(m_VisibleSize.height-Coordinate.y-140)/60);
	}
	else if ((3<m_GameLevelSet)&&(m_GameLevelSet<=7))
	{
		_result = SPoint((Coordinate.x-75)/45,(m_VisibleSize.height-Coordinate.y-140)/60);
	} 
	else
	{
		_result = SPoint((Coordinate.x-75)/45,(m_VisibleSize.height-Coordinate.y-142)/48);
	}
	return _result;
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchLocation = this->convertTouchToNodeSpace(pTouch);

	SPoint touchPoint = SPoint(0,0);
	CCSprite *checkSprite;
	int i=0;
	for (;i<spriteLists->count();++i)
	{
		checkSprite = (CCSprite*)spriteLists->objectAtIndex(i);
		if (checkSprite->boundingBox().containsPoint(touchLocation))
		{
			touchPoint = Coordinate2Point(pTouch->getLocation());
			break;
		}
	}

	if (i<spriteLists->count()||touchPoint != SPoint(0,0))
	{	
		DealToucheSprite(checkSprite,touchPoint);
	}
	return true;
}

void GameLayer::DealToucheSprite(CCSprite* sprite,SPoint point)
{
	if (0 == m_SelecePointCount)
	{
		selectSprite1 = sprite;
		selectPoint1 = point;
		CCPoint markPosition;
		markPosition = Point2Coordinate(point);
		markSprite1->setVisible(true);
		markSprite1->setPosition(markPosition);
		markSprite2->setVisible(false);
		m_SelecePointCount++;
	}
	else if(selectPoint1 !=point)
	{
		selectSprite2 = sprite;
		selectPoint2 = point;
		CCPoint markPosition;
		markPosition = Point2Coordinate(point);
		markSprite2->setPosition(markPosition);
		markSprite2->setVisible(true);


		m_SelecePointCount = 0;
		if(!Link2Sprite(selectSprite1,selectSprite2,selectPoint1,selectPoint2))
		{
			markSprite1->setVisible(false);
			markSprite2->setVisible(false);
		}
	}
}

bool GameLayer::Link2Sprite(CCSprite* sprite1,CCSprite* sprite2,SPoint point1,SPoint point2)
{
	SPoint Inflectionpoint1;
	SPoint Inflectionpoint2;

	int linkResult = m_LinkLogic.LinkJudge(point1,point2,Inflectionpoint1,Inflectionpoint2);
	if (0 == linkResult)
	{
		return false;
	}
	m_drawNode->setDrawPoint(linkResult,Point2Coordinate(point1),Point2Coordinate(point2)
		,Point2Coordinate(Inflectionpoint1),Point2Coordinate(Inflectionpoint2));
	m_drawNode->setVisible(true);

	CCDelayTime* delay = CCDelayTime::create(DELAYTIME);
	CCHide* hide = CCHide::create();
	CCPlace* place = CCPlace::create(ccp(-100,-100));

	CCFiniteTimeAction* _drawNodeDelayHide = CCSequence::create(delay,hide,NULL);
	CCFiniteTimeAction* _markDelayHide1 = CCSequence::create(delay,hide,NULL);
	CCFiniteTimeAction* _markDelayHide2 = CCSequence::create(delay,hide,NULL);
	CCFiniteTimeAction* _spriteDelayHide1 = CCSequence::create(delay,place,NULL);
	CCFiniteTimeAction* _spriteDelayHide2 = CCSequence::create(delay,place,NULL);

	markSprite1->runAction(_markDelayHide1);
	markSprite2->runAction(_markDelayHide2);
	sprite1->runAction(_spriteDelayHide1);
	sprite2->runAction(_spriteDelayHide2);
	m_drawNode->runAction(_drawNodeDelayHide);
	if (m_bIsOpenAudio)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("link.wav");
	AddProgressTime();
	m_totalRemainNodeNum -= 2;
	if (0 != m_totalRemainNodeNum)
	{

		if(!m_LinkLogic.SolutionOfJudge())
		{
			m_LinkLogic.Refresh((int**)map);
			for (int i = 0; i != spriteLists->count(); ++i)
			{
				this->removeChild( (CCSprite*)spriteLists->objectAtIndex(i) );
			}
			spriteLists->removeAllObjects();

			for (int i=1;i<m_row-1;++i)
			{
				for(int j=1;j<m_col-1;++j)
				{
					if (0 == (*(map + i*m_col + j)))
					{
						continue;
					}
					CCSprite* sprite =  CCSprite::createWithSpriteFrame(_sourcePngs->spriteFrameByName(CCString::createWithFormat("%d.png",(*(map + i*m_col + j)))->getCString()));
					SPoint blockPosition= SPoint(j,i);
					CCPoint blockCoordinate = ccp(0,0);
					blockCoordinate = Point2Coordinate(blockPosition);
					if (blockCoordinate.x !=0 && blockCoordinate.y !=0)
					{
						sprite->setPosition(blockCoordinate);
						this->addChild(sprite);
						spriteLists->addObject(sprite);
					}
				}
			}

			spriteLists->retain();
		}
	}
	else if (0 == m_totalRemainNodeNum)
	{
		m_GameLevelSet++;
		//CCDelayTime* delay = CCDelayTime::create(1);
		//this->runAction(delay);
		//LoadMap();
		this->schedule(SEL_SCHEDULE(&GameLayer::reLoadMap),1000,1,0.5);
	}
	ScoreCalcu();
	return true;
}

void GameLayer::reLoadMap(float dt)
{
	LoadMap();
	unschedule(SEL_SCHEDULE(&GameLayer::reLoadMap));
}

void GameLayer::registerWithTouchDispatcher()
{ 
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);  
	CCLayer::registerWithTouchDispatcher();  
}

void GameLayer::AddProgressTime()
{
	int secTime = m_comingCount / 60; //because 60 frame/second
	
	m_comingCount -= (GAMETIME * 4);
	if (m_comingCount < 0)
		m_comingCount = 0;
	
	if ((GAMETIME - (m_comingCount / 60)) > m_curRetainSec)
	{
		GameLayer::removeChildByTag(secTime);
		CCLog("add tag = %d,  delect tag = %d", (m_comingCount / 60), secTime);
		CCString *str = CCString::createWithFormat("剩余时间%d秒",GAMETIME - (m_comingCount / 60));
		m_curRetainSec = GAMETIME - (m_comingCount / 60);
		std::string restTime(str->getCString());
		CCLabelTTF *showTime = CCLabelTTF::create(GBKToUTF(restTime), "Arial", 30);
		showTime->setAnchorPoint(ccp(0,0));
		showTime->setPosition(ccp(240, 850));
		ccColor3B color = ccc3(0,255,0);
		showTime->setColor(color);
		GameLayer::addChild(showTime, 1, (m_comingCount / 60));	
	}

	planeHPTimer->setPercentage(planeHPTimer->getPercentage() - (100.0f / (60 * GAMETIME)) * (GAMETIME * 4));
}


//每帧进来一次
void  GameLayer::GameSchedule(float t)
{
	if(m_bIsSuspend)
		return;
	m_comingCount += m_GameLevelSet;
	int secTime = m_comingCount / 60;  //because 60 frame/second

	CCLabelTTF *showTime;
	if (secTime >= 1 && m_curRetainSec != (GAMETIME - secTime))
	{
		GameLayer::removeChildByTag(secTime-1);
		CCString *str = CCString::createWithFormat("剩余时间%d秒",GAMETIME - secTime);
		m_curRetainSec = GAMETIME - secTime;
		std::string restTime(str->getCString());
		showTime = CCLabelTTF::create(GBKToUTF(restTime), "Arial", 30);
		showTime->setAnchorPoint(ccp(0,0));
		showTime->setPosition(ccp(240, 850));
		ccColor3B color = ccc3(0,255,0);
		showTime->setColor(color);
		GameLayer::addChild(showTime, 1, secTime);
		CCLog("剩余时间%d秒   add = %d, remove = %d",GAMETIME - secTime,  secTime, secTime - 1);
	}
	if (1 == m_comingCount)
	{
		if(NULL == GameLayer::getChildByTag(0))  
		{
			CCString *str = CCString::createWithFormat("剩余时间%d秒",GAMETIME - secTime);
			m_curRetainSec = GAMETIME - secTime;
			std::string restTime(str->getCString());
			showTime = CCLabelTTF::create(GBKToUTF(restTime), "Arial", 30);
			showTime->setAnchorPoint(ccp(0,0));
			showTime->setPosition(ccp(240, 850));
			ccColor3B color = ccc3(0,255,0);
			showTime->setColor(color);
			GameLayer::addChild(showTime, 0, secTime);	
		}
	}

	planeHPTimer->setPercentage(100.0f / (60 * GAMETIME) + planeHPTimer->getPercentage());
	

	if (planeHPTimer->getPercentage() == 100.0f )
	{
		GameLayer::removeChildByTag(secTime);
		std::string tips = "时间到!";
		CCLabelTTF *font = CCLabelTTF::create(GBKToUTF(tips), "Arial", 30);
		font->setAnchorPoint(ccp(0,0));
		font->setPosition(ccp(270, 850));
		ccColor3B color = ccc3(255,0,0);
		font->setColor(color);
		GameLayer::addChild(font);
		GameLayer::unschedule(SEL_SCHEDULE(&GameLayer::GameSchedule));
		GameLayer::setTouchEnabled(false);

		//游戏结束弹出提示对话框
		PoptipLayer *pl = PoptipLayer::create("popuplayer/BackGround2.png");
		CCPoint TitlePosition = ccp(320, 580);
		std::string Title = "Game Over!";
		ccColor3B TitleColor = ccc3(255, 0, 0);
		pl->setTitle(Title.c_str(), TitlePosition, TitleColor, 50);
		CCPoint positon1 = ccp(220, 400);
		std::string ReturnTip = "返回主菜单";
		pl->addButton("back.png", "back.png", GBKToUTF(ReturnTip), positon1, SEL_MenuHandler(&GameLayer::BacktoMainMenu),0);
		CCPoint positon2 = ccp(400, 400);
		std::string RestartTip = "重新开始";
		pl->addButton("play.png", "play.png", GBKToUTF(RestartTip), positon2, SEL_MenuHandler(&GameLayer::onRestartGame), 0);

		GameLayer::addChild(pl, 2);

		return;
	}
}


void GameLayer::onMenu(CCObject* pSender)
{
	CCMenuItem *pMenuItem = (CCMenuItem *)pSender;
	int f = pMenuItem->getTag();
	switch (pMenuItem->getTag())
	{
	case 0:
		{
			onRefreshLayout();
			break;
		}

	case 1:
		{
			onHelpCallBack();
			break;
		}
		break;
	case 2:
		{
			onBacktoMainMenu();
			break;
		}
	case 3:
		{
			onSuspend();
			break;
		}
	default:
		break;
	}
}


void GameLayer::onHelpCallBack()
{
	//帮助次数用完或者时间到了
	if (0 == m_helpNum || planeHPTimer->getPercentage() == 100.0)
		return;

	//上次提示的一对玩家没有消除，则本次帮助次数不减
	if (m_LastHelpPoint1.x != 0)
	{
		bool exit1 = false;
		bool exit2 = false;
		for (int i=0;i<spriteLists->count();++i)
		{
			CCSprite *checkSprite = (CCSprite*)spriteLists->objectAtIndex(i);
			if (checkSprite->boundingBox().containsPoint(m_LastHelpPoint1))
			{
				exit1 = true;
			}
			if (checkSprite->boundingBox().containsPoint(m_LastHelpPoint2))
			{
				exit2 = true;
			}
		}
		if (!exit1 && !exit2)
		{
			--m_helpNum;
		}
	}
	else
		--m_helpNum;


	//剩余帮助次数显示部分
	char buf[1024] = {0};
	sprintf(buf, "%d", m_helpNum);
	std::string tips(buf);
	GameLayer::removeChild(helpFont);
	helpFont = CCLabelTTF::create(tips.c_str(), "Arial", 20);
	helpFont->setAnchorPoint(ccp(0,0));
	helpFont->setPosition(ccp(410, 180));
	ccColor3B color = ccc3(255,0,0);
	helpFont->setColor(color);
	GameLayer::addChild(helpFont, 1);

	//提示两个可以消除的元素，闪烁效果
	if(m_helpNum >= 0)
	{
		CCSprite *sp1 = NULL;
		CCSprite *sp2 = NULL;
		CCActionInterval *actionBlink1 = CCBlink::create(1.0, 5);
		CCActionInterval *actionBlink2 = CCBlink::create(1.0, 5);

		SPoint p1, p2;
		int result = m_LinkLogic.Help(p1, p2);
		if ( 0 == result)
		{
			CCPoint drawPointstart = Point2Coordinate(p1);
			CCPoint drawPointend = Point2Coordinate(p2);

			for (int i=0;i<spriteLists->count();++i)
			{
				CCSprite *checkSprite = (CCSprite*)spriteLists->objectAtIndex(i);
				if (checkSprite->boundingBox().containsPoint(drawPointstart))
					sp1 = checkSprite;
				if (checkSprite->boundingBox().containsPoint(drawPointend))
					sp2 = checkSprite;
			}

			sp1->runAction(actionBlink1);
			sp2->runAction(actionBlink2);

			m_LastHelpPoint1 = drawPointstart;
			m_LastHelpPoint2 = drawPointend;
		}

		else //无解的情况
			;
	}	
}

void GameLayer::onRefreshLayout()
{
	//游戏时间到
	if(planeHPTimer->getPercentage() == 100.0)
		return;


	m_LinkLogic.Refresh((int**)map);
	for (int i = 0; i != spriteLists->count(); ++i)
	{
		this->removeChild( (CCSprite*)spriteLists->objectAtIndex(i) );
	}
	spriteLists->removeAllObjects();

	for (int i=1;i<m_row -1;++i)
	{
		for(int j=1;j<m_col -1;++j)
		{
			if (0 == (*(map + i*m_col + j)))
			{
				continue;
			}
			CCSprite* sprite =  CCSprite::createWithSpriteFrame(_sourcePngs->spriteFrameByName(CCString::createWithFormat("%d.png",(*(map + i*m_col + j)))->getCString()));
			SPoint blockPosition= SPoint(j,i);
			CCPoint blockCoordinate = ccp(0,0);
			blockCoordinate = Point2Coordinate(blockPosition);
			if (blockCoordinate.x !=0 && blockCoordinate.y !=0)
			{
				sprite->setPosition(blockCoordinate);
				this->addChild(sprite);
				spriteLists->addObject(sprite);
			}
		}
	}

	spriteLists->retain();
}

void GameLayer::onBacktoMainMenu()
{
	CCScene *pScene = GameStartInterface::scene();
	CCTransitionScene * ts=CCTransitionPageTurn::create(0.5, pScene, false);
	CCDirector::sharedDirector()->replaceScene(ts);
}

void GameLayer::BacktoMainMenu(cocos2d::CCObject *pObject)
{
	CCScene *pScene = GameStartInterface::scene();
	CCTransitionScene * ts=CCTransitionPageTurn::create(0.5, pScene, false);
	CCDirector::sharedDirector()->replaceScene(ts);
}

void GameLayer::onSuspend()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);
	PopupLayer* pl = PopupLayer::create("background.png", 0);
	pl->addButton("continue.png", "continue.png", "",pCenter, 5);
	pl->setCallbackFunc(this, callfuncN_selector(GameLayer::onResume));
	cocos2d::CCDirector::sharedDirector()->pause();
	m_bIsSuspend = true;
	this->addChild(pl,10,1024);
}

void GameLayer::onResume(cocos2d::CCNode *pNode)
{
	switch(pNode->getTag())
	{
	case 5:   //set voice
		{
			cocos2d::CCDirector::sharedDirector()->resume();
			m_bIsSuspend = false;
			this->removeChildByTag(1024);
		}
	default:
		break;
	}

}

void GameLayer::ScoreCalcu()
{
	m_gameScoreRecode += m_GameLevelSet;
	ScoreShow();

}

void GameLayer::ScoreShow()
{
	if(NULL != getChildByTag(GAMESCORETAG))
		removeChildByTag(GAMESCORETAG);

	CCString *str = CCString::createWithFormat("分数：%u",m_gameScoreRecode);
	std::string szScore(str->getCString());
	CCLabelTTF *showScore = CCLabelTTF::create(GBKToUTF(szScore), "Arial", 35);

	showScore->setAnchorPoint(ccp(0,0));
	showScore->setPosition(ccp(260, 750));
	ccColor3B color = ccc3(255,0,0);
	showScore->setColor(color);
	showScore->setTag(GAMESCORETAG);
	addChild(showScore);
}

void GameLayer::SaveScore()
{
	int hightestScode = CCUserDefault::sharedUserDefault()->getIntegerForKey("hightest_score", 0);
	if (hightestScode < m_gameScoreRecode)
		CCUserDefault::sharedUserDefault()->setIntegerForKey("hightest_score", m_gameScoreRecode);
}

void GameLayer::onRestartGame(cocos2d::CCObject *pObject)
{
	CCScene *pScene = GameScene::create();
	CCTransitionScene * s1=CCTransitionMoveInT::create(0.5, pScene);
	CCDirector::sharedDirector()->replaceScene(s1);
}