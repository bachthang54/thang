#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameStartScene.h"
#include "InsertNameScene.h"
#include "MenuGameScene.h"
#include "GameOverScene.h"

#include "fstream"
#include "ui/CocosGUI.h"



#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	/////////

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	////
	/////////////////////////////
	//add background
	bgMenu = Sprite::create("bgmenu2.png");
	bgMenu->setScaleX(visibleSize.width / bgMenu->getContentSize().width);
	bgMenu->setScaleY(visibleSize.height / bgMenu->getContentSize().height);
	bgMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));
	bgMenu->setAnchorPoint(Vec2(0, 0));

	addChild(bgMenu);

	// add close item
	auto closeItem = MenuItemImage::create(
		"1.png",
		"2.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'1.png' and '2.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//add loginFacebook item
	loginFacebookItem = MenuItemImage::create(
		"loginfacebook.png",
		"loginfacebook.png",
		CC_CALLBACK_1(HelloWorld::menuLoginFacebookCallback, this));

	if (loginFacebookItem == nullptr ||
		loginFacebookItem->getContentSize().width <= 0 ||
		loginFacebookItem->getContentSize().height <= 0)
	{
		problemLoading("'loginfacebook.png' and 'loginfacebook.png'");
	}
	else
	{
		float x = visibleSize.width / 2;
		float y = visibleSize.height / 2;
		loginFacebookItem->setPosition(Vec2(x, y));
		loginFacebookItem->setScaleX(visibleSize.width / (3 * loginFacebookItem->getContentSize().width));//scale x bang 1/3 chieu rong man hinh
		loginFacebookItem->setScaleY(visibleSize.height / (5 * loginFacebookItem->getContentSize().height));////scale y bang 1/5 chieu cao man hinh
	}

	// create menu, it's an autorelease object
	menuLoginFacebook = Menu::create(loginFacebookItem, NULL);
	menuLoginFacebook->setPosition(Vec2::ZERO);
	this->addChild(menuLoginFacebook, 1);

	//tao co so du lieu

	char check[30];
	char* errMsg = NULL;
	std::string sqlstr;
	int result;

	std::string dbPath = FileUtils::getInstance()->getWritablePath() + "data.sql";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (!FileUtils::getInstance()->isFileExist(dbPath.c_str())) {
		Data data =
			FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->fullPathForFilename(path.c_str()));
		unsigned char* readData = data.getBytes();
		FILE* file = fopen(dbPath.c_str(), “wb”);
		fwrite(readData, data.getSize(), 1, file);
		fclose(file);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Data data1 =
		FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->fullPathForFilename(dbPath.c_str()));
	unsigned char* readData = data1.getBytes();
	if (!FileUtils::getInstance()->isFileExist(dbPath)) {
		std::ofstream outfile(dbPath.c_str(), std::ofstream::out);
		outfile.write((char*)&readData[0], data1.getSize());
		outfile.close();
	}
#endif
	result = sqlite3_open(dbPath.c_str(), &data);

	sqlite3_exec(data, "create table PLAYER(ID text primary key not null,NAME text not null,SCORE interger not null)", NULL, NULL, NULL);

	insertSql("12345", "ngoc", "5");
	insertSql("20133", "thang", "1000");
	insertSql("1997", "huy", "1");



	if (sqlite3_close(data) == SQLITE_OK) CCLOG("close ok 1");


	//cocos compile -s D:\cocos2dx\project\DemoFlappyBird -p android --android-studio
	return true;
}


//khi an nut dang nhap facebook ->goi dang nhap va xet database
void HelloWorld::menuLoginFacebookCallback(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	std::string dbPath = FileUtils::getInstance()->getWritablePath() + "data.sql";
	Player player = Player("", "", "0");


	//dang nhap vao facebook
#ifdef SDKBOX_ENABLED

	if (!sdkbox::PluginFacebook::isLoggedIn())
	{

		sdkbox::PluginFacebook::login();
	}

	player.setPlayerId(sdkbox::PluginFacebook::getUserID());



#endif
	sqlite3_stmt *stmt = nullptr;
	sqlite3 *database;
	bool checkLogin = false;
	bool repScene = false;
	if (sqlite3_open(dbPath.c_str(), &database) == SQLITE_OK&&strcmp("", player.getPlayerId().c_str()) != 0)
		//if (sqlite3_open(dbPath.c_str(), &database)==SQLITE_OK)
	{

		if (sqlite3_prepare_v2(database, "select * from PLAYER", -1, &stmt, nullptr) == SQLITE_OK)
		{

			sqlite3_step(stmt);
			checkLogin = false;

			while (1)

			{

				if (sqlite3_column_text(stmt, 0) == NULL)
				{
					sqlite3_step(stmt);
					continue;
				}
				//std::string sql = "update PLAYER set SCORE = 15 where ID = " + player.getPlayerId();
				//				sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
				std::string sbuffer = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
				CCLOG("%s", sbuffer.c_str());
				if (strcmp(sbuffer.c_str(), player.getPlayerId().c_str()) == 0)

				{
					CCLOG("co bang nhau");

					player.setPlayerName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));


					checkLogin = true;
					repScene = true;
					break;

				}


				if (sqlite3_step(stmt) == SQLITE_DONE)
				{
					repScene = true;
					break;//ket thuc duyet database
				}
			}


		}
		sqlite3_finalize(stmt);
	}


	else
	{
		loginFacebookItem->setOpacity(0);
		menuLoginFacebook->setEnabled(false);
		facebookErroItem = MenuItemImage::create(
			"erroLogin.png",
			"erroLogin.png",
			CC_CALLBACK_1(HelloWorld::menuLoginFacebookErro, this));

		if (facebookErroItem == nullptr ||
			facebookErroItem->getContentSize().width <= 0 ||
			facebookErroItem->getContentSize().height <= 0)
		{
			problemLoading("'erroLogin.png' and 'erroLogin.png'");
		}
		else
		{
			float x = visibleSize.width / 2;
			float y = visibleSize.height / 2;
			facebookErroItem->setPosition(Vec2(x, y));
			facebookErroItem->setOpacity(200);
			facebookErroItem->setScaleX(2.5*visibleSize.width / (5 * facebookErroItem->getContentSize().width));
			facebookErroItem->setScaleY(visibleSize.height / (4 * facebookErroItem->getContentSize().height));
		}

		menuLoginErro = Menu::create(facebookErroItem, NULL);
		menuLoginErro->setPosition(Vec2::ZERO);
		this->addChild(menuLoginErro, 1);


	}

	//chuyen man hinh
	if (checkLogin == true && repScene == true)
	{

		if (sqlite3_close(database) == SQLITE_OK) CCLOG("close ok 2");
		this->removeAllChildren();
		auto sqe = MenuGame::createScene(player.getPlayerId(), player.getPlayerName());
		Director::getInstance()->replaceScene(sqe);
	}
	//neu chua co du lieu ve nguoi dung login ->them du lieu
	if (checkLogin == false && repScene == true)

	{


		if (sqlite3_close(database) == SQLITE_OK) CCLOG("close ok 2");
		this->removeAllChildren();
		auto sqe = InsertName::createScene(player.getPlayerId(), player.getPlayerName());
		Director::getInstance()->replaceScene(sqe);

	}

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{

}

//rhem du lieu vao database
void HelloWorld::insertSql(std::string ID, std::string NAME, std::string SCORE)
{
	std::string sql = "INSERT INTO PLAYER VALUES('" + ID + "','" + NAME + "'," + SCORE + ")";
	CCLOG("%s", sql.c_str());

	//THEM DATA
	sqlite3_exec(data, sql.c_str(), NULL, NULL, NULL);
}

void HelloWorld::menuLoginFacebookErro(cocos2d::Ref* pSender)
{

	this->removeChild(menuLoginErro);
	loginFacebookItem->setOpacity(255);
	menuLoginFacebook->setEnabled(true);

}