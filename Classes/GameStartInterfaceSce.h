#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "basefun.h"

class GameStartInterface : public cocos2d::CCLayer
{
public:
	GameStartInterface();
    virtual bool init();  
    static cocos2d::CCScene* scene();
    
    void menuCloseCallback(CCObject* pSender);
	virtual void registerWithTouchDispatcher();

	void popupLayer();
	void buttonCallback(cocos2d::CCNode *pNode);
	void popupHightestScore();

	void LoadMusic();
	void RemoveMusic();
    
    CREATE_FUNC(GameStartInterface);
private:
	bool m_bMusicIsRuning_;
};

#endif // __HELLOWORLD_SCENE_H__
