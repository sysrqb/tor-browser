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
// ALTGR indicates that shift is off, altgr is on.
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

  KEY("Alt", AltLeft, 18)
  KEY("ArrowDown", ArrowDown, 40)
  KEY("ArrowLeft", ArrowLeft, 37)
  KEY("ArrowRight", ArrowRight, 39)
  KEY("ArrowUp", ArrowUp, 38)
  KEY("Backspace", Backspace, 8)
  KEY("CapsLock", CapsLock, 20)
  KEY("ContextMenu", ContextMenu, 93)
  KEY("Control", ControlLeft, 17)
  KEY("Delete", Delete, 46)
  KEY("End", End, 35)
  KEY("Enter", Enter, 13)
  KEY("Escape", Escape, 27)
  KEY("Help", Help, 6)
  KEY("Home", Home, 36)
  KEY("Insert", Insert, 45)
  KEY("Meta", OSLeft, 91)
  KEY("PageDown", PageDown, 34)
  KEY("PageUp", PageUp, 33)
  KEY("Pause", Pause, 19)
  KEY("PrintScreen", PrintScreen, 44)
  KEY("ScrollLock", ScrollLock, 145)
  KEY("Shift", ShiftLeft, 16)
  KEY("Tab", Tab, 9)
  // Leaving "Clear" key unimplemented; it's inconsistent between platforms.

  KEY(" ", Space, 32)
  KEY(",", Comma, 188)
  SHIFT("<", Comma, 188)
  KEY(".", Period, 190)
  SHIFT(">", Period, 190)
  KEY("/", Slash, 191)
  SHIFT("?", Slash, 191)
  KEY(";", Semicolon, 59)
  SHIFT(":", Semicolon, 59)
  KEY("'", Quote, 222)
  SHIFT("\"", Quote, 222)
  KEY("[", BracketLeft, 219)
  SHIFT("{", BracketLeft, 219)
  KEY("]", BracketRight, 221)
  SHIFT("}", BracketRight, 221)
  KEY("`", Backquote, 192)
  SHIFT("~", Backquote, 192)
  KEY("\\", Backslash, 220)
  SHIFT("|", Backslash, 220)
  KEY("-", Minus, 173)
  SHIFT("_", Minus, 173)
  KEY("=", Equal, 61)
  SHIFT("+", Equal, 61)

  SHIFT("A", KeyA, 65)
  SHIFT("B", KeyB, 66)
  SHIFT("C", KeyC, 67)
  SHIFT("D", KeyD, 68)
  SHIFT("E", KeyE, 69)
  SHIFT("F", KeyF, 70)
  SHIFT("G", KeyG, 71)
  SHIFT("H", KeyH, 72)
  SHIFT("I", KeyI, 73)
  SHIFT("J", KeyJ, 74)
  SHIFT("K", KeyK, 75)
  SHIFT("L", KeyL, 76)
  SHIFT("M", KeyM, 77)
  SHIFT("N", KeyN, 78)
  SHIFT("O", KeyO, 79)
  SHIFT("P", KeyP, 80)
  SHIFT("Q", KeyQ, 81)
  SHIFT("R", KeyR, 82)
  SHIFT("S", KeyS, 83)
  SHIFT("T", KeyT, 84)
  SHIFT("U", KeyU, 85)
  SHIFT("V", KeyV, 86)
  SHIFT("W", KeyW, 87)
  SHIFT("X", KeyX, 88)
  SHIFT("Y", KeyY, 89)
  SHIFT("Z", KeyZ, 90)

  KEY("a", KeyA, 65)
  KEY("b", KeyB, 66)
  KEY("c", KeyC, 67)
  KEY("d", KeyD, 68)
  KEY("e", KeyE, 69)
  KEY("f", KeyF, 70)
  KEY("g", KeyG, 71)
  KEY("h", KeyH, 72)
  KEY("i", KeyI, 73)
  KEY("j", KeyJ, 74)
  KEY("k", KeyK, 75)
  KEY("l", KeyL, 76)
  KEY("m", KeyM, 77)
  KEY("n", KeyN, 78)
  KEY("o", KeyO, 79)
  KEY("p", KeyP, 80)
  KEY("q", KeyQ, 81)
  KEY("r", KeyR, 82)
  KEY("s", KeyS, 83)
  KEY("t", KeyT, 84)
  KEY("u", KeyU, 85)
  KEY("v", KeyV, 86)
  KEY("w", KeyW, 87)
  KEY("x", KeyX, 88)
  KEY("y", KeyY, 89)
  KEY("z", KeyZ, 90)

  KEY("F1", F1, 112)
  KEY("F2", F2, 113)
  KEY("F3", F3, 114)
  KEY("F4", F4, 115)
  KEY("F5", F5, 116)
  KEY("F6", F6, 117)
  KEY("F7", F7, 118)
  KEY("F8", F8, 119)
  KEY("F9", F9, 120)
  KEY("F10", F10, 121)
  KEY("F11", F11, 122)
  KEY("F12", F12, 123)
  KEY("F13", F13, 124)
  KEY("F14", F14, 125)
  KEY("F15", F15, 126)
  KEY("F16", F16, 127)
  KEY("F17", F17, 128)
  KEY("F18", F18, 129)
  KEY("F19", F19, 130)
  KEY("F20", F20, 131)
  KEY("F21", F21, 132)
  KEY("F22", F22, 133)
  KEY("F23", F23, 134)
  KEY("F24", F24, 135)

  KEY("0", Digit0, 48)
  KEY("1", Digit1, 49)
  KEY("2", Digit2, 50)
  KEY("3", Digit3, 51)
  KEY("4", Digit4, 52)
  KEY("5", Digit5, 53)
  KEY("6", Digit6, 54)
  KEY("7", Digit7, 55)
  KEY("8", Digit8, 56)
  KEY("9", Digit9, 57)

  SHIFT(")", Digit0, 48)
  SHIFT("!", Digit1, 49)
  SHIFT("@", Digit2, 50)
  SHIFT("#", Digit3, 51)
  SHIFT("$", Digit4, 52)
  SHIFT("%", Digit5, 53)
  SHIFT("^", Digit6, 54)
  SHIFT("&", Digit7, 55)
  SHIFT("*", Digit8, 56)
  SHIFT("(", Digit9, 57)

  // Inverted exclamation mark (U+00A1)
  // Key of the Spanish keyboard layout
  KEY(u"\u00A1", Equal, 61);

  // Cent sign (U+00A2)
  // Not very common, but it is found at
  // this key on the Canadian French keyboard
  ALTGR(u"\u00A2", Digit4, 52);

  // Pound sign (U+00A3)
  // Key of the UK keyboard layout
  SHIFT(u"\u00A3", Digit3, 51);

  // Currency sign (U+00A4)
  // Key of Danish, Estonian, Faroese,
  // Norwegian, and Swedish keyboard layouts
  SHIFT(u"\u00A4", Digit4, 52);

  // Broken bar (U+00A6)
  // Irish and UK keyboards
  ALTGR(u"\u00A6", Backquote, 192);

  // Section sign (U+00A7)
  // Danish, Dutch, Faroese, Norwegian keyboards
  SHIFT(u"\u00A7", Backquote, 192);

  // Diaeresis (umlaut) (Dead key) (U+00A8)
  // Dutch, Finnish, Swedish keyboard layouts
  // The Danish layout uses shift
  KEY(u"\u00A8", BracketRight, 221);

  // Feminine ordinal indicator (U+00AA)
  // Brazilian keyboard layout
  ALTGR(u"\u00AA", BracketRight, 221);

  // Left-pointing double angle quotation mark (U+00AB)
  // Canadian French keyboard layout
  // It seems like many languages use this character,
  // but it is strangely absent from most keyboards.
  KEY(u"\u00AB", IntlBackslash, 220);

  // Not sign (U+00AC)
  // Canadian French, Brazilian, Spanish keyboards
  ALTGR(u"\u00AC", Digit6, 54);

  // Soft hyphen (U+00AD)
  // Canadian French keyboard
  SHIFT(u"\u00AD", Period, 190)

  // Macron (U+00AF)
  // TODO needs more information
  // ALTGR(u"\u00AF", Comma, 188)

  // Degree sign (U+00B0)
  // Finnish keyboard layout
  ALTGR(u"\u00B0", Digit0, 48);
  SHIFT(u"\u00B0", Digit0, 48);

  // Plus-minus sign (U+00B1)
  // TODO needs more information
  // ALTGR(u"\u00B1", Digit1, 49);

  // Superscript two (U+00B2)
  // Brazilian, Dutch, German keyboard layouts
  ALTGR(u"\u00B2", Digit2, 50);

  // Superscript three (U+00B3)
  // Brazilian, Dutch, German keyboard layouts
  ALTGR(u"\u00B3", Digit3, 51);

  // Acute (Dead) (U+00B4)
 // Danish, Estonian, Faroese, Finnish, Swedish keyboard layouts
  KEY(u"\u00B4", Equal, 61);

  // Micro sign (U+00B5)
  // Danish, Dutch, Faroese, Canadian-French, German, Icelandic,
  // Norwegian, Wedish keyboard layout
  ALTGR(u"\u00B5", KeyM, 77);

  // Pilcrow sign (U+00B6)
  // TODO needs more information
  // ALTGR(u"\u00B6", Digit7, 55);

  // Middle dot (U+00B7)
  // TODO needs more information
  // ALTGR(u"\u00B7", Digit8, 56);

  // Cedilla (Dead) (U+00B8)
  // Dutch, Finnish keybaord layout
  ALTGR(u"\u00B8", Equal, 61);

  // Superscript one (U+00B9)
  // Brazilian, Dutch keyboard layout
  ALTGR(u"\u00B9", Digit1, 49);

  // Masculine ordinal indicator (U+00BA)
  // Brazilian keyboard layout
  KEY(u"\u00BA", Quote, 222);

  // Right-pointing double angle quotation mark (U+00BB)
  // Canadian-French keyboard layout
  // See comment on U+00AB
  SHIFT(u"\u00BB", IntlBackslash, 220);

  // Vulgar fraction one quarter (U+00BC)
  // TODO needs more information
  //ALTGR(u"\u00BC", Digit4, 52);

  // Vulgar fraction one half (U+00BD)
  // Turkish keyboard layout
  ALTGR(u"\u00BD", Digit5, 53);

  // Vulgar fraction three quarters (U+00BE)
  // TODO needs more information
  // ALTGR(u"\u00BE", Digit6, 54)

  // Inverted question mark (U+00BF)
  // Spanish keyboard layout
  SHIFT(u"\u00BF", Equal, 61);

  // Latin capital letter A with acute (U+00C1)
  // Irish, Norwegian, UK keyboard layouts
  ALTGR(u"\u00C1", KeyA, 65)
  SHIFT(u"\u00C1", KeyA, 65)

  // Latin capital letter A with circumflex (U+00C2)
  // Norwegian keyboard layout
  ALTGR(u"\u00C2", KeyQ, 81)
  SHIFT(u"\u00C2", KeyQ, 81)

  // Latin capital letter A with diaeresis (U+00C4)
  // Estonian, Finnish, German, Swedish keyboard layouts
  SHIFT(u"\u00C4", Quote, 222);

  // Latin capital letter A with ring above (U+00C5)
  // Danish, Faroese, Finnish, Norwegian, Swdish keyboard layouts
  SHIFT(u"\u00C5", BracketLeft, 219)

  // Latin capital letter AE (U+00C6)
  // Danish, Faroese, Icelandic keyboard layouts
  SHIFT(u"\u00C6", Semicolon, 59)

  // Latin capital letter C with cedilla (U+00C7)
  // Spanish, Turkish keyboard layouts
  SHIFT(u"\u00C7", Backslash, 220);

  // Latin capital letter E with acute (U+00C9)
  // Irish, UK keyboard layouts
  ALTGR(u"\u00C9", KeyE, 69)
  SHIFT(u"\u00C9", KeyE, 69)

  // Latin capital letter I with acute (U+00CD)
  // Irish, UK keyboard layouts
  ALTGR(u"\u00CD", KeyI, 73)
  SHIFT(u"\u00CD", KeyI, 73)

  // Latin capital letter I with diaeresis (U+00CF)
  // TODO needs more information
  // ALTGR(u"\u00CF", KeyI, 73)
  // SHIFT(u"\u00CF", KeyI, 73)

  // Latin capital letter eth (U+00D0)
  // Finnish, Norwegian keyboard layouts
  ALTGR(u"\u00D0", KeyD, 68)
  SHIFT(u"\u00D0", KeyD, 68)

  // Latin capital letter N with tilde (U+00D1)
  // Spanish keyboard layout
  SHIFT(u"\u00D1", Semicolon, 59)

  // Latin capital letter O with acute (U+00D3)
  // Irish, UK, Norwegian keyboard layouts
  ALTGR(u"\u00D3", KeyO, 79)
  SHIFT(u"\u00D3", KeyO, 79)

  // Latin capital letter O with tilde (U+00D5)
  // TODO needs more information
  // ALTGR(u"\u00D5", KeyO, 79)
  // SHIFT(u"\u00D5", KeyO, 79)

  // Latin capital letter O with diaeresis (U+00D6)
  // Estonian, Finnish, German, Swedish keyboard layouts
  SHIFT(u"\u00D6", Semicolon, 59)

  // Latin capital letter O with stroke (U+00D8)
  // TODO needs more information
  // SHIFT(u"\u00D8", Quote, 222);

  // Latin capital letter U with acute (U+00DA)
  // Irish, UK keyboard layouts
  ALTGR(u"\u00DA", KeyU, 85)
  SHIFT(u"\u00DA", KeyU, 85)

  // Latin capital letter U with diaeresis (U+00DC)
  // Estonian, German keyboard layout
  SHIFT(u"\u00DC", BracketLeft, 219)

  // Latin capital letter thorn (U+00DE)
  // Finnish keyboard layout
  ALTGR(u"\u00DE", KeyT, 84)
  SHIFT(u"\u00DE", KeyT, 84)

  // Latin small letter sharp S (U+00DF)
  // Dutch, Finnish, Turkish keyboard layout
  ALTGR(u"\u00DF", KeyS, 83)

  // Latin small letter A with grave (U+00E0)
  // Italian keyboard layout
  KEY(u"\u00E0", Quote, 222);

  // Latin small letter A with acute (U+00E1)
  // Irish, Norwegian, UK keyboard layouts
  ALTGR(u"\u00E1", KeyA, 65)

  // Latin small letter A with circumflex (U+00E2)
  // Norwegian keyboard layout
  ALTGR(u"\u00E2", KeyQ, 81)

  // Latin small letter A with diaeresis (U+00E4)
  // Estonian, Finnish, German, Swedish keyboard layouts
  KEY(u"\u00E4", Quote, 222);

  // Latin small letter A with ring above (U+00E5)
  // Danish, Faroese, Finnish, Norwegian, Swedish keyboard
  // layouts
  KEY(u"\u00E5", BracketLeft, 219)

  // Latin small letter AE (U+00E6)
  // Turkish keyboard layout
  ALTGR(u"\u00E6", KeyA, 65)
  SHIFT(u"\u00E6", KeyA, 65)

  // Latin small letter C with cedilla (U+00E7)
  // Brazilian, Italian, Portuguese keyboard layouts
  SHIFT(u"\u00E7", Semicolon, 59)

  // Latin small letter E with grave (U+00E8)
  // Italian keyboard layout
  KEY(u"\u00E8", BracketLeft, 219);

  // Latin small letter E with acute (U+00E9)
  // Turkish keyboard layout
  SHIFT(u"\u00E9", Backquote, 192)

  // Latin small letter I with grave (U+00EC)
  // Italian keyboard layout
  KEY(u"\u00EC", Equal, 61);

  // Latin small letter I with acute (U+00ED)
  // Irish, UK keyboard layouts
  ALTGR(u"\u00ED", KeyI, 73)

  // Latin small letter I with diaeresis (U+00EF)
  // Norwegian keyboard layout
  ALTGR(u"\u00EF", KeyI, 73)

  // Latin small letter eth (U+00F0)
  // Finnish, Norwegian keyboard layouts
  ALTGR(u"\u00F0", KeyD, 68)

  // Latin small letter N with tilde (U+00F1)
  // Spanish keyboard layout
  KEY(u"\u00F1", Semicolon, 59)

  // Latin small letter O with grave (U+00F2)
  // Italisn keyboard layout
  KEY(u"\u00F2", Semicolon, 59)

  // Latin small letter O with acute (U+00F3)
  // Portuguese keyboard layout
  KEY(u"\u00F3", BracketLeft, 219)

  // Latin small letter O with tilde (U+00F5)
  // TODO needs more information
  // KEY(u"\u00F5", BracketRight, 221);

  // Latin small letter O with diaeresis (U+00F6)
  // Estonian, FInnish, German, Swedish keyboard layouts
  KEY(u"\u00F6", Semicolon, 59)

  // Division sign (U+00F7)
  // TODO needs more information
  // ALTGR(u"\u00F7", Equal, 61);
  // SHIFT(u"\u00F7", Equal, 61);

  // Latin small letter O with stroke (U+00F8)
  // Danish, Faroese keyboard layouts
  KEY(u"\u00F8", Quote, 222);

  // Latin small letter U with grave (U+00F9)
  // Italisn keyboard layout
  KEY(u"\u00F9", Backslash, 220);

  // Latin small letter U with acute (U+00FA)
  // Irish, UK keyboard layouts
  ALTGR(u"\u00FA", KeyU, 85)

  // Latin small letter U with diaeresis (U+00FC)
  // Estonian, German keyboard layouts
  KEY(u"\u00FC", BracketLeft, 219)

  // Latin small letter thorn (U+00FE)
  // Finnish keyboard layout
  ALTGR(u"\u00FE", KeyT, 84)

  // Latin capital letter A with ogonek (U+0104)
  // TODO needs more information
  // ALTGR(u"\u0104", Quote, 222);

  // Latin small letter A with ogonek (U+0105)
  // TODO needs more information
  // KEY(u"\u0105", Quote, 222);

  // Latin capital letter C with acute (U+0106)
  // TODO needs more information
  // ALTGR(u"\u0106", Slash, 191)
  // SHIFT(u"\u0106", Slash, 191)

  // Latin small letter C with acute (U+0107)
  // TODO needs more information
  // SHIFT(u"\u0107", Slash, 191)

  // Latin capital letter C with caron (U+010C)
  // Norwegian keyboard layout
  ALTGR(u"\u010C", KeyC, 67)
  SHIFT(u"\u010C", KeyC, 67)

  // Latin small letter C with caron (U+010D)
  // Norwegian keyboard layout
  ALTGR(u"\u010D", KeyC, 67)

  // Latin capital letter E with ogonek (U+0118)
  // TODO needs more information
  // ALTGR(u"\u0118", Quote, 222);
  // SHIFT(u"\u0118", Quote, 222);

  // Latin small letter E with ogonek (U+0119)
  // TODO needs more information
  // SHIFT(u"\u0119", Quote, 222);

  // Latin small letter dotless I (U+0131)
  // Turkish keyboard layout
  KEY(u"\u0131", KeyI, 73)

  // Latin small letter Kra (U+0138)
  // Finnish keyboard layout
  ALTGR(u"\u0138", KeyK, 75)

  // Latin capital letter L with stroke (U+0141)
  // TODO needs more information
  // SHIFT(u"\u0141", Semicolon, 59)

  // Latin small letter L with stroke (U+0142)
  // TODO needs more information
  // KEY(u"\u0142", Semicolon, 59)

  // Latin capital letter N with acute (U+0143)
  // TODO needs more information
  // ALTGR(u"\u0143", Period, 190)
  // SHIFT(u"\u0143", Period, 190)

  // Latin small letter N with acute (U+0144)
  // TODO needs more information
  // SHIFT(u"\u0144", Period, 190)

  // Latin capital letter Eng (U+014A)
  // Norwegian keyboard layout
  ALTGR(u"\u014A", KeyN, 78)
  SHIFT(u"\u014A", KeyN, 78)

  // Latin small letter Eng (U+014B)
  // Norwegian keyboard layout
  ALTGR(u"\u014B", KeyN, 78)

  // Latin capital ligature OE (U+0152)
  // Finnish keyboard layout
  ALTGR(u"\u0152", KeyO, 79)
  SHIFT(u"\u0152", KeyO, 79)

  // Latin small ligature OE (U+0153)
  // Finnish keyboard layout
  ALTGR(u"\u0153", KeyO, 79)

  // Latin capital letter S with acute (U+015A)
  // TODO needs more information
  // ALTGR(u"\u015A", Comma, 188)
  // SHIFT(u"\u015A", Comma, 188)

  // Latin small letter S with acute (U+015B)
  // TODO needs more information
  // SHIFT(u"\u015B", Comma, 188)

  // Latin capital letter S with cedilla (U+015E)
  // Turkish keyboard layout
  SHIFT(u"\u015E", Semicolon, 59)

  // Latin small letter S with cedilla (U+015F)
  // Turkish keyboard layout
  KEY(u"\u015F", Semicolon, 59)

  // Latin capital letter S with caron (U+0160)
  // Estonian, Norwegian keyboard layouts
  ALTGR(u"\u0160", KeyS, 83)
  SHIFT(u"\u0160", KeyS, 83)

  // Latin small letter S with caron (U+0161)
  // Estonian, Norwegian keyboard layouts
  ALTGR(u"\u0161", KeyS, 83)

  // Latin capital letter T with stroke (U+0166)
  // Norwegian keyboard layout
  ALTGR(u"\u0166", KeyT, 84)
  SHIFT(u"\u0166", KeyT, 84)

  // Latin small letter T with stroke (U+0167)
  // Norwegian keyboard layout
  ALTGR(u"\u0167", KeyT, 84)

  // Latin capital letter Z with acute (U+0179)
  // TODO needs more information
  // ALTGR(u"\u0179", BracketLeft, 219)
  // SHIFT(u"\u0179", BracketLeft, 219)

  // Latin small letter Z with acute (U+017A)
  // TODO needs more information
  // SHIFT(u"\u017A", BracketLeft, 219)

  // Latin capital letter Z with dot above (U+017B)
  // TODO needs more information
  // ALTGR(u"\u017B", Minus, 173)
  // SHIFT(u"\u017B", Minus, 173)

  // Latin small letter Z with dot above (U+017C)
  // TODO needs more information
  // ALTGR(u"\u017C", Minus, 173)

  // Latin capital letter Z with caron (U+017D)
  // Estonian, Norwegian keyboard layouts
  ALTGR(u"\u017D", KeyZ, 90)
  SHIFT(u"\u017D", KeyZ, 90)

  // Latin small letter Z with caron (U+017E)
  // Estonian, Norwegian keyboard layouts
  ALTGR(u"\u017E", KeyZ, 90)

  // Latin capital letter schwa (U+018F)
  // Finnish keyboard layout
  ALTGR(u"\u018F", KeyA, 65)
  SHIFT(u"\u018F", KeyA, 65)

  // Latin capital letter ezh (U+01B7)
  // TODO needs more information
  // ALTGR(u"\u01B7", KeyZ, 90)
  // SHIFT(u"\u01B7", KeyZ, 90)

  // Latin capital letter A with caron (U+01CD)
  // Norwegian keyboard layout
  ALTGR(u"\u01CD", Quote, 222);
  SHIFT(u"\u01CD", Quote, 222);

  // Latin small letter A with caron (U+01CE)
  // Norwegian keyboard layout
  ALTGR(u"\u01CE", Quote, 222);

  // Latin capital letter G with stroke (U+01E4)
  // Norwegian keyboard layout
  ALTGR(u"\u01E4", KeyF, 70)
  SHIFT(u"\u01E4", KeyF, 70)

  // Latin small letter G with stroke (U+01E5)
  // Norwegian keyboard layout
  ALTGR(u"\u01E5", KeyF, 70)

  // Latin capital letter G with caron (U+01E6)
  // Turkish keyboard layout
  SHIFT(u"\u01E6", BracketLeft, 219)

  // Latin small letter G with caron (U+01E7)
  // Turkish keyboard layout
  KEY(u"\u01E6", BracketLeft, 219)

  // Latin capital letter K with caron (U+01E8)
  // Norwegian keyboard layout
  ALTGR(u"\u01E8", KeyK, 75)
  SHIFT(u"\u01E8", KeyK, 75)

  // Latin small letter K with caron (U+01E9)
  // Norwegian keyboard layout
  ALTGR(u"\u01E9", KeyK, 75)

  // Latin capital letter ezh with caron (U+01EE)
  // Norwegian keyboard layout
  ALTGR(u"\u01EE", KeyV, 86)
  SHIFT(u"\u01EE", KeyV, 86)

  // Latin small letter ezh with caron (U+01EF)
  // Norwegian keyboard layout
  ALTGR(u"\u01EF", KeyV, 86)

  // Latin capital letter H with caron (U+021E)
  // Norwegian keyboard layout
  ALTGR(u"\u021E", KeyH, 72)
  SHIFT(u"\u021E", KeyH, 72)

  // Latin small letter H with caron (U+021F)
  // Norwegian keyboard layout
  ALTGR(u"\u021F", KeyH, 72)

  // Latin small letter schwa (U+0259)
  // Finnish keyboard layout
  ALTGR(u"\u0259", KeyA, 65)

  // Latin small letter ezh (U+0292)
  // TODO needs more information
  // ALTGR(u"\u0292", KeyZ, 90)

  // Modifier letter circumflex (Dead) (U+02C6)
  // Danish, Finnish, Swedish key8board layouts
  SHIFT(u"\u02C6", BracketRight, 221);

  // Caron (Dead) (U+02C7)
  // Finnish keyboard layout
  SHIFT(u"\u02C7", Backslash, 220);
  ALTGR(u"\u02C7", Backslash, 220);

  // Modifier letter grave (Dead) (U+02CB)
  // Danish, Estonian, Faroese, Finnish, Swedish keyboard layouts
  SHIFT(u"\u02CB", Equal, 61);

  // Breve (Dead) (U+02D8)
  // Finnish keyboard layout
  SHIFT(u"\u02D8", Backslash, 220);
  ALTGR(u"\u02D8", Backslash, 220);

  // Ogonek (Dead) (U+02DB)
  // Finnish keyboard layout
  ALTGR(u"\u02DB", Equal, 61);
  SHIFT(u"\u02DB", Equal, 61);

  // Double acute (Dead) (U+02DD)
  // Finnish keyboard layout
  ALTGR(u"\u02DD", BracketLeft, 219)

  // Combining horn above (Dead) (U+0309)
  // Finnish keyboard layout
  SHIFT(u"\u0309", KeyP, 80)
  ALTGR(u"\u0309", KeyP, 80)

  // Combining horn (Dead) (U+031B)
  // Finnish keyboard layout
  ALTGR(u"\u031B", KeyP, 80)

  // Per mille sign (U+2030)
  // Finnish keyboard layout
  ALTGR(u"\u2030", Digit5, 53)

  // Euro sign (U+20AC)
  // Estonian, Finnish, German, Icelandic,
  // Italian, Norwegian, Portuguese, Swedish,
  // Turkish key8board layouts
  ALTGR(u"\u20AC", KeyE, 69)

  // Trade mark sign (U+2122)
  // TODO needs more information
  // ALTGR(u"\u2122", Digit6, 54)
}
