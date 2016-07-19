// startManexTrader.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "startManexTrader.h"
#include <comdef.h>
#include <mshtml.h>

#import <shdocvw.dll>
#import <mshtml.tlb>


#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;                                // ���݂̃C���^�[�t�F�C�X
WCHAR szTitle[MAX_LOADSTRING];                  // �^�C�g�� �o�[�̃e�L�X�g
WCHAR szWindowClass[MAX_LOADSTRING];            // ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void NewIE();
void IEBusyWait(SHDocVw::IWebBrowser2Ptr pIE);
TCHAR* getIniVal(TCHAR *buf, TCHAR* name, int size);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: �����ɃR�[�h��}�����Ă��������B
	HRESULT hr = ::CoInitialize(NULL);
	if (FAILED(hr)) {
		return -1;
	}



	// �O���[�o������������������Ă��܂��B
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_STARTMANEXTRADER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STARTMANEXTRADER));

	MSG msg;

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STARTMANEXTRADER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_STARTMANEXTRADER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

#define BUTTON2 200

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 500, 300, nullptr, nullptr, hInstance, nullptr);

	HWND hButton1, hButton2;

	hButton2 = CreateWindow(TEXT("BUTTON"),
		TEXT("�{�^��2"), WS_CHILD | WS_VISIBLE,
		10, 43, 75, 23, hWnd, (HMENU)BUTTON2, hInst, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:    ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E�̕`��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case BUTTON2:
			// MessageBox(hWnd, L"test", L"�m�F", MB_OK);
			NewIE();
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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: HDC ���g�p����`��R�[�h�������ɒǉ����Ă�������...
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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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

void NewIE()
{
	SHDocVw::IWebBrowser2Ptr pIE;

	//IE�̋N��
	HRESULT hr = pIE.CreateInstance(__uuidof(SHDocVw::InternetExplorer));
	//	HRESULT hr = pIE.CreateInstance(_T("InternetExplorer.Application"));
	if (FAILED(hr)) {
		TCHAR str[256];
		wsprintf(str, _T("NewIE#CreateInstance error:%x\n"), hr);
		::OutputDebugString(str);
		return;
	}

	//IE�̕\��
	pIE->PutVisible(VARIANT_TRUE);
	

	pIE->Navigate(_T("https://www.deal.matsui.co.jp/ITS/login/MemberLogin.jsp"));
	IEBusyWait(pIE);


	VARIANT data;

	MSHTML::IHTMLDocument2Ptr pHtmlDoc;
	pHtmlDoc = pIE->GetDocument();

	MSHTML::IHTMLElementCollectionPtr allElements = pHtmlDoc->all;

	MSHTML::IHTMLElementPtr pLogin;

	TCHAR buf[256];

	for (long i = 0;i < allElements->length;i++)
	{
		MSHTML::IHTMLElementPtr spElement = allElements->item(i, i);
		_bstr_t tag = spElement->tagName;

		if (_tcscmp((LPCTSTR)tag, _T("INPUT")) != 0)
			continue;

		_bstr_t tagName = spElement->getAttribute(_T("name"), 0);
		_bstr_t tagId = spElement->getAttribute(_T("id"), 0);

		if (tagName.GetBSTR() == nullptr) {
			if (tagId.GetBSTR() == nullptr) {
				continue;
			}
			else {
				if (_tcscmp((LPCTSTR)tagId, _T("btn_opn_login")) == 0) {
					pLogin = spElement;
				}
				continue;
			}
		}



		if (_tcscmp((LPCTSTR)tagName, _T("clientCD")) == 0) {
			MSHTML::IHTMLInputTextElementPtr pText = (MSHTML::IHTMLInputTextElementPtr)spElement;
			getIniVal(buf, _T("username"), sizeof(buf));
			pText->Putvalue(_bstr_t(buf));
		}

		if (_tcscmp((LPCTSTR)tagName, _T("passwd")) == 0) {
			MSHTML::IHTMLInputTextElementPtr pText = (MSHTML::IHTMLInputTextElementPtr)spElement;
			getIniVal(buf, _T("password"), sizeof(buf));
			pText->Putvalue(_bstr_t(buf));
		}

		//_bstr_t valName = ::SysAllocString(L"value");
		//BSTR strSrc = ::SysAllocString(L"testAAAA");
		//VariantInit(&data);
		//data.vt = VT_BSTR;
		//data.bstrVal = strSrc;
		//spElement->setAttribute(valName, &data, 0);
	}

	// Do Login
	pLogin->click();

	IEBusyWait(pIE);

	pHtmlDoc = nullptr;
	pHtmlDoc = pIE->GetDocument();
	MSHTML::IHTMLElementCollectionPtr allFrames = pHtmlDoc->all;

	OutputDebugString(_T("���O�C���I"));
	OutputDebugString(_T("\n"));
	for (long i = 0;i < allFrames->length;i++)
	{
		MSHTML::IHTMLElementPtr spElement = allFrames->item(i, i);
		_bstr_t tag = spElement->tagName;

		if (_tcscmp((LPCTSTR)tag, _T("FRAME")) != 0)
			continue;

		MSHTML::IHTMLFrameBase2* frame = NULL;

		spElement->QueryInterface(::IID_IHTMLFrameBase2, (void**)&frame);

		if (spElement == NULL) {
			continue;
		}
		//spElement->Release();


		_bstr_t fName = spElement->getAttribute(_T("name"), 0);

		if (frame == NULL || _tcscmp(fName, _T("LM"))) {
			continue;	// frame�ȊO�̗v�f�͂����ł͑ΏۊO�Ƃ���
		}
		MSHTML::IHTMLWindow2* wnd = NULL;
		frame->get_contentWindow(&wnd);

		if (frame == NULL) {
			continue;
		}

		frame->Release();
		if (wnd == NULL) {
			continue;	// ���Ԃ񂱂��͎��s���Ȃ��͂�..
		}
		MSHTML::IHTMLDocument2* frameDoc = NULL;
		wnd->get_document(&frameDoc);




		MSHTML::IHTMLElementCollectionPtr nFrames = frameDoc->all;
		for (long i = 0;i < nFrames->length;i++)
		{
			MSHTML::IHTMLElementPtr spElement = nFrames->item(i, i);
			_bstr_t tag = spElement->tagName;
			if (_tcscmp((LPCTSTR)tag, _T("A")) != 0)
				continue;
			_bstr_t href = spElement->GetinnerHTML();

			if (_tcscmp((LPCTSTR)href, _T("�l�b�g�X�g�b�N�E�n�C�X�s�[�h")) != 0)
				continue;

			spElement->click();
			break;
		}
	}

	
	IEBusyWait(pIE);

	pHtmlDoc = nullptr;
	pHtmlDoc = pIE->GetDocument();
	MSHTML::IHTMLElementCollectionPtr nFrames2 = pHtmlDoc->all;

	for (long i = 0;i < nFrames2->length;i++)
	{
		MSHTML::IHTMLElementPtr spElement = nFrames2->item(i, i);
		_bstr_t tag = spElement->tagName;

		if (_tcscmp((LPCTSTR)tag, _T("FRAME")) != 0)
			continue;

		MSHTML::IHTMLFrameBase2* frame = NULL;

		spElement->QueryInterface(::IID_IHTMLFrameBase2, (void**)&frame);

		if (spElement == NULL) {
			continue;
		}
		//spElement->Release();


		_bstr_t fName = spElement->getAttribute(_T("name"), 0);

		if (frame == NULL || _tcscmp(fName, _T("CT"))) {
			continue;	// frame�ȊO�̗v�f�͂����ł͑ΏۊO�Ƃ���
		}
		MSHTML::IHTMLWindow2* wnd = NULL;
		frame->get_contentWindow(&wnd);

		if (frame == NULL) {
			continue;
		}

		frame->Release();
		if (wnd == NULL) {
			continue;	// ���Ԃ񂱂��͎��s���Ȃ��͂�..
		}
		MSHTML::IHTMLDocument2* frameDoc = NULL;
		wnd->get_document(&frameDoc);

		
		MSHTML::IHTMLElementCollectionPtr nFrames = frameDoc->all;
		for (long i = 0;i < nFrames->length;i++)
		{
			MSHTML::IHTMLElementPtr spElement = nFrames->item(i, i);

			_bstr_t tag = spElement->tagName;

			OutputDebugString(tag);
			OutputDebugString(_T("\n"));

			if (_tcscmp((LPCTSTR)tag, _T("IMG")) != 0)
				continue;

			_bstr_t tagName = spElement->getAttribute(_T("name"), 0);

			if (tagName.GetBSTR() == NULL) {
				continue;
			}

			if (_tcscmp((LPCTSTR)tagName, _T("kidouButton")) != 0)
				continue;

			_bstr_t nh = spElement->parentElement->getAttribute(_T("href"), 0);

			pIE->Navigate(nh);
			
			break;
		}
	}


}

TCHAR* getIniVal(TCHAR *buf, TCHAR* name, int size)
{
	DWORD  ret = 0;

	ret = GetPrivateProfileString(_T("account"), name, _T("nouser"), buf, size, _T(".\\app.ini"));

	return buf;
}


void IEBusyWait(SHDocVw::IWebBrowser2Ptr pIE)
{
	if (pIE == NULL) {
		return;
	}

	VARIANT_BOOL bBusy;
	while (1) {
		HRESULT hr = pIE->get_Busy(&bBusy);
		if (FAILED(hr)) {
			// �Ȃɂ��G���[���Ă���
			break;
		}
		if (bBusy == S_OK) {
			break;
		}

		Sleep(1000);
	}
}