#ifndef __INSERTNAME_SCENE_H__
#define __INSERTNAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "sqlite3.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;

class InsertName : public cocos2d::Scene
{


public:
	std::string tmp;
	cocos2d::MenuItemImage* okItem;
	cocos2d::Menu *menuOkItem;
	cocos2d::ui::TextField *textField;
	cocos2d::Sprite * sprite, *sprite1, *spriteText;
	sqlite3 * database;
    static cocos2d::Scene* createScene(std::string playerId, std::string);
    virtual bool init();
    
	Player player=Player("","","0");

	bool checkName(std::string name);
	bool onTouchBegan(Touch *touch, Event *unused_event);

	void menuOkItemCallback(cocos2d::Ref* pSender);

	void insertSql(std::string ID, std::string NAME, std::string SCORE);
	void textFunc(Ref* sender, ui::TextField::EventType type);

	bool enterName();
    
    // implement the "static create()" method manually
    CREATE_FUNC(InsertName);
};

#endif // __INSERTNAME_SCENE_H__
