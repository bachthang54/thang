#include "InsertNameScene.h"
#include "HelloWorldScene.h"
#include "MenuGameScene.h"
#include "GameStartScene.h"
#include "SimpleAudioEngine.h"
#include "sqlite3.h"
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif

USING_NS_CC;
static std::string id;
static std::string name;
Scene* InsertName::createScene(std::string playerId, std::string na)

{
	name = na;
	id = playerId;
	CCLOG("id insername : %s", id.c_str());
    return InsertName::create();
	
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in InsertNameScene.cpp\n");
}

// on "init" you need to initialize your instance
bool InsertName::init()
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
	player.setPlayerId(id);
	//add background
	auto bgMenu = Sprite::create("bgmenu2.png");
	bgMenu->setScaleX(visibleSize.width / bgMenu->getContentSize().width);
	bgMenu->setScaleY(visibleSize.height / bgMenu->getContentSize().height);
	bgMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));
	bgMenu->setAnchorPoint(Vec2(0, 0));

	addChild(bgMenu);

	//////////////

	enterName();

    return true;
}


//nhan ten nguoi choi nhap tu textfield
bool InsertName::enterName()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	std::string dbPath = FileUtils::getInstance()->getWritablePath() + "data.sql";
	sqlite3_stmt *stmt = nullptr;
	std::string check;

	this->removeChild(sprite);
	this->removeChild(sprite1);

	if (sqlite3_open(dbPath.c_str(), &database) == SQLITE_OK)
	{
		
		if (sqlite3_prepare_v2(database, "select * from PLAYER", -1, &stmt, nullptr) == SQLITE_OK)
		{
			//creat menuitem ok

			auto okItem = MenuItemImage::create(
				"okItem.png",
				"okItem.png",
				CC_CALLBACK_1(InsertName::menuOkItemCallback, this));

			if (okItem == nullptr ||
				okItem->getContentSize().width <= 0 ||
				okItem->getContentSize().height <= 0)
			{
				problemLoading("'okItem.png' and 'okItem.png'");
			}
			else
			{
				float x = origin.x + visibleSize.width / 2;
				float y = origin.y + visibleSize.height / 3;
				okItem->setAnchorPoint(Vec2(0.5, 0));
				okItem->setPosition(Vec2(x, y));
				okItem->setOpacity(200);

				okItem->setScaleX(visibleSize.width / (10 * okItem->getContentSize().width));
				okItem->setScaleY(visibleSize.height / (7 * okItem->getContentSize().height));
			}

			// create menu, it's an autorelease object
			menuOkItem = Menu::create(okItem, NULL);
			menuOkItem->setEnabled(true);
			menuOkItem->setPosition(Vec2::ZERO);



			//InsertName nhap ten nguoi choi
			spriteText = Sprite::create("textfied.png");
			spriteText->setScaleX(3 * visibleSize.width / (5 * spriteText->getContentSize().width));
			spriteText->setScaleY(visibleSize.height / (3 * spriteText->getContentSize().height));
			spriteText->setAnchorPoint(Vec2(0.5, 0.5));
			spriteText->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
			spriteText->setOpacity(200);

			

			///////////////
			

			textField = ui::TextField::create("Enter your name", "Arial", 18);
			textField->setOpacity(200);
			textField->setAnchorPoint(Vec2(0.5, 0));
			textField->setColor(cocos2d::Color3B::BLACK);
			textField->setPosition(Vec2(visibleSize.width / 2+origin.x, origin.y+visibleSize.height / 2));
			textField->setEnabled(true);

		

			///////////
			
			////////////////////
			textField->addEventListener(CC_CALLBACK_2(InsertName::textFunc,this));

			///////////////////////////
			//////////////////
			sqlite3_step(stmt);

			while (1)
			{
				std::string buf;

				if (sqlite3_column_text(stmt, 1) == NULL)
				{
					continue;
				}
				else
				{
					buf = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

					if (strcmp(buf.c_str(), tmp.c_str()) == 0)
					{

						return false;
						break;
					}

				}
			if (sqlite3_step(stmt) == SQLITE_DONE) break;//ket thuc duyet database

			}
			this->addChild(spriteText);
			this->addChild(textField);
			this->addChild(menuOkItem);

			return true;
		}
		sqlite3_finalize(stmt);
	}
	sqlite3_close(database);

}

//kiem tra ten hop le va them vao datatbase
void InsertName::menuOkItemCallback(cocos2d::Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (enterName() == false)
	{
		menuOkItem->setEnabled(false);
		textField->setEnabled(false);
		sprite = Sprite::create("erroName.png");

		sprite->setScaleX(visibleSize.width / (2 * sprite->getContentSize().width));
		sprite->setScaleY(visibleSize.height / (6 * sprite->getContentSize().height));
		sprite->setAnchorPoint(Vec2(0.5, 0));
		sprite->setPosition(Vec2(origin.x + visibleSize.width / 2, 5 * visibleSize.height / 6 + origin.y));
		sprite->setOpacity(200);


		this->addChild(sprite);
		////////////
		auto touchListener = EventListenerTouchOneByOne::create();

		touchListener->onTouchBegan = CC_CALLBACK_2(InsertName::onTouchBegan, this);

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	

	}


	else if (checkName(tmp) == false)
	{
		sprite1 = Sprite::create("erroChar.png");
		
		sprite1->setScaleX(  visibleSize.width / (2 * sprite1->getContentSize().width));
		sprite1->setScaleY(visibleSize.height / (6 * sprite1->getContentSize().height));
		sprite1->setAnchorPoint(Vec2(0.5, 0));
		sprite1->setPosition(Vec2(origin.x + visibleSize.width / 2, 5*visibleSize.height/6 + origin.y));
		sprite1->setOpacity(200);

		this->addChild(sprite1);


		auto touchListener = EventListenerTouchOneByOne::create();

		touchListener->onTouchBegan = CC_CALLBACK_2(InsertName::onTouchBegan, this);

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	}

	else
	{
		player.setPlayerName(tmp);
		InsertName::insertSql(id, player.getPlayerName(), player.getPlayerScore());
		this->removeAllChildren();
		auto sqe = MenuGame::createScene(id,player.getPlayerName());
		Director::getInstance()->replaceScene(sqe);
	}
	
}
//khi nguoi choi cham man hinh
bool InsertName::onTouchBegan(Touch *touch, Event *unused_event)
{

	enterName();
	return true;
}


//them data vao database
void InsertName::insertSql(std::string ID, std::string NAME, std::string SCORE)
{
	std::string sql = "INSERT INTO PLAYER VALUES('" + ID + "','" + NAME + "'," + SCORE + ")";
	CCLOG("%s", sql.c_str());

	//THEM DATA
	sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);

	CCLOG("%d", sqlite3_exec(database, "select count(ID) from PLAYER", NULL, NULL, NULL));
}

//call fun textfield
void InsertName::textFunc(Ref* sender, ui::TextField::EventType type)
{
	auto text = dynamic_cast<ui::TextField*>(sender);
	switch (type)
	{
	case ui::TextField::EventType::ATTACH_WITH_IME:

		CCLOG("displayed keyboard");
		break;
	case ui::TextField::EventType::DETACH_WITH_IME:
		CCLOG("dismissed keyboard");
		break;
	case ui::TextField::EventType::INSERT_TEXT:
		CCLOG("inserted text : %s", text->getString().c_str());
		tmp = text->getString().c_str();
			break;
	case ui::TextField::EventType::DELETE_BACKWARD:
		CCLOG("deleted backward");
		break;
	default:    break;
	}
}


bool InsertName::checkName(std::string name)
{
	const char*a;
	a = name.c_str();
	for (int i = 0; i <= strlen(a);i++)
	{
		if (strlen(a) >= 4 && ((a[i] == 32 || (a[i] > 47 && a[i] < 58)) || (a[i] > 64 && a[i] < 91) || (a[i] > 96 && a[i] < 123)))
		{
			return true;
		}
		else {
			return false;
			break;
		}
	}

}


