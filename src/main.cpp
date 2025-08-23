#include "olcPixelGameEngine.h"

class ShepGame : public olc::PixelGameEngine
{
public:
	ShepGame()
	{
		// Name your application
		sAppName = "Shep";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		return true;
	}
};

int main()
{
	ShepGame demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}