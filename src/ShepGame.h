#pragma region License
/*
License (OLC-3)
~~~~~~~~~~~~~~~

Copyright 2024 Joshua Sigona <sigonasr2@gmail.com>

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions or derivations of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions or derivative works in binary form must reproduce the above
copyright notice. This list of conditions and the following	disclaimer must be
reproduced in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may
be used to endorse or promote products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	"AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL,	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

Portions of this software are copyright © 2024 The FreeType
Project (www.freetype.org). Please see LICENSE_FT.txt for more information.
All rights reserved.
*/
#pragma endregion
#pragma once

#include "olcUTIL_Geometry2D.h"
#include "olcUTIL_Hardware3D.h"
#include "olcPixelGameEngine.h"
#include "util.h"
#include "Light.h"
#include "GameObject.h"

class ShepGame : public olc::PixelGameEngine
{
public:
	ShepGame();
	void LoadObj(const std::string&filename); //filename is the base filename inside the assets/models directory. NOTE: Textures loaded from this function are also based from the assets/models directory and match the model name Ex. building1.obj expects building1.png.
	void LoadSprite(const std::string&filename); //filename is the base filename inside the assets/gfx directory
	void AddGameObject(const GameObject&newObj);
	void AddLight(const Light&light);
	const Renderable&GetSpr(const std::string&filename)const;
	const hw3d::mesh&GetMesh(const std::string&filename)const;
	const hw3d::mesh&GetFloorMesh()const;
	const hw3d::mesh&GetSpriteMesh()const;
	const bool SpriteExists(const std::string&filename)const;
	static ShepGame&Game();
private:

	static ShepGame*game;

	olc::mf4d matWorld;
	olc::mf4d matView;
	olc::mf4d matProject;
	
	std::unordered_map<std::string,Renderable>assets;
	std::unordered_map<std::string,hw3d::mesh>meshes;
	std::vector<GameObject>objects;
	std::vector<Light>lights;

	hw3d::mesh meshSpr,meshFloorSpr;
	
	void InitializeSpriteMesh();
	void InitializeFloorSpriteMesh();

	virtual bool OnUserCreate()override;

	vf3d pos{0,-3.04,-10};
	float rot{28.9};

	virtual bool OnUserUpdate(float fElapsedTime)override;
};