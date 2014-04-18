#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include <layers_scenes_transitions_nodes\CCScene.h>
#include <cocos2d.h>
#include "Gamelayer.h"

class GameScene :
	public cocos2d::CCScene
{
public:
	GameScene(void);
	~GameScene(void);

	virtual bool init();

	CREATE_FUNC(GameScene);

	GameLayer *m_GameLayer;
};
#endif