// aWindow.h: interface for the aWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AWINDOW_H__C67DFC0A_0313_4343_95C1_F238F90D0BDD__INCLUDED_)
#define AFX_AWINDOW_H__C67DFC0A_0313_4343_95C1_F238F90D0BDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include <tImage.h>
//##ModelId=3DF7241B0193
class aWindow  
{
private:
	HDC			_hDC;		// Private GDI Device Context
	HGLRC		_hRC;		// Permanent Rendering Context
	HWND		_hWnd;		// Holds Our Window Handle
	HINSTANCE	_hInstance;	// Holds The Instance Of The Application
	char		_className[255];
	
	BOOL		quit;			// If a quit message is recieved
	MSG			msg;			// Message storage
	BOOL		active;			// True if window is active
	BOOL		fullscreen;		// Fullscreen flag

	tImage *frame;
	unsigned int clientSizeX, clientSizeY;
public:
	int		handleMsg(UINT msg, WPARAM wParam, LPARAM lParam);
	void	swapBuffers();
	void	killWindow();
	BOOL	isActive();
	void	setActive(BOOL a);
	BOOL	doQuit();
	BOOL	updateMessages();
	BOOL	createWindow(char* className, char* title, int width, int height, int bits, int depth, bool fullscreenflag);
	
	static int CALLBACK msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	aWindow();
	aWindow(tImage *_frame);
	virtual ~aWindow();

};

#endif // !defined(AFX_AWINDOW_H__C67DFC0A_0313_4343_95C1_F238F90D0BDD__INCLUDED_)
