// aWindow.cpp: implementation of the aWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "aWindow.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aWindow::aWindow()
{
	quit = FALSE;
	active = FALSE;
	_hDC = NULL;
	_hRC = NULL;
	_hWnd = NULL;
	frame = 0;
}
//##ModelId=3DF7241B03D7
aWindow::aWindow(tImage *_frame)
{
	quit = FALSE;
	active = FALSE;
	_hDC = NULL;
	_hRC = NULL;
	_hWnd = NULL;
	frame = _frame;
}

//##ModelId=3DF7241C000D
aWindow::~aWindow()
{
    frame = 0;
}
/*
	Creates an window
		title	- name of window (as displayed on top of it in windowed mode)
		width	- window width in pixels
		height	- window height in pixels
		bits	- color bit depth (8/16/32)
		depth	- z buffer depth (16/24/32)
		fullscreenflag - if == 1 -> fullscreen, else windowed mode
*/
BOOL aWindow::createWindow( char* className, char *title, int width, int height, int bits, int depth, bool fullscreenflag)
{
	//unsigned int		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASSEX	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	_hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	strcpy(_className, className);

	//All WNDCLASSEX properties are set:
    wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;// | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) msgProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= sizeof(aWindow*);						// This is some extra data!
	wc.hInstance		= _hInstance;						// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= _className;							// Set The Class Name
	wc.hIconSm			= NULL;

	if (!RegisterClassEx(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		DWORD error = GetLastError();
		return FALSE;											// Return FALSE
	}
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;
	if (!(_hWnd = CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								_className,							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								_hInstance,							// Instance
								(void*)this))){						// Pass this object To WM_CREATE {
		this->killWindow();
		return FALSE;
	}
	
	/*if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(_hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								_className,							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								_hInstance,							// Instance
								(void*)this)))						// Pass this object To WM_CREATE
	{
		this->killWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		depth,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// YES Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(_hDC=GetDC(_hWnd)))							// Did We Get A Device Context?
	{
		this->killWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(_hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		this->killWindow();
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(_hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		this->killWindow();
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(_hRC=wglCreateContext(_hDC)))				// Are We Able To Get A Rendering Context?
	{
		this->killWindow();
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(_hDC,_hRC))					// Try To Activate The Rendering Context
	{
		this->killWindow();
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}*/

	ShowWindow(_hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(_hWnd);						// Slightly Higher Priority
	SetFocus(_hWnd);	

	return TRUE;
}
/*
	Listen for messages, not encapsulated in aWindow yet!... HOW?
  
*/
int CALLBACK aWindow::msgProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	 aWindow* win = 0; // always initialise to zero!

    
    if(uMsg == WM_NCCREATE)
    {
        // yes, we have a WM_NCCREATE message (this is the very first message that
        // is generated and sent to a window when it's created. Now when this
        // message is received, the wParam argument is not used at all, but the
        // lParam argument contains a pointer to a CREATESTRUCT. So we can access
        // the data within the structure by casting the lParam to a CREATESTRUCT*
        // like so.
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;

        // now, inside the createstruct there is an item called lpCreateParams,
        // and it is this item that is contains the pointer that we specified
        // as the last parameter to the CreateWindow function when we created
        // the window. So we can cast that value back to a aWindow pointer,
        // store it in the win variable that we declared before, and route the
        // message off to the class for handling
        win = (aWindow*)cs->lpCreateParams;

        //Store the pointer to out window as userdata(GWL_USERDATA)
        ::SetWindowLong(hWnd, GWL_USERDATA, (long)win);

        // right! we've now stored the pointer with the window handle. we can
        // move on to bigger and better things

        // we must store the window handle in our _win variable, so that it
        // can be used elsewhere in the code.
        win->_hWnd = hWnd;
    }
    else
    {
        // when we reach this bit of the code, it means that we've received another
        // kind of message apart from WM_NCCREATE. This also means that the
        // pointer to the window class has already been stored with the window,
        // so we can access that value using the GetWindowLong function. We just
        // have to specify the GWL_USERDATA flag again, and cast the return value
        // of the function back to a MainWindow*.
	
        win = (aWindow*)::GetWindowLong(hWnd, GWL_USERDATA);

        // ok, now we have the pointer to the class, we are ready for message
        // routing!
    }
	

	// Pass All Unhandled Messages To DefWindowProc
	return(win != 0 ?
        win->handleMsg(uMsg, wParam, lParam) :
		::DefWindowProc(hWnd, uMsg, wParam, lParam));
}

/* 
	Updates messages
	If there is no messages it returns false
*/
BOOL aWindow::updateMessages()
{
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
	{
		if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
		{
			quit=TRUE;							// If So quit=TRUE
		}
		else									// If Not, Deal With Window Messages
		{
			TranslateMessage(&msg);				// Translate The Message
			DispatchMessage(&msg);				// Dispatch The Message
		}
		return TRUE;		//return true if there are messages
	}
	return FALSE;			//else return false
}
/*
	If this returns true, a quit message has been recieved
*/
BOOL aWindow::doQuit()
{
	return quit;
}



void aWindow::setActive(BOOL a)
{
	active = a;
}

BOOL aWindow::isActive()
{
	return active;
}

void aWindow::killWindow()
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (_hRC)											// Do We Have A Rendering Context?
	{
		/*if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(_hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}*/
		_hRC=NULL;										// Set RC To NULL
	}

	if (_hDC && !ReleaseDC(_hWnd,_hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		_hDC=NULL;										// Set DC To NULL
	}

	if (_hWnd && !DestroyWindow(_hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		_hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass(_className,_hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		_hInstance=NULL;									// Set hInstance To NULL
	}

}

void aWindow::swapBuffers()
{
	SwapBuffers(_hDC);					// Swap Buffers (Double Buffering)
}


//##ModelId=3DF7241B02B5
int aWindow::handleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				setActive(TRUE);						// Program Is Active
			}
			else
			{
				setActive(FALSE);						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		case WM_PAINT:
		{
			//update image
			PAINTSTRUCT ps;
			HDC tmpHDC = BeginPaint(this->_hWnd, &ps);

			//draw the rectangle from ps.rcPaint
			//u_int y=0;
			for (u_int x=ps.rcPaint.left; x<ps.rcPaint.right && x < frame->getWidth(); x++) {
				for (u_int y=ps.rcPaint.top; y<ps.rcPaint.bottom && y < frame->getHeight(); y++) {
					SetPixel(tmpHDC,x,y,
						RGB(min(255,frame->samplePixel(x,y).R*255), 
						min(255,frame->samplePixel(x,y).G*255), 
						min(255,frame->samplePixel(x,y).B*255)) );
				}
			}
			EndPaint(this->_hWnd, &ps);
			return 0;
		}
		/*case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}*/

		case WM_SIZE:								// Resize The OpenGL Window
		{
			clientSizeX = LOWORD(lParam);
			clientSizeY = HIWORD(lParam);
			//ren->resizeScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		/*case WM_LBUTTONDOWN:
		{
			//catch the click position, and reset newPos
			io->mouseLButton(true);
			io->mouseMove(LOWORD(lParam),HIWORD(lParam)); 
			return 0;
		}
		case WM_LBUTTONUP:
		{
			io->mouseLButton(false);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			io->mouseRButton(true);
			io->mouseMove(LOWORD(lParam),HIWORD(lParam)); 
			return 0;
		}
		case WM_RBUTTONUP:
		{
			io->mouseRButton(false);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			//get the mouse position
			io->mouseMove(LOWORD(lParam),HIWORD(lParam)); 
			return 0;
		}*/
		
	}

	//all unhandled messages passed to default message handler
	return DefWindowProc(this->_hWnd,uMsg,wParam,lParam);
}	
