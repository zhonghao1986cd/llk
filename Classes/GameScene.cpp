#include "GameScene.h"

GameScene::GameScene()
{
	this->m_GameLayer = NULL;
}

GameScene::~GameScene()
{

}

bool GameScene::init()
{
	 if (!CCScene::init())
	 {
		 return false;
	 }

	 m_GameLayer = GameLayer::create();

	 this->addChild(m_GameLayer);

	 return true;
}