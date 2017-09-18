#include "HelloWorldScene.h"
#include "Public.h"
#include <tchar.h>
#include <cstring>

#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace rapidjson;
USING_NS_CC;
#define CountArray(Arr) (sizeof(Arr)/sizeof(Arr[0]))
void YYNewChar(char* pData, WORD wDataSize, const char* format, ...)
{
	memset(pData, 0, wDataSize);

	va_list ap;
	va_start(ap, format);
	vsnprintf(pData, wDataSize, format, ap);
	va_end(ap);
}

#define YYDefineChar(name, size, format, ...) \
	char name[size]; \
	memset(name, 0, size);\
	YYNewChar(name, size, format, ##__VA_ARGS__);

const char* cc_Nd_Message = "Message";

//std::string TCHAR2STRING(TCHAR *STR)
//{
//
//	int iLen = WideCharToMultiByte(CP_ACP, 0,STR, -1, NULL, 0, NULL, NULL);
//
//	char* chRtn =new char[iLen*sizeof(char)];
//
//	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
//
//	std::string str(chRtn);
//
//	return str;
//
//}
//
//void StringToWstring(std::wstring& szDst, std::string str)
//{
//	std::string temp = str;
//	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL,0); 
//	wchar_t * wszUtf8 = new wchar_t[len+1]; 
//	memset(wszUtf8, 0, len * 2 + 2); 
//	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
//	szDst = wszUtf8;
//	std::wstring r = wszUtf8;
//	delete[] wszUtf8;
//}


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
//按返回键退出 start
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	 EventListenerKeyboard *listener = EventListenerKeyboard::create();
	 listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed,this);
	 _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	auto listner = EventListenerKeyboard::create();
	listner->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
#endif
//按返回键退出 end
	time_t lt= time(NULL);
	struct tm* local = localtime(&lt);

	char currentData[32];
	memset(currentData,0,sizeof(currentData));
	strftime(currentData,32,"%Y-%m-%d",local);

	Label* pLabelDate = Label::createWithTTF(std::string(currentData),"fonts/Marker Felt.ttf", 24);
	pLabelDate->setPosition(Vec2(visibleSize.width + origin.x - pLabelDate->getContentSize().width*2, visibleSize.height + origin.y-pLabelDate->getContentSize().height));
	addChild(pLabelDate,2);
	pLabelDate->setString(std::string(currentData));

	m_pLabelOSTime = Label::createWithTTF(std::string(currentData),"fonts/Marker Felt.ttf", 24);
	formShowTime();
	m_pLabelOSTime->setPosition(pLabelDate->getContentSize().width+pLabelDate->getPositionX(),pLabelDate->getPositionY());
	addChild(m_pLabelOSTime,2);
	printf("时间\n");	
	this->schedule(schedule_selector(HelloWorld::updateTime),0.1f);
	
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(HelloWorld::ObserverFunction),cc_Nd_Message,NULL);
	auto ObserverItem = MenuItemImage::create(
		"CloseSelected.png",
		"CloseNormal.png",
		CC_CALLBACK_1(HelloWorld::menuObserverCallback, this));

	ObserverItem->setPosition(Vec2(origin.x + ObserverItem->getContentSize().width ,
		origin.y + ObserverItem->getContentSize().height/2));
	auto pMenu = Menu::create(ObserverItem, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu, 1);

	//
	auto CaptureScreenItem = MenuItemImage::create("CloseSelected.png",
		"CloseNormal.png",
		CC_CALLBACK_1(HelloWorld::capture, this));
	CaptureScreenItem->setPosition(Vec2(origin.x + visibleSize.width /2,
		origin.y + visibleSize.height/2));
	auto pMenuScreenItem = Menu::create(CaptureScreenItem, NULL);
	pMenuScreenItem->setPosition(Vec2::ZERO);
	pMenuScreenItem->setScale(5.0);
	this->addChild(pMenuScreenItem, 2);

	//json解析
//	WriteToJson(".\\ClientUpdate.json");
	ReadFromJson("ClientUpdate.json");
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

}


HelloWorld::HelloWorld()
{
	time_t lt= time(NULL);
	struct tm* local = localtime(&lt);
	m_currentHour = local->tm_hour;
	m_currentMin = local->tm_min;
	m_currentSec = local->tm_sec;
	m_currentYear = local->tm_year + 1900;
	m_currentMonth = local->tm_mon + 1;
	m_currentMday = local->tm_mday;
}


HelloWorld::~HelloWorld()
{
	NotificationCenter::getInstance()->removeObserver(this,cc_Nd_Message);
}

void HelloWorld::updateTime(float dt)
{
//	log("updateTime: %f\n",dt);
	//当秒到59,分未到59时 分钟加1，秒清0 
	if(m_currentSec == 59 && m_currentMin != 59)
	{
		m_currentMin++;
		m_currentSec = 0;
	}
	//当秒和分都为59 时不为23时 ，秒和分钟都清0，时钟加1 
	else if(m_currentSec == 59 && m_currentMin == 59 && m_currentHour != 23)
	{
		m_currentMin = 0;
		m_currentSec = 0;
		m_currentHour++;
	}
	//当秒和分都为59 时为23时 ，秒，分钟和时钟都清0
	else if(m_currentSec == 59 && m_currentMin == 59 && m_currentHour == 23)
	{
		m_currentMin = 0;
		m_currentSec = 0;
		m_currentHour= 0;
	}
	//其它情况秒钟进行不断加1
	else
		m_currentSec++;
	formShowTime();
	Sleep(1000);
}
void HelloWorld::formShowTime()
{
	YYDefineChar(timeStr,32,"%02d:%02d:%02d",m_currentHour,m_currentMin,m_currentSec);
	m_pLabelOSTime->setString(timeStr);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorld::ObserverFunction(Ref* object)
{
	log("You click menuButton");
}

void HelloWorld::menuObserverCallback(Ref* pSender)
{
	NotificationCenter::getInstance()->postNotification(cc_Nd_Message,NULL);
}

void HelloWorld::capture(Ref* pSender)
{
	char chTime[64];
	sprintf(chTime,"%4d-%02d-%02d_%02d-%02d-%02d",m_currentYear,m_currentMonth,m_currentMday,m_currentHour,m_currentMin,m_currentSec);
	string szScreenShotFileName = toString("Screen_shot_",chTime,".png");

	log("ScreenShot");

	utils::captureScreen([](bool succedd,std::string outputFile)
	{		
		if(succedd)
		{
			log("%s",outputFile.c_str());
		}
		else
		{
			log("Capture screen failed\n");
		}
	},szScreenShotFileName);
}

void HelloWorld::afterCapture(bool succed,const std::string& outputFile)
{
	if(!succed)
	{
		log("Capture screen failed\n");
		return;
	}
	log("%s",outputFile.c_str());

}

void HelloWorld::WriteToJson(std::string strJsonFile)
{
	Document writedoc;
	writedoc.SetObject();
	Document::AllocatorType& allocator = writedoc.GetAllocator();
	//rapidjson::Value array(kArrayType);
	//rapidjson::Value object(kObjectType);

	//writedoc.AddMember("isOpenUpdate",1,allocator);
	//writedoc.AddMember("UPDATE_URL","http://new.13shui.cc:81/CheckUpdating.aspx",allocator);
	//writedoc.AddMember("PACKAGE_PLATFORM_ANDROID","com.xianwan.androidssz",allocator);
	//writedoc.AddMember("PACKAGE_PLATFORM_IOS","com.xianwan.iosssz",allocator);

	rapidjson::Value arr(kArrayType);
	rapidjson::Value object(kObjectType);
	object.AddMember("isOpenUpdate", 1, allocator);
	object.AddMember("UPDATE_URL", "http://new.13shui.cc:81/CheckUpdating.aspx", allocator);
	object.AddMember("PACKAGE_PLATFORM_ANDROID", "com.xianwan.androidssz", allocator);
	object.AddMember("PACKAGE_PLATFORM_IOS", "com.xianwan.iosssz", allocator);
	arr.PushBack(object, allocator);
	writedoc.AddMember("ClientUpdate", arr, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	writedoc.Accept(writer);

	string path = FileUtils::getInstance()->getWritablePath();
	path.append(strJsonFile);
	FILE *fpFile = fopen(path.c_str(),"wb");
	if(fpFile)
	{
		fputs(buffer.GetString(),fpFile);
		fclose(fpFile);
	}
	log("%s",buffer.GetString());
}

void HelloWorld::ReadFromJson(std::string strJsonFile)
{
	Document readdoc;
	std::string load_str = FileUtils::getInstance()->getStringFromFile(strJsonFile);
	readdoc.Parse<0>(load_str.c_str());
	if(readdoc.HasParseError())
		log("GetParsError%s\n",readdoc.GetParseError());
	if(!readdoc.IsObject())
		return ;
	/*	rapidjson::Value& _jsonisOpenUpdate = readdoc["isOpenUpdate"];
		log("isOpenUpdate = %d\n",_jsonisOpenUpdate.GetInt());
		rapidjson::Value& _jsonUPDATE_URL = readdoc["UPDATE_URL"];
		log("UPDATE_URL = %s\n",_jsonUPDATE_URL.GetString());
		rapidjson::Value& _jsonPACKAGE_PLATFORM_ANDROID = readdoc["PACKAGE_PLATFORM_ANDROID"];
		log("PACKAGE_PLATFORM_ANDROID = %s\n",_jsonPACKAGE_PLATFORM_ANDROID.GetString());
		rapidjson::Value& _jsonPACKAGE_PLATFORM_IOS = readdoc["PACKAGE_PLATFORM_IOS"];
		log("PACKAGE_PLATFORM_IOS = %s\n",_jsonPACKAGE_PLATFORM_IOS);*/

	rapidjson::Value& _arr = readdoc["ClientUpdate"];
	if (_arr.IsArray())
	{
		for (int i = 0; i < _arr.Capacity(); i++)
		{
			rapidjson::Value& arrydoc = _arr[i];
			if (arrydoc.HasMember("isOpenUpdate"))
			{
				//rapidjson::Value& _jsonisOpenUpdate = _arr["isOpenUpdate"];
				log("isOpenUpdate = %d\n", arrydoc["isOpenUpdate"].GetInt());
			}
			if (arrydoc.HasMember("UPDATE_URL"))
			{
				//rapidjson::Value& _jsonUPDATE_URL = _arr["UPDATE_URL"];
				log("UPDATE_URL = %s\n", arrydoc["UPDATE_URL"].GetString());
			}
			if (arrydoc.HasMember("PACKAGE_PLATFORM_ANDROID"))
			{
				//rapidjson::Value& _jsonPACKAGE_PLATFORM_ANDROID = _arr["PACKAGE_PLATFORM_ANDROID"];
				log("PACKAGE_PLATFORM_ANDROID = %s\n", arrydoc["PACKAGE_PLATFORM_ANDROID"].GetString());
			}
			if (arrydoc.HasMember("PACKAGE_PLATFORM_IOS"))
			{
				//rapidjson::Value& _jsonPACKAGE_PLATFORM_IOS = _arr["PACKAGE_PLATFORM_IOS"];
				log("PACKAGE_PLATFORM_IOS = %s\n", arrydoc["PACKAGE_PLATFORM_IOS"].GetString());
			}
		}
	}
}
