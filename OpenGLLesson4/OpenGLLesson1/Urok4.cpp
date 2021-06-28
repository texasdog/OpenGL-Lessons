#include <windows.h>                      // ������������ ���� ��� Windows
#include <gl\gl.h>                        // ������������ ���� ��� OpenGL32 ����������
#include <gl\glu.h>                       // ������������ ���� ��� GLu32 ����������
#include "glaux.h"

HGLRC  hRC = NULL;              // ���������� �������� ����������
HDC  hDC = NULL;              // ��������� �������� ���������� GDI
HWND  hWnd = NULL;              // ����� ����� �������� ���������� ����
HINSTANCE  hInstance;              // ����� ����� �������� ���������� ����������

bool  keys[256];                // ������, ������������ ��� �������� � �����������
bool  active = true;                // ���� ���������� ����, ������������� � true �� ���������
bool  fullscreen = true;              // ���� ������ ����, ������������� � ������������� �� ���������

GLfloat rtri;           // ���� ��� ������������
GLfloat rquad;          // ���� ��� ����������������

LRESULT  CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // �������� ������� WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)        // �������� ������ � ���������������� ���� GL

{
    if (height == 0)              // �������������� ������� �� ����
    {
        height = 1;
    }

    glViewport(0, 0, width, height);          // ����� ������� ������� ������
    glMatrixMode(GL_PROJECTION);            // ����� ������� ��������
    glLoadIdentity();              // ����� ������� ��������

    // ���������� ����������� �������������� �������� ��� ����

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);            // ����� ������� ���� ������
    glLoadIdentity();              // ����� ������� ���� ������

}
int InitGL(GLvoid)                // ��� ��������� ������� OpenGL ���������� �����
{
    glShadeModel(GL_SMOOTH);            // ��������� ������� �������� �����������
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);          // ������� ������ � ������ ����

    glClearDepth(1.0f);              // ��������� ������� ������ �������
    glEnable(GL_DEPTH_TEST);            // ��������� ���� �������
    glDepthFunc(GL_LEQUAL);            // ��� ����� �������
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // ��������� � ���������� �����������
    return true;                // ������������� ������ �������
}

int DrawGLScene(GLvoid)                // ����� ����� ����������� ��� ����������

{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // �������� ����� � ����� �������
    glLoadIdentity();              // �������� ������� �������
    glTranslatef(-1.5f, 0.0f, -6.0f);         // ��������� ����� �� 1.5 ������� �
                                                // � ����� �� 6.0
    glRotatef(rtri, 0.0f, 1.0f, 0.0f);         // �������� ������������ �� ��� Y

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);      // ������� ����
    glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);      // �������� ����
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);      // ����� ����
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();
    
    glLoadIdentity();
    glTranslatef(1.5f, 0.0f, -6.0f);          // ����� ������ �� 1.5
    glRotatef(rquad, 1.0f, 0.0f, 0.0f);        // �������� �� ��� X
    
    glTranslatef(1.0f, 0.0f, 0.0f);           // ������� ������ �� 3 �������
    
    glColor3f(0.5f, 0.5f, 1.0f);      // ��������� ����� ���� ������ ���� ���
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, 0.0f);  // ����� ������
    glVertex3f(1.0f, 1.0f, 0.0f);  // ������ ������
    glVertex3f(1.0f, -1.0f, 0.0f);  // ������ �����
    glVertex3f(-1.0f, -1.0f, 0.0f);  // ����� �����
    glEnd();
    rtri += 0.01f;             // ���������� ���������� �������� ��� ������������
    rquad -= 0.15f;           // ���������� ���������� �������� ��� ��������
    return true;                // ���������� ������ �������

}

GLvoid KillGLWindow(GLvoid)              // ���������� ���������� ����
{
    if (fullscreen)              // �� � ������������� ������?
    {
        ChangeDisplaySettings(NULL, 0);        // ���� ��, �� ������������� ������� � ������� �����
        ShowCursor(true);            // �������� ������ �����
    }

    if (hRC)                // ���������� �� �������� ����������?
    {
        if (!wglMakeCurrent(NULL, NULL))        // �������� �� ���������� RC � DC?

        {
            MessageBox(NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        }

        if (!wglDeleteContext(hRC))        // �������� �� ������� RC?
        {
            MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }
        hRC = NULL;              // ���������� RC � NULL
    }
    if (hDC && !ReleaseDC(hWnd, hDC))          // �������� �� ���������� DC?

    {

        MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hDC = NULL;                // ���������� DC � NULL

    }
    if (hWnd && !DestroyWindow(hWnd))            // �������� �� ���������� ����?

    {

        MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hWnd = NULL;                // ���������� hWnd � NULL

    }
    if (!UnregisterClass(L"OpenGL", hInstance))        // �������� �� ����������������� �����

    {

        MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hInstance = NULL;                // ���������� hInstance � NULL

    }
}
BOOL CreateGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint    PixelFormat;              // ������ ��������� ����� ������
    WNDCLASS  wc;                // ��������� ������ ����
    DWORD    dwExStyle;              // ����������� ����� ����
    DWORD    dwStyle;              // ������� ����� ����
    RECT WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values

    WindowRect.left = (long)0;              // ���������� ����� ������������ � 0
    WindowRect.right = (long)width;              // ���������� ������ ������������ � Width
    WindowRect.top = (long)0;                // ���������� ������� ������������ � 0
    WindowRect.bottom = (long)height;              // ���������� ������ ������������ � Height

    fullscreen = fullscreenflag;              // ������������� �������� ���������� ���������� fullscreen

    hInstance = GetModuleHandle(NULL);        // ������� ���������� ������ ����������
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // ���������� ��� ����������� � ������ ������� DC
    wc.lpfnWndProc = (WNDPROC)WndProc;          // ��������� ��������� ���������
    wc.cbClsExtra = 0;              // ��� �������������� ���������� ��� ����
    wc.cbWndExtra = 0;              // ��� �������������� ���������� ��� ����
    wc.hInstance = hInstance;            // ������������� ����������
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);        // ��������� ������ �� ���������
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);        // ��������� ��������� �����
    wc.hbrBackground = NULL;              // ��� �� ��������� ��� GL
    wc.lpszMenuName = NULL;              // ���� � ���� �� �����
    wc.lpszClassName = L"OpenGL";            // ������������� ��� ������

    if (!RegisterClass(&wc))              // �������� ���������������� ����� ����
    {
        MessageBox(NULL, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // ����� � ����������� �������� �������� false
    }

    if (fullscreen)                // ������������� �����?
    {
        DEVMODE dmScreenSettings;            // ����� ����������
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));    // ������� ��� �������� ���������

        dmScreenSettings.dmSize = sizeof(dmScreenSettings);      // ������ ��������� Devmode
        dmScreenSettings.dmPelsWidth = width;        // ������ ������
        dmScreenSettings.dmPelsHeight = height;        // ������ ������
        dmScreenSettings.dmBitsPerPel = bits;        // ������� �����
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// ����� �������

        // �������� ���������� ��������� ����� � �������� ���������.  ����������: CDS_FULLSCREEN ������� ������ ����������.

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // ���� ������������ � ������������� ����� ����������, ����� ���������� ��� ��������: ������� ����� ��� �����.
            if (MessageBox(NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
                L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
            {
                fullscreen = false;          // ����� �������� ������ (fullscreen = false)
            }
            else

            {
                // ������������� ����, ���������� ������������ � �������� ����.

                MessageBox(NULL, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP);
                return false;            // ����� � ����������� �������� false
            }
        }
    }
    if (fullscreen)                  // �� �������� � ������������� ������?
    {
        dwExStyle = WS_EX_APPWINDOW;          // ����������� ����� ����
        dwStyle = WS_POPUP;            // ������� ����� ����
        ShowCursor(false);              // ������ ��������� �����
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // ����������� ����� ����
        dwStyle = WS_OVERLAPPEDWINDOW;        // ������� ����� ����
    }
    AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);      // ��������� ���� ���������� �������
    if (!(hWnd = CreateWindowEx(dwExStyle,          // ����������� ����� ��� ����
        L"OpenGL",          // ��� ������
        title,            // ��������� ����
        WS_CLIPSIBLINGS |        // ��������� ����� ��� ����
        WS_CLIPCHILDREN |        // ��������� ����� ��� ����
        dwStyle,          // ���������� ����� ��� ����
        0, 0,            // ������� ����
        WindowRect.right - WindowRect.left,    // ���������� ���������� ������
        WindowRect.bottom - WindowRect.top,    // ���������� ���������� ������
        NULL,            // ��� �������������
        NULL,            // ��� ����
        hInstance,          // ���������� ����������
        NULL)))          // �� ������� ������ �� WM_CREATE (???)
    {
        KillGLWindow();                // ������������ �����
        MessageBox(NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // ������� false
    }
    static  PIXELFORMATDESCRIPTOR pfd =            // pfd �������� Windows ����� ����� ����� �� ����� ������� �������

    {
      sizeof(PIXELFORMATDESCRIPTOR),            // ������ ����������� ������� ������� ��������
      1,                  // ����� ������
      PFD_DRAW_TO_WINDOW |              // ������ ��� ����
      PFD_SUPPORT_OPENGL |              // ������ ��� OpenGL
      PFD_DOUBLEBUFFER,              // ������ ��� �������� ������
      PFD_TYPE_RGBA,                // ��������� RGBA ������
      bits,                  // ���������� ��� ������� �����
      0, 0, 0, 0, 0, 0,              // ������������� �������� �����
      0,                  // ��� ������ ������������
      0,                  // ��������� ��� ������������
      0,                  // ��� ������ ����������
      0, 0, 0, 0,                // ���� ���������� ������������
      32,                  // 32 ������ Z-����� (����� �������)
      0,                  // ��� ������ ���������
      0,                  // ��� ��������������� �������
      PFD_MAIN_PLANE,                // ������� ���� ���������
      0,                  // ���������������
      0, 0, 0                  // ����� ���� ������������
    };

    if (!(hDC = GetDC(hWnd)))              // ����� �� �� �������� �������� ����������?
    {
        KillGLWindow();                // ������������ �����
        MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // ������� false
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))        // ������ �� ���������� ������ �������?
    {
        KillGLWindow();                // ������������ �����
        MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // ������� false
    }

    if (!SetPixelFormat(hDC, PixelFormat, &pfd))          // �������� �� ���������� ������ �������?
    {
        KillGLWindow();                // ������������ �����
        MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // ������� false
    }

    if (!(hRC = wglCreateContext(hDC)))          // �������� �� ���������� �������� ����������?
    {
        KillGLWindow();                // ������������ �����
        MessageBox(NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // ������� false
    }

    if (!wglMakeCurrent(hDC, hRC))            // ����������� ������������ �������� ����������
    {
        KillGLWindow();                // ������������ �����
        MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // ������� false
    }

    ShowWindow(hWnd, SW_SHOW);              // �������� ����
    SetForegroundWindow(hWnd);              // ������ ������� ���������
    SetFocus(hWnd);                // ���������� ����� ���������� �� ���� ����
    ReSizeGLScene(width, height);              // �������� ����������� ��� ������ OpenGL ������.

    if (!InitGL())                  // ������������� ������ ��� ���������� ����
    {
        KillGLWindow();                // ������������ �����
        MessageBox(NULL, L"Initialization Failed.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // ������� false
    }
    return true;                  // �� � �������!
}

LRESULT CALLBACK WndProc(HWND  hWnd,            // ���������� ������� ����
    UINT  uMsg,            // ��������� ��� ����� ����
    WPARAM  wParam,            // �������������� ����������
    LPARAM  lParam)            // �������������� ����������
{
    switch (uMsg)                // �������� ��������� ��� ����
    {
    case WM_ACTIVATE:            // �������� ��������� ���������� ����

    {

        if (!HIWORD(wParam))          // ��������� ��������� �����������
        {
            active = true;          // ��������� �������
        }
        else
        {
            active = false;          // ��������� ������ �� �������
        }
        return 0;            // ������������ � ���� ��������� ���������

    }
    case WM_SYSCOMMAND:            // ������������� ��������� �������
    {
        switch (wParam)            // ������������� ��������� �����
        {
        case SC_SCREENSAVE:        // �������� �� ���������� �����������
        case SC_MONITORPOWER:        // �������� �� ������� ������� � ����� ���������� �������?
            return 0;          // ������������� ��
        }
        break;              // �����
    }
    case WM_CLOSE:              // �� �������� ��������� � ��������?

    {
        PostQuitMessage(0);          // ��������� ��������� � ������
        return 0;            // ��������� �����
    }
    case WM_KEYUP:              // ���� �� �������� �������
    {
        keys[wParam] = false;          //  ���� ���, �� ����������� ���� ������ false
        return 0;            // ������������
    }
    case WM_SIZE:              // �������� ������� OpenGL ����
    {
        ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // ������� �����=Width, ������� �����=Height
        return 0;            // ������������
    }
    }
    // ���������� ��� �������������� ��������� DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE  hInstance,        // ���������� ����������
    HINSTANCE  hPrevInstance,        // ���������� ������������� ����������
    LPSTR    lpCmdLine,        // ��������� ��������� ������
    int    nCmdShow)        // ��������� ����������� ����
{
    MSG  msg;              // ��������� ��� �������� ��������� Windows
    BOOL  done = false;            // ���������� ���������� ��� ������ �� �����
// ���������� ������������, ����� ����� ������ �� ������������

    if (MessageBox(NULL, L"������ �� �� ��������� ���������� � ������������� ������?", L"��������� � ������������� ������?", MB_YESNO | MB_ICONQUESTION) == IDNO)
    {
        fullscreen = false;          // ������� �����
    }
    // ������� ���� OpenGL ����
    if (!CreateGLWindow(L"NeHe OpenGL ����", 1024, 768, 32, fullscreen))
    {
        return 0;              // �����, ���� ���� �� ����� ���� �������
    }
    while (!done)                // ���� ������������, ���� done �� ����� true
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))    // ���� �� � ������� �����-������ ���������?
        {
            if (msg.message == WM_QUIT)        // �� ������� ��������� � ������?
            {
                done = true;          // ���� ���, done=true
            }
            else              // ���� ���, ������������ ���������
            {
                TranslateMessage(&msg);        // ��������� ���������

                DispatchMessage(&msg);        // �������� ���������

            }

        }
        else                // ���� ��� ���������
        {
            // ������������� �����.
            if (active)          // ������� �� ���������?
            {
                if (keys[VK_ESCAPE])        // ���� �� ������ ������� ESC?
                {
                    done = true;      // ESC ������� �� �������� ���������� ���������
                }
                else            // �� ����� ��� ������, ������� �����.
                {
                    DrawGLScene();        // ������ �����
                    SwapBuffers(hDC);    // ������ ����� (������� �����������)
                }
            }

            if (keys[VK_F1])          // ���� �� ������ F1?
            {
                keys[VK_F1] = false;        // ���� ���, ������ �������� ������ ������� �� false
                KillGLWindow();          // ��������� ������� ����
                fullscreen = !fullscreen;      // ����������� �����
                // ���������� ���� OpenGL ����
                if (!CreateGLWindow(L"NeHe OpenGL ���������", 1024, 768, 32, fullscreen))
                {
                    return 0;        // �������, ���� ��� ����������
                }
            }
        }
    }
    // Shutdown
    KillGLWindow();                // ��������� ����
    return (msg.wParam);              // ������� �� ���������
}