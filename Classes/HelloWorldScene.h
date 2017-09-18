#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"



class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	HelloWorld();
	~HelloWorld();
	void updateTime(float dt);
	void formShowTime();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void ObserverFunction(Ref* object);

	void menuObserverCallback(Ref* pSender);

	void capture(Ref* pSender);
	void afterCapture(bool succed,const std::string& outputFile);

	void WriteToJson(std::string strJsonFile);
	void ReadFromJson(std::string strJsonFile);
private:
	int								m_currentHour;
	int								m_currentMin;
	int								m_currentSec;
	cocos2d::Label*					m_pLabelOSTime;
	int								m_currentYear;
	int								m_currentMonth;
	int								m_currentMday;

};

#endif // __HELLOWORLD_SCENE_H__
