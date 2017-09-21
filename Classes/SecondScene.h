#ifndef _SECOND_SCENE_H_
#define _SECOND_SCENE_H_

#include "ui/CocosGUI.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace ui;
class SecondScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(SecondScene);
	SecondScene();
	void InitTime();
	void InitExitLayer();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void YYKeyMenuClick(EventKeyboard::KeyCode keyCode, Event* pEvent);

	//cocos2d::Node* ParseLayerJsonFile(pConstChar szFile,SecondScene* pLayer = NULL);
	void exitGame(Ref* sender,TouchEventType type);
	void enterLayerUI(Ref* sender, TouchEventType type);
	void cancelExitGame(Ref* sender, TouchEventType type);
//½ØÆÁ
	void capture(Ref* pSender, TouchEventType type);
	void captureImpl();
private:
//Ê±¼ä
	int		m_currentHour;
	int		m_currentMin;
	int		m_currentSec;
	int		m_currentYear;
	int		m_currentMonth;
	int		m_currentMday;
	cocos2d::ui::Widget* LayerExitUI;
	cocos2d::ui::Widget* LayerTestUI;
	Button *m_btnSure;
	Button *m_btnCancel;

};
#endif