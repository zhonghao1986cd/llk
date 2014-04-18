
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

bool PoptipLayer::init(){
	bool bRef = false;
	do{
		CC_BREAK_IF(!CCLayer::init());
		this->setContentSize(CCSizeZero);

		//����ʵ�ֱ����䰵Ч��
		CCSprite *backSpri = CCSprite::create("popuplayer/back.png");
		backSpri->setOpacity(80);
		backSpri->setAnchorPoint(ccp(0,0));
		backSpri->setPosition(ccp(0,0));
		this->addChild(backSpri);

		//���������¼��Ĳ���
		setTouchEnabled(true);

		bRef = true;
	}while(0);
	return bRef;
}

void PoptipLayer::registerWithTouchDispatcher(){
	// ����Ĵ������ȼ�����Ϊ -128 �Ᵽ֤�ˣ������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

}

bool PoptipLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	CCLog("PopupLayer touch");
	return true;
}

PoptipLayer* PoptipLayer::create(const char *backgroundImage){
	PoptipLayer* ml = PoptipLayer::create();
	CCSprite *sp = CCSprite::create(backgroundImage);
	ml->setSpriteBackGround(CCSprite::create(backgroundImage));
	ml->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	return ml;
}

void PoptipLayer::setTitle(const char *title, CCPoint &pos, int fontsize){
	CCLabelTTF* ltfTitle = CCLabelTTF::create(title, "Arial", fontsize);
	ltfTitle->setColor(ccc3(255,0,0));
	//m_tilePos_ = pos;
	setLabelTitle(ltfTitle);
}

void PoptipLayer::setContentText(const char *text, CCPoint &contentPos, int fontsize, int padding, int paddingTop){
	CCLabelTTF* ltf = CCLabelTTF::create(text, "", fontsize);
	ltf->setColor(ccc3(255,0,0));
	setLabelContentText(ltf);
}

void PoptipLayer::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun){
 
}


bool PoptipLayer::addButton(const char* normalImage, const char* selectedImage, const char* title, CCPoint &pos, int tag/* = 0*/)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);

	// ����ͼƬ�˵���ť
	CCMenuItemImage *normal = CCMenuItemImage::create(normalImage, normalImage);
	CCMenuItemImage *checked = CCMenuItemImage::create(selectedImage, selectedImage);
	//�������ذ�ťѡ������ûص�����
	CCMenuItemToggle *toggle = CCMenuItemToggle::createWithTarget(this,
		menu_selector(PoptipLayer::buttonCallback),
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

bool PoptipLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);

	// ����ͼƬ�˵���ť
	CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage, this, menu_selector(PoptipLayer::buttonCallback));
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

//��������²������ͨ��
void PoptipLayer::buttonCallback(cocos2d::CCObject *pSender){
	CCNode* node = dynamic_cast<CCNode*>(pSender);
	CCLog("touch tag: %d", node->getTag());


	switch(node->getTag())
	{
	case 1:          //sure
		{
			this->removeFromParent();
			break;
		}
	case 2:        //cancel
		{ 
			this->removeFromParent();   
			break;
		}
	default:
		break;
	}
}

void PoptipLayer::onEnter(){
	CCLayer::onEnter();

	
}

void PoptipLayer::onExit(){

	CCLog("popup on exit.");
	CCLayer::onExit();
}
