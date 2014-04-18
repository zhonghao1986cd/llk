//
//  PopupLayer.cpp
//  TestCpp
//
//  Created by leafsoar on 7/29/13.
//
//

#include "PopupLayer.h"

PopupLayer::PopupLayer()
: m__pMenu(NULL)
, m_contentPadding(0)
, m_contentPaddingTop(0)
, m_callbackListener(NULL)
, m_callback(NULL)
, m__sfBackGround(NULL)
, m__s9BackGround(NULL)
, m__ltContentText(NULL)
, m__ltTitle(NULL)
, m_curSetAudio_(false)
, m_curSetVoice_(false)
{
    
}

PopupLayer::~PopupLayer(){
    CC_SAFE_RELEASE(m__pMenu);
    CC_SAFE_RELEASE(m__sfBackGround);
    CC_SAFE_RELEASE(m__ltContentText);
    CC_SAFE_RELEASE(m__ltTitle);
    CC_SAFE_RELEASE(m__s9BackGround);
}

bool PopupLayer::init(){
    bool bRef = false;
    do{
        CC_BREAK_IF(!CCLayer::init());
        this->setContentSize(CCSizeZero);
        
        // 初始化需要的 Menu
        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        setMenuButton(menu);

		//用于实现背景变暗效果
		CCSprite *backSpri = CCSprite::create("popuplayer/back.png");
		backSpri->setOpacity(40);
		backSpri->setAnchorPoint(ccp(0,0));
		backSpri->setPosition(ccp(0,0));
		this->addChild(backSpri);	

		//开启触摸事件的捕获
        setTouchEnabled(true);
        
        bRef = true;
    }while(0);
    return bRef;
}

void PopupLayer::registerWithTouchDispatcher(){
    // 这里的触摸优先级设置为 -128 这保证了，屏蔽下方的触摸
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

}

bool PopupLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCLog("PopupLayer touch");
    return true;
}

PopupLayer* PopupLayer::create(const char *backgroundImage, unsigned int buttonCount){
    PopupLayer* ml = PopupLayer::create();
    ml->setSpriteBackGround(CCSprite::create(backgroundImage));
    ml->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	ml->setButtonCount(buttonCount);
		
    return ml;
}

void  PopupLayer::setButtonCount(unsigned buttonCount)
{
	m_hasButtonCount = buttonCount;
}

void PopupLayer::setTitle(const char *title, CCPoint &pos, int fontsize){
    CCLabelTTF* ltfTitle = CCLabelTTF::create(title, "Arial", fontsize);
	ltfTitle->setColor(ccc3(255,0,0));
	m_tilePos_ = pos;
    setLabelTitle(ltfTitle);
}

void PopupLayer::setContentText(const char *text, CCPoint &contentPos, ccColor3B &Contexecolor, int fontsize, int padding, int paddingTop){
    CCLabelTTF* ltf = CCLabelTTF::create(text, "", fontsize);
	ltf->setColor(Contexecolor);
    setLabelContentText(ltf);
    m_contentPadding = padding;
    m_contentPaddingTop = paddingTop;
	m_contentPos_ = contentPos;
}

void PopupLayer::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun){
    m_callbackListener = target;
    m_callback = callfun;    
}


bool PopupLayer::addButton(const char* normalImage, const char* selectedImage, const char* title, CCPoint &pos, bool open/* = true*/, bool IsAudio/* = true*/, int tag/* = 0*/)
{
	if(IsAudio)
		m_curSetAudio_ = open;
	else
		m_curSetVoice_ = open;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);

	//根据xml文件来决定声效于声音开启否

	// 创建图片菜单按钮
	CCMenuItemImage *normal = CCMenuItemImage::create(normalImage, normalImage);
	CCMenuItemImage *checked = CCMenuItemImage::create(selectedImage, selectedImage);
	//创建开关按钮选项，并设置回调函数
	CCMenuItemToggle *toggle = CCMenuItemToggle::createWithTarget(this,
		menu_selector(PopupLayer::buttonCallback),
		normal,
		checked,
		NULL
		);
	toggle->setTag(tag);
	toggle->setPosition(pCenter);
	toggle->setPosition(pos);

	getMenuButton()->addChild(toggle);
	return true;

}
bool PopupLayer::addButton(const char* normalImage, const char* selectedImage, const char* title, CCPoint &pos,int tag/* = 0*/)
{
	//根据xml文件来决定声效于声音开启否

	// 创建图片菜单按钮
	CCMenuItemImage *normal = CCMenuItemImage::create(normalImage, normalImage);
	CCMenuItemImage *checked = CCMenuItemImage::create(selectedImage, selectedImage);
	//创建开关按钮选项，并设置回调函数
	CCMenuItemToggle *toggle = CCMenuItemToggle::createWithTarget(this,
		menu_selector(PopupLayer::buttonCallback),
		normal,
		checked,
		NULL
		);
	toggle->setTag(tag);
	toggle->setPosition(pos);
	//toggle->setPosition(pos);

	getMenuButton()->addChild(toggle);
	return true;

}

bool PopupLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag){
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);
    
    // 创建图片菜单按钮
    CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage, this, menu_selector(PopupLayer::buttonCallback));
    menuImage->setTag(tag);
    menuImage->setPosition(pCenter);
    
    CCSize imenu = menuImage->getContentSize();
    CCLabelTTF* ttf = CCLabelTTF::create(title, "", 20);
    ttf->setColor(ccc3(0, 0, 0));
    ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
    menuImage->addChild(ttf);
    
    getMenuButton()->addChild(menuImage);
    return true;
}

//用于完成下层的数据通信
void PopupLayer::buttonCallback(cocos2d::CCObject *pSender){
    CCNode* node = dynamic_cast<CCNode*>(pSender);
    CCLog("touch tag: %d", node->getTag());

	switch(node->getTag())
	{
	case 1:          //sure
		{
			//write back to xml file
			CCUserDefault::sharedUserDefault()->setBoolForKey("audio", m_curSetAudio_);
			CCUserDefault::sharedUserDefault()->setBoolForKey("voice", m_curSetVoice_);
			this->removeFromParent();
			break;
		}
	case 2:        //cancel
		{ 
			this->removeFromParent();   
			break;
		}
	case 3:      //set audio
		{
			m_curSetAudio_ = m_curSetAudio_?false:true; 
			break;
		}
	case 4:     //set voice 
		{
			m_curSetVoice_ = m_curSetVoice_?false:true;
			break;
		}
	default:
		break;
	}

	if (m_callback && m_callbackListener){
		(m_callbackListener->*m_callback)(node);
	}
}

void PopupLayer::onEnter(){
    CCLayer::onEnter();
    
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);
    
    CCSize contentSize;

	if (getContentSize().equals(CCSizeZero)) {
		getSpriteBackGround()->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(getSpriteBackGround(), 0, 0);
		contentSize = getSpriteBackGround()->getTexture()->getContentSize();
	} else {
		CCScale9Sprite *background = getSprite9BackGround();
		background->setContentSize(getContentSize());
		background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(background, 0, 0);
		contentSize = getContentSize();
	}
    
    this->addChild(getMenuButton());
	int g = getMenuButton()->getChildrenCount();
    float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount() + 1 - m_hasButtonCount);
    
    CCArray *array = getMenuButton()->getChildren();
    CCObject* pObj = NULL;
    int i = 0;
    CCARRAY_FOREACH(array, pObj){
        CCNode* node = dynamic_cast<CCNode*>(pObj);
		if (i < (getMenuButton()->getChildrenCount() - m_hasButtonCount))
			node->setPosition(ccp( winSize.width / 2 - contentSize.width / 2 + btnWidth * (i + 1), winSize.height / 2 - contentSize.height / 3));
		else
			break;
		i++;
	}

	if (getLabelTitle()){
		getLabelTitle()->setPosition(m_tilePos_);
		this->addChild(getLabelTitle());
	}


	if (getLabelContentText()){
		CCLabelTTF* ltf = getLabelContentText();
		ltf->setPosition(m_contentPos_);
		this->addChild(ltf);
	}
}

void PopupLayer::onExit(){

	CCLog("popup on exit.");
	CCLayer::onExit();
}
