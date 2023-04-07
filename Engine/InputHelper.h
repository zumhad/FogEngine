#pragma once

#include "Core.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

enum MouseInput
{
	MOUSE_X,
	MOUSE_Y,
	MOUSE_SCROLL,

	COUNT_MOUSE = 3
};

enum KeyInput
{
	KEY_ESCAPE = DIK_ESCAPE,
	KEY_1 = DIK_1,
	KEY_2 = DIK_2,
	KEY_3 = DIK_3,
	KEY_4 = DIK_4,
	KEY_5 = DIK_5,
	KEY_6 = DIK_6,
	KEY_7 = DIK_7,
	KEY_8 = DIK_8,
	KEY_9 = DIK_9,
	KEY_0 = DIK_0,
	KEY_MINUS = DIK_MINUS,
	KEY_EQUALS = DIK_EQUALS,
	KEY_BACK = DIK_BACK,
	KEY_TAB = DIK_TAB,
	KEY_Q = DIK_Q,
	KEY_W = DIK_W,
	KEY_E = DIK_E,
	KEY_R = DIK_R,
	KEY_T = DIK_T,
	KEY_Y = DIK_Y,
	KEY_U = DIK_U,
	KEY_I = DIK_I,
	KEY_O = DIK_O,
	KEY_P = DIK_P,
	KEY_LBRACKET = DIK_LBRACKET,
	KEY_RETURN = DIK_RETURN,
	KEY_LCONTROL = DIK_LCONTROL,
	KEY_A = DIK_A,
	KEY_S = DIK_S,
	KEY_D = DIK_D,
	KEY_F = DIK_F,
	KEY_G = DIK_G,
	KEY_H = DIK_H,
	KEY_J = DIK_J,
	KEY_K = DIK_K,
	KEY_L = DIK_L,
	KEY_SEMICOLON = DIK_SEMICOLON,
	KEY_APOSTROPHE = DIK_APOSTROPHE,
	KEY_GRAVE = DIK_GRAVE,
	KEY_LSHIFT = DIK_LSHIFT,
	KEY_BACKSLASH = DIK_BACKSLASH,
	KEY_Z = DIK_Z,
	KEY_X = DIK_X,
	KEY_C = DIK_C,
	KEY_V = DIK_V,
	KEY_B = DIK_B,
	KEY_N = DIK_N,
	KEY_M = DIK_M,
	KEY_COMMA = DIK_COMMA,
	KEY_PERIOD = DIK_PERIOD,
	KEY_SLASH = DIK_SLASH,
	KEY_RSHIFT = DIK_RSHIFT,
	KEY_MULTIPLY = DIK_MULTIPLY,
	KEY_LMENU = DIK_LMENU,
	KEY_SPACE = DIK_SPACE,
	KEY_CAPITAL = DIK_CAPITAL,
	KEY_F1 = DIK_F1,
	KEY_F2 = DIK_F2,
	KEY_F3 = DIK_F3,
	KEY_F4 = DIK_F4,
	KEY_F5 = DIK_F5,
	KEY_F6 = DIK_F6,
	KEY_F7 = DIK_F7,
	KEY_F8 = DIK_F8,
	KEY_F9 = DIK_F9,
	KEY_F10 = DIK_F10,
	KEY_NUMLOCK = DIK_NUMLOCK,
	KEY_SCROLL = DIK_SCROLL,
	KEY_NUMPAD7 = DIK_NUMPAD7,
	KEY_NUMPAD8 = DIK_NUMPAD8,
	KEY_NUMPAD9 = DIK_NUMPAD9,
	KEY_SUBTRACT = DIK_SUBTRACT,
	KEY_NUMPAD4 = DIK_NUMPAD4,
	KEY_NUMPAD5 = DIK_NUMPAD5,
	KEY_NUMPAD6 = DIK_NUMPAD6,
	KEY_ADD = DIK_ADD,
	KEY_NUMPAD1 = DIK_NUMPAD1,
	KEY_NUMPAD2 = DIK_NUMPAD2,
	KEY_NUMPAD3 = DIK_NUMPAD3,
	KEY_NUMPAD0 = DIK_NUMPAD0,
	KEY_DECIMAL = DIK_DECIMAL,
	KEY_OEM_102 = DIK_OEM_102,
	KEY_F11 = DIK_F11,
	KEY_F12 = DIK_F12,
	KEY_F13 = DIK_F13,
	KEY_F14 = DIK_F14,
	KEY_F15 = DIK_F15,
	KEY_KANA = DIK_KANA,
	KEY_ABNT_C1 = DIK_ABNT_C1,
	KEY_CONVERT = DIK_CONVERT,
	KEY_NOCONVERT = DIK_NOCONVERT,
	KEY_YEN = DIK_YEN,
	KEY_ABNT_C2 = DIK_ABNT_C2,
	KEY_NUMPADEQUALS = DIK_NUMPADEQUALS,
	KEY_PREVTRACK = DIK_PREVTRACK,
	KEY_AT = DIK_AT,
	KEY_COLON = DIK_COLON,
	KEY_UNDERLINE = DIK_UNDERLINE,
	KEY_KANJI = DIK_KANJI,
	KEY_STOP = DIK_STOP,
	KEY_AX = DIK_AX,
	KEY_UNLABELED = DIK_UNLABELED,
	KEY_NEXTTRACK = DIK_NEXTTRACK,
	KEY_NUMPADENTER = DIK_NUMPADENTER,
	KEY_RCONTROL = DIK_RCONTROL,
	KEY_MUTE = DIK_MUTE,
	KEY_CALCULATOR = DIK_CALCULATOR,
	KEY_PLAYPAUSE = DIK_PLAYPAUSE,
	KEY_MEDIASTOP = DIK_MEDIASTOP,
	KEY_VOLUMEDOWN = DIK_VOLUMEDOWN,
	KEY_VOLUMEUP = DIK_VOLUMEUP,
	KEY_WEBHOME = DIK_WEBHOME,
	KEY_NUMPADCOMMA = DIK_NUMPADCOMMA,
	KEY_DIVIDE = DIK_DIVIDE,
	KEY_SYSRQ = DIK_SYSRQ,
	KEY_RMENU = DIK_RMENU,
	KEY_PAUSE = DIK_PAUSE,
	KEY_HOME = DIK_HOME,
	KEY_UP = DIK_UP,
	KEY_PRIOR = DIK_PRIOR,
	KEY_LEFT = DIK_LEFT,
	KEY_RIGHT = DIK_RIGHT,
	KEY_END = DIK_END,
	KEY_DOWN = DIK_DOWN,
	KEY_NEXT = DIK_NEXT,
	KEY_INSERT = DIK_INSERT,
	KEY_DELETE = DIK_DELETE,
	KEY_LWIN = DIK_LWIN,
	KEY_RWIN = DIK_RWIN,
	KEY_APPS = DIK_APPS,
	KEY_POWER = DIK_POWER,
	KEY_SLEEP = DIK_SLEEP,
	KEY_WAKE = DIK_WAKE,
	KEY_WEBSEARCH = DIK_WEBSEARCH,
	KEY_WEBFAVORITES = DIK_WEBFAVORITES,
	KEY_WEBREFRESH = DIK_WEBREFRESH,
	KEY_WEBSTOP = DIK_WEBSTOP,
	KEY_WEBFORWARD = DIK_WEBFORWARD,
	KEY_WEBBACK = DIK_WEBBACK,
	KEY_MYCOMPUTER = DIK_MYCOMPUTER,
	KEY_MAIL = DIK_MAIL,
	KEY_MEDIASELECT = DIK_MEDIASELECT,
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_6,
	MOUSE_7,
	MOUSE_8,

	COUNT_KEY = 256
};
