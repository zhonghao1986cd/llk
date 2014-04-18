
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class PoptipLayer: public CCLayer{
public:
	PoptipLayer();
	~PoptipLayer();

	virtual bool init();
	CREATE_FUNC(PoptipLayer);

	virtual void registerWithTouchDispatcher(void);
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	static PoptipLayer* create(const char* backgroundImage);

	void setTitle(const char* title, CCPoint &pos, int fontsize = 20);
	void setContentText(const char* text, CCPoint &contentPos, int fontsize = 20, int padding = 50, int paddintTop = 100);

	void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);

	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);
	bool addButton(const char* normalImage, const char* selectedImage, const char* title, CCPoint &pos, int tag = 0);
	virtual void onEnter();
	virtual void onExit();

private:

	void buttonCallback(CCObject* pSender);

	CC_SYNTHESIZE_RETAIN(CCMenu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(CCSprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltContentText, LabelContentText);
};
