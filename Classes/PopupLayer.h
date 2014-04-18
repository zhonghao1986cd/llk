//
//  PopupLayer.h
//  TestCpp
//
//  Created by leafsoar on 7/29/13.
//
//

#ifndef TestCpp_PopupLayer_h
#define TestCpp_PopupLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class PopupLayer: public CCLayer{
public:
    PopupLayer();
    ~PopupLayer();
    
    virtual bool init();
    CREATE_FUNC(PopupLayer);
    
    virtual void registerWithTouchDispatcher(void);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    static PopupLayer* create(const char* backgroundImage, unsigned int buttonCount);
    
    void setTitle(const char* title, CCPoint &pos, int fontsize = 20);
    void setContentText(const char* text, CCPoint &contentPos,  ccColor3B &Contexecolor, int fontsize = 20, int padding = 50, int paddintTop = 100);

    void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);
    
    bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);
	bool addButton(const char* normalImage, const char* selectedImage, const char* title, CCPoint &pos,  bool open = true, bool IsAudio = true, int tag = 0); 
   	bool addButton(const char* normalImage, const char* selectedImage, const char* title, CCPoint &pos, int tag = 0); 
	virtual void onEnter();
    virtual void onExit();

	bool getSetvoi(){return m_curSetVoice_;}
	bool getSetAud(){return m_curSetAudio_;}
    
private:
    
    void buttonCallback(CCObject* pSender);

    // 文字内容两边的空白区
    int m_contentPadding;
    int m_contentPaddingTop;
    
    CCObject* m_callbackListener;
    SEL_CallFuncN m_callback;

	CCPoint m_tilePos_;
	CCPoint m_contentPos_;

	bool m_curSetAudio_;      
	bool m_curSetVoice_;      
//	CCArray *m_pos;
	unsigned int m_hasButtonCount;		//可控button数

    CC_SYNTHESIZE_RETAIN(CCMenu*, m__pMenu, MenuButton);
    CC_SYNTHESIZE_RETAIN(CCSprite*, m__sfBackGround, SpriteBackGround);
    CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m__s9BackGround, Sprite9BackGround);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltTitle, LabelTitle);
    CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltContentText, LabelContentText);

	void setButtonCount(unsigned buttonCount);
};

#endif
