#define _CRT_SECURE_NO_WARNINGS


#include <windows.h>                      // Заголовочный файл для Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>                        // Заголовочный файл для OpenGL32 библиотеки
#include <gl\glu.h>                       // Заголовочный файл для GLu32 библиотеки
// include "glaux.h"
#pragma comment (lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib,"User32.lib")

HGLRC  hRC = NULL;              // Постоянный контекст рендеринга
HDC  hDC = NULL;              // Приватный контекст устройства GDI
HWND  hWnd = NULL;              // Здесь будет хранится дескриптор окна
HINSTANCE  hInstance;              // Здесь будет хранится дескриптор приложения

bool  keys[256];                // Массив, используемый для операций с клавиатурой
bool  active = true;                // Флаг активности окна, установленный в true по умолчанию
bool  fullscreen = true;              // Флаг режима окна, установленный в полноэкранный по умолчанию

GLfloat rtri;           // Угол для треугольника
GLfloat rquad;          // Угол для четырехугольника
GLfloat	xrot;			// Вращение X
GLfloat	yrot;			// Y
GLfloat	zrot;			// Z

GLuint	texture;		// Место для одной текстуры

LRESULT  CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // Прототип функции WndProc


bool NeHeLoadBitmap(LPTSTR szFileName, GLuint& texid)					// Creates Texture From A Bitmap File
{
    HBITMAP hBMP;														// Handle Of The Bitmap
    BITMAP	BMP;														// Bitmap Structure

    glGenTextures(1, &texid);											// Create The Texture
    hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

    if (!hBMP)															// Does The Bitmap Exist?
        return FALSE;													// If Not Return False

    GetObject(hBMP, sizeof(BMP), &BMP);									// Get The Object
                                                                        // hBMP:        Handle To Graphics Object
                                                                        // sizeof(BMP): Size Of Buffer For Object Information
                                                                        // &BMP:        Buffer For Object Information

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);								// Pixel Storage Mode (Word Alignment / 4 Bytes)

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texid);								// Bind To The Texture ID
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Min Filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Mag Filter
    glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

    DeleteObject(hBMP);													// Delete The Object

    return TRUE;														// Loading Was Successful
}
int LoadGLTextures() // Load Bitmaps And Convert To Textures
{
    if (NeHeLoadBitmap((LPSTR)"data\\texture.bmp", texture))
    {
        return TRUE;
    }
    return FALSE;
}




GLvoid ReSizeGLScene(GLsizei width, GLsizei height)        // Изменить размер и инициализировать окно GL

{
    if (height == 0)              // Предотвращение деления на ноль
    {
        height = 1;
    }

    glViewport(0, 0, width, height);          // Сброс текущей области вывода
    glMatrixMode(GL_PROJECTION);            // Выбор матрицы проекций
    glLoadIdentity();              // Сброс матрицы проекции

    // Вычисление соотношения геометрических размеров для окна

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);            // Выбор матрицы вида модели
    glLoadIdentity();              // Сброс матрицы вида модели

}

int InitGL(GLvoid)                // Все установки касаемо OpenGL происходят здесь
{

    if (!LoadGLTextures()) // Jump To Texture Loading Routine ( NEW )
    {
        return FALSE; // If Texture Didn’t Load Return FALSE
    }
    glEnable(GL_TEXTURE_2D);		// Разрешение наложение текстуры
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    return true;                // Инициализация прошла успешно
}


int DrawGLScene(GLvoid)                // Здесь будет происходить вся прорисовка
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);		// Вращение по оси X
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);		// Вращение по оси Y
    glRotatef(zrot, 0.0f, 0.0f, 1.0f);		// Вращение по оси Z
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    // Передняя грань
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);	// Низ лево
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Верх лево

                    // Задняя грань
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);	// Низ лево

                    // Верхняя грань
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Низ лево
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);	// Верх право

                    // Нижняя грань
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	// Низ лево
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);	// Низ право

                    // Правая грань
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);	// Верх лево
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);	// Низ лево

                    // Левая грань
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Низ лево
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);	// Низ право
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);	// Верх право
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);	// Верх лево

    glEnd();

    xrot += 0.3f;			// Ось вращения X
    yrot += 0.2f;			// Ось вращения Y
    zrot += 0.4f;			// Ось вращения Z
    return true;                // Прорисовка прошла успешно

}

GLvoid KillGLWindow(GLvoid)              // Корректное разрушение окна
{
    if (fullscreen)              // Мы в полноэкранном режиме?
    {
        ChangeDisplaySettings(NULL, 0);        // Если да, то переключаемся обратно в оконный режим
        ShowCursor(true);            // Показать курсор мышки
    }

    if (hRC)                // Существует ли Контекст Рендеринга?
    {
        if (!wglMakeCurrent(NULL, NULL))        // Возможно ли освободить RC и DC?

        {
            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        }

        if (!wglDeleteContext(hRC))        // Возможно ли удалить RC?
        {
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }
        hRC = NULL;              // Установить RC в NULL
    }
    if (hDC && !ReleaseDC(hWnd, hDC))          // Возможно ли уничтожить DC?

    {

        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hDC = NULL;                // Установить DC в NULL

    }
    if (hWnd && !DestroyWindow(hWnd))            // Возможно ли уничтожить окно?

    {

        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hWnd = NULL;                // Установить hWnd в NULL

    }
    if (!UnregisterClass("OpenGL", hInstance))        // Возможно ли разрегистрировать класс

    {

        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hInstance = NULL;                // Установить hInstance в NULL

    }
}
BOOL CreateGLWindow(LPCSTR title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint    PixelFormat;              // Хранит результат после поиска
    WNDCLASS  wc;                // Структура класса окна
    DWORD    dwExStyle;              // Расширенный стиль окна
    DWORD    dwStyle;              // Обычный стиль окна
    RECT WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values

    WindowRect.left = (long)0;              // Установить левую составляющую в 0
    WindowRect.right = (long)width;              // Установить правую составляющую в Width
    WindowRect.top = (long)0;                // Установить верхнюю составляющую в 0
    WindowRect.bottom = (long)height;              // Установить нижнюю составляющую в Height

    fullscreen = fullscreenflag;              // Устанавливаем значение глобальной переменной fullscreen

    hInstance = GetModuleHandle(NULL);        // Считаем дескриптор нашего приложения
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // Перерисуем при перемещении и создаём скрытый DC
    wc.lpfnWndProc = (WNDPROC)WndProc;          // Процедура обработки сообщений
    wc.cbClsExtra = 0;              // Нет дополнительной информации для окна
    wc.cbWndExtra = 0;              // Нет дополнительной информации для окна
    wc.hInstance = hInstance;            // Устанавливаем дескриптор
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);        // Загружаем иконку по умолчанию
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);        // Загружаем указатель мышки
    wc.hbrBackground = NULL;              // Фон не требуется для GL
    wc.lpszMenuName = NULL;              // Меню в окне не будет
    wc.lpszClassName = "OpenGL";            // Устанавливаем имя классу

    if (!RegisterClass(&wc))              // Пытаемся зарегистрировать класс окна
    {
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // Выход и возвращение функцией значения false
    }

    if (fullscreen)                // Полноэкранный режим?
    {
        DEVMODE dmScreenSettings;            // Режим устройства
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));    // Очистка для хранения установок

        dmScreenSettings.dmSize = sizeof(dmScreenSettings);      // Размер структуры Devmode
        dmScreenSettings.dmPelsWidth = width;        // Ширина экрана
        dmScreenSettings.dmPelsHeight = height;        // Высота экрана
        dmScreenSettings.dmBitsPerPel = bits;        // Глубина цвета
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// Режим Пикселя

        // Пытаемся установить выбранный режим и получить результат.  Примечание: CDS_FULLSCREEN убирает панель управления.

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // Если переключение в полноэкранный режим невозможно, будет предложено два варианта: оконный режим или выход.
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
                "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
            {
                fullscreen = false;          // Выбор оконного режима (fullscreen = false)
            }
            else

            {
                // Выскакивающее окно, сообщающее пользователю о закрытие окна.

                MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                return false;            // Выход и возвращение функцией false
            }
        }
    }
    if (fullscreen)                  // Мы остались в полноэкранном режиме?
    {
        dwExStyle = WS_EX_APPWINDOW;          // Расширенный стиль окна
        dwStyle = WS_POPUP;            // Обычный стиль окна
        ShowCursor(false);              // Скрыть указатель мышки
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // Расширенный стиль окна
        dwStyle = WS_OVERLAPPEDWINDOW;        // Обычный стиль окна
    }
    AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);      // Подбирает окну подходящие размеры
    if (!(hWnd = CreateWindowEx(dwExStyle,          // Расширенный стиль для окна
        "OpenGL",          // Имя класса
        title,            // Заголовок окна
        WS_CLIPSIBLINGS |        // Требуемый стиль для окна
        WS_CLIPCHILDREN |        // Требуемый стиль для окна
        dwStyle,          // Выбираемые стили для окна
        0, 0,            // Позиция окна
        WindowRect.right - WindowRect.left,    // Вычисление подходящей ширины
        WindowRect.bottom - WindowRect.top,    // Вычисление подходящей высоты
        NULL,            // Нет родительского
        NULL,            // Нет меню
        hInstance,          // Дескриптор приложения
        NULL)))          // Не передаём ничего до WM_CREATE (???)
    {
        KillGLWindow();                // Восстановить экран
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // Вернуть false
    }
    static  PIXELFORMATDESCRIPTOR pfd =            // pfd сообщает Windows каким будет вывод на экран каждого пикселя

    {
      sizeof(PIXELFORMATDESCRIPTOR),            // Размер дескриптора данного формата пикселей
      1,                  // Номер версии
      PFD_DRAW_TO_WINDOW |              // Формат для Окна
      PFD_SUPPORT_OPENGL |              // Формат для OpenGL
      PFD_DOUBLEBUFFER,              // Формат для двойного буфера
      PFD_TYPE_RGBA,                // Требуется RGBA формат
      bits,                  // Выбирается бит глубины цвета
      0, 0, 0, 0, 0, 0,              // Игнорирование цветовых битов
      0,                  // Нет буфера прозрачности
      0,                  // Сдвиговый бит игнорируется
      0,                  // Нет буфера накопления
      0, 0, 0, 0,                // Биты накопления игнорируются
      32,                  // 32 битный Z-буфер (буфер глубины)
      0,                  // Нет буфера трафарета
      0,                  // Нет вспомогательных буферов
      PFD_MAIN_PLANE,                // Главный слой рисования
      0,                  // Зарезервировано
      0, 0, 0                  // Маски слоя игнорируются
    };

    if (!(hDC = GetDC(hWnd)))              // Можем ли мы получить Контекст Устройства?
    {
        KillGLWindow();                // Восстановить экран
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // Вернуть false
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))        // Найден ли подходящий формат пикселя?
    {
        KillGLWindow();                // Восстановить экран
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // Вернуть false
    }

    if (!SetPixelFormat(hDC, PixelFormat, &pfd))          // Возможно ли установить Формат Пикселя?
    {
        KillGLWindow();                // Восстановить экран
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // Вернуть false
    }

    if (!(hRC = wglCreateContext(hDC)))          // Возможно ли установить Контекст Рендеринга?
    {
        KillGLWindow();                // Восстановить экран
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // Вернуть false
    }

    if (!wglMakeCurrent(hDC, hRC))            // Попробовать активировать Контекст Рендеринга
    {
        KillGLWindow();                // Восстановить экран
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // Вернуть false
    }

    ShowWindow(hWnd, SW_SHOW);              // Показать окно
    SetForegroundWindow(hWnd);              // Слегка повысим приоритет
    SetFocus(hWnd);                // Установить фокус клавиатуры на наше окно
    ReSizeGLScene(width, height);              // Настроим перспективу для нашего OpenGL экрана.

    if (!InitGL())                  // Инициализация только что созданного окна
    {
        KillGLWindow();                // Восстановить экран
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;                // Вернуть false
    }
    return true;                  // Всё в порядке!
}

LRESULT CALLBACK WndProc(HWND  hWnd,            // Дескриптор нужного окна
    UINT  uMsg,            // Сообщение для этого окна
    WPARAM  wParam,            // Дополнительная информация
    LPARAM  lParam)            // Дополнительная информация
{
    switch (uMsg)                // Проверка сообщения для окна
    {
    case WM_ACTIVATE:            // Проверка сообщения активности окна

    {

        if (!HIWORD(wParam))          // Проверить состояние минимизации
        {
            active = true;          // Программа активна
        }
        else
        {
            active = false;          // Программа теперь не активна
        }
        return 0;            // Возвращаемся в цикл обработки сообщений

    }
    case WM_SYSCOMMAND:            // Перехватываем системную команду
    {
        switch (wParam)            // Останавливаем системный вызов
        {
        case SC_SCREENSAVE:        // Пытается ли запустится скринсейвер
        case SC_MONITORPOWER:        // Пытается ли монитор перейти в режим сбережения энергии?
            return 0;          // Предотвращаем эт
        }
        break;              // Выход
    }
    case WM_CLOSE:              // Мы получили сообщение о закрытие?

    {
        PostQuitMessage(0);          // Отправить сообщение о выходе
        return 0;            // Вернуться назад
    }
    case WM_KEYUP:              // Была ли отпущена клавиша
    {
        keys[wParam] = false;          //  Если так, мы присваиваем этой ячейке false
        return 0;            // Возвращаемся
    }
    case WM_SIZE:              // Изменены размеры OpenGL окна
    {
        ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // Младшее слово=Width, старшее слово=Height
        return 0;            // Возвращаемся
    }
    }
    // пересылаем все необработанные сообщения DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE  hInstance,        // Дескриптор приложения
    HINSTANCE  hPrevInstance,        // Дескриптор родительского приложения
    LPSTR    lpCmdLine,        // Параметры командной строки
    int    nCmdShow)        // Состояние отображения окна
{
    MSG  msg;              // Структура для хранения сообщения Windows
    BOOL  done = false;            // Логическая переменная для выхода из цикла
// Спрашивает пользователя, какой режим экрана он предпочитает

    if (MessageBox(NULL, "Хотите ли Вы запустить приложение в полноэкранном режиме?", "Запустить в полноэкранном режиме?", MB_YESNO | MB_ICONQUESTION) == IDNO)
    {
        fullscreen = false;          // Оконный режим
    }
    // Создать наше OpenGL окно
    if (!CreateGLWindow("NeHe OpenGL окно", 1024, 768, 32, fullscreen))
    {
        return 0;              // Выйти, если окно не может быть создано
    }
    while (!done)                // Цикл продолжается, пока done не равно true
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))    // Есть ли в очереди какое-нибудь сообщение?
        {
            if (msg.message == WM_QUIT)        // Мы поучили сообщение о выходе?
            {
                done = true;          // Если так, done=true
            }
            else              // Если нет, обрабатывает сообщения
            {
                TranslateMessage(&msg);        // Переводим сообщение

                DispatchMessage(&msg);        // Отсылаем сообщение

            }

        }
        else                // Если нет сообщений
        {
            // Прорисовываем сцену.
            if (active)          // Активна ли программа?
            {
                if (keys[VK_ESCAPE])        // Было ли нажата клавиша ESC?
                {
                    done = true;      // ESC говорит об останове выполнения программы
                }
                else            // Не время для выхода, обновим экран.
                {
                    DrawGLScene();        // Рисуем сцену
                    SwapBuffers(hDC);    // Меняем буфер (двойная буферизация)
                }
            }

            if (keys[VK_F1])          // Была ли нажата F1?
            {
                keys[VK_F1] = false;        // Если так, меняем значение ячейки массива на false
                KillGLWindow();          // Разрушаем текущее окно
                fullscreen = !fullscreen;      // Переключаем режим
                // Пересоздаём наше OpenGL окно
                if (!CreateGLWindow("NeHe OpenGL структура", 1024, 768, 32, fullscreen))
                {
                    return 0;        // Выходим, если это невозможно
                }
            }
        }
    }
    // Shutdown
    KillGLWindow();                // Разрушаем окно
    return (msg.wParam);              // Выходим из программы
}