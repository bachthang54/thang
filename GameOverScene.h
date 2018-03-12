#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Scene
{
public:
   // static cocos2d::Scene* createScene();
	
	static cocos2d::Scene* createScene(int , std::string ,std::string,bool);


    virtual bool init();
	void loadData();
	void highScore(int i, bool check);
	void BubbleSort();
	void updateSql(  std::string ID, std::string SCORE);
	void menuBackCallBack(cocos2d::Ref* pSender);
	void menuPlayAgainCallBack(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameOver);
};

#endif // __GAMEOVER_SCENE_H__
