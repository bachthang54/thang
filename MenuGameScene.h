#ifndef __MENUGAME_SCENE_H__
#define __MENUGAME_SCENE_H__

#include "cocos2d.h"

class MenuGame : public cocos2d::Scene
{
public:
   // static cocos2d::Scene* createScene();
	static cocos2d::Scene* createScene(std::string , std::string);
	cocos2d::MenuItemImage *playItem, *collectItem, *hightScoreItem, *quitItem;
	cocos2d::Menu *menuPlay, *menuCollect, *menuHightScore, *menuQuit;

    virtual bool init();
	void menuPlayCallback(cocos2d::Ref* pSender);
	void menuCollectCallback(cocos2d::Ref* pSender);
	void menuHightScoreCallback(cocos2d::Ref* pSender);
	void menuQuitCallback(cocos2d::Ref* pSender);

    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuGame);
};

#endif // __MENUGAME_SCENE_H__
