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

	Renderable space;

	bool OnUserCreate() override
	{
		space.Load("assets/gfx/space.png");
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));

		DrawDecal({},space.Decal(),{1.f,1.f},WHITE,olc::GFX3DTransform::TRANSFORM_REQUIRED);
		return true;
	}
	virtual void Apply3DTransform(std::vector<DecalInstance>&decals)override{
		decals.clear();
	}
};

int main()
{
	ShepGame demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}