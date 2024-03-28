#include <windows.h>   
#include <glad/glad.h>
#include <gl/glu.h>
#include <cstdio>
#include <stdarg.h>
#include <cmath>
#include <iostream>
using namespace std;


HWND hWnd;
HINSTANCE hInst;

int height = GetSystemMetrics(SM_CYSCREEN);
int width = GetSystemMetrics(SM_CXSCREEN);
float wth = 1.0f;
float htw = 1.0f;
float scale = 1.0f;
float sx = 0.0f,sy = 0.0f;
int func = 0;
static HGLRC hRC;
static HDC hDC;
bool keys[256];

GLvoid ChooseFunc(){
    while(true){
        cin >> func;
    }
}

GLvoid InitGL(GLsizei Width, GLsizei Height){
    gladLoadGL();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,Width,Height);
    if (width > height){
        glOrtho(-1.0 * ((float)width/height), 1.0 * ((float)width/height), -1.0, 1.0, -1.0, 1.0);
    }
    else {
        glOrtho(-1.0, 1.0, -1.0 * ((float)height/width), 1.0 * ((float)height/width), -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

GLvoid KillGLWindow(GLvoid)
{
    ChangeDisplaySettings(NULL,0);
    ShowCursor(TRUE);
    if (hRC)
    {
        wglMakeCurrent(NULL,NULL);
        wglDeleteContext(hRC);
        hRC=NULL;
    }
    if (hDC && !ReleaseDC(hWnd,hDC)) 
    {
        hDC=NULL;
    }
    if (hWnd && !DestroyWindow(hWnd))
    {
        hWnd=NULL;
    }
    if (!UnregisterClass("Grafs",hInst))
    {
        hInst=NULL;
    }
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
    if (Height==0){
        Height=1;
    }
    glViewport(0,0,Width,Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (Width > Height){
        glOrtho(-1.0 * ((float)Width/Height), 1.0 * ((float)Width/Height), -1.0, 1.0, -1.0, 1.0);
    }
    else {
        glOrtho(-1.0, 1.0, -1.0 * ((float)Height/Width), 1.0 * ((float)Height/Width), -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

GLvoid DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glScalef(scale,scale,1.0f);
    glTranslatef(sx,sy,0.0f);

    glBegin(GL_LINES);

    glColor3f(1.0f,0.0f,0.0f);//x
    glVertex3f(-200.0f,0.0f,0.0f);
    glVertex3f(200.0f,0.0f,0.0f);

    glColor3f(0.0f,1.0f,0.0f);//y
    glVertex3f(0.0f,-200.0f,0.0f);
    glVertex3f(0.0f,200.0f,0.0f);

    glColor3f(1.0f,0.0f,0.0f);
    for(float x = -200.0f;x <= 200.0f;x += 0.1f){
        glVertex3f(x,0.05f,0.0f);
	glVertex3f(x,-0.05f,0.0f);
    }

    glColor3f(0.0f,1.0f,0.0f);
    for(float y = -200.0f;y <= 200.0f;y += 0.1f){
        glVertex3f(0.05f,y,0.0f);
	glVertex3f(-0.05f,y,0.0f);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f,0.0f,1.0f);
    for(float x = -200.0f;x <= 200.0f;x += 0.001f){
        float y;
        switch(func)
        {
            case 0:
                y = x;
                break;
            case 1:
                y = x*x;
                break;
            case 2:
                y = x*x*x;
                break;
            case 3:
                y = sqrt(x);
                break;
            case 4:
                y = tan(abs(x));
                break;
            case 5:
                y = pow(x,0.3f);
                break;
            case 6:
                y = cbrt(x*x) + 0.9f * sqrt((3.3f - x*x)) * sin(12*M_PI*x);
                break;
        }
        glVertex3f(x,y,0.0f);
    }
    glEnd();

}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    RECT Screen;
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        16,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    switch(message){
        case WM_CREATE:
            hDC = GetDC(hWnd);
            PixelFormat = ChoosePixelFormat(hDC, &pfd);
            if (!PixelFormat)
            {
                PostQuitMessage(0);
                break;
            }
            if(!SetPixelFormat(hDC,PixelFormat,&pfd))
            {
                PostQuitMessage(0);
                break;
            }
            hRC = wglCreateContext(hDC);
            if(!hRC)
            {
                PostQuitMessage(0);
                break;
            }
            if(!wglMakeCurrent(hDC, hRC))
            {
                PostQuitMessage(0);
                break;
            }
            GetClientRect(hWnd, &Screen);
            InitGL(Screen.right, Screen.bottom);
            break;
        case WM_DESTROY:
        case WM_CLOSE:
            ChangeDisplaySettings(NULL, 0);
            wglMakeCurrent(hDC,NULL);
            wglDeleteContext(hRC);
            ReleaseDC(hWnd,hDC);
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            keys[wParam] = true;
            break;
        case WM_KEYUP:
            keys[wParam] = false;
            break;
        case WM_SIZE:
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
            break;
        case WM_SETCURSOR:
            ShowCursor(true);
        break;
        case WM_MOUSEWHEEL:
           if((int)wParam > 0){
               scale *= 1.5f;
           }
           else
           {
               scale *= 0.8f;
           }
        break;
        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    MSG msg;
    WNDCLASS wc;
    hInst = hInstance;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC) WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "Grafs";
    if(!RegisterClass(&wc))
    {    
        return FALSE;
    }
    hWnd = CreateWindow("Grafs","OpenGL",WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,0,0,width,height,NULL,NULL,hInstance,NULL);
    if(!hWnd)
    {
        return FALSE;
    }
    DEVMODE dmScreenSettings;

    memset(&dmScreenSettings, 0, sizeof(DEVMODE));
    dmScreenSettings.dmSize = sizeof(DEVMODE);
    dmScreenSettings.dmPelsWidth = width;
    dmScreenSettings.dmPelsHeight = height;
    dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    SetFocus(hWnd);
    

    if (width > height){
        wth = ((float)width/height);
    }
    else {
        htw = ((float)height/width);
    }

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ChooseFunc, NULL, 0, NULL);

    while (true)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
               return TRUE;
            }
        }

        DrawGLScene();

        SwapBuffers(hDC);

        if(keys[VK_UP]){
            sy-=0.005f*(1/scale);
        }
        if(keys[VK_DOWN]){
            sy+=0.005f*(1/scale);
        } 
        if(keys[VK_LEFT]){
            sx+=0.005f*(1/scale);
        } 
        if(keys[VK_RIGHT]){
            sx-=0.005f*(1/scale);
        }
        if(keys[VK_ESCAPE]){
            KillGLWindow();
            return 0;
        }
    }
}