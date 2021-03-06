#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "io.h"
#include <string>

using namespace CocosDenshion;

#define BG_MUSIC "music/background.mp3"
#define EFFECT_MUSIC "music/electroswitch.mp3"

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}

bool MainMenu::init()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BG_MUSIC);
	playBackgroundMusic(BG_MUSIC);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_MUSIC);
	if (!Layer::init())
	{
		return false;
	}
	for (auto i : menuOpen)
	{
		i = false;
	}
	auto mDirector = Director::getInstance();
	auto mWinHeight = mDirector->getWinSize().height;
	auto mWinWidth = mDirector->getWinSize().width;

	//
	mMiniMapLayer = MiniMapLayer::create();
	mMiniMapLayer->setPointSize(8);
	mMiniMapLayer->setMapSize(22, 22);
	mMiniMapLayer->setShape(1);
	mMiniMapLayer->setDragFrameInvisible(false);
	mMiniMapLayer->setPosition(Vec2(250,0));
	mMiniMapLayer->setVisible(false);
	addChild(mMiniMapLayer, 5);

	//global
	menuOpen[0] = true;
	userDefault = UserDefault::getInstance();
	dictionary = Dictionary::createWithContentsOfFile(std::string{ "dictionary/" + userDefault->getStringForKey("language") + ".xml" }.c_str());
	dictionary->retain();
	//title
	mTitle = Label::createWithTTF(getDicValue("Befall"), "fonts/STXIHEI.TTF", 120);
	mTitle->setPosition(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height - mTitle->getContentSize().height * 3 / 4);
	mTitle->setColor(Color3B(0,0,0));
	mTitle->setOpacity(0);
	addChild(mTitle);
	mTitle->runAction(EaseExponentialInOut::create(FadeIn::create(1)));
	//version
	mVersionLabel = Label::createWithTTF(getDicValue("Version") + getDicValue("VersionNumber"), "fonts/STXIHEI.TTF", 16);
	mVersionLabel->setPosition(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 8);
	mVersionLabel->setColor(Color3B(100,100,100));
	mVersionLabel->setOpacity(0);
	addChild(mVersionLabel);
	mVersionLabel->runAction(EaseExponentialInOut::create(FadeIn::create(1)));
	//菜单共两级，根菜单叫做root，以root中的选项命名二级菜单
	//root
	auto startLabel = createMenuLabel("start");
	auto start = MenuItemLabel::create(startLabel, CC_CALLBACK_1(MainMenu::enterCallback, this, "root", "start", 0));
	items["start"] = start;
	auto settingLabel = createMenuLabel("setting");
	auto setting = MenuItemLabel::create(settingLabel, CC_CALLBACK_1(MainMenu::enterCallback, this, "root", "setting", 0));
	items["setting"] = setting;

	auto exitLabel = createMenuLabel("exit");
	auto exit = MenuItemLabel::create(exitLabel, CC_CALLBACK_1(MainMenu::enterCallback, this,"root", "exit", 0));
	items["exit"] = exit;
	rootMenu = Menu::create(start, setting, exit, NULL);
	menuMap["root"] = rootMenu;
	rootMenu->alignItemsVerticallyWithPadding(10);
	rootMenu->setOpacity(0);
	//rootMenu->setOpacity(255);
	//start Menu
	auto storyLabel = createMenuLabel("story");
	auto story = MenuItemLabel::create(storyLabel, CC_CALLBACK_0(MainMenu::enterGameCallback, this, GameModeEnum::story));
	items["story"] = story;
	closeModeSet.insert(GameModeEnum::story);
	auto vsAiLabel = createMenuLabel("vsAi");
	auto vsAi = MenuItemLabel::create(vsAiLabel, CC_CALLBACK_0(MainMenu::enterGameCallback, this, GameModeEnum::vsAi));
	items["vsAi"] = vsAi;
	closeModeSet.insert(GameModeEnum::vsAi);
	//auto vsPlayerLabel = createMenuLabel("vsPlayer");
	//auto vsPlayer = MenuItemLabel::create(vsPlayerLabel, CC_CALLBACK_0(MainMenu::enterGameCallback, this, GameModeEnum::vsPlayer));
	//items["vsPlayer"] = vsPlayer;

	auto vsPlayerLabel = createMenuLabel("vsPlayer");
	auto vsPlayer = MenuItemLabel::create(vsPlayerLabel, CC_CALLBACK_1(MainMenu::enterCallback, this, "start", "mapName", 1));
	items["vsPlayer"] = vsPlayer;

	auto vsNetLabel = createMenuLabel("vsNet");
	auto vsNet = MenuItemLabel::create(vsNetLabel, CC_CALLBACK_1(MainMenu::enterCallback, this, "start", "net", 1));
	items["vsNet"] = vsNet;

	auto backLabel = createMenuLabel("back");
	auto back = MenuItemLabel::create(backLabel, CC_CALLBACK_1(MainMenu::backCallback,this, "start", "root", 1));
	items["back"] = back;

	startMenu = Menu::create(story, vsAi, vsPlayer, vsNet, back, NULL);
	startMenu->alignItemsVerticallyWithPadding(10);
	startMenu->setVisible(false);
	menuMap["start"] = startMenu;
	addChild(startMenu);
	//setting Menu
	char * str;
	if (userDefault->getBoolForKey("musicOn"))
	{
		str = "musicOn";
	}
	else
	{
		str = "musicOff";
	}
	auto musicLabel = createMenuLabel(str);
	auto music = MenuItemLabel::create(musicLabel, CC_CALLBACK_1(MainMenu::settingCallback,this, "music"));
	items["music"] = music;
	char * str2;
	if (userDefault->getBoolForKey("seOn"))
	{
		str2 = "seOn";
	}
	else
	{
		str2 = "seOff";
	}
	auto soundEffectLabel = createMenuLabel(str2);
	auto soundEffect = MenuItemLabel::create(soundEffectLabel, CC_CALLBACK_1(MainMenu::settingCallback,this, "se"));
	items["se"] = soundEffect;

	char * str3;
	if (userDefault->getBoolForKey("mistOn"))
	{
		str3 = "mistOn";
	}
	else
	{
		str3 = "mistOff";
	}
	auto MistLabel = createMenuLabel(str3);
	auto Mist = MenuItemLabel::create(MistLabel, CC_CALLBACK_1(MainMenu::settingCallback, this, "mist"));
	items["mist"] = Mist;

	char * str4;
	std::string themeStr = userDefault->getStringForKey("theme");
	if (themeStr == "grey")
	{
		str4 = "themeGrey";
	}
	else if (themeStr == "pink")
	{
		str4 = "themePink";
	}
	auto ThemeLabel = createMenuLabel(str4);
	auto Theme = MenuItemLabel::create(ThemeLabel, CC_CALLBACK_1(MainMenu::settingCallback, this, "theme"));
	items["theme"] = Theme;

	auto backLabelInSetting = createMenuLabel("back");
	auto backInSetting = MenuItemLabel::create(backLabelInSetting, CC_CALLBACK_1(MainMenu::backCallback,this, "setting", "root", 1));
	items["backInSetting"] = backInSetting;

	settingMenu = Menu::create(music, soundEffect, Mist, Theme ,backInSetting, NULL);
	settingMenu->alignItemsVerticallyWithPadding(10);
	settingMenu->setVisible(false);
	menuMap["setting"] = settingMenu;
	addChild(settingMenu);

	//exit Menu
	auto ensureLabel = createMenuLabel("ensureExit");
	auto ensure = MenuItemLabel::create(ensureLabel, CC_CALLBACK_1(MainMenu::exitCallback,this));
	items["ensure"] = ensure;
	auto backLabelInExit = createMenuLabel("back");
	auto backInExit= MenuItemLabel::create(backLabelInExit, CC_CALLBACK_1(MainMenu::backCallback,this,"exit", "root", 1));
	items["backInExit"] = backInExit;
	exitMenu = Menu::create(ensure, backInExit, NULL);
	exitMenu->alignItemsVerticallyWithPadding(10);
	exitMenu->setVisible(false);
	menuMap["exit"] = exitMenu;
	addChild(exitMenu);

	//map choose menu
	_finddata_t FileInfo;
	string strfind = "map\\*.*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);
	if (Handle == -1L)
	{
		CCLOG("can not match the folder");
	}
	do{
		if (FileInfo.attrib & _A_SUBDIR)
		{
			//directory
			//if( (strcmp(FileInfo.name, ".") != 0) 
			//	&& (strcmp(FileInfo.name, "..") != 0))
			//{}
		}
		else
		{
			std::string newFile;
			for (char * tempC = FileInfo.name; *tempC; tempC++)
			{
				CCLOG("%c", *tempC);
				newFile.push_back(*tempC);
			}

			if (newFile.size() >= 4)
			{
				if (newFile[newFile.size() - 1] == 'm'
					&& newFile[newFile.size() - 2] == 'f'
					&& newFile[newFile.size() - 3] == 'b'
					&& newFile[newFile.size() - 4] == '.')
				{
					mapNameArray.push_back(newFile);
				}
			}
		}
	} while (_findnext(Handle, &FileInfo) == 0);
	_findclose(Handle);

	mapMenu = Menu::create();
	for (int i = 0; i < mapNameArray.size(); i++)
	{
		string mapName = mapNameArray[i];
		string mapNameCut = mapName;
		mapNameCut.pop_back();
		mapNameCut.pop_back();
		mapNameCut.pop_back();
		mapNameCut.pop_back();
		auto mapLabel = createMenuLabel((char *)mapNameCut.c_str());
		auto map = MenuItemLabel::create(mapLabel, CC_CALLBACK_1(MainMenu::displayMapCallBack, this, mapName));
		items[mapName] = map;
		mapMenu->addChild(map);
	}
	auto backInMapLabel = createMenuLabel("cancel");
	auto backInMap = MenuItemLabel::create(backInMapLabel, CC_CALLBACK_1(MainMenu::backCallback, this, "mapName", "start", 2));
	items["backInMenu"] = backInMap;
	mapMenu->addChild(backInMap);

	auto confirmLabel = createMenuLabel("ensureExit");
	confirm = MenuItemLabel::create(confirmLabel, CC_CALLBACK_0(MainMenu::enterGameCallback, this, GameModeEnum::vsPlayer));
	items["confirm"] = confirm;
	confirm->setVisible(false);
	mapMenu->addChild(confirm);
	
	mapMenu->alignItemsVerticallyWithPadding(10);
	mapMenu->setVisible(false);
	menuMap["mapName"] = mapMenu;
	addChild(mapMenu);

	//net Menu
	auto serverLabel = createMenuLabel("server");
	auto server = MenuItemLabel::create(serverLabel, CC_CALLBACK_1(MainMenu::displayEditBoxCallback,this,"server"));
	items["server"] = server;
	auto clientlabel = createMenuLabel("client");
	auto client= MenuItemLabel::create(clientlabel, CC_CALLBACK_1(MainMenu::displayEditBoxCallback,this,"client"));
	items["client"] = client;
	auto backInNetLabel = createMenuLabel("back");
	auto backInNet = MenuItemLabel::create(backInNetLabel, CC_CALLBACK_1(MainMenu::backCallback,this,"net", "start", 2));
	items["backInNet"] = backInNet;
	netMenu = Menu::create(server, client/*, backInNet*/, NULL);
	netMenu->addChild(backInNet);
	netMenu->alignItemsVerticallyWithPadding(10);
	netMenu->setVisible(false);
	menuMap["net"] = netMenu;
	addChild(netMenu);
	//server
	editBoxInServer = createMenuLabel("inputPort");
	editBoxInServer->setPosition(
		Vec2(
		Director::getInstance()->getWinSize().width / 2,
		Director::getInstance()->getWinSize().height / 2 + editBoxInServer->getContentSize().height + 10
		)
		);
	editBoxInServer->setColor(Color3B(0,0,0));
	auto rectInServer = DrawNode::create();
	float halfWidthRectS = 100;
	rectInServer->drawRect(
		Vec2(Director::getInstance()->getWinSize().width / 2 - halfWidthRectS, editBoxInServer->getBoundingBox().getMinY()),
		Vec2(Director::getInstance()->getWinSize().width / 2 + halfWidthRectS, editBoxInServer->getBoundingBox().getMaxY()),
		Color4F(0.5,0.5,0.5,1)
		);
	auto cancelInServerLabel = createMenuLabel("cancel");
	auto cancelInServer = MenuItemLabel::create(cancelInServerLabel, CC_CALLBACK_1(MainMenu::cancelCallback, this, "server"));
	items["cancelInServer"] = cancelInServer;
	auto hostLabel= createMenuLabel("host");
	auto host = MenuItemLabel::create(hostLabel, CC_CALLBACK_0(MainMenu::enterGameCallback, this, GameModeEnum::server));
	items["host"] = host;
	auto serverMenu = Menu::create(host, cancelInServer, NULL);
	serverMenu->alignItemsHorizontallyWithPadding(30);
	serverLayer = Layer::create();
	serverLayer->addChild(rectInServer);
	serverLayer->addChild(editBoxInServer);
	serverLayer->addChild(serverMenu);
	serverLayer->setVisible(false);
	addChild(serverLayer);
	//client
	editBoxInClient = createMenuLabel("inputIp");
	editBoxInClient->setPosition(
		Vec2(
		Director::getInstance()->getWinSize().width / 2,
		Director::getInstance()->getWinSize().height / 2 + editBoxInClient->getContentSize().height + 10
		)
		);
	editBoxInClient->setColor(Color3B(0,0,0));
	auto rectInClient= DrawNode::create();
	float halfWidthRectC = 200;
	rectInClient->drawRect(
		Vec2(Director::getInstance()->getWinSize().width / 2 - halfWidthRectC, editBoxInClient->getBoundingBox().getMinY()),
		Vec2(Director::getInstance()->getWinSize().width / 2 + halfWidthRectC, editBoxInClient->getBoundingBox().getMaxY()),
		Color4F(0.5,0.5,0.5,1)
		);
	auto cancelInClientLabel = createMenuLabel("cancel");
	auto cancelInClient = MenuItemLabel::create(cancelInClientLabel, CC_CALLBACK_1(MainMenu::cancelCallback, this, "client"));
	items["cancelInClient"] = cancelInClient;
	auto connectLabel = createMenuLabel("connect");
	auto connect = MenuItemLabel::create(connectLabel, CC_CALLBACK_0(MainMenu::enterGameCallback, this, GameModeEnum::client));
	items["connect"] = connect;
	auto clientMenu = Menu::create(connect, cancelInClient, NULL);
	clientMenu->alignItemsHorizontallyWithPadding(30);
	clientLayer= Layer::create();
	clientLayer->addChild(rectInClient);
	clientLayer->addChild(editBoxInClient);
	clientLayer->addChild(clientMenu);
	clientLayer->setVisible(false);
	addChild(clientLayer);

	//setfonts
	for (auto i : items)
	{
		if ( i.first == "vsAi" || i.first == "story" )
		{
			i.second->setColor(Color3B(100, 100, 100));
		}
		else
		{
			i.second->setColor(Color3B(0, 0, 0));
		}
	}
	for (auto i : menuMap)
	{
		if (i.first != std::string{ "root" })
		{
			i.second->setPosition(i.second->getPosition().x + 100, i.second->getPosition().y);
		}
	}
	//addchildren
	addChild(rootMenu);
	//fade in effect
	auto menuFadeIn = FadeIn::create(1);
	rootMenu->runAction(EaseExponentialInOut::create(menuFadeIn));
	//listener
	mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(MainMenu::onMouseMove, this);
	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MainMenu::onKeyReleased, this);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	return true;
}

void MainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (serverLayer->isVisible())
	{
		std::string nowStr = editBoxInServer->getString();
        if (nowStr == std::string{getDicValue("inputPort")} )
        {
            nowStr = "";
        }
		if ((keyCode >= EventKeyboard::KeyCode::KEY_0) && (keyCode <= EventKeyboard::KeyCode::KEY_9))
		{
			if (nowStr.length() < 6)
				nowStr += '0' + static_cast<int>(keyCode)-static_cast<int>(EventKeyboard::KeyCode::KEY_0);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
		{
			if (nowStr.length() > 0)
			{
				nowStr.pop_back();
			}
			if (nowStr == std::string{""})
            {
                nowStr = getDicValue("inputPort");
            }
		}
		if (keyCode == static_cast<EventKeyboard::KeyCode>(35) )
		//enter
		{
			//CCLOG("enter pressed %d", keyCode);
			enterGameCallback(GameModeEnum::server);
		}
		editBoxInServer->setString(nowStr.c_str());
	}
	if (clientLayer->isVisible())
	{
		std::string nowStr = editBoxInClient->getString();
        if (nowStr == std::string{getDicValue("inputIp")} )
        {
            nowStr = "";
        }
		if ((keyCode >= EventKeyboard::KeyCode::KEY_0) && (keyCode <= EventKeyboard::KeyCode::KEY_9))
		{
			if (nowStr.length() < 22)
				nowStr += '0' + static_cast<int>(keyCode)-static_cast<int>(EventKeyboard::KeyCode::KEY_0);
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_SEMICOLON)
		{
			if (nowStr.length() < 22)
				nowStr += ':';
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_PERIOD)
		{
			if (nowStr.length() < 22)
				nowStr += '.';
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
		{
			if (nowStr.length() > 0)
			{
				nowStr.pop_back();
			}
            if (nowStr == std::string{""})
            {
                nowStr = getDicValue("inputIp");
            }
		}
		if (keyCode == static_cast<EventKeyboard::KeyCode>(35) )
		{
			//CCLOG("enter pressed: %d",keyCode);
			enterGameCallback(GameModeEnum::client);
		}
		editBoxInClient->setString(nowStr.c_str());
	}
}

void MainMenu::enterCallback(Ref * sender, const std::string & thisMenu, const std::string & rightMenu, int nowDeep)
{
	playEffect(EFFECT_MUSIC);
	//CCLOG("entercallback called");
	if (menuOpen[nowDeep])
	{
		menuOpen[nowDeep]= false;
		auto mainMenuFadeOut = FadeOut::create(0.3);
		auto hide = Hide::create();
		auto moveLeft = MoveBy::create(0.3, Vec2(-100, 0));
		auto fadeOutSpawn = Spawn::create(mainMenuFadeOut, moveLeft, NULL);
		auto fadeOutSequence = Sequence::create(fadeOutSpawn, hide, NULL);
		menuMap[thisMenu]->runAction(EaseExponentialInOut::create(fadeOutSequence));
		//CCLOG("clicked!");
		//2rd menu
		Menu * menu;
		menu = menuMap[rightMenu];
		menu->setVisible(true);
		menu->setOpacity(0);
		auto fadeIn = FadeIn::create(0.5);
		auto moveLeft1 = MoveBy::create(0.3, Vec2(-100, 0));
		auto fadeInSpawn = Spawn::create(fadeIn, moveLeft1, NULL);
		menu->runAction(EaseExponentialInOut::create(fadeInSpawn));
		menuOpen[nowDeep + 1] = true;
	}
}

void MainMenu::backCallback(Ref * sender, const std::string & thisMenu, const std::string & leftMenu, int nowDeep)
{
	playEffect(EFFECT_MUSIC);
	CCASSERT(nowDeep > 0, "nowDeep < 1");
	if (menuOpen[nowDeep])
	{
		menuOpen[nowDeep] = false;
		Menu * menu;
		menu = menuMap[thisMenu];
		auto moveRight = MoveBy::create(0.3, Vec2(100, 0));
		auto mainMenuFadeOut = FadeOut::create(0.3);
		auto fadeOutSpawn = Spawn::create(mainMenuFadeOut, moveRight, NULL);
		auto hide = Hide::create();
		auto fadeOutSequence = Sequence::create(fadeOutSpawn, hide, NULL);
		menu->runAction(EaseExponentialInOut::create(fadeOutSequence));
		menuMap[leftMenu]->setVisible(true);
		menuMap[leftMenu]->setOpacity(0);
		auto fadeIn = FadeIn::create(0.5);
		auto moveRight1 = MoveBy::create(0.3, Vec2(100, 0));
		auto fadeInSpawn = Spawn::create(fadeIn, moveRight1, NULL);
		menuMap[leftMenu]->runAction(EaseExponentialInOut::create(fadeInSpawn));
		menuMap[leftMenu]->setOpacity(255);
		menuMap[leftMenu]->setVisible(true);
		menuOpen[nowDeep - 1] = true;
	}
	if (thisMenu == "mapName")
	{
		mMiniMapLayer->setVisible(false);
		//mapNameLabel->setVisible(false);
		confirm->setVisible(false);
	}
}

void MainMenu::enterGameCallback(GameModeEnum gamemode)
{
	playEffect(EFFECT_MUSIC);
	if (closeModeSet.find(gamemode) != closeModeSet.end())
	{
	}
	else
	//enable modes
	{
		if (gamemode == GameModeEnum::server)
		{
			std::string portStr = editBoxInServer->getString();
			//CCLOG("%s", portStr);
			int port = -1;
			bool inputIllegal = false;
			//check input
			for (auto i : portStr)
			{
				if (i > '9' || i < '0')
				{
					CCLOG("port illegal");
					inputIllegal = true;
					break;
				}
			}
			if (inputIllegal)
			{
			}
			else
			{
				//get int port
				port = 0;
				for (auto i : portStr)
				{
					port *= 10;
					port += i - '0';
				}
			}
			//save port
			userDefault->setIntegerForKey("port", port);
			userDefault->setIntegerForKey("gamemode", GameModeEnum::server);
		}
		else if (gamemode == GameModeEnum::client)
		{
			std::string ipAndPortStr = editBoxInClient->getString();
			//check input
			//check finish, get ip and port
			if (! checkClientInput(ipAndPortStr))
			{
				CCLOG("illegal input");
			}
			else
			{
				CCLOG("correct input");
				std::string ip;
				std::string portStr;
				int indexColon;
				for (auto i = 0; i < ipAndPortStr.length(); ++i)
				{
					if (ipAndPortStr[i] == ':')
					{
						indexColon = i;
						break;
					}
					ip.push_back(ipAndPortStr[i]);
				}
				for (auto i = indexColon + 1; i < ipAndPortStr.length(); ++i)
				{
					portStr.push_back(ipAndPortStr[i]);
				}
				//get port
				int port = 0;
				for (auto ch : portStr)
				{
					port *= 10;
					port += ch - '0';
				}
				//save ip and port
				userDefault->setStringForKey("ip", ip);
				userDefault->setIntegerForKey("port", port);
				userDefault->setIntegerForKey("gamemode", GameModeEnum::client);
			}
		}
		else if (gamemode == GameModeEnum::vsPlayer )
		{
			if (confirm->isVisible())
			{
				userDefault->setIntegerForKey("gamemode", GameModeEnum::vsPlayer);
			}
		}
		//enter game!
		//auto gameScene = GameScene::create();
		//Director::getInstance()->pushScene(Director::getInstance()->getRunningScene());
		Director::getInstance()->pushScene(TransitionFade::create(0.7,GameScene::create()));
	}
}

void MainMenu::displayEditBoxCallback(Ref * sender, const std::string & mode)
{
	playEffect(EFFECT_MUSIC);
	auto netMenuFadeOut = FadeOut::create(0.5);
	netMenu->runAction(netMenuFadeOut);
	netMenu->setVisible(false);
	if (mode == std::string{ "server" })
	{
		auto serverLayerFadeIn = FadeIn::create(0.5);
		serverLayer->runAction(serverLayerFadeIn);
		serverLayer->setVisible(true);
	}
	if (mode == std::string{ "client" })
	{
		auto clientLayerFadeIn = FadeIn::create(0.5);
		clientLayer->runAction(clientLayerFadeIn);
		clientLayer->setVisible(true);
	}
}

void MainMenu::displayMapCallBack(Ref * sender, const std::string & mapName)
{
	playEffect(EFFECT_MUSIC);
	//auto netMenuFadeOut = FadeOut::create(0.5);
	//mapMenu->runAction(netMenuFadeOut);
	//mapMenu->setVisible(false);
	if (!mMiniMapLayer->isVisible())
	{
		mMiniMapLayer->setVisible(true);
	}
	if (!confirm->isVisible())
	{
		confirm->setVisible(true);
	}
	userDefault->setStringForKey("map", mapName);
	for (auto i : items)
	{
		for (auto j : mapNameArray)
		{
			if (i.first == j)
			{
				i.second->setColor(Color3B(0, 0, 0));
			}
		}
	}
	items[mapName]->setColor(Color3B(255, 0, 0));

	refreshMiniMap(mapName);

}

void MainMenu::cancelCallback(Ref * sender, const std::string & thisMenu)
{
	playEffect(EFFECT_MUSIC);
	if (thisMenu == std::string{ "server" })
	{
		auto serverLayerFadeOut = FadeOut::create(0.5);
		serverLayer->runAction(serverLayerFadeOut);
		serverLayer->setVisible(false);
		auto netMenuFadeIn = FadeIn::create(0.5);
		netMenu->runAction(netMenuFadeIn);
		netMenu->setVisible(true);
	}
	if (thisMenu == std::string{ "client" })
	{
		auto clientLayerFadeOut = FadeOut::create(0.5);
		clientLayer->runAction(clientLayerFadeOut);
		clientLayer->setVisible(false);
		auto netMenuFadeIn = FadeIn::create(0.5);
		netMenu->runAction(netMenuFadeIn);
		netMenu->setVisible(true);
	}

}

void MainMenu::settingCallback(Ref * sender, const std::string & setting)
{
	playEffect(EFFECT_MUSIC);
	if (setting == std::string{ "music" })
	{
		if (!userDefault->getBoolForKey("musicOn"))
		{
			//std::string str = getDicValue("musicOn");
			std::string str = std::string{ dictionary->valueForKey(std::string{ "musicOn" })->getCString() };
			//CCLOG("%s", str.c_str());
			items["music"]->setString(str);
			//items["music"]->setString("fuck");
			userDefault->setBoolForKey("musicOn", true);
		}
		else
		{
			//std::string str = getDicValue("musicOff");
			std::string str = std::string{ dictionary->valueForKey(std::string{ "musicOff" })->getCString() };
			//CCLOG("%s", str.c_str());
			items["music"]->setString(str);
			//items["music"]->setString("you");
			userDefault->setBoolForKey("musicOn", false);
		}
	}
	playBackgroundMusic(BG_MUSIC);
	if (setting == std::string{ "se" })
	{
		if (!userDefault->getBoolForKey("seOn"))
		{
			//items["soundEffect"]->setString(mTools::strUTF8("音效   开"));
			items["se"]->setString(getDicValue("seOn"));
			userDefault->setBoolForKey("seOn", true);
		}
		else
		{
			//items["soundEffect"]->setString(mTools::strUTF8("音效   关"));
			items["se"]->setString(getDicValue("seOff"));
			userDefault->setBoolForKey("seOn", false);
		}
	}
	if (setting == std::string{ "mist" })
	{
		if (!userDefault->getBoolForKey("mistOn"))
		{
			items["mist"]->setString(getDicValue("mistOn"));
			userDefault->setBoolForKey("mistOn", true);
		}
		else
		{
			items["mist"]->setString(getDicValue("mistOff"));
			userDefault->setBoolForKey("mistOn", false);
		}
	}
	if (setting == std::string{ "theme" })
	{
		if (userDefault->getStringForKey("theme") == "grey")
		{
			items["theme"]->setString(getDicValue("themePink"));
			userDefault->setStringForKey("theme", "pink");
		}
		else if (userDefault->getStringForKey("theme") == "pink")
		{
			items["theme"]->setString(getDicValue("themeGrey"));
			userDefault->setStringForKey("theme", "grey");
		}
	}
}

void MainMenu::exitCallback(Ref *sender)
{
	Director::getInstance()->end();
}

void MainMenu::onMouseMove(Event * event)
{
	auto e = static_cast<EventMouse *>(event);
	Vec2 mouseCoordinate;
	mouseCoordinate.x = e->getLocation().x;
	mouseCoordinate.y = 2 * Director::getInstance()->getWinSize().height - e->getLocation().y;
	//CCLOG("(%f,%f)", mouseCoordinate.x, mouseCoordinate.y);
	for (auto i : items)
	{
		auto box = i.second->boundingBox();
		auto size = i.second->getContentSize();
		auto pos = i.second->getPosition();
		float x = pos.x + Director::getInstance()->getWinSize().width / 2 - size.width / 2;
		float y = pos.y + Director::getInstance()->getWinSize().height / 2 - size.height / 2;
		float width = size.width;
		float height = size.height;
		Rect rect(x, y, width, height);
		if (rect.containsPoint(mouseCoordinate))
		{
			/*debug
			auto drawN = DrawNode::create();
			drawN->drawRect(Vec2(x, y), Vec2(x + width, y + height), Color4F(0, 0, 0, 1));
			drawN->drawRect(Vec2(box.getMinX(), box.getMinY()), Vec2(box.getMaxX(),box.getMaxY()),Color4F(1,0,0,1));
			MainMenu::addChild(drawN, 10);
			*/
			//CCLOG("con!");
			if ( i.first == "vsAi" || i.first == "story" )
			{
				i.second->setColor(Color3B(255, 0, 0));
			}
			else
			{
				i.second->setScale(1.2);
			}
		}
		else
		{
			i.second->setScale(1);
			if ( i.first == "vsAi" || i.first == "story" )
			{
				i.second->setColor(Color3B(100, 100, 100));
			}
			else
			{
				i.second->setColor(Color3B(0, 0, 0));
			}
		}
	}
}

std::string MainMenu::getDicValue(char * str)
{
	 CCLOG("getdicValue: result: %s",dictionary->valueForKey(str)->getCString());
	 return std::string{dictionary->valueForKey(str)->getCString()};
}

Label * MainMenu::createMenuLabel(char * str)
{
	if (getDicValue(str) == "")
	{
		return Label::createWithTTF(str, "fonts/STXIHEI.TTF", 30);
	}
	return Label::createWithTTF(getDicValue(str), "fonts/STXIHEI.TTF", 30);
}

bool MainMenu::checkClientInput(std::string & ipAndPortStr)
{
	int numOfColon = 0;
	int numOfDot = 0;
	for (auto i = 0; i < ipAndPortStr.length(); ++ i)
	{
		if (ipAndPortStr[i] == '.')
		{
			++numOfDot;
			continue;
		}
		if (ipAndPortStr[i] == ':')
		{
			if (numOfDot < 3)
			{
				return false;
			}
			if (i + 1 >= ipAndPortStr.length())
			{
				//an : in the end => no port
				return false;
			}
			else
			{
				++numOfColon;
			}
		}
	}
	if ((numOfDot != 3) || (numOfColon != 1))
	{
		return false;
	}
	return true;
}

void MainMenu::playEffect(const char * filePath)
{
	if (userDefault->getBoolForKey("seOn"))
	{
		SimpleAudioEngine::getInstance()->playEffect(filePath);
	}
}

void MainMenu::playBackgroundMusic(const char * filePath)
{
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		CCLOG("BackgroundMusic is Playing");
		if (!userDefault->getBoolForKey("musicOn"))
		{
			CCLOG("BackgroundMusic shouldn't Playing");
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		}
	}
	else
	{
		CCLOG("BackgroundMusic isn't Playing");
		if (userDefault->getBoolForKey("musicOn"))
		{
			CCLOG("BackgroundMusic should Playing");
			SimpleAudioEngine::getInstance()->playBackgroundMusic(filePath, true);
		}
	}
}

void MainMenu::refreshMiniMap(std::string mapName)
{
	auto jsonFile = FileUtils::getInstance()->fullPathForFilename("map/" + mapName);
	ssize_t size = 0;
	unsigned char * loadStr = FileUtils::getInstance()->getFileData(jsonFile, "r", &size);
	std::string jsonStr = std::string( (const char *)loadStr, size );

	mBasePosition.clear();
	mResourceMap.clear();

	rapidjson::Document jDocument;
	jDocument.Parse<0>(jsonStr.c_str());
	if (jDocument.HasParseError())
	{
		CCLOG("unitDisplay.json parse error!");
	}
	if (!jDocument.IsObject())
	{
		CCLOG("error : not object!");
	}
	rapidjson::Value & mapElements = jDocument["mapelement"];
	if (mapElements.IsArray())
	{
		for (int i = 0; i < mapElements.Size(); ++i)
		{
			rapidjson::Value & item = mapElements[i];
			MyPointStruct point = { item["position"]["X"].GetInt(), item["position"]["Y"].GetInt() };
			//CCLOG("point: %d,%d; num: %d", point.x, point.y, item["numHitPoint"].GetInt());
			std::string element{ item["element"].GetString() };
			//CCLOG("element: %s", element.c_str());
			if (element == "base0")
			{
				mBasePosition.push_back(point);
				continue;
			}
			if (element == "base1")
			{	
				mBasePosition.push_back(point);
				continue;
			}
			if (element == "resourceFixed")
			{
				Unit unit = {
					UnitEnum::fixedResource,
					NULL,
					UnitStateEnum::attacked,
					NULL,
				};
				mResourceMap[point] = unit;
				continue;
			}
			if (element == "resourceRandom")
			{
				//CCLOG("a resourceR!");
				Unit unit = {
					UnitEnum::randomResource,
					NULL,
					UnitStateEnum::attacked,
					NULL
				};
				mResourceMap[point] = unit;
				continue;
			}
		}
	}

	refreshMiniMap();
}

void MainMenu::refreshMiniMap()
{
	std::set<MyPointStruct> unitSet[2];
	std::set<MyPointStruct> unitSetN[2];
	std::set<MyPointStruct> fixedRSet;
	std::set<MyPointStruct> randomRSet;

	for (int i = 0; i < mBasePosition.size() && i < 2; ++i)
	{
		unitSet[i].insert(mBasePosition[i]);
	}

	for (const auto & i : mResourceMap)
	{
		if (i.second.type == fixedResource)
		{
			fixedRSet.insert(i.first);
			continue;
		}
		if (i.second.type == randomResource)
		{
			randomRSet.insert(i.first);
			continue;
		}
	}
	mMiniMapLayer->refresh(unitSet[0], unitSetN[0], unitSet[1], unitSetN[1], fixedRSet, randomRSet);
}