/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
// See https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/keyCode

// When privacy.resistFingerprinting is active, we hide the user's use of
// the numpad, right modifer keys, and any non-QWERTY US English keyboard.
//
// All keys on the standard QWERTY US keyboard are mapped onto those codes,
// as expected. These include [0-9a-zA-Z`~,<.>/?;:'"\[{\]}\\|-_=+ ], plus:
// Alt, ArrowDown, ArrowLeft, ArrowRight, ArrowUp, Backspace, CapsLock,
// ContextMenu, Control, Delete, End, Enter, Escape, Help, Home, Insert,
// Meta, PageDown, PageUp, Pause, PrintScreen, ScrollLock, Shift, Tab
//
// All other characters and symbols are mapped onto keys/codes based on
// a survey of existing keyboard layouts containing the characters and
// choosing the most common key.

#include "nsString.h"
#include "nsDataHashtable.h"
#include "mozilla/StaticMutex.h"

// KEY_INTERNAL is called by KEY, SHIFT or ALTGR.
#define KEY_INTERNAL(key, code, keyCode, shift, altgr) {          \
  gCodes->Put(NS_LITERAL_STRING(key), NS_LITERAL_STRING(#code));  \
  gKeyCodes->Put(NS_LITERAL_STRING(key), keyCode);                \
  gShiftStates->Put(NS_LITERAL_STRING(key), shift);               \
  gAltGrStates->Put(NS_LITERAL_STRING(key), altgr);               \
}

// KEY, SHIFT, and ALTGR assign a consensus codeName and keyCode for the
// given keyName.
//
// KEY indicates that shift is off, altgr is off.
#define KEY(key, code, keyCode) KEY_INTERNAL(key, code, keyCode, false, false)
// SHIFT indicates that shift is on, altgr is off.
#define SHIFT(key, code, keyCode) KEY_INTERNAL(key, code, keyCode, true, false)
// ALTGR indicates that shift is on, altgr is on.
#define ALTGR(key, code, keyCode) KEY_INTERNAL(key, code, keyCode, false, true)

// Four global constant static maps.
// gCodes provides a codeName for each keyName.
static nsDataHashtable<nsStringHashKey, nsString>* gCodes;
// gKeyCodes provides a keyCode for each keyName.
static nsDataHashtable<nsStringHashKey, uint32_t>* gKeyCodes;
// gShiftStates provides a shift value for each keyName.
static nsDataHashtable<nsStringHashKey, bool>* gShiftStates;
// gAltGrStates provides an AltGr value for each keyName.
static nsDataHashtable<nsStringHashKey, bool>* gAltGrStates;

static StaticMutex createKeyCodesMutex;

// Populate the global static maps gCodes, gKeyCodes, gShiftStates, and
// gAltGrStates with their constant values.
static void createKeyCodes()
{
  if (gCodes) return;

  StaticMutexAutoLock lock(createKeyCodesMutex);

  gCodes = new nsDataHashtable<nsStringHashKey, nsString>();
  gKeyCodes = new nsDataHashtable<nsStringHashKey, uint32_t>();
  gShiftStates = new nsDataHashtable<nsStringHashKey, bool>();
  gAltGrStates = new nsDataHashtable<nsStringHashKey, bool>();

  static_assert(nsIDOMKeyEvent::DOM_VK_ALT == 18,
               "nsIDOMKeyEvent::DOM_VK_ALT value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_DOWN == 40,
               "nsIDOMKeyEvent::DOM_VK_DOWN value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_LEFT == 37,
               "nsIDOMKeyEvent::DOM_VK_LEFT value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_RIGHT == 39,
               "nsIDOMKeyEvent::DOM_VK_RIGHT value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_UP == 38,
               "nsIDOMKeyEvent::DOM_VK_UP value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_BACK_SPACE == 8,
               "nsIDOMKeyEvent::DOM_VK_BACK_SPACE value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_CAPS_LOCK == 20,
               "nsIDOMKeyEvent::DOM_VK_CAPS_LOCK value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_CONTEXT_MENU == 93,
               "nsIDOMKeyEvent::DOM_VK_CONTEXT_MENU value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_CONTROL == 17,
               "nsIDOMKeyEvent::DOM_VK_CONTROL value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_DELETE == 46,
               "nsIDOMKeyEvent::DOM_VK_DELETE value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_END == 35,
               "nsIDOMKeyEvent::DOM_VK_END value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_ESCAPE == 27,
               "nsIDOMKeyEvent::DOM_VK_ESCAPE value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_HELP == 6,
               "nsIDOMKeyEvent::DOM_VK_HELP value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_HOME == 36,
               "nsIDOMKeyEvent::DOM_VK_HOME value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_INSERT == 45,
               "nsIDOMKeyEvent::DOM_VK_INSERT value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_WIN == 91,
               "nsIDOMKeyEvent::DOM_VK_WIN value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_PAGE_DOWN == 34,
               "nsIDOMKeyEvent::DOM_VK_PAGE_DOWN value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_PAGE_UP == 33,
               "nsIDOMKeyEvent::DOM_VK_PAGE_UP value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_PAUSE == 19,
               "nsIDOMKeyEvent::DOM_VK_PAUSE value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_PRINTSCREEN == 44,
               "nsIDOMKeyEvent::DOM_VK_PRINTSCREEN value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_SCROLL_LOCK == 145,
               "nsIDOMKeyEvent::DOM_VK_SCROLL_LOCK value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_SHIFT == 16,
               "nsIDOMKeyEvent::DOM_VK_SHIFT value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_TAB == 9,
               "nsIDOMKeyEvent::DOM_VK_TAB value changed");

  KEY("Alt", AltLeft, nsIDOMKeyEvent::DOM_VK_ALT)
  KEY("ArrowDown", ArrowDown, nsIDOMKeyEvent::DOM_VK_DOWN)
  KEY("ArrowLeft", ArrowLeft, nsIDOMKeyEvent::DOM_VK_LEFT)
  KEY("ArrowRight", ArrowRight, nsIDOMKeyEvent::DOM_VK_RIGHT)
  KEY("ArrowUp", ArrowUp, nsIDOMKeyEvent::DOM_VK_UP)
  KEY("Backspace", Backspace, nsIDOMKeyEvent::DOM_VK_BACK_SPACE)
  KEY("CapsLock", CapsLock, nsIDOMKeyEvent::DOM_VK_CAPS_LOCK)
  KEY("ContextMenu", ContextMenu, nsIDOMKeyEvent::DOM_VK_CONTEXT_MENU)
  KEY("Control", ControlLeft, nsIDOMKeyEvent::DOM_VK_CONTROL)
  KEY("Delete", Delete, nsIDOMKeyEvent::DOM_VK_DELETE)
  KEY("End", End, nsIDOMKeyEvent::DOM_VK_END)
  constexpr auto kDOM_VK_ENTER = 0x0E;
  KEY("Enter", Enter, kDOM_VK_ENTER)
  KEY("Escape", Escape, nsIDOMKeyEvent::DOM_VK_ESCAPE)
  KEY("Help", Help, nsIDOMKeyEvent::DOM_VK_HELP)
  KEY("Home", Home, nsIDOMKeyEvent::DOM_VK_HOME)
  KEY("Insert", Insert, nsIDOMKeyEvent::DOM_VK_INSERT)
  KEY("Meta", OSLeft, nsIDOMKeyEvent::DOM_VK_WIN)
  KEY("PageDown", PageDown, nsIDOMKeyEvent::DOM_VK_PAGE_DOWN)
  KEY("PageUp", PageUp, nsIDOMKeyEvent::DOM_VK_PAGE_UP)
  KEY("Pause", Pause, nsIDOMKeyEvent::DOM_VK_PAUSE)
  KEY("PrintScreen", PrintScreen, nsIDOMKeyEvent::DOM_VK_PRINTSCREEN)
  KEY("ScrollLock", ScrollLock, nsIDOMKeyEvent::DOM_VK_SCROLL_LOCK)
  KEY("Shift", ShiftLeft, nsIDOMKeyEvent::DOM_VK_SHIFT)
  KEY("Tab", Tab, nsIDOMKeyEvent::DOM_VK_TAB)
  // Leaving "Clear" key unimplemented; it's inconsistent between platforms.

  static_assert(nsIDOMKeyEvent::DOM_VK_SPACE == 32,
               "nsIDOMKeyEvent::DOM_VK_SPACE value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_COMMA == 188,
               "nsIDOMKeyEvent::DOM_VK_COMMA value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_PERIOD == 190,
               "nsIDOMKeyEvent::DOM_VK_PERIOD value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_SLASH == 191,
               "nsIDOMKeyEvent::DOM_VK_SLASH value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_SEMICOLON == 59,
               "nsIDOMKeyEvent::DOM_VK_SEMICOLON value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_QUOTE == 222,
               "nsIDOMKeyEvent::DOM_VK_QUOTE value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET == 219,
               "nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET == 221,
               "nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_BACK_QUOTE == 192,
               "nsIDOMKeyEvent::DOM_VK_BACK_QUOTE value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_BACK_SLASH == 220,
               "nsIDOMKeyEvent::DOM_VK_BACK_SLASH value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_HYPHEN_MINUS == 173,
               "nsIDOMKeyEvent::DOM_VK_HYPHEN_MINUS value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_EQUALS == 61,
               "nsIDOMKeyEvent::DOM_VK_EQUALS value changed");

  KEY(" ", Space, nsIDOMKeyEvent::DOM_VK_SPACE)
  KEY(",", Comma, nsIDOMKeyEvent::DOM_VK_COMMA)
  SHIFT("<", Comma, nsIDOMKeyEvent::DOM_VK_COMMA)
  KEY(".", Period, nsIDOMKeyEvent::DOM_VK_PERIOD)
  SHIFT(">", Period, nsIDOMKeyEvent::DOM_VK_PERIOD)
  KEY("/", Slash, nsIDOMKeyEvent::DOM_VK_SLASH)
  SHIFT("?", Slash, nsIDOMKeyEvent::DOM_VK_SLASH)
  KEY(";", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)
  SHIFT(":", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)
  KEY("'", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE)
  SHIFT("\"", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE)
  KEY("[", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)
  SHIFT("{", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)
  KEY("]", BracketRight, nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET)
  SHIFT("}", BracketRight, nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET)
  KEY("`", Backquote, nsIDOMKeyEvent::DOM_VK_BACK_QUOTE)
  SHIFT("~", Backquote, nsIDOMKeyEvent::DOM_VK_BACK_QUOTE)
  KEY("\\", Backslash, nsIDOMKeyEvent::DOM_VK_BACK_SLASH)
  constexpr auto DOM_VK_INTL_BACK_SLASH = nsIDOMKeyEvent::DOM_VK_BACK_SLASH;
  SHIFT("|", Backslash, nsIDOMKeyEvent::DOM_VK_BACK_SLASH)
  KEY("-", Minus, nsIDOMKeyEvent::DOM_VK_HYPHEN_MINUS)
  SHIFT("_", Minus, nsIDOMKeyEvent::DOM_VK_HYPHEN_MINUS)
  KEY("=", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS)
  SHIFT("+", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS)

  static_assert(nsIDOMKeyEvent::DOM_VK_A == 65,
               "nsIDOMKeyEvent::DOM_VK_A value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_B == 66,
               "nsIDOMKeyEvent::DOM_VK_B value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_C == 67,
               "nsIDOMKeyEvent::DOM_VK_C value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_D == 68,
               "nsIDOMKeyEvent::DOM_VK_D value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_E == 69,
               "nsIDOMKeyEvent::DOM_VK_E value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F == 70,
               "nsIDOMKeyEvent::DOM_VK_F value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_G == 71,
               "nsIDOMKeyEvent::DOM_VK_G value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_H == 72,
               "nsIDOMKeyEvent::DOM_VK_H value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_I == 73,
               "nsIDOMKeyEvent::DOM_VK_I value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_J == 74,
               "nsIDOMKeyEvent::DOM_VK_J value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_K == 75,
               "nsIDOMKeyEvent::DOM_VK_K value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_L == 76,
               "nsIDOMKeyEvent::DOM_VK_L value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_M == 77,
               "nsIDOMKeyEvent::DOM_VK_M value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_N == 78,
               "nsIDOMKeyEvent::DOM_VK_N value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_O == 79,
               "nsIDOMKeyEvent::DOM_VK_O value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_P == 80,
               "nsIDOMKeyEvent::DOM_VK_P value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_Q == 81,
               "nsIDOMKeyEvent::DOM_VK_Q value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_R == 82,
               "nsIDOMKeyEvent::DOM_VK_R value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_S == 83,
               "nsIDOMKeyEvent::DOM_VK_S value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_T == 84,
               "nsIDOMKeyEvent::DOM_VK_T value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_U == 85,
               "nsIDOMKeyEvent::DOM_VK_U value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_V == 86,
               "nsIDOMKeyEvent::DOM_VK_V value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_W == 87,
               "nsIDOMKeyEvent::DOM_VK_W value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_X == 88,
               "nsIDOMKeyEvent::DOM_VK_X value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_Y == 89,
               "nsIDOMKeyEvent::DOM_VK_Y value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_Z == 90,
               "nsIDOMKeyEvent::DOM_VK_Z value changed");

  SHIFT("A", KeyA, nsIDOMKeyEvent::DOM_VK_A)
  SHIFT("B", KeyB, nsIDOMKeyEvent::DOM_VK_B)
  SHIFT("C", KeyC, nsIDOMKeyEvent::DOM_VK_C)
  SHIFT("D", KeyD, nsIDOMKeyEvent::DOM_VK_D)
  SHIFT("E", KeyE, nsIDOMKeyEvent::DOM_VK_E)
  SHIFT("F", KeyF, nsIDOMKeyEvent::DOM_VK_F)
  SHIFT("G", KeyG, nsIDOMKeyEvent::DOM_VK_G)
  SHIFT("H", KeyH, nsIDOMKeyEvent::DOM_VK_H)
  SHIFT("I", KeyI, nsIDOMKeyEvent::DOM_VK_I)
  SHIFT("J", KeyJ, nsIDOMKeyEvent::DOM_VK_J)
  SHIFT("K", KeyK, nsIDOMKeyEvent::DOM_VK_K)
  SHIFT("L", KeyL, nsIDOMKeyEvent::DOM_VK_L)
  SHIFT("M", KeyM, nsIDOMKeyEvent::DOM_VK_M)
  SHIFT("N", KeyN, nsIDOMKeyEvent::DOM_VK_N)
  SHIFT("O", KeyO, nsIDOMKeyEvent::DOM_VK_O)
  SHIFT("P", KeyP, nsIDOMKeyEvent::DOM_VK_P)
  SHIFT("Q", KeyQ, nsIDOMKeyEvent::DOM_VK_Q)
  SHIFT("R", KeyR, nsIDOMKeyEvent::DOM_VK_R)
  SHIFT("S", KeyS, nsIDOMKeyEvent::DOM_VK_S)
  SHIFT("T", KeyT, nsIDOMKeyEvent::DOM_VK_T)
  SHIFT("U", KeyU, nsIDOMKeyEvent::DOM_VK_U)
  SHIFT("V", KeyV, nsIDOMKeyEvent::DOM_VK_V)
  SHIFT("W", KeyW, nsIDOMKeyEvent::DOM_VK_W)
  SHIFT("X", KeyX, nsIDOMKeyEvent::DOM_VK_X)
  SHIFT("Y", KeyY, nsIDOMKeyEvent::DOM_VK_Y)
  SHIFT("Z", KeyZ, nsIDOMKeyEvent::DOM_VK_Z)

  KEY("a", KeyA, nsIDOMKeyEvent::DOM_VK_A)
  KEY("b", KeyB, nsIDOMKeyEvent::DOM_VK_B)
  KEY("c", KeyC, nsIDOMKeyEvent::DOM_VK_C)
  KEY("d", KeyD, nsIDOMKeyEvent::DOM_VK_D)
  KEY("e", KeyE, nsIDOMKeyEvent::DOM_VK_E)
  KEY("f", KeyF, nsIDOMKeyEvent::DOM_VK_F)
  KEY("g", KeyG, nsIDOMKeyEvent::DOM_VK_G)
  KEY("h", KeyH, nsIDOMKeyEvent::DOM_VK_H)
  KEY("i", KeyI, nsIDOMKeyEvent::DOM_VK_I)
  KEY("j", KeyJ, nsIDOMKeyEvent::DOM_VK_J)
  KEY("k", KeyK, nsIDOMKeyEvent::DOM_VK_K)
  KEY("l", KeyL, nsIDOMKeyEvent::DOM_VK_L)
  KEY("m", KeyM, nsIDOMKeyEvent::DOM_VK_M)
  KEY("n", KeyN, nsIDOMKeyEvent::DOM_VK_N)
  KEY("o", KeyO, nsIDOMKeyEvent::DOM_VK_O)
  KEY("p", KeyP, nsIDOMKeyEvent::DOM_VK_P)
  KEY("q", KeyQ, nsIDOMKeyEvent::DOM_VK_Q)
  KEY("r", KeyR, nsIDOMKeyEvent::DOM_VK_R)
  KEY("s", KeyS, nsIDOMKeyEvent::DOM_VK_S)
  KEY("t", KeyT, nsIDOMKeyEvent::DOM_VK_T)
  KEY("u", KeyU, nsIDOMKeyEvent::DOM_VK_U)
  KEY("v", KeyV, nsIDOMKeyEvent::DOM_VK_V)
  KEY("w", KeyW, nsIDOMKeyEvent::DOM_VK_W)
  KEY("x", KeyX, nsIDOMKeyEvent::DOM_VK_X)
  KEY("y", KeyY, nsIDOMKeyEvent::DOM_VK_Y)
  KEY("z", KeyZ, nsIDOMKeyEvent::DOM_VK_Z)

  static_assert(nsIDOMKeyEvent::DOM_VK_F1 == 112,
               "nsIDOMKeyEvent::DOM_VK_F1 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F2 == 113,
               "nsIDOMKeyEvent::DOM_VK_F2 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F3 == 114,
               "nsIDOMKeyEvent::DOM_VK_F3 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F4 == 115,
               "nsIDOMKeyEvent::DOM_VK_F4 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F5 == 116,
               "nsIDOMKeyEvent::DOM_VK_F5 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F6 == 117,
               "nsIDOMKeyEvent::DOM_VK_F6 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F7 == 118,
               "nsIDOMKeyEvent::DOM_VK_F7 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F8 == 119,
               "nsIDOMKeyEvent::DOM_VK_F8 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F9 == 120,
               "nsIDOMKeyEvent::DOM_VK_F9 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F10 == 121,
               "nsIDOMKeyEvent::DOM_VK_F10 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F11 == 122,
               "nsIDOMKeyEvent::DOM_VK_F11 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F12 == 123,
               "nsIDOMKeyEvent::DOM_VK_F12 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F13 == 124,
               "nsIDOMKeyEvent::DOM_VK_F14 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F14 == 125,
               "nsIDOMKeyEvent::DOM_VK_F14 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F15 == 126,
               "nsIDOMKeyEvent::DOM_VK_F15 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F16 == 127,
               "nsIDOMKeyEvent::DOM_VK_F16 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F17 == 128,
               "nsIDOMKeyEvent::DOM_VK_F17 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F18 == 129,
               "nsIDOMKeyEvent::DOM_VK_F18 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F19 == 130,
               "nsIDOMKeyEvent::DOM_VK_F19 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F20 == 131,
               "nsIDOMKeyEvent::DOM_VK_F20 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F21 == 132,
               "nsIDOMKeyEvent::DOM_VK_F21 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F22 == 133,
               "nsIDOMKeyEvent::DOM_VK_F22 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F23 == 134,
               "nsIDOMKeyEvent::DOM_VK_F23 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_F24 == 135,
               "nsIDOMKeyEvent::DOM_VK_F24 value changed");

  KEY("F1", F1, nsIDOMKeyEvent::DOM_VK_F1)
  KEY("F2", F2, nsIDOMKeyEvent::DOM_VK_F2)
  KEY("F3", F3, nsIDOMKeyEvent::DOM_VK_F3)
  KEY("F4", F4, nsIDOMKeyEvent::DOM_VK_F4)
  KEY("F5", F5, nsIDOMKeyEvent::DOM_VK_F5)
  KEY("F6", F6, nsIDOMKeyEvent::DOM_VK_F6)
  KEY("F7", F7, nsIDOMKeyEvent::DOM_VK_F7)
  KEY("F8", F8, nsIDOMKeyEvent::DOM_VK_F8)
  KEY("F9", F9, nsIDOMKeyEvent::DOM_VK_F9)
  KEY("F10", F10, nsIDOMKeyEvent::DOM_VK_F10)
  KEY("F11", F11, nsIDOMKeyEvent::DOM_VK_F11)
  KEY("F12", F12, nsIDOMKeyEvent::DOM_VK_F12)
  KEY("F13", F13, nsIDOMKeyEvent::DOM_VK_F13)
  KEY("F14", F14, nsIDOMKeyEvent::DOM_VK_F14)
  KEY("F15", F15, nsIDOMKeyEvent::DOM_VK_F15)
  KEY("F16", F16, nsIDOMKeyEvent::DOM_VK_F16)
  KEY("F17", F17, nsIDOMKeyEvent::DOM_VK_F17)
  KEY("F18", F18, nsIDOMKeyEvent::DOM_VK_F18)
  KEY("F19", F19, nsIDOMKeyEvent::DOM_VK_F19)
  KEY("F20", F20, nsIDOMKeyEvent::DOM_VK_F20)
  KEY("F21", F21, nsIDOMKeyEvent::DOM_VK_F21)
  KEY("F22", F22, nsIDOMKeyEvent::DOM_VK_F22)
  KEY("F23", F23, nsIDOMKeyEvent::DOM_VK_F23)
  KEY("F24", F24, nsIDOMKeyEvent::DOM_VK_F24)

  static_assert(nsIDOMKeyEvent::DOM_VK_0 == 48,
               "nsIDOMKeyEvent::DOM_VK_0 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_1 == 49,
               "nsIDOMKeyEvent::DOM_VK_1 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_2 == 50,
               "nsIDOMKeyEvent::DOM_VK_2 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_3 == 51,
               "nsIDOMKeyEvent::DOM_VK_3 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_4 == 52,
               "nsIDOMKeyEvent::DOM_VK_4 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_5 == 53,
               "nsIDOMKeyEvent::DOM_VK_5 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_6 == 54,
               "nsIDOMKeyEvent::DOM_VK_6 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_7 == 55,
               "nsIDOMKeyEvent::DOM_VK_7 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_8 == 56,
               "nsIDOMKeyEvent::DOM_VK_8 value changed");
  static_assert(nsIDOMKeyEvent::DOM_VK_9 == 57,
               "nsIDOMKeyEvent::DOM_VK_9 value changed");

  KEY("0", Digit0, nsIDOMKeyEvent::DOM_VK_0)
  KEY("1", Digit1, nsIDOMKeyEvent::DOM_VK_1)
  KEY("2", Digit2, nsIDOMKeyEvent::DOM_VK_2)
  KEY("3", Digit3, nsIDOMKeyEvent::DOM_VK_3)
  KEY("4", Digit4, nsIDOMKeyEvent::DOM_VK_4)
  KEY("5", Digit5, nsIDOMKeyEvent::DOM_VK_5)
  KEY("6", Digit6, nsIDOMKeyEvent::DOM_VK_6)
  KEY("7", Digit7, nsIDOMKeyEvent::DOM_VK_7)
  KEY("8", Digit8, nsIDOMKeyEvent::DOM_VK_8)
  KEY("9", Digit9, nsIDOMKeyEvent::DOM_VK_9)

  SHIFT(")", Digit0, nsIDOMKeyEvent::DOM_VK_0)
  SHIFT("!", Digit1, nsIDOMKeyEvent::DOM_VK_1)
  SHIFT("@", Digit2, nsIDOMKeyEvent::DOM_VK_2)
  SHIFT("#", Digit3, nsIDOMKeyEvent::DOM_VK_3)
  SHIFT("$", Digit4, nsIDOMKeyEvent::DOM_VK_4)
  SHIFT("%", Digit5, nsIDOMKeyEvent::DOM_VK_5)
  SHIFT("^", Digit6, nsIDOMKeyEvent::DOM_VK_6)
  SHIFT("&", Digit7, nsIDOMKeyEvent::DOM_VK_7)
  SHIFT("*", Digit8, nsIDOMKeyEvent::DOM_VK_8)
  SHIFT("(", Digit9, nsIDOMKeyEvent::DOM_VK_9)

  // Inverted exclamation mark (U+00A1)
  ALTGR(u"\u00A1", Digit1, nsIDOMKeyEvent::DOM_VK_1);
  SHIFT(u"\u00A1", Digit1, nsIDOMKeyEvent::DOM_VK_1);

  // Cent sign (U+00A2)
  ALTGR(u"\u00A2", Digit4, nsIDOMKeyEvent::DOM_VK_4);

  // Pound sign (U+00A3)
  ALTGR(u"\u00A3", Digit3, nsIDOMKeyEvent::DOM_VK_3);

  // Currency sign (U+00A4)
  SHIFT(u"\u00A4", Digit4, nsIDOMKeyEvent::DOM_VK_4);

  // Broken bar (U+00A6)
  ALTGR(u"\u00A6", Backquote, nsIDOMKeyEvent::DOM_VK_BACK_QUOTE);

  // Section sign (U+00A7)
  SHIFT(u"\u00A7", Backquote, nsIDOMKeyEvent::DOM_VK_BACK_QUOTE);

  // Diaeresis (Dead) (U+00A8)
  KEY(u"\u00A8", BracketRight, nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET);

  // Feminine ordinal indicator (U+00AA)
  SHIFT(u"\u00AA", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Left-pointing double angle quotation mark (U+00AB)
  KEY(u"\u00AB", IntlBackslash, DOM_VK_INTL_BACK_SLASH);

  // Not sign (U+00AC)
  ALTGR(u"\u00AC", Digit6, nsIDOMKeyEvent::DOM_VK_6);

  // Soft hyphen (U+00AD)
  SHIFT(u"\u00AD", Period, nsIDOMKeyEvent::DOM_VK_PERIOD)

  // Macron (U+00AF)
  ALTGR(u"\u00AF", Comma, nsIDOMKeyEvent::DOM_VK_COMMA)

  // Degree sign (U+00B0)
  ALTGR(u"\u00B0", Digit0, nsIDOMKeyEvent::DOM_VK_0);
  SHIFT(u"\u00B0", Digit0, nsIDOMKeyEvent::DOM_VK_0);

  // Plus-minus sign (U+00B1)
  ALTGR(u"\u00B1", Digit1, nsIDOMKeyEvent::DOM_VK_1);

  // Superscript two (U+00B2)
  ALTGR(u"\u00B2", Digit2, nsIDOMKeyEvent::DOM_VK_2);

  // Superscript three (U+00B3)
  ALTGR(u"\u00B3", Digit3, nsIDOMKeyEvent::DOM_VK_3);

  // Acute (Dead) (U+00B4)
  KEY(u"\u00B4", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);

  // Micro sign (U+00B5)
  ALTGR(u"\u00B5", KeyM, nsIDOMKeyEvent::DOM_VK_M);

  // Pilcrow sign (U+00B6)
  ALTGR(u"\u00B6", Digit7, nsIDOMKeyEvent::DOM_VK_7);

  // Middle dot (U+00B7)
  ALTGR(u"\u00B7", Digit8, nsIDOMKeyEvent::DOM_VK_8);

  // Cedilla (Dead) (U+00B8)
  ALTGR(u"\u00B8", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);

  // Superscript one (U+00B9)
  ALTGR(u"\u00B9", Digit1, nsIDOMKeyEvent::DOM_VK_1);

  // Masculine ordinal indicator (U+00BA)
  KEY(u"\u00BA", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Right-pointing double angle quotation mark (U+00BB)
  SHIFT(u"\u00BB", IntlBackslash, DOM_VK_INTL_BACK_SLASH);

  // Vulgar fraction one quarter (U+00BC)
  ALTGR(u"\u00BC", Digit4, nsIDOMKeyEvent::DOM_VK_4);

  // Vulgar fraction one half (U+00BD)
  SHIFT(u"\u00BD", Backquote, nsIDOMKeyEvent::DOM_VK_BACK_QUOTE);

  // Vulgar fraction three quarters (U+00BE)
  ALTGR(u"\u00BE", Digit6, nsIDOMKeyEvent::DOM_VK_6)

  // Inverted question mark (U+00BF)
  SHIFT(u"\u00BF", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);

  // Latin capital letter A with acute (U+00C1)
  ALTGR(u"\u00C1", KeyA, nsIDOMKeyEvent::DOM_VK_A)
  SHIFT(u"\u00C1", KeyA, nsIDOMKeyEvent::DOM_VK_A)

  // Latin capital letter A with circumflex (U+00C2)
  ALTGR(u"\u00C2", KeyQ, nsIDOMKeyEvent::DOM_VK_Q)
  SHIFT(u"\u00C2", KeyQ, nsIDOMKeyEvent::DOM_VK_Q)

  // Latin capital letter A with diaeresis (U+00C4)
  SHIFT(u"\u00C4", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin capital letter A with ring above (U+00C5)
  SHIFT(u"\u00C5", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)

  // Latin capital letter AE (U+00C6)
  SHIFT(u"\u00C6", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin capital letter C with cedilla (U+00C7)
  SHIFT(u"\u00C7", Backslash, nsIDOMKeyEvent::DOM_VK_BACK_SLASH);

  // Latin capital letter E with acute (U+00C9)
  ALTGR(u"\u00C9", KeyE, nsIDOMKeyEvent::DOM_VK_E)
  SHIFT(u"\u00C9", KeyE, nsIDOMKeyEvent::DOM_VK_E)

  // Latin capital letter I with acute (U+00CD)
  ALTGR(u"\u00CD", KeyI, nsIDOMKeyEvent::DOM_VK_I)
  SHIFT(u"\u00CD", KeyI, nsIDOMKeyEvent::DOM_VK_I)

  // Latin capital letter I with diaeresis (U+00CF)
  ALTGR(u"\u00CF", KeyI, nsIDOMKeyEvent::DOM_VK_I)
  SHIFT(u"\u00CF", KeyI, nsIDOMKeyEvent::DOM_VK_I)

  // Latin capital letter eth (U+00D0)
  ALTGR(u"\u00D0", KeyD, nsIDOMKeyEvent::DOM_VK_D)
  SHIFT(u"\u00D0", KeyD, nsIDOMKeyEvent::DOM_VK_D)

  // Latin capital letter N with tilde (U+00D1)
  SHIFT(u"\u00D1", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin capital letter O with acute (U+00D3)
  ALTGR(u"\u00D3", KeyO, nsIDOMKeyEvent::DOM_VK_O)
  SHIFT(u"\u00D3", KeyO, nsIDOMKeyEvent::DOM_VK_O)

  // Latin capital letter O with tilde (U+00D5)
  ALTGR(u"\u00D5", KeyO, nsIDOMKeyEvent::DOM_VK_O)
  SHIFT(u"\u00D5", KeyO, nsIDOMKeyEvent::DOM_VK_O)

  // Latin capital letter O with tilde (U+00D5)
  SHIFT(u"\u00D5", BracketRight, nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET);

  // Latin capital letter O with diaeresis (U+00D6)
  SHIFT(u"\u00D6", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin capital letter O with stroke (U+00D8)
  SHIFT(u"\u00D8", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin capital letter U with acute (U+00DA)
  ALTGR(u"\u00DA", KeyU, nsIDOMKeyEvent::DOM_VK_U)
  SHIFT(u"\u00DA", KeyU, nsIDOMKeyEvent::DOM_VK_U)

  // Latin capital letter U with diaeresis (U+00DC)
  SHIFT(u"\u00DC", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)

  // Latin capital letter thorn (U+00DE)
  ALTGR(u"\u00DE", KeyT, nsIDOMKeyEvent::DOM_VK_T)
  SHIFT(u"\u00DE", KeyT, nsIDOMKeyEvent::DOM_VK_T)

  // Latin small letter sharp S (U+00DF)
  ALTGR(u"\u00DF", KeyS, nsIDOMKeyEvent::DOM_VK_S)

  // Latin small letter A with grave (U+00E0)
  KEY(u"\u00E0", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin small letter A with acute (U+00E1)
  ALTGR(u"\u00E1", KeyA, nsIDOMKeyEvent::DOM_VK_A)

  // Latin small letter A with circumflex (U+00E2)
  ALTGR(u"\u00E2", KeyQ, nsIDOMKeyEvent::DOM_VK_Q)

  // Latin small letter A with diaeresis (U+00E4)
  KEY(u"\u00E4", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin small letter A with ring above (U+00E5)
  KEY(u"\u00E5", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)

  // Latin small letter AE (U+00E6)
  KEY(u"\u00E6", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin small letter C with cedilla (U+00E7)
  SHIFT(u"\u00E7", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin small letter E with grave (U+00E8)
  KEY(u"\u00E8", BracketRight, nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET);

  // Latin small letter E with acute (U+00E9)
  ALTGR(u"\u00E9", KeyE, nsIDOMKeyEvent::DOM_VK_E)

  // Latin small letter I with grave (U+00EC)
  KEY(u"\u00EC", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);

  // Latin small letter I with acute (U+00ED)
  ALTGR(u"\u00ED", KeyI, nsIDOMKeyEvent::DOM_VK_I)

  // Latin small letter I with diaeresis (U+00EF)
  ALTGR(u"\u00EF", KeyI, nsIDOMKeyEvent::DOM_VK_I)

  // Latin small letter eth (U+00F0)
  ALTGR(u"\u00F0", KeyD, nsIDOMKeyEvent::DOM_VK_D)

  // Latin small letter N with tilde (U+00F1)
  KEY(u"\u00F1", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin small letter O with grave (U+00F2)
  KEY(u"\u00F2", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin small letter O with acute (U+00F3)
  ALTGR(u"\u00F3", KeyO, nsIDOMKeyEvent::DOM_VK_O)

  // Latin small letter O with tilde (U+00F5)
  KEY(u"\u00F5", BracketRight, nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET);

  // Latin small letter O with diaeresis (U+00F6)
  KEY(u"\u00F6", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Division sign (U+00F7)
  ALTGR(u"\u00F7", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);
  SHIFT(u"\u00F7", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);

  // Latin small letter O with stroke (U+00F8)
  KEY(u"\u00F8", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin small letter U with grave (U+00F9)
  KEY(u"\u00F9", Backslash, nsIDOMKeyEvent::DOM_VK_BACK_SLASH);

  // Latin small letter U with acute (U+00FA)
  ALTGR(u"\u00FA", KeyU, nsIDOMKeyEvent::DOM_VK_U)

  // Latin small letter U with diaeresis (U+00FC)
  KEY(u"\u00FC", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)

  // Latin small letter thorn (U+00FE)
  ALTGR(u"\u00FE", KeyT, nsIDOMKeyEvent::DOM_VK_T)

  // Latin capital letter A with ogonek (U+0104)
  ALTGR(u"\u0104", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin small letter A with ogonek (U+0105)
  KEY(u"\u0105", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin capital letter C with acute (U+0106)
  ALTGR(u"\u0106", Slash, nsIDOMKeyEvent::DOM_VK_SLASH)
  SHIFT(u"\u0106", Slash, nsIDOMKeyEvent::DOM_VK_SLASH)

  // Latin small letter C with acute (U+0107)
  SHIFT(u"\u0107", Slash, nsIDOMKeyEvent::DOM_VK_SLASH)

  // Latin capital letter C with caron (U+010C)
  ALTGR(u"\u010C", KeyC, nsIDOMKeyEvent::DOM_VK_C)
  SHIFT(u"\u010C", KeyC, nsIDOMKeyEvent::DOM_VK_C)

  // Latin small letter C with caron (U+010D)
  ALTGR(u"\u010D", KeyC, nsIDOMKeyEvent::DOM_VK_C)

  // Latin capital letter E with ogonek (U+0118)
  ALTGR(u"\u0118", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);
  SHIFT(u"\u0118", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin small letter E with ogonek (U+0119)
  SHIFT(u"\u0119", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin small letter dotless I (U+0131)
  ALTGR(u"\u0131", KeyI, nsIDOMKeyEvent::DOM_VK_I)

  // Latin small letter Kra (U+0138)
  ALTGR(u"\u0138", KeyK, nsIDOMKeyEvent::DOM_VK_K)

  // Latin capital letter L with stroke (U+0141)
  SHIFT(u"\u0141", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin small letter L with stroke (U+0142)
  KEY(u"\u0142", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin capital letter N with acute (U+0143)
  ALTGR(u"\u0143", Period, nsIDOMKeyEvent::DOM_VK_PERIOD)
  SHIFT(u"\u0143", Period, nsIDOMKeyEvent::DOM_VK_PERIOD)

  // Latin small letter N with acute (U+0144)
  SHIFT(u"\u0144", Period, nsIDOMKeyEvent::DOM_VK_PERIOD)

  // Latin capital letter Eng (U+014A)
  ALTGR(u"\u014A", KeyN, nsIDOMKeyEvent::DOM_VK_N)
  SHIFT(u"\u014A", KeyN, nsIDOMKeyEvent::DOM_VK_N)

  // Latin small letter Eng (U+014B)
  ALTGR(u"\u014B", KeyN, nsIDOMKeyEvent::DOM_VK_N)

  // Latin capital ligature OE (U+0152)
  ALTGR(u"\u0152", KeyO, nsIDOMKeyEvent::DOM_VK_O)
  SHIFT(u"\u0152", KeyO, nsIDOMKeyEvent::DOM_VK_O)

  // Latin small ligature OE (U+0153)
  ALTGR(u"\u0153", KeyO, nsIDOMKeyEvent::DOM_VK_O)

  // Latin capital letter S with acute (U+015A)
  ALTGR(u"\u015A", Comma, nsIDOMKeyEvent::DOM_VK_COMMA)
  SHIFT(u"\u015A", Comma, nsIDOMKeyEvent::DOM_VK_COMMA)

  // Latin small letter S with acute (U+015B)
  SHIFT(u"\u015B", Comma, nsIDOMKeyEvent::DOM_VK_COMMA)

  // Latin capital letter S with cedilla (U+015E)
  SHIFT(u"\u015E", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin small letter S with cedilla (U+015F)
  KEY(u"\u015F", Semicolon, nsIDOMKeyEvent::DOM_VK_SEMICOLON)

  // Latin capital letter S with caron (U+0160)
  ALTGR(u"\u0160", KeyS, nsIDOMKeyEvent::DOM_VK_S)
  SHIFT(u"\u0160", KeyS, nsIDOMKeyEvent::DOM_VK_S)

  // Latin small letter S with caron (U+0161)
  ALTGR(u"\u0161", KeyS, nsIDOMKeyEvent::DOM_VK_S)

  // Latin capital letter T with stroke (U+0166)
  ALTGR(u"\u0166", KeyT, nsIDOMKeyEvent::DOM_VK_T)
  SHIFT(u"\u0166", KeyT, nsIDOMKeyEvent::DOM_VK_T)

  // Latin small letter T with stroke (U+0167)
  ALTGR(u"\u0167", KeyT, nsIDOMKeyEvent::DOM_VK_T)

  // Latin capital letter Z with acute (U+0179)
  ALTGR(u"\u0179", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)
  SHIFT(u"\u0179", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)

  // Latin small letter Z with acute (U+017A)
  SHIFT(u"\u017A", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)

  // Latin capital letter Z with dot above (U+017B)
  ALTGR(u"\u017B", Minus, nsIDOMKeyEvent::DOM_VK_HYPHEN_MINUS)
  SHIFT(u"\u017B", Minus, nsIDOMKeyEvent::DOM_VK_HYPHEN_MINUS)

  // Latin small letter Z with dot above (U+017C)
  ALTGR(u"\u017C", Minus, nsIDOMKeyEvent::DOM_VK_HYPHEN_MINUS)

  // Latin capital letter Z with caron (U+017D)
  ALTGR(u"\u017D", KeyZ, nsIDOMKeyEvent::DOM_VK_Z)
  SHIFT(u"\u017D", KeyZ, nsIDOMKeyEvent::DOM_VK_Z)

  // Latin small letter Z with caron (U+017E)
  ALTGR(u"\u017E", KeyZ, nsIDOMKeyEvent::DOM_VK_Z)

  // Latin capital letter schwa (U+018F)
  ALTGR(u"\u018F", KeyA, nsIDOMKeyEvent::DOM_VK_A)
  SHIFT(u"\u018F", KeyA, nsIDOMKeyEvent::DOM_VK_A)

  // Latin capital letter ezh (U+01B7)
  ALTGR(u"\u01B7", KeyZ, nsIDOMKeyEvent::DOM_VK_Z)
  SHIFT(u"\u01B7", KeyZ, nsIDOMKeyEvent::DOM_VK_Z)

  // Latin capital letter A with caron (U+01CD)
  ALTGR(u"\u01CD", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);
  SHIFT(u"\u01CD", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin small letter A with caron (U+01CE)
  ALTGR(u"\u01CE", Quote, nsIDOMKeyEvent::DOM_VK_QUOTE);

  // Latin capital letter G with stroke (U+01E4)
  ALTGR(u"\u01E4", KeyF, nsIDOMKeyEvent::DOM_VK_F)
  SHIFT(u"\u01E4", KeyF, nsIDOMKeyEvent::DOM_VK_F)

  // Latin small letter G with stroke (U+01E5)
  ALTGR(u"\u01E5", KeyF, nsIDOMKeyEvent::DOM_VK_F)

  // Latin capital letter G with caron (U+01E6)
  ALTGR(u"\u01E6", KeyG, nsIDOMKeyEvent::DOM_VK_G)
  SHIFT(u"\u01E6", KeyG, nsIDOMKeyEvent::DOM_VK_G)

  // Latin small letter G with caron (U+01E7)
  ALTGR(u"\u01E7", KeyG, nsIDOMKeyEvent::DOM_VK_G)

  // Latin capital letter K with caron (U+01E8)
  ALTGR(u"\u01E8", KeyK, nsIDOMKeyEvent::DOM_VK_K)
  SHIFT(u"\u01E8", KeyK, nsIDOMKeyEvent::DOM_VK_K)

  // Latin small letter K with caron (U+01E9)
  ALTGR(u"\u01E9", KeyK, nsIDOMKeyEvent::DOM_VK_K)

  // Latin capital letter ezh with caron (U+01EE)
  ALTGR(u"\u01EE", KeyV, nsIDOMKeyEvent::DOM_VK_V)
  SHIFT(u"\u01EE", KeyV, nsIDOMKeyEvent::DOM_VK_V)

  // Latin small letter ezh with caron (U+01EF)
  ALTGR(u"\u01EF", KeyV, nsIDOMKeyEvent::DOM_VK_V)

  // Latin capital letter H with caron (U+021E)
  ALTGR(u"\u021E", KeyH, nsIDOMKeyEvent::DOM_VK_H)
  SHIFT(u"\u021E", KeyH, nsIDOMKeyEvent::DOM_VK_H)

  // Latin small letter H with caron (U+021F)
  ALTGR(u"\u021F", KeyH, nsIDOMKeyEvent::DOM_VK_H)

  // Latin small letter schwa (U+0259)
  ALTGR(u"\u0259", KeyA, nsIDOMKeyEvent::DOM_VK_A)

  // Latin small letter ezh (U+0292)
  ALTGR(u"\u0292", KeyZ, nsIDOMKeyEvent::DOM_VK_Z)

  // Modifier letter circumflex (Dead) (U+02C6)
  SHIFT(u"\u02C6", BracketRight, nsIDOMKeyEvent::DOM_VK_CLOSE_BRACKET);

  // Caron (Dead) (U+02C7)
  SHIFT(u"\u02C7", Backslash, nsIDOMKeyEvent::DOM_VK_BACK_SLASH);
  ALTGR(u"\u02C7", Backslash, nsIDOMKeyEvent::DOM_VK_BACK_SLASH);

  // Modifier letter grave (Dead) (U+02CB)
  SHIFT(u"\u02CB", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);

  // Breve (Dead) (U+02D8)
  SHIFT(u"\u02D8", Backslash, nsIDOMKeyEvent::DOM_VK_BACK_SLASH);
  ALTGR(u"\u02D8", Backslash, nsIDOMKeyEvent::DOM_VK_BACK_SLASH);

  // Ogonek (Dead) (U+02DB)
  ALTGR(u"\u02DB", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);
  SHIFT(u"\u02DB", Equal, nsIDOMKeyEvent::DOM_VK_EQUALS);

  // Double acute (Dead) (U+02DD)
  ALTGR(u"\u02DD", BracketLeft, nsIDOMKeyEvent::DOM_VK_OPEN_BRACKET)

  // Combining horn above (Dead) (U+0309)
  SHIFT(u"\u0309", KeyP, nsIDOMKeyEvent::DOM_VK_P)
  ALTGR(u"\u0309", KeyP, nsIDOMKeyEvent::DOM_VK_P)

  // Combining horn (Dead) (U+031B)
  ALTGR(u"\u031B", KeyP, nsIDOMKeyEvent::DOM_VK_P)

  // Per mille sign (U+2030)
  ALTGR(u"\u2030", Digit5, nsIDOMKeyEvent::DOM_VK_5)

  // Euro sign (U+20AC)
  ALTGR(u"\u20AC", KeyE, nsIDOMKeyEvent::DOM_VK_E)

  // Trade mark sign (U+2122)
  ALTGR(u"\u2122", Digit6, nsIDOMKeyEvent::DOM_VK_6)
}
