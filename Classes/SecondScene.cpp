#include "Define.h"
#include "SecondScene.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
//#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

using namespace cocostudio;
//using namespace ui;
USING_NS_CC_EXT;

#define LayerTestUI_TAG 100
#define LayerExitUI_TAG 101

cocos2d::Scene* SecondScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SecondScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool SecondScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	//Size visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();


	LayerTestUI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Json/LayerTest.json");
	//	UI->setPosition(Vec2(100, 100));
	this->addChild(LayerTestUI,1);
	LayerTestUI->setTag(100);
	Button *pExitBtn = (Button *)Helper::seekWidgetByName(LayerTestUI, "Button_ExitGame");
	pExitBtn->addTouchEventListener(this, toucheventselector(SecondScene::enterLayerUI));

	Button *pCaptureScreen = (Button*)Helper::seekWidgetByName(LayerTestUI, "Button_CaptureScreen");
	pCaptureScreen->addTouchEventListener(this, toucheventselector(SecondScene::capture));
	

	LayerExitUI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Json/LayerExit.json");
	this->addChild(LayerExitUI);
	LayerExitUI->setTag(LayerExitUI_TAG);
	LayerExitUI->setVisible(false);
	(Button *)m_btnSure = (Button *)Helper::seekWidgetByName(LayerExitUI, "Btn_Sure");
	m_btnSure->addTouchEventListener(LayerExitUI, toucheventselector(SecondScene::exitGame));

	(Button *)m_btnCancel = (Button*)Helper::seekWidgetByName(LayerExitUI, "Btn_Cancel");
	m_btnCancel->addTouchEventListener(LayerExitUI, toucheventselector(SecondScene::cancelExitGame));

	return true;
}

void SecondScene::menuCloseCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

SecondScene::SecondScene()
{

	InitTime();
	InitExitLayer();
}

void SecondScene::InitTime()
{
	time_t lt = time(NULL);
	struct tm* local = localtime(&lt);
	m_currentHour = local->tm_hour;
	m_currentMin = local->tm_min;
	m_currentSec = local->tm_sec;
	m_currentYear = local->tm_year + 1900;
	m_currentMonth = local->tm_mon + 1;
	m_currentMday = local->tm_mday;
}

void SecondScene::InitExitLayer()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	EventListenerKeyboard *listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(SecondScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	auto listner = EventListenerKeyboard::create();
	listner->onKeyReleased = CC_CALLBACK_2(SecondScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
}

void SecondScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	YYKeyMenuClick(keyCode,event);
}

void SecondScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	YYKeyMenuClick(keyCode,event);
}

void SecondScene::YYKeyMenuClick(EventKeyboard::KeyCode keyCode, Event* pEvent)
{
	Director::getInstance()->end();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void SecondScene::exitGame(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		 Director::getInstance()->end();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		 exit(0);
#endif
	}
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}

}

void SecondScene::enterLayerUI(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		LayerExitUI->setVisible(true);
		LayerTestUI->setVisible(false);
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}
}

void SecondScene::cancelExitGame(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}

}

void SecondScene::capture(Ref* pSender, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		captureImpl();
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}
	
}

void SecondScene::captureImpl()
{
	char chTime[64];
	sprintf(chTime, "%4d-%02d-%02d %02d_%02d_%02d", m_currentYear, m_currentMonth, m_currentMday, m_currentHour, m_currentMin, m_currentSec);
	string szScreenShotFileName = toString("Screen_shot_", chTime, ".png");

	log("ScreenShot");

	utils::captureScreen([](bool succedd, std::string outputFile)
	{
		if (succedd)
		{
			log("%s", outputFile.c_str());
		}
		else
		{
			log("Capture screen failed\n");
		}
	}, szScreenShotFileName);
}

