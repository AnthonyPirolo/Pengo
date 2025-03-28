//#include <SDL.h>
//#include "Gamepad.h"
//#include <minwinbase.h>
//#include <winerror.h>
//
//const WORD XINPUT_Buttons[] =
//{
//      XINPUT_GAMEPAD_A,
//      XINPUT_GAMEPAD_B,
//      XINPUT_GAMEPAD_X,
//      XINPUT_GAMEPAD_Y,
//      XINPUT_GAMEPAD_DPAD_UP,
//      XINPUT_GAMEPAD_DPAD_DOWN,
//      XINPUT_GAMEPAD_DPAD_LEFT,
//      XINPUT_GAMEPAD_DPAD_RIGHT,
//      XINPUT_GAMEPAD_START,
//      XINPUT_GAMEPAD_BACK
//};
//
//Gamepad::Gamepad() {}
//
//Gamepad::Gamepad(int iIndex)
//{
//    m_GamepadIndex = iIndex - 1;
//}
//
//XINPUT_STATE Gamepad::GetState() const
//{
//    XINPUT_STATE GamepadState;
//
//    ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));
//
//    XInputGetState(m_GamepadIndex, &GamepadState);
//
//    return GamepadState;
//}
//
//int Gamepad::GetIndex() const
//{
//    return m_GamepadIndex;
//}
//
//bool Gamepad::IsConnected()
//{
//    ZeroMemory(&m_State, sizeof(XINPUT_STATE));
//
//    DWORD Result = XInputGetState(m_GamepadIndex, &m_State);
//
//    if (Result == ERROR_SUCCESS)
//        return true;
//    else
//        return false;
//}
//
//bool Gamepad::GetButtonPressed(int a_iButton) const
//{
//    if (m_State.Gamepad.wButtons & XINPUT_Buttons[a_iButton])
//    {
//        return true;
//    }
//
//    return false;
//}
//
//bool Gamepad::GetButtonDown(int a_iButton) const
//{
//    return bGamepad_ButtonsDown[a_iButton];
//}
//
//void Gamepad::Update()
//{
//    m_State = GetState();
//
//    for (int i = 0; i < ButtonCount; i++)
//    {
//        bButtonStates[i] = (m_State.Gamepad.wButtons &
//            XINPUT_Buttons[i]) == XINPUT_Buttons[i];
//
//        bGamepad_ButtonsDown[i] = !bPrev_ButtonStates[i] &&
//            bButtonStates[i];
//    }
//}
//
//XButtonIDs::XButtonIDs()
//{
//    A = 0;
//    B = 1;
//    X = 2;
//    Y = 3;
//
//    DPad_Up = 4;
//    DPad_Down = 5;
//    DPad_Left = 6;
//    DPad_Right = 7;
//
//    Start = 12;
//    Back = 13;
//}