#include "Mouse.h"

void Mouse::Initialize()
{
	position = D3DXVECTOR3(0, 0, 0);

	wheelStatus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wheelOldStatus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wheelMoveValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ZeroMemory(buttonCurStatus,	sizeof(BYTE) * MAX_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(BYTE) * MAX_MOUSE);
	ZeroMemory(buttonMap,		sizeof(BYTE) * MAX_MOUSE);

	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_MOUSE);
	ZeroMemory(buttonCount, sizeof(UINT)  * MAX_MOUSE);

	timeDblClk = GetDoubleClickTime();
	startDblClk[0] = (DWORD)GetTickCount64();

	for (int i = 1; i < MAX_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	DWORD line = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &line, 0);
}

void Mouse::Update()
{
	memcpy(buttonOldStatus, buttonCurStatus, sizeof(buttonOldStatus));

	ZeroMemory(buttonCurStatus, sizeof(buttonCurStatus));
	ZeroMemory(buttonMap, sizeof(buttonMap));

	buttonCurStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonCurStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonCurStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	for (DWORD i = 0; i < MAX_MOUSE; i++)
	{
		int oldStatus = buttonOldStatus[i];
		int curStatus = buttonCurStatus[i];

		if (oldStatus == 0 && curStatus == 1)
			buttonMap[i] = (BYTE)ButtonState::BUTTON_DOWN;
		else if (oldStatus == 1 && curStatus == 0)
			buttonMap[i] = (BYTE)ButtonState::BUTTON_UP;
		else if (oldStatus == 1 && curStatus == 1)
			buttonMap[i] = (BYTE)ButtonState::BUTTON_PRESS;
		else
			buttonMap[i] = (BYTE)ButtonState::BUTTON_NONE;
	}

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(handle, &point);

	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	wheelStatus.x = float(point.x);
	wheelStatus.y = float(point.y);

	wheelMoveValue = wheelStatus - wheelOldStatus;
	wheelOldStatus.z = wheelStatus.z;

	DWORD tButtonStatus = (DWORD)GetTickCount64();
	for (DWORD i = 0; i < MAX_MOUSE; i++)
	{
		if (buttonMap[i] == (BYTE)ButtonState::BUTTON_DOWN)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1)
				startDblClk[i] = tButtonStatus;
		}

		if (buttonMap[i] == (BYTE)ButtonState::BUTTON_UP)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)
			{
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk)
					buttonMap[i] = (BYTE)ButtonState::BUTTON_DBLCLK;

				buttonCount[i] = 0;
			}
		}
	}

	//ClientToScreen(handle, &point);
	//
	//oldPos = position;
	//
	//position.x = point.x;
	//position.y = point.y;
	//
	//delta = position - oldPos;

	//delta = Vector3(0, 0, 0);
}

LRESULT Mouse::MouseProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_MOUSEMOVE:
		oldPos = position;
		
		position.x = (float)LOWORD(lParam);
		position.y = (float)HIWORD(lParam);
		
		//delta = position - oldPos;

		return TRUE;
	case WM_MOUSEWHEEL:
		wheelOldStatus.z = wheelStatus.z;
		wheelStatus.z += (float)HIWORD(wParam);
		return TRUE;
	default:
		return FALSE;
	}
	
	return FALSE;
}

void Mouse::GetMouseRay(Vector3 & outOrigin, Vector3 & outDir)
{
	Vector3 mPos = GetPosition();

	float width = CONTEXT->GetViewport()->GetWidth();
	float height = CONTEXT->GetViewport()->GetHeight();

	float ndcX = (2.0f * mPos.x / width) - 1.0f;
	float ndcY = 1.0f - (2.0f * mPos.y / height);

	Matrix view = CONTEXT->GetViewMatrix();
	Matrix proj = CONTEXT->GetProjectionMatrix();

	Matrix invView, invProj;
	D3DXMatrixInverse(&invView, nullptr, &view);
	D3DXMatrixInverse(&invProj, nullptr, &proj);

	Vector4 rayClip(ndcX, ndcY, -1.0f, 1.0f);

	Vector4 rayView;
	D3DXVec4Transform(&rayView, &rayClip, &invProj);

	rayView.z = 1.0f;
	rayView.w = 0.0f;

	Vector3 rayDir(rayView.x, rayView.y, rayView.z);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &invView);
	D3DXVec3Normalize(&rayDir, &rayDir);

	outOrigin = CONTEXT->GetCamera()->GetPosition();
	outDir = rayDir;
}
