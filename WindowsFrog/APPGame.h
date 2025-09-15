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
			Render(g, font, solidBrush);
			dialogCollision == false;
		//	Sleep(16);

		}
	}
private:
	void UpdateApp(MSG* msg);
	void Render(Graphics& g, Font& font, SolidBrush& solidBrush);
}App;


void AppGame::UpdateApp(MSG* msg)
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		msg->message = WM_QUIT;
	}
}
void AppGame::Render(Graphics& g, Font& font, SolidBrush& solidBrush)
{
	if(dialogCollision == false)
	{
		location[player->currentLocation].hBack.showBack(g);
		player->Sprite.show(g);
		player->move();

		for (int i = 0; i < location[player->currentLocation].Persona.size(); i++) {
			location[player->currentLocation].Persona[i]->dialog(player);
			location[player->currentLocation].Persona[i]->Sprite.show(g);
			location[player->currentLocation].Persona[i]->move();
		}
		//Health_bar.Show();

		for (int i = 0; i < location[player->currentLocation].walls.size(); i++) {
			location[player->currentLocation].walls[i].Sprite.show(g);
		}
		for (int i = 0; i < location[player->currentLocation].healingFlask.size(); i++) {
			location[player->currentLocation].healingFlask[i].Sprite.show(g);
			location[player->currentLocation].healingFlask[i].healing(player, i);
		}
		for (int i = 0; i < location[player->currentLocation].spike.size(); i++) {
			location[player->currentLocation].spike[i].Sprite.show(g);
			location[player->currentLocation].spike[i].damage(player);
		}
		for (int i = 0; i < location[player->currentLocation].portal.size(); i++) {
			location[player->currentLocation].portal[i].Sprite.show(g);
			location[player->currentLocation].portal[i].Portal(player);
		}


		//ProcessDash();//рывок
		//Ellipse(window.context, mouse.x - sz, mouse.y - sz, mouse.x + sz, mouse.y + sz);


		float ls = .2 * length(player_view.x, player->Sprite.x, player_view.y, player->Sprite.y) / 500.;
		ls = max(ls - .2, 0.1);
		ls = min(ls, 1);

		float cameraHalfWidth = (window.width / 2.) / scale;
		float cameraHalfHeight = (window.height / 2.) / scale;

		float targetX = player->Sprite.x;
		float targetY = player->Sprite.y;

		targetX = max(0 + cameraHalfWidth,
			min(window.width - cameraHalfWidth, targetX));
		targetY = max(0 + cameraHalfHeight,
			min(window.height - cameraHalfHeight, targetY));

		player_view.x = lerp(player_view.x, targetX, 0.1f);
		player_view.y = lerp(player_view.y, targetY, 0.1f);

	}
	else 
	{

		float txtX = 0;
		float txtY = window.height - (window.height / 5.);
		location[player->currentLocation].Persona[name]->DialogSprite.x = 0;
		location[player->currentLocation].Persona[name]->DialogSprite.y = window.height - (window.height / 4.);
		location[player->currentLocation].Persona[name]->DialogSprite.showDialog(g);
		PointF txtBounds(txtX, txtY);
		g.DrawString(L"Hello lkszdfngm,dznfg.m,nzd.fm,gn.z,mdnfg.m,znd.fgm,nz.d,mfng.zm,dfng.dm,fgn.z,mdfng.,zmdnfg.m,nzd.fm,gn.zdmf,gn", -1, &font, txtBounds, &solidBrush);
		//dialogCollision = false;

	}
	BitBlt(hdc, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);


}