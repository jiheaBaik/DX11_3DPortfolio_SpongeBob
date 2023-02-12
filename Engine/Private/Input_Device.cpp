#include "..\Public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}

HRESULT CInput_Device::Initialize_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		return E_FAIL;

	m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr);
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyboard->Acquire();

	m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr);	
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

HRESULT CInput_Device::Update()
{
	if (nullptr == m_pKeyboard ||
		nullptr == m_pMouse)
		return E_FAIL;

	m_pKeyboard->GetDeviceState(256, m_byKeyState);

	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	return S_OK;
}

void CInput_Device::Free()
{
	
}
