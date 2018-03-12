#include "GameStartScene.h"

#include "InsertNameScene.h"
#include "MenuGameScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif

USING_NS_CC;
static int score;
static std::string buf[5][2];
static std::string name;
static std::string id;
bool checkUpdate=false;
bool checkScene;
Scene* GameOver::createScene(int s , std::string i,std::string n,bool check)
{
	score = s;

	id = i;

	checkScene = check;
	CCLOG("id overgame : %s", id.c_str());

	name = n;
    return GameOver::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameOverScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOver::init()
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
	auto bgMenu = Sprite::create("highscore.png");
	bgMenu->setScaleX(visibleSize.width / bgMenu->getContentSize().width);
	bgMenu->setScaleY(visibleSize.height / bgMenu->getContentSize().height);
	bgMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));
	bgMenu->setAnchorPoint(Vec2(0, 0));

	addChild(bgMenu);
	
	//add playagain icon and back icon

	auto playItem = MenuItemImage::create(
		"playAgain1.png",
		"playAgain2.png",
		CC_CALLBACK_1(GameOver::menuPlayAgainCallBack, this));

	if (playItem == nullptr ||
		playItem->getContentSize().width <= 0 ||
		playItem->getContentSize().height <= 0)
	{
		problemLoading("'playAgain1.png' and 'playAgain2.png'");
	}
	else
	{
		float x = origin.x+visibleSize.width/2 ;
		float y = origin.y + 17.5 * visibleSize.height / 20;
		playItem->setPosition(Vec2(x, y));
		playItem->setAnchorPoint(Vec2(0.5, 0));
		playItem->setOpacity(255);
		playItem->setScaleX(visibleSize.width / (6 * playItem->getContentSize().width));
		playItem->setScaleY(visibleSize.height / (10 * playItem->getContentSize().height));
	}

	// create menu, it's an autorelease object
	auto menuPlayAgain = Menu::create(playItem, NULL);
	menuPlayAgain->setPosition(Vec2::ZERO);
	

	//add collect
	auto backItem = MenuItemImage::create(
		"back1.png",
		"back2.png",
		CC_CALLBACK_1(GameOver::menuBackCallBack, this));

	if (backItem == nullptr ||
		backItem->getContentSize().width <= 0 ||
		backItem->getContentSize().height <= 0)
	{
		problemLoading("'back1.png' and 'back2.png'");
	}
	else
	{
		float x = origin.x ;
		float y = origin.y + 17.5* visibleSize.height / 20;
		backItem->setPosition(Vec2(x, y));
		backItem->setAnchorPoint(Vec2(0, 0));
		backItem->setOpacity(180);
		backItem->setScaleX(visibleSize.width / (6 * backItem->getContentSize().width));
		backItem->setScaleY(visibleSize.height / (10 * backItem->getContentSize().height));
	}

	// create menu, it's an autorelease object
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 1);

	//hien thi diem so nguoi choi neu chuyen tu man hinh gamestart toi
	if (checkScene == true)
	{
		std::string str = "Your score : " + StringUtils::toString(score);
		auto addScore = ui::Text::create(str, "arial.ttf", 16);
		addScore->setAnchorPoint(Vec2(0.5, 0));
		addScore->setColor(cocos2d::Color3B::RED);
		addScore->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 4 * visibleSize.height / 5));
		this->addChild(addScore);
		this->addChild(menuPlayAgain, 1);

	}


	//xu ly data

	updateSql(id, StringUtils::toString(score));
	loadData();

	/*if (score >= atoi(buf[4][1].c_str()))
	{
 
		buf[4][1] = StringUtils::toString(score);
		buf[4][0] = name;
		BubbleSort();
	}*/

	bool check;
	//hien thi bang highscore
	for (int i = 0; i < 5; i++)
	{
		check = false;
		if (strcmp(buf[i][0].c_str(), name.c_str()) == 0) check = true;
		highScore(i, check);
	}
	//////////////////
	


    return true;
}

//load data from database
void GameOver::loadData()
{
	std::string dbPath = FileUtils::getInstance()->getWritablePath() + "data.sql";
	sqlite3 *database;
	sqlite3_stmt *stmt = nullptr;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			buf[i][j] = "___";
		}
	}
	if (sqlite3_open(dbPath.c_str(), &database) == SQLITE_OK)

	{
		
		if (sqlite3_prepare_v2(database, "select * from PLAYER", -1, &stmt, nullptr) == SQLITE_OK)
		{

			sqlite3_step(stmt);
			
			for(int i=0;;i++)

			{


				if (sqlite3_column_text(stmt, 0) == NULL)
				{
					sqlite3_step(stmt);
					continue;
				}

				//kiem tra diem nguoi choi

				std::string bufScore= reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

				if (i < 5)
				{
					buf[i][0] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));//luu ten
					buf[i][1] = bufScore;//luu diem
					BubbleSort();
				}
				
				else
				{
					std::string tmpName;
	
					tmpName= reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
					if (atoi(bufScore.c_str()) >= atoi(buf[4][1].c_str()))
					{
						buf[4][1] = bufScore;
						buf[4][0] = tmpName;
						BubbleSort();
					}
				}
				
				///////////////
				if (sqlite3_step(stmt) == SQLITE_DONE) break;//ket thuc duyet database

			}

			
	
			
		}
		sqlite3_finalize(stmt);
	}

	if (sqlite3_close(database) == SQLITE_OK) CCLOG("close ok");

	
	
}
//sap xep mang luu diem theo nho dan
void GameOver::BubbleSort()
{
	bool swapped;
	for (int i = 1; i <= 5; i++)
	{
		swapped = false;

		for (int j = 1; j < 5; j++)
		{

			if (buf[j][1].c_str() == NULL) break;
			else if (atoi(buf[j - 1][1].c_str())< atoi(buf[j][1].c_str()))
			{
				swap(buf[j - 1][1], buf[j][1]);
				swap(buf[j - 1][0], buf[j][0]);
			}
			

		}
	}
	
	
	
}

void GameOver::highScore(int i,bool check)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto textName = ui::Text::create(buf[i][0], "arial.ttf", 20);
	float x = origin.x + visibleSize.width / 3;
	float y;
	switch (i)
	{
		case 0: 
		{
			y = origin.y + 11 * visibleSize.height / 20;
			break;
		}
		case 1:
		{
			y = origin.y + 9 * visibleSize.height / 20;
			break;
		}
		case 2:
		{
			y = origin.y + 7 * visibleSize.height / 20;
			break;
		}
		case 3:
		{
			y = origin.y + 5 * visibleSize.height / 20;
			break;
		}
		case 4:
		{
			y = origin.y + 3 * visibleSize.height / 20;
			break;
		}
		default: break;
	}
	textName->setPosition(Vec2(x, y));
	textName->setAnchorPoint(Vec2(0, 0));

	auto poiter = Sprite::create("poiter.png");
	poiter->setAnchorPoint(Vec2(0, 0));
	poiter->setPosition(Vec2(x - 100, y));
	poiter->setScale(visibleSize.width / (17*poiter->getContentSize().width));

	auto textScore = ui::Text::create(buf[i][1], "arial.ttf", 20);
	textScore->setPosition(Vec2(2 * visibleSize.width / 3, y));

	if (check == true)
	{
		textScore->setColor(cocos2d::Color3B::RED);
		textName->setColor(cocos2d::Color3B::RED);
		this->addChild(poiter);
	}
	else {
		textScore->setColor(cocos2d::Color3B::BLACK);
		textName->setColor(cocos2d::Color3B::BLACK);
	}
	textScore->setAnchorPoint(Vec2(0, 0));
	//add
	this->addChild(textName);
	this->addChild(textScore);
}

//update database

void GameOver::updateSql(std::string ID ,std::string SCORE)
{
	std::string dbPath = FileUtils::getInstance()->getWritablePath() + "data.sql";
	sqlite3 *database;
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_open(dbPath.c_str(), &database) == SQLITE_OK)
	{

		if (sqlite3_prepare_v2(database, "select * from PLAYER", -1, &stmt, nullptr) == SQLITE_OK)
		{

			sqlite3_step(stmt);

			for (int i = 0;; i++)

			{


				if (sqlite3_column_text(stmt, 0) == NULL)
				{
					sqlite3_step(stmt);
					continue;
				}

				//kiem tra diem nguoi choi
				std::string bufId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
				std::string bufScore = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
				if (strcmp(id.c_str(), bufId.c_str()) == 0 && score>atoi(bufScore.c_str()))
				{
					std::string sql = "update PLAYER set SCORE = " + SCORE + " where ID = " + ID;
					CCLOG("%s", sql.c_str());
					char *a = 0;
					//THEM DATA
					if (sqlite3_exec(database, sql.c_str(), NULL, NULL, &a) != SQLITE_OK)
					{
						CCLOG("%s", a);
						
					}
					break;
				}
				
				///////////////
				if (sqlite3_step(stmt) == SQLITE_DONE) break;//ket thuc duyet database

			}




		}
		sqlite3_finalize(stmt);
	}

	if (sqlite3_close(database) == SQLITE_OK) CCLOG("da dong database roi");
	/////////

}

void GameOver::menuBackCallBack(cocos2d::Ref* pSender)
{
	this->removeAllChildren();
	auto sqe = MenuGame::createScene(id, name);
	Director::getInstance()->replaceScene(sqe);
}

void GameOver::menuPlayAgainCallBack(cocos2d::Ref* pSender)
{
	this->removeAllChildren();
	auto sqe = GameStart::createScene(id, name);
	Director::getInstance()->replaceScene(sqe);
}

