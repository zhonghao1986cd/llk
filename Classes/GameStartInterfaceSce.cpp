#include "GameStartInterfaceSce.h"
#include "SimpleAudioEngine.h"  
#include <string>
#include <vector>
#include "GameScene.h"
#include "PopupLayer.h"

USING_NS_CC;

GameStartInterface::GameStartInterface()
	: m_bMusicIsRuning_(false)
{

}

CCScene* GameStartInterface::scene()
{
    CCScene *scene = CCScene::create();
    
    GameStartInterface *layer = GameStartInterface::create();

    scene->addChild(layer);

    return scene;
}

bool GameStartInterface::init()
{

    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();



	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::create("bg.png");

	// position the sprite on the center of the screen
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);


	CCMenu* pMenu = CCMenu::create();
	pMenu->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height/2));

	//{一下只是供测试使用
	std::vector<std::string> wordString;
	wordString.push_back("退 出 游 戏");
	wordString.push_back("最 高 分 数");
	wordString.push_back("设 置 游 戏");
	wordString.push_back("游 戏 开 始");

	//for(int i = 0; i < 4; ++i)
	//{
	//	GBKToUTF(wordString[3 - i]);
	//	CCLabelTTF* label = CCLabelTTF::create(wordString[3 - i].c_str(), "Thonburi", 48); 
	//	ccColor3B color = ccc3(255, 0, 0); 
	//	label->setColor(color);
	//	CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(GameStartInterface::menuCloseCallback));
	//	pMenuItem->setPosition(ccp(origin.x, origin.y + label->getContentSize().height * (-i) + 5));
	//	pMenu->addChild(pMenuItem, i + 1);
	//}
	CCMenuItemImage *pStartMenu = CCMenuItemImage::create(
		"start.png",
		"start.png",
		this,
		menu_selector(GameStartInterface::menuCloseCallback));
	pStartMenu->setPosition(ccp(origin.x, origin.y + 100));
	pMenu->addChild(pStartMenu, 1);

	CCMenuItemImage *pSetMenu = CCMenuItemImage::create(
		"gameset.png",
		"gameset.png",
		this,
		menu_selector(GameStartInterface::menuCloseCallback));
	pSetMenu->setPosition(ccp(origin.x, origin.y));
	pMenu->addChild(pSetMenu, 2);

	CCMenuItemImage *pRecordMenu = CCMenuItemImage::create(
		"record.png",
		"record.png",
		this,
		menu_selector(GameStartInterface::menuCloseCallback));
	pRecordMenu->setPosition(ccp(origin.x, origin.y - 100));
	pMenu->addChild(pRecordMenu, 3);

	CCMenuItemImage *pEndMenu = CCMenuItemImage::create(
		"end.png",
		"end.png",
		this,
		menu_selector(GameStartInterface::menuCloseCallback));
	pEndMenu->setPosition(ccp(origin.x, origin.y -200));
	pMenu->addChild(pEndMenu, 4);
	this->addChild(pMenu);

	 //}

	 //{一下是供图片找到后正式使用
 //   CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
 //                                       "CloseNormal.png",
 //                                       "CloseSelected.png",
 //                                       this,
 //                                       menu_selector(GameStartInterface::menuCloseCallback));
 //   
	//pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
 //                               origin.y + pCloseItem->getContentSize().height/2));
 //   CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);

	 //}




	//music judge by though file
	bool bAudio = CCUserDefault::sharedUserDefault()->getBoolForKey("audio", true);
	bool bVoice = CCUserDefault::sharedUserDefault()->getBoolForKey("voice", true);
	if(true == bVoice)
		LoadMusic();  //open music
    return true;
}

void GameStartInterface::LoadMusic()
{
	if (!m_bMusicIsRuning_)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("真英雄.mp3", true);
		m_bMusicIsRuning_ = true;
	}
}

void GameStartInterface::RemoveMusic()
{
	if (m_bMusicIsRuning_)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		m_bMusicIsRuning_ = false;
	}
}



//game set attribute callfun
void GameStartInterface::popupLayer()
{
	PopupLayer* pl = PopupLayer::create("popuplayer/BackGround.png", 4);
//	pl->setContentSize(CCSizeMake(400, 300));
	pl->setCallbackFunc(this, callfuncN_selector(GameStartInterface::buttonCallback));

	//button info
	bool bAudio = CCUserDefault::sharedUserDefault()->getBoolForKey("audio", true);
	bool bVoice = CCUserDefault::sharedUserDefault()->getBoolForKey("voice", true);

	pl->addButton("popuplayer/pop_button.png", "popuplayer/pop_button.png", "sure", 1);
	pl->addButton("popuplayer/pop_button.png", "popuplayer/pop_button.png", "cancle", 2);
	if (bAudio)
		pl->addButton("popuplayer/open.png", "popuplayer/close.png", "", ccp(350,480), true, true, 3);
	else
		pl->addButton("popuplayer/close.png", "popuplayer/open.png", "", ccp(350,480), false, true, 3);
	if (bVoice)
		pl->addButton("popuplayer/open.png", "popuplayer/close.png", "", ccp(350,450), true, false, 4);
	else
		pl->addButton("popuplayer/close.png", "popuplayer/open.png", "", ccp(350,450), false, false, 4);

		pl->addButton("popuplayer/audio.png", "popuplayer/audio.png", "", ccp(225,480),5);
		pl->addButton("popuplayer/voice.png", "popuplayer/voice.png", "",ccp(225,450),6);


	this->addChild(pl);
}



void GameStartInterface::popupHightestScore()
{
	PopupLayer* pl = PopupLayer::create("popuplayer/BackGround.png", 0);
	pl->setContentSize(CCSizeMake(400, 300));
	int hightScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("hightest_score", 0);
	CCString *score = CCString::createWithFormat("最高分数：%d", hightScore);
	std::string str(score->getCString());
	//CCLabelTTF *showScore = CCLabelTTF::create(GBKToUTF(str), "Arial", 35);

	pl->setContentText(GBKToUTF(str), ccp(300,480), ccc3(255,255,255), 40);

	pl->addButton("popuplayer/pop_button.png", "popuplayer/pop_button.png", "sure", 1);
	pl->addButton("popuplayer/pop_button.png", "popuplayer/pop_button.png", "cancle", 2);

	this->addChild(pl);
}


//popuplayer class button callfun
void GameStartInterface::buttonCallback(cocos2d::CCNode *pNode){
	switch(pNode->getTag())
	{
	case 1: //sure
		{
			bool bVoice = CCUserDefault::sharedUserDefault()->getBoolForKey("voice", true);
			if(true == bVoice)
				LoadMusic();  //open music
			else
				RemoveMusic();  //close music
			break;
		}
	case 2:  //cancel
	case 3:  //set audio
	case 4:   //set voice
		{
			break;
		}
	default:
		break;
	}
}

void GameStartInterface::menuCloseCallback(CCObject* pSender)
{

	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int nIdx = pMenuItem->getZOrder();
	switch(nIdx)
	{
	case 1:     //play game
		{
			CCScene *pScene = GameScene::create();
			CCTransitionScene * s1=CCTransitionCrossFade::create(0, pScene);
			CCDirector::sharedDirector()->replaceScene(s1);
			break;
		}
	case 2:   //set attribute
		{
			popupLayer();
			break;
		}
	case 3:   //show hightest score
		{
			popupHightestScore();
			break;
		}
	case 4:        //exit game
		{
			CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
			break;
		}
	default:
		break;
	}
}

void GameStartInterface::registerWithTouchDispatcher()
{ 
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);  
	CCLayer::registerWithTouchDispatcher();  
}

