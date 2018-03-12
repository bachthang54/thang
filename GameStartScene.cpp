#include "GameStartScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MenuGameScene.h"
#include "GameOverScene.h"
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif

USING_NS_CC;
bool isFly = false;
int score = 0;
static std::string id;
static std::string name;
Scene* GameStart::createScene(std::string i, std::string n)
{
	id = i;
	CCLOG("id gamestart : %s", id.c_str());
	name = n;
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);//set vat ly box2d
	auto layer = GameStart::create();
	layer->setPhysicWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameStartScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameStart::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//add attack button
	/*auto attackItem = MenuItemImage::create(
		"attack.png",
		"attack.png",
		CC_CALLBACK_1(GameStart::menuAttackCallBack, this));

	if (attackItem == nullptr ||
		attackItem->getContentSize().width <= 0 ||
		attackItem->getContentSize().height <= 0)
	{
		problemLoading("'attack.png' and 'attack.png'");
	}
	else
	{
		float x = origin.x;
		float y = origin.y+visibleSize.height/5;
		attackItem->setPosition(Vec2(x, y));
		attackItem->setAnchorPoint(Vec2(0, 0));
		attackItem->setOpacity(180);
		attackItem->setScale(visibleSize.width / (7 * attackItem->getContentSize().width));
		attackItem->setOpacity(80);

	}

	// create menu, it's an autorelease object
	auto menuAtack = Menu::create(attackItem, NULL);
	menuAtack->setPosition(Vec2::ZERO);
	this->addChild(menuAtack, 1);*/

	/////////////////////////////
	auto attackButton = Sprite::create("attack.png");

	float x = origin.x+visibleSize.width / 7;
	float y = origin.y +2* visibleSize.height / 5;
	attackButton->setPosition(Vec2(x, y));
	attackButton->setAnchorPoint(Vec2(1, 0));
	attackButton->setOpacity(180);
	attackButton->setScale(visibleSize.width / (7 * attackButton->getContentSize().width));
	attackButton->setOpacity(80);
	


	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 1);
	edgeBody->setCollisionBitmask(4);
	edgeBody->setContactTestBitmask(true);

	//tao edgeNode
	auto edgeNode = Node::create();
	edgeNode->setAnchorPoint(Vec2(0.5, 0));
	edgeNode->setPosition(origin.x+visibleSize.width / 2,origin.y+ visibleSize.height / 2);//vi tri edgeNode o trung tam
	edgeNode->setPhysicsBody(edgeBody);
	addChild(edgeNode);
	//////////////
	//nen mat dat
	//base -> Sprite::create("base.png");
	base = Sprite::create("base.png");
	base->setAnchorPoint(Vec2(0, 0));//set mo neo cho base
	base->setPosition(Vec2(origin.x, origin.y));//set vi tri 
	base->setScaleY(visibleSize.height / (10 * base->getContentSize().height));//tang chieu cao mat dat bang 1/5 man hinh
	base->setScaleX(visibleSize.width / base->getContentSize().width);// tang chieu rong mat dat bang chieu rong man hinh
	auto bodyBase = PhysicsBody::createBox(base->getContentSize());
	bodyBase->setCollisionBitmask(5);
	bodyBase->setContactTestBitmask(true);
	bodyBase->setDynamic(false);//set trong luc cho mat dat false
	base->setPhysicsBody(bodyBase);

	//load background
	srand(time(NULL));
	int a = rand() % 2;
	if (a == 0)
	{
		backGround = Sprite::create("bgPlay.png");

	}
	else backGround = Sprite::create("bgPlay.png");

	backGround->setAnchorPoint(Vec2(0, 0));
	backGround->setPosition(Vec2(origin.x, origin.y));
	backGround->setScaleX(visibleSize.width / backGround->getContentSize().width);//set chieu rong bg bang chieu rong man hinh
	backGround->setScaleY((6 * visibleSize.height) / (5 * backGround->getContentSize().height));//set chieu cao cho bg bang chieu cao man hinh-chieu cao base

	this->schedule(schedule_selector(GameStart::creatPipe), 1.5);// lap lai ham goi ong nuoc sau moi 2 s


	addChild(backGround);
	addChild(base);
	/////////////////////////

	GameStart::createBird(1);
	
	
	/////////////////////////

	//doan sau xu ly touch
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(GameStart::onTouchBegan, this);

	touchListener->onTouchMoved = CC_CALLBACK_2(GameStart::onTouchMoved, this);

	touchListener->onTouchEnded = CC_CALLBACK_2(GameStart::onTouchEnded, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	

	//////////////////////////
	//xu ly va cham vat ly
	auto listenerContact = EventListenerPhysicsContact::create();
	listenerContact->onContactBegin = CC_CALLBACK_1(GameStart::onPhysicsContact, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerContact, this);

	////////////////////////
	//hien thi diem so

	//auto label = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize*0.1);
	label = Label::create("0", "arial.ttf", 24);
	label->setPosition(Vec2(origin.x+visibleSize.width / 2,origin.y+ visibleSize.height*0.85));
	label->setColor(Color3B::RED);
	addChild(label);
	
	
	this->addChild(attackButton);
	GameStart::createWizard(1);
	//this->schedule(schedule_selector(GameStart::removeAttack), 0.05);
	///////////////////////
	return true;
}

//ham tao animation bird/dragon
cocos2d::Animation * GameStart::createAnimationBird(std::string name, int pFrameOder, float delay)
{
	Vector<SpriteFrame*>animFrames;
	animFrames.reserve(pFrameOder);

	for (int i = 1; i <= pFrameOder; i++)
	{
		char buf[20] = { 0 };
		sprintf(buf, "%d.png", i);
		std::string str = name + buf;//ten cua anh

		printf("%s", buf);
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		animFrames.pushBack(frame);

	}
	animationBird = Animation::createWithSpriteFrames(animFrames, delay);
	return animationBird;
}

//annimation wizard

cocos2d::Animation * GameStart::createAnimationWizard(std::string name, int pFrameOder, float delay)
{
	Vector<SpriteFrame*>animFrames;
	animFrames.reserve(pFrameOder);

	for (int i = 1; i <= pFrameOder; i++)
	{
		char buf[20] = { 0 };
		sprintf(buf, "%d.png", i);
		std::string str = name + buf;//ten cua anh

		printf("%s", buf);
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		animFrames.pushBack(frame);

	}
	animationWizard = Animation::createWithSpriteFrames(animFrames, delay);
	return animationWizard;
}

//tao ong nuoc//lau dai
void GameStart::creatPipe(float dt)
{
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//doan sau tao lau dai(ong nuoc)
	float birdHeight = visibleSize.height / 10;
	srand(time(NULL));
	int a = rand() % 2;
	if (a == 0)
	{
		 pipeTop = Sprite::create("tree.png");
		 pipeBot = Sprite::create("castle.png");

	}
	else
	{
		 pipeTop = Sprite::create("tree.png");
		 pipeBot = Sprite::create("tower.png");
	}


	//set mo neo va toa do cho pipe

	float pipeTopScaleX, pipeTopScaleY, pipeBotScaleX, pipeBotScaleY;//kich thuoc cua ong khoi tren va duoi
																	 //cai dat cac vi tri , kich thuoc ong khoi random
	float ran = random(0, 1);
	if (ran <= 0.3)
	{
		ran = 0.3;
	}
	else if (ran >= 0.7) ran = 0.7;

	pipeTopScaleX = visibleSize.width / (8 * pipeTop->getContentSize().width);
	pipeTopScaleY = ((9 * visibleSize.height / 10) - 3 * birdHeight) / pipeTop->getContentSize().height;
	pipeBotScaleX = visibleSize.width / (8 * pipeBot->getContentSize().width);
	pipeBotScaleY = ((9 * visibleSize.height / 10) - 3 * birdHeight) / pipeBot->getContentSize().height;

	pipeTop->setScaleX(pipeTopScaleX);
	pipeTop->setScaleY(ran*pipeTopScaleY);
	pipeBot->setScaleX(pipeBotScaleX);
	pipeBot->setScaleY(pipeBotScaleY*(1 - ran));

	//set vat ly cho pipe
	auto pipeTopBody = PhysicsBody::createBox(pipeTop->getContentSize());
	pipeTopBody->setDynamic(false);
	pipeTopBody->setCollisionBitmask(2);
	pipeTopBody->setContactTestBitmask(true);
	pipeTop->setPhysicsBody(pipeTopBody);

	auto pipeBotBody = PhysicsBody::createBox(pipeBot->getContentSize());
	pipeBotBody->setDynamic(false);
	pipeBotBody->setCollisionBitmask(2);
	pipeBotBody->setContactTestBitmask(true);
	pipeBot->setPhysicsBody(pipeBotBody);

	pipeTop->setAnchorPoint(Vec2(0.5, 0));
	pipeTop->setPosition(Vec2(origin.x+visibleSize.width, origin.y+visibleSize.height));
	pipeTop->setRotation(180);
	auto actionTopMoveTo = MoveTo::create(4, Vec2(-(visibleSize.width+origin.x)/2, origin.y+visibleSize.height));//di chuyen ve ben trai

	pipeTop->runAction(actionTopMoveTo);
	

	pipeBot->setAnchorPoint(Vec2(0.5, 0));
	pipeBot->setPosition(Vec2(origin.x+visibleSize.width, origin.y+visibleSize.height / 10));
	auto actionBotMoveTo= MoveTo::create(4, Vec2(-(visibleSize.width+origin.x)/2, origin.y+visibleSize.height / 10));

	pipeBot->runAction(actionBotMoveTo);


	//tao node kiem tra tinh diem

	auto node = Node::create();
	auto nodeBody = PhysicsBody::createBox(Size(1, visibleSize.height));
	nodeBody->setDynamic(false);
	nodeBody->setCollisionBitmask(3);
	nodeBody->setContactTestBitmask(true);
	node->setPhysicsBody(nodeBody);
	node->setAnchorPoint(Vec2(0.5, 0.5));
	float nodeX =origin.x+ visibleSize.width;
	float nodeY = origin.y+visibleSize.height / 2;
	node->setPosition(Vec2(nodeX, nodeY));
	auto nodeMove = MoveTo::create(5, Vec2(-nodeX, nodeY));
	node->runAction(nodeMove);
	addChild(node);
	addChild(pipeTop);
	addChild(pipeBot);

	
}
//////////////////////////

void GameStart::createBird(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/////////////////////
	spriteNode = SpriteBatchNode::create("dragon.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dragon.plist");
	bird = Sprite::createWithSpriteFrameName("dragon1.png");
	bird->setScale(visibleSize.height / (10 * bird->getContentSize().height));
	bird->setPosition(Vec2(origin.x+visibleSize.width / 2,origin.y+ 7 * visibleSize.height / 12));

	//tao vat ly cho chim
	auto birdBody = PhysicsBody::createBox(bird->getContentSize());
	birdBody->setDynamic(false);// bird co trong luc
	birdBody->setCollisionBitmask(1);
	birdBody->setContactTestBitmask(true);
	bird->setPhysicsBody(birdBody);
	spriteNode->addChild(bird);
	this->addChild(spriteNode);
	


	auto animate = Animate::create(GameStart::createAnimationBird("dragon", 8, 0.1));
	animate->retain();
	bird->runAction(RepeatForever::create(animate));//lap vo han
}
//tao wizard

void GameStart::createWizard(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/////////////////////
	spriteNodeWizard = SpriteBatchNode::create("wizard.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("wizard.plist");
	wizard = Sprite::createWithSpriteFrameName("wizard1.png");
	wizard->setScale(visibleSize.height / (12 * wizard->getContentSize().height));
	wizard->setPosition(Vec2( 4*visibleSize.width/5, origin.y + 7 * visibleSize.height / 12));


	/*auto wizardBody = PhysicsBody::createBox(bird->getContentSize());
	wizardBody->setDynamic(false);// bird co trong luc
	wizardBody->setCollisionBitmask(7);
	wizardBody->setContactTestBitmask(true);
	wizard->setPhysicsBody(wizardBody);
	spriteNode->addChild(wizard);
	this->addChild(spriteNodeWizard);*/
	this->addChild(wizard);


	auto animate = Animate::create(GameStart::createAnimationBird("wizard", 3, 0.15));
	animate->retain();
	wizard->runAction(RepeatForever::create(animate));//lap vo han
}



//////////////////////////
bool GameStart::onPhysicsContact(cocos2d::PhysicsContact &contact)//xu ly va cham vat ly
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto shapeA = contact.getShapeA()->getBody();
	auto shapeB = contact.getShapeB()->getBody();
	//kiem tra la va cham chim va ong khoi
	if ((shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 2)
		|| (shapeA->getCollisionBitmask() == 2 && shapeB->getCollisionBitmask() == 1)
		|| (shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 4)
		|| (shapeA->getCollisionBitmask() == 4 && shapeB->getCollisionBitmask() == 1)
		|| (shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 5)
		|| (shapeA->getCollisionBitmask() == 5 && shapeB->getCollisionBitmask() == 1))
	{
		//chuyen scene game sang gameEnd
	

		/////////////////////

		
		int tmp = score;
		score = 0;
		auto sqe = GameOver::createScene(tmp, id,name,true);
		Director::getInstance()-> replaceScene(sqe);

		return false;
	}
	//kiem tra la va cham chim va node
	else if ((shapeA->getCollisionBitmask() == 1 && shapeB->getCollisionBitmask() == 3)
		|| (shapeA->getCollisionBitmask() == 3 && shapeB->getCollisionBitmask() == 1))
	{

		score++;
		tempScore = __String::createWithFormat("%i", score);
		label->setString(tempScore->getCString());
		CCLOG("%d", score);
	}

	/*if ((shapeA->getCollisionBitmask() == 6 && shapeB->getCollisionBitmask() == 7) || (shapeA->getCollisionBitmask() == 7 && shapeB->getCollisionBitmask() == 6))
	{
		this->removeChild(spriteNodeWizard);
		addWizard = true;
	}*/
	return true;
}

/////////////////////////
void GameStart::update(float dt)//updat chim bay /roi
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (isFly == true)
	{
		auto birdMove = MoveBy::create(0.1, Vec2(0, visibleSize.height / 25));
		bird->runAction(birdMove);
	}
	else
	{
		auto birdMove = MoveBy::create(0.5, Vec2(0, -visibleSize.height / 50));
		bird->runAction(birdMove);
	}
}
////////////////////////
bool GameStart::onTouchBegan(Touch *touch, Event *unused_event)
{
	this->removeChild(attack);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 location = touch->getLocation();

	/*if (location.x > 0 && location.x <= (visibleSize.width / 7) && location.y > (2 * visibleSize.height / 5) && location.y < (visibleSize.width / 7 + 2 * visibleSize.height / 5))
	{
		addAtk = true;
		attack = Sprite::create("dragonatk.png");
		attack->setAnchorPoint(Vec2(0, 0.5));
		attack->setScaleX(visibleSize.width / (2 * attack->getContentSize().width));
		attack->setScaleY(visibleSize.height / (5 * attack->getContentSize().height));
		float x = bird->getPosition().x;
		float y = bird->getPosition().y;
		attack->setPosition(Vec2(x + visibleSize.height / 10, y));

		//set vat ly
		auto atkBody = PhysicsBody::createBox(bird->getContentSize());
		atkBody->setDynamic(false);// bird co trong luc
		atkBody->setCollisionBitmask(6);
		atkBody->setContactTestBitmask(true);
		attack->setPhysicsBody(atkBody);


		////////////////////////
		this->addChild(attack);
		
	}
	*/
	
	isFly = true;
	this->schedule(schedule_selector(GameStart::update), 0.05);
	return true;
}

void GameStart::onTouchMoved(Touch *touch, Event *unused_event)
{
	printf("adsasd");
}

void GameStart::onTouchEnded(Touch *touch, Event *unused_event)
{
	
	isFly = false;
	this->schedule(schedule_selector(GameStart::update), 0.05);
}

/*
void GameStart::menuAttackCallBack(cocos2d::Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	addAtk = true;
	attack = Sprite::create("dragonatk.png");
	attack->setAnchorPoint(Vec2(0, 0.5));
	attack->setScaleX(visibleSize.width / (2 * attack->getContentSize().width));
	attack->setScaleY(visibleSize.height / (5 * attack->getContentSize().height));
	float x = bird->getPosition().x;
	float y = bird->getPosition().y;
	attack->setPosition(Vec2(x+visibleSize.height/10, y));

	/*set vat ly
	auto atkBody = PhysicsBody::createBox(bird->getContentSize());
	atkBody->setDynamic(false);// bird co trong luc
	atkBody->setCollisionBitmask(6);
	atkBody->setContactTestBitmask(true);
	attack->setPhysicsBody(atkBody);


	////////////////////////
	this->addChild(attack);
	//this->schedule(schedule_selector(GameStart::removeAttack), 0.075);
}*/

//remove attck

/*void GameStart::removeAttack(float dt)
{
	if (addAtk == true)
	{
		this->removeChild(attack);
		addAtk = false;
	}
	
}*/