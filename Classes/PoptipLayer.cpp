#include "PoptipLayer.h"

PoptipLayer::PoptipLayer()
: m__pMenu(NULL)
, m__sfBackGround(NULL)
, m__s9BackGround(NULL)
, m__ltContentText(NULL)
, m__ltTitle(NULL)
{


}

PoptipLayer::~PoptipLayer()
{
	CC_SAFE_RELEASE(m__pMenu);
    CC_SAFE_RELEASE(m__sfBackGround);
    CC_SAFE_RELEASE(m__ltContentText);
    CC_SAFE_RELEASE(m__ltTitle);
    CC_SAFE_RELEASE(m__s9BackGround);
}

bool PoptipLayer::init()
{
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
		backSpri->setOpacity(80);
		backSpri->setAnchorPoint(ccp(0,0));
		backSpri->setPosition(ccp(0,0));
		this->addChild(backSpri);

		//开启触摸事件的捕获
		setTouchEnabled(true);

		bRef = true;
	}while(false);

	return bRef;
}

void PoptipLayer::registerWithTouchDispatcher()
{
	// 这里的触摸优先级设置为 -128 这保证了，屏蔽下方的触摸
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

}

bool PoptipLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCLog("PopupLayer touch");
	return true;
}

PoptipLayer* PoptipLayer::create(const char *backgroundImage)
{
	PoptipLayer* ml = PoptipLayer::create();
	CCSprite *sp = CCSprite::create(backgroundImage);
	ml->setSpriteBackGround(CCSprite::create(backgroundImage));
	ml->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));

	return ml;
}

void PoptipLayer::setTitle(const char *title, CCPoint &pos, ccColor3B &color, int fontsize)
{
	CCLabelTTF* ltfTitle = CCLabelTTF::create(title, "Arial", fontsize);
	ltfTitle->setColor(color);
	ltfTitle->setPosition(pos);
	setLabelTitle(ltfTitle);
}

bool PoptipLayer::addButton(const char* normalImage, const char* selectedImage, const char* title, CCPoint &pos, SEL_MenuHandler callfun, int tag/* = 0*/)
{
	// 创建图片菜单按钮
	CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage, this, callfun);
	menuImage->setTag(tag);
	menuImage->setPosition(pos);

	CCSize imenu = menuImage->getContentSize();
	CCLabelTTF* ttf = CCLabelTTF::create(title, "", 25);
	ttf->setColor(ccc3(0, 0, 0));
	//if (title == "重新开始")
	//{
	//}
	
	ttf->setPosition(ccp(imenu.width / 2, -imenu.height /2 ));
	menuImage->addChild(ttf);

	getMenuButton()->addChild(menuImage);
	return true;
}

void PoptipLayer::onEnter()
{
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
	this->addChild(getLabelTitle());
}

void PoptipLayer::onExit(){

	CCLog("popup on exit.");
	CCLayer::onExit();
}
