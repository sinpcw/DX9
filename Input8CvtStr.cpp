#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//		owl::engine::CvtStr(unsigned char)
//***********************************************************************************************************************************************************************
const wchar_t * owl::engine::CvtStr(unsigned char _in){
	static	wchar_t 	cText[32]	= { 0 };
	switch(_in){
		default:				cText[0]	= '\0';						break;
		case OVK_ESCAPE:		wcscpy_s(cText, 31, L"ESCAPE");			break;
		case OVK_1:				wcscpy_s(cText, 31, L"1");				break;
		case OVK_2:				wcscpy_s(cText, 31, L"2");				break;
		case OVK_3:				wcscpy_s(cText, 31, L"3");				break;
		case OVK_4:				wcscpy_s(cText, 31, L"4");				break;
		case OVK_5:				wcscpy_s(cText, 31, L"5");				break;
		case OVK_6:				wcscpy_s(cText, 31, L"6");				break;
		case OVK_7:				wcscpy_s(cText, 31, L"7");				break;
		case OVK_8:				wcscpy_s(cText, 31, L"8");				break;
		case OVK_9:				wcscpy_s(cText, 31, L"9");				break;
		case OVK_0:				wcscpy_s(cText, 31, L"0");				break;
		case OVK_MINUS:			wcscpy_s(cText, 31, L"-");				break;
		case OVK_EQUALS:		wcscpy_s(cText, 31, L"^");				break;
		case OVK_BACK:			wcscpy_s(cText, 31, L"BACK SPACE");		break;
		case OVK_TAB:			wcscpy_s(cText, 31, L"TAB");			break;
		case OVK_Q:				wcscpy_s(cText, 31, L"Q");				break;
		case OVK_W:				wcscpy_s(cText, 31, L"W");				break;
		case OVK_E:				wcscpy_s(cText, 31, L"E");				break;
		case OVK_R:				wcscpy_s(cText, 31, L"R");				break;
		case OVK_T:				wcscpy_s(cText, 31, L"T");				break;
		case OVK_Y:				wcscpy_s(cText, 31, L"Y");				break;
		case OVK_U:				wcscpy_s(cText, 31, L"U");				break;
		case OVK_I:				wcscpy_s(cText, 31, L"I");				break;
		case OVK_O:				wcscpy_s(cText, 31, L"O");				break;
		case OVK_P:				wcscpy_s(cText, 31, L"P");				break;
		case OVK_LBRACKET:		wcscpy_s(cText, 31, L"[");				break;
		case OVK_RBRACKET:		wcscpy_s(cText, 31, L"]");				break;
		case OVK_RETURN:		wcscpy_s(cText, 31, L"ENTER");			break;
		case OVK_LCONTROL:		wcscpy_s(cText, 31, L"CONTROL(L)");		break;
		case OVK_RCONTROL:		wcscpy_s(cText, 31, L"CONTROL(R)");		break;
		case OVK_A:				wcscpy_s(cText, 31, L"A");				break;
		case OVK_S:				wcscpy_s(cText, 31, L"S");				break;
		case OVK_D:				wcscpy_s(cText, 31, L"D");				break;
		case OVK_F:				wcscpy_s(cText, 31, L"F");				break;
		case OVK_G:				wcscpy_s(cText, 31, L"G");				break;
		case OVK_H:				wcscpy_s(cText, 31, L"H");				break;
		case OVK_J:				wcscpy_s(cText, 31, L"J");				break;
		case OVK_K:				wcscpy_s(cText, 31, L"K");				break;
		case OVK_L:				wcscpy_s(cText, 31, L"L");				break;
		case OVK_SEMICOLON:		wcscpy_s(cText, 31, L";");				break;
		case OVK_AT:			wcscpy_s(cText, 31, L"@");				break;
		case OVK_LSHIFT:		wcscpy_s(cText, 31, L"SHIFT(L)");		break;
		case OVK_RSHIFT:		wcscpy_s(cText, 31, L"SHIFT(R)");		break;
		case OVK_BACKSLASH:		wcscpy_s(cText, 31, L"BACKSLASH");		break;
		case OVK_Z:				wcscpy_s(cText, 31, L"Z");				break;
		case OVK_X:				wcscpy_s(cText, 31, L"X");				break;
		case OVK_C:				wcscpy_s(cText, 31, L"C");				break;
		case OVK_V:				wcscpy_s(cText, 31, L"V");				break;
		case OVK_B:				wcscpy_s(cText, 31, L"B");				break;
		case OVK_N:				wcscpy_s(cText, 31, L"N");				break;
		case OVK_M:				wcscpy_s(cText, 31, L"M");				break;
		case OVK_COMMA:			wcscpy_s(cText, 31, L",");				break;
		case OVK_PERIOD:		wcscpy_s(cText, 31, L".");				break;
		case OVK_SLASH:			wcscpy_s(cText, 31, L"/");				break;
		case OVK_SPACE:			wcscpy_s(cText, 31, L"SPACE");			break;
		case OVK_NUMPAD0:		wcscpy_s(cText, 31, L"NUMPAD(0)");		break;
		case OVK_NUMPAD1:		wcscpy_s(cText, 31, L"NUMPAD(1)");		break;
		case OVK_NUMPAD2:		wcscpy_s(cText, 31, L"NUMPAD(2)");		break;
		case OVK_NUMPAD3:		wcscpy_s(cText, 31, L"NUMPAD(3)");		break;
		case OVK_NUMPAD4:		wcscpy_s(cText, 31, L"NUMPAD(4)");		break;
		case OVK_NUMPAD5:		wcscpy_s(cText, 31, L"NUMPAD(5)");		break;
		case OVK_NUMPAD6:		wcscpy_s(cText, 31, L"NUMPAD(6)");		break;
		case OVK_NUMPAD7:		wcscpy_s(cText, 31, L"NUMPAD(7)");		break;
		case OVK_NUMPAD8:		wcscpy_s(cText, 31, L"NUMPAD(8)");		break;
		case OVK_NUMPAD9:		wcscpy_s(cText, 31, L"NUMPAD(9)");		break;
		case OVK_DECIMAL:		wcscpy_s(cText, 31, L"NUMPAD(.)");		break;
		case OVK_ADD:			wcscpy_s(cText, 31, L"NUMPAD(+)");		break;
		case OVK_SUBTRACT:		wcscpy_s(cText, 31, L"NUMPAD(-)");		break;
		case OVK_MULTIPLY:		wcscpy_s(cText, 31, L"NUMPAD(*)");		break;
		case OVK_DIVIDE:		wcscpy_s(cText, 31, L"NUMPAD(/)");		break;
		case OVK_NUMPADENTER:	wcscpy_s(cText, 31, L"NUMPAD(ENTER)");	break;
		case OVK_F1:			wcscpy_s(cText, 31, L"F1");				break;
		case OVK_F2:			wcscpy_s(cText, 31, L"F2");				break;
		case OVK_F3:			wcscpy_s(cText, 31, L"F3");				break;
		case OVK_F4:			wcscpy_s(cText, 31, L"F4");				break;
		case OVK_F5:			wcscpy_s(cText, 31, L"F5");				break;
		case OVK_F6:			wcscpy_s(cText, 31, L"F6");				break;
		case OVK_F7:			wcscpy_s(cText, 31, L"F7");				break;
		case OVK_F8:			wcscpy_s(cText, 31, L"F8");				break;
		case OVK_F9:			wcscpy_s(cText, 31, L"F9");				break;
		case OVK_F10:			wcscpy_s(cText, 31, L"F10");			break;
		case OVK_F11:			wcscpy_s(cText, 31, L"F11");			break;
		case OVK_F12:			wcscpy_s(cText, 31, L"F12");			break;
		case OVK_KANA:			wcscpy_s(cText, 31, L"”¼Šp/‘SŠp");		break;
		case OVK_CONVERT:		wcscpy_s(cText, 31, L"•ÏŠ·");			break;
		case OVK_NOCONVERT:		wcscpy_s(cText, 31, L"–³•ÏŠ·");			break;
		case OVK_YEN:			wcscpy_s(cText, 31, L"\\");				break;
	}
	return cText;
}
