#pragma once
float lerp(float x1, float x2, float a)
{
	return x1 * (1 - a) + x2 * a;
}

float length(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
class AppGame
{
public:
	void FrameGo()
	{
		
		MSG msg;
		BOOL gbool = true;
		Graphics g(window.context);
		Font font(L"Times New Roman", 20.f, FontStyleBold);
		Pen blackPen(Color(255, 0, 0, 0), 3);
		SolidBrush solidBrush(Color::Black); 
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
			Render(g, font, solidBrush, blackPen);

		//	Sleep(16);

		}
	}
private:
	int dialogState = 0;
	bool keyProcessed = false;
	void UpdateApp(MSG* msg);
	void Render(Graphics& g, Font& font, SolidBrush& solidBrush, Pen& blackPen);
	void Init();
}App;


