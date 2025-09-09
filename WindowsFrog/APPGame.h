#pragma once

class AppGame
{
public:
	void FrameGo()
	{
		MSG msg;
		BOOL gbool = true;
		Graphics g(hdc);
		LoadSVGDataMap(L"LVL0");
		while (gbool)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				UpdateApp(&msg);
				if (msg.message == WM_QUIT)
				{
					gbool = false;
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Render();
		}
	}
private:
	void UpdateApp(MSG* msg);
	void Render();
}App;


void AppGame::UpdateApp(MSG* msg)
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		msg->message = WM_QUIT;
	}
}
void AppGame::Render()
{

	//Sleep(16);
	location[player->currentLocation].hBack.showBack();
	player->Sprite.show();
}