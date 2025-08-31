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

#include "olcUTIL_Hardware3D.h"
#include "olcPixelGameEngine.h"

using Action=std::vector<Key>;

class GameSettings{
public:
	inline static constexpr bool LIGHTS_VISIBLE{true};
	inline static constexpr bool DEBUG_CAMERA{true};

	inline static constexpr float PLAYER_SPD{5.f};
	inline static vf3d CAMERA_FOLLOW_POS{0,-3.29,-2.5};
	inline static float CAMERA_FOLLOW_SPD{8.f};
	inline static float CAMERA_TILT{54.54};

	inline static float FRAME_TIME_PER_CHAR{1/30.f};
	inline static Action CONFIRM_DIALOG{{Key::ENTER,Key::SPACE,Key::Z,Key::SHIFT,Key::RETURN}};

	#define S inline static std::string_view
	class Text{
		S INTRO{"Welcome! You are about to begin a great journey of your own."};
		S INTRO_2{"This land is inhabited by creatures called Shep! "
			"Where they come from and their mysteries have yet to be discovered. Why are they here?"};
		S INTRO_3{"Speaking of, what is your name?"};
		S INTRO_4{"Your adventure is about to unfold, come by my lab when you are ready! I will see you there."};
		S MOM_INTRO{"Oh {}! I know today is the big day, before you go I want to give you this!"};
		S MOM_SHAPEDIA{"It's a Shapedia! As you explore it will keep track of your findings. I'm sure it will come in handy!"};
		S MOM_OUTFIT{"Also are you sure you want to be in that outfit? It's a bit tacky..."};
		S MOM_OUTFIT2{"Oh, you like your knight outfit that much don't you? I understand. Have fun today {}!"};
	};
	#undef S

	using TileID=uint32_t;
	using TileSetFilenamePair=std::pair<std::string,std::string>;
	using GraphicFilename=std::string;

	inline static const std::unordered_map<TileID,TileSetFilenamePair>TILE_MODELS{
		{296,{"Field Tiles.tsx","tree.png"}},
		{321,{"Field Tiles.tsx","tree.png"}},
		{319,{"Field Tiles.tsx","tree.png"}},
		{344,{"Field Tiles.tsx","tree.png"}},
	};
	inline static std::unordered_map<TileID,GraphicFilename>CONVERTED_TILE_MODELS;
};