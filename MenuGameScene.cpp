#include "GameStartScene.h"
#include "HelloWorldScene.h"
#include "InsertNameScene.h"
#include "MenuGameScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif

USING_NS_CC;
static std::string id;
static std::string name;

Scene* MenuGame::createScene(std::string i, std::string n)
{
	
	id = i;
	CCLOG("id menugame: %s", id.c_str());
	name = n;
    return MenuGame::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MenuGameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
	
	//add background
	auto bgMenu = Sprite::create("bgmenu2.png");
	bgMenu->setScaleX(visibleSize.width / bgMenu->getContentSize().width);
	bgMenu->setScaleY(visibleSize.height / bgMenu->getContentSize().height);
	bgMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));
	bgMenu->setAnchorPoint(Vec2(0, 0));

	addChild(bgMenu);

	///////////////////
	//add hello player
	//hien thong bao dang nhap
	auto spriteNot = Sprite::create("not.png");
	spriteNot->setScaleX(2.5 * visibleSize.width / (5 * spriteNot->getContentSize().width));
	spriteNot->setScaleY(1 * visibleSize.height / (5 * spriteNot->getContentSize().height));
	spriteNot->setAnchorPoint(Vec2(0.5, 0));
	spriteNot->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height));
	spriteNot->setOpacity(200);
	auto moveTo1 = MoveTo::create(3, Vec2(origin.x + visibleSize.width / 2, origin.y + 4 * visibleSize.height / 5));

	auto moveTo = MoveTo::create(0.1, Vec2(origin.x + visibleSize.width / 2, 10* visibleSize.height  + origin.y));

	auto sq1 = Sequence::create(moveTo1, moveTo,nullptr);
	spriteNot->runAction(sq1);

	/////////////////////
	std::string str = "Welcom Back!, " + name;
	auto label = Label::createWithTTF(str.c_str(), "fonts/Marker Felt.ttf", 12);

	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen

		label->setScaleX(2.5 * visibleSize.width / (5 * spriteNot->getContentSize().width));
		label->setScaleY(1 * visibleSize.height / (6 * spriteNot->getContentSize().height));
		label->setAnchorPoint(Vec2(0.5, 0.5));
		label->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height + origin.y));
		label->setOpacity(200);
		label->setColor(cocos2d::Color3B::RED);
		auto moveTo2 = MoveTo::create(3, Vec2(origin.x + visibleSize.width / 2, 4.5*visibleSize.height / 5 + origin.y));
		auto sq2 = Sequence::create(moveTo2, moveTo,nullptr);
		label->runAction(sq2);


	}
	this->addChild(spriteNot);
	this->addChild(label);
	////////////////////
	//add play icon
	playItem = MenuItemImage::create(
		"play1.png",
		"play2.png",
		CC_CALLBACK_1(MenuGame::menuPlayCallback, this));

	if (playItem == nullptr ||
		playItem->getContentSize().width <= 0 ||
		playItem->getContentSize().height <= 0)
	{
		problemLoading("'play1.png' and 'play2.png'");
	}
	else
	{
		float x = origin.x+visibleSize.width / 2;
		float y =origin.y+ 11*visibleSize.height / 20;
		playItem->setPosition(Vec2(x, y));
		playItem->setAnchorPoint(Vec2(0.5, 0));
		playItem->setOpacity(180);
		playItem->setScaleX(visibleSize.width / (6 * playItem->getContentSize().width));//scale x bang 1/3 chieu rong man hinh
		playItem->setScaleY(visibleSize.height / (10 * playItem->getContentSize().height));////scale y bang 1/5 chieu cao man hinh
	}

	// create menu, it's an autorelease object
	menuPlay= Menu::create(playItem, NULL);
	menuPlay->setPosition(Vec2::ZERO);
	this->addChild(menuPlay, 1);

	//add collect
	collectItem = MenuItemImage::create(
		"collect1.png",
		"collect2.png",
		CC_CALLBACK_1(MenuGame::menuCollectCallback, this));

	if (collectItem == nullptr ||
		collectItem->getContentSize().width <= 0 ||
		collectItem->getContentSize().height <= 0)
	{
		problemLoading("'collect1.png' and 'collect2.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + 9 * visibleSize.height / 20;
		collectItem->setPosition(Vec2(x, y));
		collectItem->setPosition(Vec2(x, y));
		collectItem->setOpacity(180);
		collectItem->setScaleX(visibleSize.width / (6 * collectItem->getContentSize().width));//scale x bang 1/3 chieu rong man hinh
		collectItem->setScaleY(visibleSize.height / (10 * collectItem->getContentSize().height));////scale y bang 1/5 chieu cao man hinh
	}

	// create menu, it's an autorelease object
	menuCollect = Menu::create(collectItem, NULL);
	menuCollect->setPosition(Vec2::ZERO);
	this->addChild(menuCollect, 1);
	//add score icon
	hightScoreItem = MenuItemImage::create(
		"score1.png",
		"score2.png",
		CC_CALLBACK_1(MenuGame::menuHightScoreCallback, this));

	if (hightScoreItem == nullptr ||
		hightScoreItem->getContentSize().width <= 0 ||
		hightScoreItem->getContentSize().height <= 0)
	{
		problemLoading("'score1.png' and 'score2.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + 3 * visibleSize.height / 10;
		hightScoreItem->setPosition(Vec2(x, y));
		hightScoreItem->setPosition(Vec2(x, y));
		hightScoreItem->setOpacity(180);
		hightScoreItem->setScaleX(visibleSize.width / (6 * collectItem->getContentSize().width));//scale x bang 1/3 chieu rong man hinh
		hightScoreItem->setScaleY(visibleSize.height / (10 * collectItem->getContentSize().height));////scale y bang 1/5 chieu cao man hinh
	}

	// create menu, it's an autorelease object
	menuHightScore = Menu::create(hightScoreItem, NULL);
	menuHightScore->setPosition(Vec2::ZERO);
	this->addChild(menuHightScore, 1);

	//add quit icon
	quitItem = MenuItemImage::create(
		"quit1.png",
		"quit2.png",
		CC_CALLBACK_1(MenuGame::menuQuitCallback, this));

	if (quitItem == nullptr ||
		quitItem->getContentSize().width <= 0 ||
		quitItem->getContentSize().height <= 0)
	{
		problemLoading("'quit1.png' and 'quit2.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + 3* visibleSize.height / 20;
		quitItem->setPosition(Vec2(x, y));
		quitItem->setPosition(Vec2(x, y));
		quitItem->setOpacity(180);
		quitItem->setScaleX(visibleSize.width / (6 * quitItem->getContentSize().width));//scale x bang 1/3 chieu rong man hinh
		quitItem->setScaleY(visibleSize.height / (10 * quitItem->getContentSize().height));////scale y bang 1/5 chieu cao man hinh
	}

	// create menu, it's an autorelease object
	menuQuit = Menu::create(quitItem, NULL);
	menuQuit->setPosition(Vec2::ZERO);
	this->addChild(menuQuit, 1);
	

    return true;
}


void MenuGame::menuPlayCallback(cocos2d::Ref* pSender)
{
	this->removeAllChildren();
	auto sqe = GameStart::createScene(id, name);
	Director::getInstance()->replaceScene(sqe);

}
void MenuGame::menuCollectCallback(cocos2d::Ref* pSender)
{

}
void MenuGame::menuHightScoreCallback(cocos2d::Ref* pSender)
{
	this->removeAllChildren();
	auto sqe = GameOver::createScene(0, id, name,false);
	Director::getInstance()->replaceScene(sqe);
}
void MenuGame::menuQuitCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

}