#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "sqlite3.h"
#include "Player.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
	sqlite3 * data;
	bool sqlOk;
	cocos2d::Sprite *bgMenu, *spriteErro;
	cocos2d::Menu *menuLoginFacebook, *menuLoginErro;
	cocos2d::MenuItemImage *loginFacebookItem, *facebookErroItem;
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuLoginFacebookCallback(cocos2d::Ref* pSender);
	void menuLoginFacebookErro(cocos2d::Ref* pSender);
	void insertSql(std::string ID, std::string NAME, std::string SCORE);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
