//#pragma once
//#define _X86_
//
//#include <Xinput.h>
//
//class Gamepad
//{
//public:
//
//    Gamepad();
//    Gamepad(int a_iIndex);
//
//    void Update();
//
//    static const WORD XINPUT_Buttons[];
//
//    bool GetButtonPressed(int a_iButton) const;
//    bool GetButtonDown(int a_iButton) const;
//
//    XINPUT_STATE GetState() const;
//    int GetIndex() const;
//    bool IsConnected();
//
//private:
//    static const int ButtonCount = 14;
//    bool bPrev_ButtonStates[ButtonCount]{};
//    bool bButtonStates[ButtonCount]{};
//
//    bool bGamepad_ButtonsDown[ButtonCount]{};
//    XINPUT_STATE m_State{};
//    int m_GamepadIndex{};
//};
//
//struct XButtonIDs
//{
//    XButtonIDs();
//
//    int A, B, X, Y;
//
//    int DPad_Up, DPad_Down, DPad_Left, DPad_Right;
//
//    int Start;
//    int Back;
//};