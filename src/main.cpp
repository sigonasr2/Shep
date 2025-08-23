/*
	Example file for olcUTIL_Hardware3D.h

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2025 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, �OneLoneCoder 2019, 2020, 2021, 2022, 2023, 2024, 2025

*/


//#define OLC_GFX_OPENGL33
#include "olcUTIL_Geometry2D.h"
#include "olcUTIL_Hardware3D.h"
#include "olcPixelGameEngine.h"
#include "util.h"


class Quad3D : public olc::PixelGameEngine
{
public:
	Quad3D()
	{
		sAppName = "Hardware3D Assetless Demo";
	}

	olc::mf4d matWorld;
	olc::mf4d matView;
	olc::mf4d matProject;
	
	olc::Renderable texCube;
	olc::utils::hw3d::mesh meshFloorSpr;
	olc::utils::hw3d::mesh meshSpr;
	olc::utils::hw3d::mesh meshLightCube;

	std::array<olc::vf3d, 64> cubes;
	std::array<olc::vf3d, 3> lights;
	
	void CreateSpriteMesh(){
		meshSpr.pos.push_back({ 0,0,0 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 0, 1 }); meshSpr.col.push_back(olc::WHITE);
		meshSpr.pos.push_back({ 1,0,0 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 1, 1 }); meshSpr.col.push_back(olc::WHITE);
		meshSpr.pos.push_back({ 1,1,0 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 1, 0 }); meshSpr.col.push_back(olc::WHITE);
		meshSpr.pos.push_back({ 0,0,0 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 0, 1 }); meshSpr.col.push_back(olc::WHITE);
		meshSpr.pos.push_back({ 1,1,0 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 1, 0 }); meshSpr.col.push_back(olc::WHITE);
		meshSpr.pos.push_back({ 0,1,0 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 0, 0 }); meshSpr.col.push_back(olc::WHITE);
	}
	void CreateFloorSpriteMesh(){
		meshFloorSpr.pos.push_back({ 0,0,0 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0.25, 0.25 }); meshFloorSpr.col.push_back(olc::WHITE);
		meshFloorSpr.pos.push_back({ 1,0,0 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0.5, 0.25 }); meshFloorSpr.col.push_back(olc::WHITE);
		meshFloorSpr.pos.push_back({ 1,0,1 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0.5, 0.0 }); meshFloorSpr.col.push_back(olc::WHITE);
		meshFloorSpr.pos.push_back({ 0,0,0 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0.25, 0.25 }); meshFloorSpr.col.push_back(olc::WHITE);
		meshFloorSpr.pos.push_back({ 1,0,1 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0.5, 0.0 }); meshFloorSpr.col.push_back(olc::WHITE);
		meshFloorSpr.pos.push_back({ 0,0,1 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0.25, 0.0 }); meshFloorSpr.col.push_back(olc::WHITE);
	}

public:
	bool OnUserCreate() override
	{
		float fAspect = float(GetScreenSize().y) / float(GetScreenSize().x);
		float S = 1.0f / (tan(3.14159f * 0.25f));
		float f = 1000.0f;
		float n = 0.1f;

		matProject(0, 0) = fAspect;	 matProject(0, 1) = 0.0f; matProject(0, 2) = 0.0f;	               matProject(0, 3) = 0.0f;
		matProject(1, 0) = 0.0f; matProject(1, 1) = 1;    matProject(1, 2) = 0.0f;                 matProject(1, 3) = 0.0f;
		matProject(2, 0) = 0.0f; matProject(2, 1) = 0.0f; matProject(2, 2) = -(f / (f - n));       matProject(2, 3) = -1.0f;
		matProject(3, 0) = 0.0f; matProject(3, 1) = 0.0f; matProject(3, 2) = -((f * n) / (f - n)); matProject(3, 3) = 0.0f;

		matWorld.identity();
		matView.identity();

		CreateSpriteMesh();
		CreateFloorSpriteMesh();

		// Why 2 cubes? the regular ones will have their vertex information recoloured

		// Create texture (so we dont need to load anything)
		texCube.Load("assets/gfx/nico-Trapper_512.png");

		// Position cubes nicely
		for(int x=0; x<8; x++)
			for (int y = 0; y < 8; y++)
			{
				float z = sin(float(x)) + cos(float(y));
				cubes[y * 8 + x] = { float(x) - 4.0f, float(z), float(y) - 4.0f };
			}


		Clear(olc::VERY_DARK_BLUE);
		HW3D_Projection(camera.GetProjectionMatrix().m);
		HW3D_SetCullMode(olc::CullMode::CCW);
		return true;
	}

	float fLightTime = 0.0f;

	hw3d::Camera3D_Orbit camera;
	vf2d moved{};
	float rot;

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		olc::mf4d m1, m2, m3, m4;

		camera.Spin(moved+=vf2d{fElapsedTime,fElapsedTime});
		camera.Update();

		using namespace olc;

		if(GetKey(olc::Key::HOME).bHeld)rot-=fElapsedTime*10;
		if(GetKey(olc::Key::END).bHeld)rot+=fElapsedTime*10;
		
		// fake a pseudo-view matrix by transforming in an identity view
		m2.rotateX(util::degToRad(rot));
		matView = camera.GetViewMatrix();

		// Clear background
		ClearBuffer(olc::BLANK, true);
		
		// Update light positions
		fLightTime += fElapsedTime;
		lights[0] = { 6.0f * sin(fLightTime * 2.5f), 6.0f * cos(fLightTime * 2.5f), 0.0f };
		lights[1] = { 0.0f, 6.0f * sin(fLightTime), 6.0f * cos(fLightTime) };
		lights[2] = { 6.0f * cos(fLightTime * 1.7f), 0.0f, 6.0f * sin(fLightTime * 1.7f) };
		
		// World Space lighting! The 3 lights are used as directional light sources
		// so i dont need to pre-compute all the geometry on the CPU. This is a 
		// limitation of the hw3d approach but like I said, its for basic 3D usage.
		const std::vector<std::reference_wrapper<utils::hw3d::mesh>>meshes{
			meshSpr,meshFloorSpr,
		};
		for(utils::hw3d::mesh&meshRef:meshes){
			for (size_t i = 0; i < meshRef.pos.size(); i += 3)
			{
				const auto& p0 = meshRef.pos[i + 0];
				const auto& p1 = meshRef.pos[i + 1];
				const auto& p2 = meshRef.pos[i + 2];

				// Hand calculate surface normal (i know i know the norms are already there...)
				olc::vf3d vCross = olc::vf3d(p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]).cross(olc::vf3d(p2[0] - p0[0], p2[1] - p0[1], p2[2] - p0[2])).norm();

				// Additive colouring
				meshRef.col[i + 0] = olc::BLACK;
				meshRef.col[i + 1] = olc::BLACK;
				meshRef.col[i + 2] = olc::BLACK;

				olc::Pixel c[] = { olc::RED, olc::GREEN, olc::BLUE };
				for (int j = 0; j < 3; j++)
				{
					olc::vf3d vLight = -lights[j].norm();
					float illum = std::max(-vCross.dot(vLight), 0.0f) * 0.8f + 0.2f;
					meshRef.col[i + 0] += olc::PixelF(illum, illum, illum, 1.0f) * c[j];
					meshRef.col[i + 1] += olc::PixelF(illum, illum, illum, 1.0f) * c[j];
					meshRef.col[i + 2] += olc::PixelF(illum, illum, illum, 1.0f) * c[j];
				}
			}
		}


		// Draw all cubes
		for (const auto& cube : cubes)
		{
			matWorld.translate(cube);
			HW3D_DrawObject((matView * matWorld).m, texCube.Decal(), meshSpr.layout, meshSpr.pos, meshSpr.uv, meshSpr.col);
		}

		// Draw light cubes
		matWorld.translate(lights[0]);
		HW3D_DrawObject((matView * matWorld).m, nullptr, meshLightCube.layout, meshLightCube.pos, meshLightCube.uv, meshLightCube.col, olc::RED);
		matWorld.translate(lights[1]);
		HW3D_DrawObject((matView * matWorld).m, nullptr, meshLightCube.layout, meshLightCube.pos, meshLightCube.uv, meshLightCube.col, olc::GREEN);
		matWorld.translate(lights[2]);
		HW3D_DrawObject((matView * matWorld).m, nullptr, meshLightCube.layout, meshLightCube.pos, meshLightCube.uv, meshLightCube.col, olc::BLUE);
		return true;
	}
};

int main()
{
	Quad3D demo;
	if (demo.Construct(1280, 720, 1, 1, false, false))
		demo.Start();
	return 0;
}