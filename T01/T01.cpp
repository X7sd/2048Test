#pragma once
#include <Application.hpp>
#include <DataManager.hpp>
#include <Input.hpp>
#include <PixelWorld.hpp>
#include <WorldMap.hpp>
#include <iostream>
#include <stdio.h>
#include <random>

#define random(a,b) (rand()%(b-a+1)+a)

PixelWorldEngine::Application tquiz = PixelWorldEngine::Application("Test Quiz");
PixelWorldEngine::DataManager data = PixelWorldEngine::DataManager(&tquiz);
PixelWorldEngine::PixelWorld print = PixelWorldEngine::PixelWorld("Print", &tquiz);
PixelWorldEngine::WorldMap map = PixelWorldEngine::WorldMap("Map", 5, 5);
PixelWorldEngine::Camera cam = PixelWorldEngine::Camera(PixelWorldEngine::RectangleF(0, 0, 600, 600));
PixelWorldEngine::Graphics::Texture2D* tilet[15];
int16_t dmap[5][5];
bool pressed; //这个变量标记是否需要更新
int key1, key2, key3, key4; //这些变量表示上下左右键的状态

auto IntToString(int Int) -> std::string {
	std::string result = "";

	while (Int > 0) {
		char c = (Int % 10) + '0';
		result = c + result;

		Int /= 10;
	}

	return result;
}

auto p2(int Int) -> std::int16_t //计算2^n
{
	std::int16_t result = 1;
	for (int e = 1; e <= Int; e++)
	{
		result *= 2;
	}
	return result;
}

auto l2(int Int) -> std::int16_t //计算log_2(n)
{
	std::int16_t result = 0;
	for (int e = Int; e > 1; e/=2)
	{
		result++;
	}
	return result;
}

void OnKeyEvent(void* sender, PixelWorldEngine::Events::KeyClickEvent* eventArg) {

}

void OnUpdate(void* sender) {
	PixelWorldEngine::Application* app = (PixelWorldEngine::Application*)sender;

	app->SetWindow((std::string)"2048 Test", 600, 600);
	int x1, y1, x2, y2, d1, d2;
	srand((unsigned)time(NULL));
	if (pressed)
	{
		int blank = 0; //计算空位个数
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 1; j <= 4; j++)
			{
				if (dmap[i][j] == 0)
				{
					blank++;
				}
			}
		}
		if (blank>=1)
		{
			do
			{
				x1 = random(1, 4);
				y1 = random(1, 4);
			} while (dmap[x1][y1] != 0);
			d1 = random(1, 8) == 1 ? 4 : 2; //我猜出4的概率大概是1/8
			dmap[x1][y1] = d1;
			if (blank >= 2)
			{
				do
				{
					x2 = random(1, 4);
					y2 = random(1, 4);
				} while (dmap[x2][y2] != 0);
				d2 = random(1, 8) == 1 ? 4 : 2;
				dmap[x2][y2] = d2;
			}
		}
		for (int i = 1; i <= 4; i++)
		{
			std::cout << dmap[i][1] << " " << dmap[i][2] << " " << dmap[i][3] << " " << dmap[i][4] << std::endl;
		}
		std::cout << std::endl; //调试,可以在命令窗口看到当前地图
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 1; j <= 4; j++)
			{
				auto t = new PixelWorldEngine::MapData();
				t->RenderObjectID[0] = dmap[i][j];
				map.SetMapData(i, j, t);
				delete(t);
			}
		}
		pressed = false;
		glm::vec2 transform(0, 0);  //更新地图
		print.SetWorldMap(&map);
		cam.Move(transform);
	}
	for (int i = 1; i <= 4; i++)  //获胜判定
	{
		for (int j = 1; j <= 4; j++)
		{
			if (dmap[i][j] == 2048)
			{
				app->SetWindow((std::string)"You Win!", 600, 600);
				Sleep(1000);
				app->HideWindow();
				exit(0);
			}
		}
	}
	bool lost = true;  //死亡判定
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			if (dmap[i][j] == 0)
			{
				lost = false;
				break;
			}
		}
	}
	if (lost)
	{
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 1; j <= 3; j++)
			{
				if (dmap[i][j] == dmap[i][j + 1])
				{
					lost = false;
					break;
				}
			}
		}
		for (int j = 1; j <= 4; j++)
		{
			for (int i = 1; i <= 3; i++)
			{
				if (dmap[i][j] == dmap[i+1][j])
				{
					lost = false;
					break;
				}
			}
		}
	}
	if (lost)
	{
		app->SetWindow((std::string)"You Lose!", 600, 600);
		Sleep(1000);
		app->HideWindow();
		exit(0);
	}
	if (PixelWorldEngine::Input::GetKeyCodeDown(PixelWorldEngine::KeyCode::Left))  //判断方向键处于何种状态(0=松开,1=刚按下,2=保持按下状态)
	{
		if (key1 < 2)
		{
			key1++;
		}
	}
	else
	{
		key1 = 0;
	}
	if (key1==1)
	{
		pressed = true;
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 1; j <= 4; j++)
			{
				if (dmap[i][j] == 0)
				{
					int k = j;
					while ((dmap[i][k] == 0) && (k < 4))
					{
						k++;
					}
					dmap[i][j] = dmap[i][k];
					dmap[i][k] = 0;
				}
				if (dmap[i][j - 1] == dmap[i][j])
				{
					dmap[i][j - 1] *= 2;
					dmap[i][j] = 0;
					int k = j;
					while ((dmap[i][k] == 0) && (k < 4))
					{
						k++;
					}
					dmap[i][j] = dmap[i][k];
					dmap[i][k] = 0;
				}
			}
		}
	}
	if (PixelWorldEngine::Input::GetKeyCodeDown(PixelWorldEngine::KeyCode::Right))
	{
		if (key2 < 2)
		{
			key2++;
		}
	}
	else
	{
		key2 = 0;
	}
	if (key2==1)
	{
		pressed = true;
		for (int i = 1; i <= 4; i++)
		{
			for (int j = 4; j >= 1; j--)
			{
				if (dmap[i][j] == 0)
				{
					int k = j;
					while ((dmap[i][k] == 0) && (k > 1))
					{
						k--;
					}
					dmap[i][j] = dmap[i][k];
					dmap[i][k] = 0;
				}
				if (dmap[i][j + 1] == dmap[i][j])
				{
					dmap[i][j + 1] *= 2;
					dmap[i][j] = 0;
					int k = j;
					while ((dmap[i][k] == 0) && (k > 1))
					{
						k--;
					}
					dmap[i][j] = dmap[i][k];
					dmap[i][k] = 0;
				}
			}
		}
	}
	if (PixelWorldEngine::Input::GetKeyCodeDown(PixelWorldEngine::KeyCode::Down))
	{
		if (key3 < 2)
		{
			key3++;
		}
	}
	else
	{
		key3 = 0;
	}
	if (key3==1)
	{
		pressed = true;
		for (int j = 1; j <= 4; j++)
		{
			for (int i = 4; i >= 1; i--)
			{
				if (dmap[i][j] == 0)
				{
					int k = i;
					while ((dmap[k][j] == 0) && (k > 1))
					{
						k--;
					}
					dmap[i][j] = dmap[k][j];
					dmap[k][j] = 0;
				}
				if (dmap[i + 1][j] == dmap[i][j])
				{
					dmap[i + 1][j] *= 2;
					dmap[i][j] = 0;
					int k = i;
					while ((dmap[k][j] == 0) && (k > 1))
					{
						k--;
					}
					dmap[i][j] = dmap[k][j];
					dmap[k][j] = 0;
				}
			}
		}
	}
	if (PixelWorldEngine::Input::GetKeyCodeDown(PixelWorldEngine::KeyCode::Up))
	{
		if (key4 < 2)
		{
			key4++;
		}
	}
	else
	{
		key4 = 0;
	}
	if (key4==1)
	{
		pressed = true;
		for (int j = 1; j <= 4; j++)
		{
			for (int i = 1; i <= 4; i++)
			{
				if (dmap[i][j] == 0)
				{
					int k = i;
					while ((dmap[k][j] == 0) && (k < 4))
					{
						k++;
					}
					dmap[i][j] = dmap[k][j];
					dmap[k][j] = 0;
				}
				if (dmap[i - 1][j] == dmap[i][j])
				{
					dmap[i - 1][j] *= 2;
					dmap[i][j] = 0;
					int k = i;
					while ((dmap[k][j] == 0) && (k < 4))
					{
						k++;
					}
					dmap[i][j] = dmap[k][j];
					dmap[k][j] = 0;
				}
			}
		}
	}
}
int main()
{
	for (int i = 0; i <= 11; i++)  //注册纹理
	{
		tilet[i] = data.RegisterTexture("./tiles/" + IntToString(p2(i)) + ".png");
	}

	for (int i = 1; i <= 4; i++)  //初始化
	{
		for (int j = 1; j <= 4; j++)
		{
			dmap[i][j] = 0;
			auto t = new PixelWorldEngine::MapData();
			t->RenderObjectID[0] = 0;
			map.SetMapData(i, j, t);
		}
	}
	pressed = true;
	key1 = 0;
	key2 = 0;
	key3 = 0;
	key4 = 0;

	print.SetWorldMap(&map);
	print.SetResolution(600, 600);
	print.SetCamera(&cam);
	print.SetRenderObjectSize(150);

	tquiz.KeyClick.push_back(OnKeyEvent);
	tquiz.Update.push_back(OnUpdate);

	tquiz.MakeWindow("2048", 600, 600);
	tquiz.SetWorld(&print);
	tquiz.ShowWindow();
	tquiz.RunLoop();
}