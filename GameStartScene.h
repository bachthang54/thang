#ifndef __GAMESTART_SCENE_H__
#define __GAMESTART_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;


class GameStart : public cocos2d::Scene
{
public:
	cocos2d::Sprite *pipeTop, *pipeBot, *base, *backGround, *bird, *wizard, *wizardAttack, *birdAttack, *attack;
	SpriteBatchNode *spriteNode, *spriteNodeWizard, *spriteNodeBirdAttack, *spriteNodeWizardAttack;
	Label *label;
	__String *tempScore;
	bool addWizard;
	bool addAtk;


	static cocos2d::Scene* createScene(std::string , std::string);

	virtual bool init();

	void creatPipe(float);

	void createBird(float);

	void createWizard(float dt);

	void update(float);

	void removeAttack(float dt);

	//void menuAttackCallBack(cocos2d::Ref* pSender);

	bool onPhysicsContact(cocos2d::PhysicsContact &contact);

	bool onTouchBegan(Touch *touch, Event *unused_event);

	void onTouchMoved(Touch *touch, Event *unused_event);

	void onTouchEnded(Touch *touch, Event *unused_event);

	cocos2d::Animation *createAnimationBird(std::string name, int pFrameOder, float delay);
	cocos2d::Animation *createAnimationWizard(std::string name, int pFrameOder, float delay);

	cocos2d::Animation *animationBird, *animationWizard;

	CREATE_FUNC(GameStart);
	// implement the "static create()" method manually
private:
	cocos2d::PhysicsWorld *world;
	void setPhysicWorld(cocos2d::PhysicsWorld *m_world)
	{
		world = m_world;
	}
};


#endif // __GAMESTART_SCENE_H__
