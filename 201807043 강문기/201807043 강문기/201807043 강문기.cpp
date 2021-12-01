// 201807043 강문기.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "201807043 강문기.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY201807043, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY201807043));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY201807043));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY201807043);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

HWND g_hWnd;
RECT  g_me, g_att;
int att_l, att_r;
bool attacking;
int g_time;
int g_point;

#define TIMER_ID_1 1		// 제한시간
#define TIMER_ID_2 2		// 공격 움직임

INT Setting()
{
	g_me.left = 270;
	g_me.top = 500;
	g_me.right = g_me.left + 30;
	g_me.bottom = g_me.top + 30;

	att_l = g_att.left = 280;
	g_att.top = 490;
	att_r = g_att.right = g_att.left + 10;
	g_att.bottom = g_att.top + 10;

	return 0;
}

DWORD WINAPI enemy(LPVOID param)
{
	HDC hdc;
	int x = (int)param;
	hdc = GetDC(g_hWnd);
	Ellipse(hdc, x, 200, 300, 300);
	Rectangle(hdc, 200, 200, 300, 300);
	//InvalidateRect(g_hWnd, NULL, FALSE);
	ReleaseDC(g_hWnd, hdc);
	ExitThread(0);
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		Setting();
		g_time = 10;
		SetTimer(hWnd, TIMER_ID_1, 1000, NULL);
		//CreateThread(NULL, 0, enemy, (LPVOID)10, 0, NULL);
	}
		break;
		
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		CreateThread(NULL, 0, enemy, (LPVOID)x, 0, NULL);
	}
		break;
	//본체 조작
	case WM_KEYUP:
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		//공격
		case VK_SPACE:
			SetTimer(hWnd, TIMER_ID_2, 10, NULL);
			attacking = true;
			break;
		//왼쪽 이동
		case VK_LEFT:
			g_me.left = g_me.left - 10;
			g_me.right = g_me.right - 10;
			att_l -= 10;
			att_r -= 10;
			break;
		//오른쪽 이동
		case VK_RIGHT:
			g_me.left = g_me.left + 10;
			g_me.right = g_me.right + 10;
			att_l += 10;
			att_r += 10;
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;

	case WM_TIMER:
	{
		if (TIMER_ID_1 == wParam)       // 제한 시간 ID 값에 의한 주기적인 호출인지 확인
		{
			// 제한 시간 감소
			g_time--;

			// 제한 시간이 만료되면.
			if (0 == g_time)
			{
				// 게임 종료
				KillTimer(hWnd, TIMER_ID_1);
				KillTimer(hWnd, TIMER_ID_2);
				MessageBox(hWnd, L"제한시간초과", L"GAME OVER", MB_OK);
				return 0;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		
		if (attacking)
		{
			g_att.top -= 10;
			g_att.bottom -= 10;
			if (g_att.top == 0)
			{
				g_att.top = 490;
				g_att.bottom = g_att.top + 10;
				g_att.left = g_me.left + 10;
				g_att.right = g_att.left + 10;
				attacking = false;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	
    case WM_PAINT:
        {
			WCHAR string[64] = { 0, };
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			wsprintfW(string, L"TIME : %d     POINT : %d", g_time, g_point);
			TextOut(hdc, 600, 30, string, lstrlenW(string));
			Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
			//if(attacking)
				Ellipse(hdc, att_l, g_att.top, att_r, g_att.bottom);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
