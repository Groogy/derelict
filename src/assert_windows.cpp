#ifdef WIN32
#include "assert.hpp"
#include <windows.h>
#include <sstream>

namespace
{
	HHOOK msgBoxHook;
	int button = 0;

void CALLBACK AbortCallback(LPHELPINFO)
{
	UnhookWindowsHookEx(msgBoxHook);
	FatalExit(0);
}

LRESULT CALLBACK CBTProc(int code, WPARAM wParam, LPARAM lParam)
{
	char className[64];
	char buffer[64];
	static HWND buttons[5];
	if (code < 0)
		return CallNextHookEx(msgBoxHook, code, wParam, lParam);

	switch (code)
	{
	case HCBT_CREATEWND:
		GetClassNameA((HWND)wParam, className, 64);
		if (!strcmp(className, "Button"))
		{
			buttons[button++] = (HWND)wParam;
		}
		break;
	case HCBT_ACTIVATE:

		for (int i = 0; i < button; ++i)
		{
			if (i == 0)
				strcpy(buffer, "Debug");
			else if (i == 1)
				strcpy(buffer, "Ignore");
			else
				strcpy(buffer, "Abort");
			SetWindowTextA(buttons[i], buffer);
		}
		return 0;
	}

	// Call the next hook, if there is one
	return CallNextHookEx(msgBoxHook, code, wParam, lParam);
}

bool ShowAssertDialog(const std::string& expr, const std::string& file, unsigned int line)
{
	bool debugBreak = false;
	std::ostringstream buf;

	buf << "Assertion:   " << expr << "\n\n" << "File:             " << file << "(" << line << ").";

	char* text = new char[buf.str().size() + 1];
	strcpy(text, buf.str().c_str());

	char caption[] = "Assertion failed.";
	
	msgBoxHook = SetWindowsHookEx(WH_CBT, CBTProc, 0, GetCurrentThreadId());

	button = 0;

	MSGBOXPARAMSA MsgParams;
	ZeroMemory(&MsgParams, sizeof(MsgParams));
	MsgParams.cbSize = sizeof(MsgParams);
	MsgParams.lpszText = text;
	MsgParams.lpszCaption = caption;
	MsgParams.dwStyle = MB_RETRYCANCEL | MB_HELP | MB_DEFBUTTON2 | MB_TOPMOST | MB_SETFOREGROUND;
	MsgParams.lpfnMsgBoxCallback = AbortCallback;

	switch (MessageBoxIndirectA(&MsgParams))
	{
	case IDRETRY:	// debug
		UnhookWindowsHookEx(msgBoxHook);
		debugBreak = true;
		break;
	case IDCANCEL:	// ignore
		UnhookWindowsHookEx(msgBoxHook);
		break;
	default:
		UnhookWindowsHookEx(msgBoxHook);
	}

	delete[] text;

	return debugBreak;
}

}

#include <iostream>

namespace testing
{

bool assert(const std::string& expr, const std::string& file, unsigned int line)
{
	bool debugBreak = ShowAssertDialog(expr, file, line);
	return debugBreak;
}

}
#endif // WIN32
