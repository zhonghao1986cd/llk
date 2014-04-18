#include "GameLayer.h"
#include <sprite_nodes\CCSpriteBatchNode.h>
#include <sprite_nodes\CCSpriteFrameCache.h>
#include "CCGL.h"

USING_NS_CC;

GameLayer::GameLayer()
{
	spriteLists = CCArray::create();
	selecePointCount = 0;
	visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	m_helpNum = HELPNUM;
}

GameLayer::~GameLayer()
{

}

bool GameLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	if(!spriteLists)
	{
		spriteLists = new CCArray();
	}
	selecePointCount = 0;

	markSprite1 = CCSprite::create("bj.png");
	markSprite1->setVisible(false);
	markSprite1->retain();
	markSprite1->setOpacity(125);
	this->addChild(markSprite1,2);
	markSprite2 = CCSprite::create("bj.png");
	markSprite2->setVisible(false);
	markSprite2->setOpacity(125);
	markSprite2->retain();
	this->addChild(markSprite2,2);

	LoadResource();

	LoadMusic();

	LoadMap();

	setTouchEnabled(true);
	return true;
}
void GameLayer::LoadResource()
{
	_sourcePngs = CCSpriteFrameCache::sharedSpriteFrameCache();
	_sourcePngs->addSpriteFramesWithFile("resourcefile.plist","resourcefile.png");
}

void GameLayer::LoadMusic()
{

}

void GameLayer::LoadMap()
{
	spriteLists->removeAllObjects();
	memset(map,0,sizeof(int)*ROW*COL);
	if (m_LinkLogic.CreateInitLayout((int**)map,ROW,COL,PICENUM))
	{
		for (int i=1;i<ROW-1;++i)
		{
			for(int j=1;j<COL-1;++j)
			{
				CCSprite* sprite =  CCSprite::createWithSpriteFrame(_sourcePngs->spriteFrameByName(CCString::createWithFormat("%d.png",map[i][j])->getCString()));
				SPoint blockPosition= SPoint(j,i);
				CCPoint blockCoordinate = ccp(0,0);
				blockCoordinate = Point2Coordinate(blockPosition);
				if (blockCoordinate.x !=0&& blockCoordinate.y !=0)
				{
					sprite->setPosition(blockCoordinate);
					this->addChild(sprite);
					spriteLists->addObject(sprite);
				}
			}
		}
	}
	spriteLists->retain();

	//加入进度条显示
	CCProgressTimer *progresstime= CCProgressTimer::create(CCSprite::create("progress_bk.png"));
	progresstime->setAnchorPoint(ccp(0, 0));
	progresstime->setPercentage(100.0f);     
	progresstime->setPosition(ccp(200, 800));    
	this->addChild(progresstime);

	planeHPTimer = CCProgressTimer::create(CCSprite::create("progress.png"));
	planeHPTimer->setType(kCCProgressTimerTypeBar);
	planeHPTimer->setAnchorPoint(ccp(0, 0));
	planeHPTimer->setMidpoint(ccp(0,1));
	planeHPTimer->setBarChangeRate(ccp(1,0));
	planeHPTimer->setPercentage(100.0f);
	planeHPTimer->setPosition(ccp(200, 800));
	this->addChild(planeHPTimer);  
	CCProgressFromTo *t = CCProgressFromTo::create(GAMETIME, 0, 100);
	planeHPTimer->runAction(t);

	this->schedule(SEL_SCHEDULE(&GameLayer::GameSchedule),1.0f);

	//刷新布局
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		this,
		menu_selector(GameLayer::onRefreshLayout));

	pCloseItem->setPosition(ccp(100,200));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);
}

CCPoint GameLayer::Point2Coordinate(SPoint& point)
{
	CCPoint _result = ccp(0,0);
	_result = ccp((80+25+50*point.col),(visibleSize.height-180-25-50*point.row));
	return _result;
}

SPoint GameLayer::Coordinate2Point(CCPoint& Coordinate)
{
	SPoint _result = SPoint(0,0);
	_result = SPoint((Coordinate.x-80)/50,(visibleSize.height-Coordinate.y-180)/50);
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
	if (0 == selecePointCount)
	{
		selectSprite1 = sprite;
		selectPoint1 = point;
		CCPoint markPosition;
		markPosition = Point2Coordinate(point);
		markSprite1->setVisible(true);
		markSprite1->setPosition(markPosition);
		markSprite2->setVisible(false);
		selecePointCount++;
	}
	else if(selectPoint1 !=point)
	{
		selectSprite2 = sprite;
		selectPoint2 = point;
		CCPoint markPosition;
		markPosition = Point2Coordinate(point);
		markSprite2->setVisible(true);
		markSprite2->setPosition(markPosition);
		selecePointCount = 0;
		Link2Sprite(selectSprite1,selectSprite2,selectPoint1,selectPoint2);
	}
}

bool GameLayer::Link2Sprite(CCSprite* sprite1,CCSprite* sprite2,SPoint point1,SPoint point2)
{
	SPoint Inflectionpoint1;
	SPoint Inflectionpoint2;

	int linkResult = m_LinkLogic.LinkJudge(point1,point2,Inflectionpoint1,Inflectionpoint2);
	if (0 == linkResult)
	{
		drawResult = drawResult;
		return false;
	}
	else if(1 == linkResult)
	{	
		drawResult = 1;
		m_drawPointstart = Point2Coordinate(point1);
		m_drawPointend = Point2Coordinate(point2);
		CCParticleMeteor * _galaxy1 = CCParticleMeteor ::create();
		float _liveTime1;
		_galaxy1->setPosition(Point2Coordinate(point1));
		_galaxy1->setStartSize(5);
		_galaxy1->setEndSize(5);
		CCFiniteTimeAction* path1 = getmovePath(linkResult,Point2Coordinate(point1),Point2Coordinate(point2),Point2Coordinate(Inflectionpoint1),Point2Coordinate(Inflectionpoint2),_liveTime1);
		_galaxy1->runAction(path1);
		_galaxy1->setLife(_liveTime1);
		this->addChild(_galaxy1,3);

		CCParticleMeteor * _galaxy2 = CCParticleMeteor ::create();
		float _liveTime2;
		_galaxy2->setPosition(Point2Coordinate(point2));
		_galaxy2->setStartSize(5);
		_galaxy2->setEndSize(5);
		CCFiniteTimeAction* path2 = getmovePath(linkResult,Point2Coordinate(point2),Point2Coordinate(point1),Point2Coordinate(Inflectionpoint1),Point2Coordinate(Inflectionpoint2),_liveTime2);
		_galaxy2->runAction(path2);
		_galaxy2->setLife(_liveTime2);
		this->addChild(_galaxy2,3);
	}
	else if (2 == linkResult)
	{
		drawResult = 2;
		m_drawPointstart = Point2Coordinate(point1);
		m_drawPointend = Point2Coordinate(point2);
		InflectionpointCCpoint1 = Point2Coordinate(Inflectionpoint1);
		CCParticleMeteor * _galaxy1 = CCParticleMeteor ::create();
		float _liveTime1;
		_galaxy1->setPosition(Point2Coordinate(point1));
		_galaxy1->setStartSize(5);
		_galaxy1->setEndSize(5);
		CCFiniteTimeAction* path1 = getmovePath(linkResult,Point2Coordinate(point1),Point2Coordinate(point2),Point2Coordinate(Inflectionpoint1),Point2Coordinate(Inflectionpoint2),_liveTime1);
		_galaxy1->runAction(path1);
		_galaxy1->setLife(_liveTime1);
		this->addChild(_galaxy1,3);

		CCParticleMeteor * _galaxy2 = CCParticleMeteor ::create();
		float _liveTime2;
		_galaxy2->setPosition(Point2Coordinate(point2));
		_galaxy2->setStartSize(5);
		_galaxy2->setEndSize(5);
		CCFiniteTimeAction* path2 = getmovePath(linkResult,Point2Coordinate(point2),Point2Coordinate(point1),Point2Coordinate(Inflectionpoint1),Point2Coordinate(Inflectionpoint2),_liveTime2);
		_galaxy2->runAction(path2);
		_galaxy2->setLife(_liveTime2);
		this->addChild(_galaxy2,3);
	}
	else if (3 == linkResult)
	{
		drawResult = 3;
		m_drawPointstart = Point2Coordinate(point1);
		m_drawPointend = Point2Coordinate(point2);
		InflectionpointCCpoint1 = Point2Coordinate(Inflectionpoint1);
		InflectionpointCCpoint2 = Point2Coordinate(Inflectionpoint2);
		CCParticleMeteor * _galaxy1 = CCParticleMeteor ::create();
		float _liveTime1;
		_galaxy1->setPosition(Point2Coordinate(point1));
		_galaxy1->setStartSize(5);
		_galaxy1->setEndSize(5);
		CCFiniteTimeAction* path1 = getmovePath(linkResult,Point2Coordinate(point1),Point2Coordinate(point2),Point2Coordinate(Inflectionpoint1),Point2Coordinate(Inflectionpoint2),_liveTime1);
		_galaxy1->runAction(path1);
		_galaxy1->setLife(_liveTime1);
		this->addChild(_galaxy1,3);

		CCParticleMeteor * _galaxy2 = CCParticleMeteor ::create();
		float _liveTime2;
		_galaxy2->setPosition(Point2Coordinate(point2));
		_galaxy2->setStartSize(5);
		_galaxy2->setEndSize(5);
		CCFiniteTimeAction* path2 = getmovePath(linkResult,Point2Coordinate(point2),Point2Coordinate(point1),Point2Coordinate(Inflectionpoint2),Point2Coordinate(Inflectionpoint1),_liveTime2);
		_galaxy2->runAction(path2);
		_galaxy2->setLife(_liveTime2);
		this->addChild(_galaxy2,3);
	}

	this->removeChild((CCNode*)sprite1);
	this->removeChild((CCNode*)sprite2);

	spriteLists->removeObject(sprite1);
	spriteLists->removeObject(sprite2);

	return true;
}

void GameLayer::registerWithTouchDispatcher()
{ 
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);  
	CCLayer::registerWithTouchDispatcher();  
}

void GameLayer::draw()
{
	if(1 == drawResult)
	{	
		ccDrawLine(m_drawPointstart,m_drawPointend);
	}
	else if (2 == drawResult)
	{
		ccDrawLine(m_drawPointstart,InflectionpointCCpoint1);
		ccDrawLine(InflectionpointCCpoint1,m_drawPointend);
	}
	else if (3 == drawResult)
	{
		ccDrawLine(m_drawPointstart,InflectionpointCCpoint1);
		ccDrawLine(InflectionpointCCpoint1,InflectionpointCCpoint2);
		ccDrawLine(InflectionpointCCpoint2,m_drawPointend);
	}

}

CCFiniteTimeAction* GameLayer::getmovePath(int Inflectionpointcount,CCPoint startpoint,CCPoint endpoint,CCPoint Inflectionpoint1,CCPoint Inflectionpoint2,float& livetime)
{
	 CCFiniteTimeAction* path = NULL;
	 CCPoint midPoint = ccp(0,0);
	 float length = 0;
	 float midlength = 0;
	 float surpluslength = 0;
	 if (1 == Inflectionpointcount)
	 {
		midPoint = ccp((startpoint.x+endpoint.x)/2,(startpoint.y+endpoint.y)/2);
		livetime = (abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y))/2*0.8;
		CCMoveTo* movepath = CCMoveTo::create(0.8f,midPoint);
		path = CCSequence::create(movepath,NULL);
	 }
	 else if (2 == Inflectionpointcount)
	 {
		length = abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y)+abs(endpoint.x - Inflectionpoint1.x)+abs(endpoint.y - Inflectionpoint1.y);
		midlength = length/2;
		livetime = midlength*0.8;

		if ((abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y))>=midlength)
		{
			if ((startpoint.x == Inflectionpoint1.x) && (startpoint.y!=Inflectionpoint1.y))
			{
				midPoint = ccp(startpoint.x,startpoint.y-(startpoint.y - Inflectionpoint1.y)/abs(startpoint.y - Inflectionpoint1.y)*midlength);
				CCMoveTo* movepath = CCMoveTo::create(0.8f,midPoint);
				path = CCSequence::create(movepath,NULL);
			}
			else if((startpoint.y == Inflectionpoint1.y) && (startpoint.x !=Inflectionpoint1.x))
			{
				midPoint = ccp(startpoint.x-(startpoint.x - Inflectionpoint1.x)/abs(startpoint.x - Inflectionpoint1.x)*midlength,startpoint.y);
				CCMoveTo* movepath = CCMoveTo::create(0.8f,midPoint);
				path = CCSequence::create(movepath,NULL);
			}
		}
		else
		{
			surpluslength = midlength - (abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y));
			if ((endpoint.x == Inflectionpoint1.x) && (endpoint.y!=Inflectionpoint1.y))
			{
				midPoint = ccp(Inflectionpoint1.x,Inflectionpoint1.y-(Inflectionpoint1.y - endpoint.y)/abs(Inflectionpoint1.y - endpoint.y)*surpluslength);
				CCMoveTo* movepath1 = CCMoveTo::create(0.8f,Inflectionpoint1);
				CCMoveTo* movepath2 = CCMoveTo::create(0.8f,midPoint);
				path = CCSequence::create(movepath1,movepath2,NULL);
			}
			else if((endpoint.y == Inflectionpoint1.y) && (endpoint.x !=Inflectionpoint1.x))
			{
				midPoint = ccp(Inflectionpoint1.x-(Inflectionpoint1.x - endpoint.x)/abs(Inflectionpoint1.x - endpoint.x)*surpluslength,Inflectionpoint1.y);
				CCMoveTo* movepath1 = CCMoveTo::create(0.8f,Inflectionpoint1);
				CCMoveTo* movepath2 = CCMoveTo::create(0.8f,midPoint);
				path = CCSequence::create(movepath1,movepath2,NULL);
			}
		}
	 }
	 else if (3 == Inflectionpointcount)
	 {
		 length = abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y)
			 +abs(Inflectionpoint1.x - Inflectionpoint2.x)+abs(Inflectionpoint1.y - Inflectionpoint2.y)
			 + abs(Inflectionpoint2.x - endpoint.x)+abs(Inflectionpoint2.y - endpoint.y);
		 midlength = length/2;
		 livetime = midlength*0.8;

		 if ((abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y))>=midlength)
		 {
			 if ((startpoint.x == Inflectionpoint1.x) && (startpoint.y!=Inflectionpoint1.y))
			 {
				 midPoint = ccp(startpoint.x,startpoint.y-(startpoint.y - Inflectionpoint1.y)/abs(startpoint.y - Inflectionpoint1.y)*midlength);
				 CCMoveTo* movepath = CCMoveTo::create(0.8f,midPoint);
				 path = CCSequence::create(movepath,NULL);
			 }
			 else if((startpoint.y == Inflectionpoint1.y) && (startpoint.x !=Inflectionpoint1.x))
			 {
				 midPoint = ccp(startpoint.x-(startpoint.x - Inflectionpoint1.x)/abs(startpoint.x - Inflectionpoint1.x)*midlength,startpoint.y);
				 CCMoveTo* movepath = CCMoveTo::create(0.8f,midPoint);
				 path = CCSequence::create(movepath,NULL);
			 }
		 }
		 else if(((abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y))<midlength) 
			 &&((abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y)+abs(Inflectionpoint1.x - Inflectionpoint2.x)+abs(Inflectionpoint1.y - Inflectionpoint2.y))>=midlength))
		 {
			 surpluslength = midlength - (abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y));
			 if ((Inflectionpoint2.x == Inflectionpoint1.x) && (Inflectionpoint2.y!=Inflectionpoint1.y))
			 {
				 midPoint = ccp(Inflectionpoint1.x,Inflectionpoint1.y-(Inflectionpoint1.y - Inflectionpoint2.y)/abs(Inflectionpoint1.y - Inflectionpoint2.y)*surpluslength);
				 CCMoveTo* movepath1 = CCMoveTo::create(0.8f,Inflectionpoint1);
				 CCMoveTo* movepath2 = CCMoveTo::create(0.8f,midPoint);
				 path = CCSequence::create(movepath1,movepath2,NULL);
			 }
			 else if((Inflectionpoint2.y == Inflectionpoint1.y) && (Inflectionpoint2.x !=Inflectionpoint1.x))
			 {
				 midPoint = ccp(Inflectionpoint1.x-(Inflectionpoint1.x - Inflectionpoint2.x)/abs(Inflectionpoint1.x - Inflectionpoint2.x)*surpluslength,Inflectionpoint1.y);
				 CCMoveTo* movepath1 = CCMoveTo::create(0.8f,Inflectionpoint1);
				 CCMoveTo* movepath2 = CCMoveTo::create(0.8f,midPoint);
				 path = CCSequence::create(movepath1,movepath2,NULL);
			 }
		 }
		 else
		 {
			 surpluslength = midlength - (abs(startpoint.x - Inflectionpoint1.x)+abs(startpoint.y - Inflectionpoint1.y)+abs(Inflectionpoint1.x - Inflectionpoint2.x)+abs(Inflectionpoint1.y - Inflectionpoint2.y));
			 if ((Inflectionpoint2.x == endpoint.x) && (Inflectionpoint2.y!=endpoint.y))
			 {
				 midPoint = ccp(Inflectionpoint2.x,Inflectionpoint2.y-(Inflectionpoint2.y - endpoint.y)/abs(Inflectionpoint1.y - endpoint.y)*surpluslength);
				 CCMoveTo* movepath1 = CCMoveTo::create(0.8f,Inflectionpoint1);
				 CCMoveTo* movepath2 = CCMoveTo::create(0.8f,Inflectionpoint2);
				 CCMoveTo* movepath3 = CCMoveTo::create(0.8f,midPoint);
				 path = CCSequence::create(movepath1,movepath2,movepath3,NULL);
			 }
			 else if((Inflectionpoint2.y == endpoint.y) && (Inflectionpoint2.x !=endpoint.x))
			 {
				 midPoint = ccp(Inflectionpoint2.x-(Inflectionpoint2.x - endpoint.x)/abs(Inflectionpoint2.x - endpoint.x)*surpluslength,Inflectionpoint2.y);
				 CCMoveTo* movepath1 = CCMoveTo::create(0.8f,Inflectionpoint1);
				 CCMoveTo* movepath2 = CCMoveTo::create(0.8f,Inflectionpoint2);
				 CCMoveTo* movepath3 = CCMoveTo::create(0.8f,midPoint);
				 path = CCSequence::create(movepath1,movepath2,movepath3,NULL);
			 }
		 }
	 }
	 return path;
}
		
void  GameLayer::GameSchedule(float t)
{
	if (planeHPTimer->getPercentage() == 100.0f )
	{
		std::string tips = "时间到!";
		CCLabelTTF *font = CCLabelTTF::create(GBKToUTF(tips), "Arial", 30);
		font->setAnchorPoint(ccp(0,0));
		font->setPosition(ccp(270, 900));
		ccColor3B color = ccc3(255,255,0);
		font->setColor(color);
		GameLayer::addChild(font, 3);
		GameLayer::unschedule(SEL_SCHEDULE(&GameLayer::GameSchedule));
		GameLayer::setTouchEnabled(false);
		return;
	}
}

void GameLayer::onHelpCallBack(CCObject* pSender)
{
	if (0 == m_helpNum)
	{
		return;
	}

	CCSprite *sp1 = NULL;
	CCSprite *sp2 = NULL;
	CCActionInterval *action1 = CCBlink::create(5.0, 5);
	CCActionInterval *action2 = CCBlink::create(5.0, 5);
			
	SPoint p1, p2, Inflectionpoint1, Inflectionpoint2;
	int result = m_LinkLogic.Help(p1, p2);
	if ( 0 == result)
	{
		m_drawPointstart = Point2Coordinate(p1);
		m_drawPointend = Point2Coordinate(p2);

		//if (2 == result)
		//{
		//	InflectionpointCCpoint1 = Point2Coordinate(Inflectionpoint1);
		//}
		//
		//else if (3 == result)
		//{
		//	InflectionpointCCpoint1 = Point2Coordinate(Inflectionpoint1);
		//	InflectionpointCCpoint2 = Point2Coordinate(Inflectionpoint2);
		//}

		for (int i=0;i<spriteLists->count();++i)
		{
			CCSprite *checkSprite = (CCSprite*)spriteLists->objectAtIndex(i);
			if (checkSprite->boundingBox().containsPoint(m_drawPointstart))
			{
				sp1 = checkSprite;
			}
			else if (checkSprite->boundingBox().containsPoint(m_drawPointend))
			{
				sp2 = checkSprite;
			}
		}

		sp1->runAction(action1);
		sp2->runAction(action2);

		--m_helpNum;
	}

	else //无解的情况
	{

	}

}

void GameLayer::onRefreshLayout(CCObject* pSender)
{
	m_LinkLogic.Refresh((int**)map);
	for (int i = 0; i != spriteLists->count(); ++i)
	{
		this->removeChild( (CCSprite*)spriteLists->objectAtIndex(i) );
	}
	spriteLists->removeAllObjects();

	for (int i=1;i<ROW-1;++i)
	{
		for(int j=1;j<COL-1;++j)
		{
			if (0 == map[i][j])
			{
				continue;
			}
			CCSprite* sprite =  CCSprite::createWithSpriteFrame(_sourcePngs->spriteFrameByName(CCString::createWithFormat("%d.png",map[i][j])->getCString()));
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
