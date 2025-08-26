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

#include "Theme.h"
#include "ShepGame.h"

Renderable Theme::sheet{};
uint8_t Theme::currentTheme{};
std::unordered_map<Theme::Color,Pixel>Theme::backCol{
	{Color::PINK,{204,102,145}},
	{Color::LILAC,{204,122,157}},
	{Color::BLUE,{81,163,204}},
	{Color::GREEN,{81,204,140}},
	{Color::RED,{178,35,41}},
	{Color::BLACK,{25,25,25}},
};

void Theme::Initialize(){
	auto&game{ShepGame::Game()};
	game.SetFontSprite("assets/gfx/font.png");
	sheet.Load("assets/gfx/CuteRPG_Icons.png",nullptr,false,false);
	std::cout<<"Theme sheet loaded"<<std::endl;
}
void Theme::DrawWindow(const geom2d::rect<int>window){
	auto&game{ShepGame::Game()};
	game.FillRectDecal(window.pos,window.size,backCol.at(Color(currentTheme)));
	game.DrawStringPropDecal(window.pos,"Hello World!!",WHITE,{4,4});
}
void Theme::ToggleNext(){
	currentTheme++;
	if(currentTheme>int(Color::BLACK))currentTheme=0;
}
void Theme::TogglePrev(){
	currentTheme--;
	if(currentTheme<0)currentTheme=int(Color::BLACK);
}
const uint8_t Theme::GetCurrentTheme(){
	return currentTheme;
}