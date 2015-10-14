#include "MyProject.h"
#include "FormBase.h"
#include "UILayer.h"

UFormBase::UFormBase()
{
	m_draggable = true;
	m_hitYMax = 30;
	m_alignVertial = 0;
	m_alignHorizontal = 0;
	m_isResReady = false;
}

void UFormBase::onReady()
{

}

UIFormID UFormBase::getFormId()
{
	return m_formId;
}

int UFormBase::getPosX()
{
	return 0;
}

void UFormBase::setPosX(int posX)
{

}

int UFormBase::getPosY()
{

}

void UFormBase::setPosY(int posY)
{

}

UILayer* UFormBase::getUiLayer()
{
	return m_uiLayer;
}
void UFormBase::setUiLayer(UILayer* layer)
{
	m_uiLayer = layer;
}

bool UFormBase::IsVisible()
{
	return false;
}

bool UFormBase::getIsResReady()
{
	return m_isResReady;
}

void UFormBase::setIsResReady(bool value)
{
	m_isResReady = value;
}

public GUIWin* UFormBase::getGUIWin()
{
	return m_GUIWin;
}