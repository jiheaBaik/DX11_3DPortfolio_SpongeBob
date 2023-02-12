#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

	
public:
	enum DIMOUSEMOVE { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };
	enum DIMOUSEBUTTON { DIMB_L, DIMB_R, DIMB_WHEEL, DIMB_END };

private:
	CInput_Device();
	virtual ~CInput_Device() = default;	
	
public:
	_byte Get_DIKeyState(_ubyte eKeyID) {
		return m_byKeyState[eKeyID];
	}

	_long Get_DIMMoveState(DIMOUSEMOVE eMouseMove)
	{
		return *((_long*)&m_MouseState + eMouseMove);
	}
	_byte Get_DIMButtonState(DIMOUSEBUTTON eMouseButton)
	{
		return m_MouseState.rgbButtons[eMouseButton];
	}

public:
	HRESULT Initialize_Input_Device(HINSTANCE hInst, HWND hwnd);
	HRESULT Update();

private:
	LPDIRECTINPUT8				m_pInput = nullptr;
	LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;		

private:
	_byte						m_byKeyState[256] = { 0 };
	DIMOUSESTATE				m_MouseState;

public:
	virtual void Free() override;
};

END