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
	static float t{};

	auto&game{ShepGame::Game()};
	geom2d::rect<int>windowFrame{window.pos-vf2d{4,4},window.size+vf2d{8,8}};
	game.FillRectDecal(windowFrame.pos-vf2d{2,2},windowFrame.size+vf2d{4,4},BLACK);
	game.DrawPartialDecal(windowFrame.pos-vf2d{8,8},sheet.Decal(),{0.f,float(currentTheme*160)},{16.f,16.f},{2,2});
	game.DrawPartialDecal(windowFrame.pos+windowFrame.size-vf2d{24,24},sheet.Decal(),{0.f,float(currentTheme*160)},{16.f,16.f},{2,2});
	game.DrawPartialDecal(windowFrame.pos+vf2d{float(windowFrame.size.x),0.f}-vf2d{24,8},sheet.Decal(),{0.f,float(currentTheme*160)},{16.f,16.f},{2,2});
	game.DrawPartialDecal(windowFrame.pos+vf2d{0.f,float(windowFrame.size.y)}-vf2d{8,24},sheet.Decal(),{0.f,float(currentTheme*160)},{16.f,16.f},{2,2});
	Pixel transparentThemeColor{backCol.at(Color(currentTheme)).r,backCol.at(Color(currentTheme)).g,backCol.at(Color(currentTheme)).b,0};

	t+=0.4f*game.GetElapsedTime();
	
	const auto DrawRotatingGradientRectDecal=[&game](const geom2d::rect<float>&rect,const Pixel col,const float&t,const bool rotatesRight=true){
		float p{rect.perimeter()};
			
		Pixel c1{col},c2{col},c3{col},c4{col};

		float TL{0.f};
		float TR{rect.top().length()/p};
		float BR{0.5f};
		float BL{TR+BR};

		c4.a=abs(0.5-(std::fmod(t,1.f)))*255*2;
		c3.a=abs(0.5-(std::fmod(t+0.25f,1.f)))*255*2;
		c2.a=abs(0.5-(std::fmod(t+0.5f,1.f)))*255*2;
		c1.a=abs(0.5-(std::fmod(t+0.75f,1.f)))*255*2;

		if(rotatesRight){
			c1.a=255-c1.a;
			c2.a=255-c2.a;
			c3.a=255-c3.a;
			c4.a=255-c4.a;
		}
		
		game.GradientFillRectDecal(rect.pos,rect.size,c1,c2,c3,c4);
	};

	DrawRotatingGradientRectDecal(geom2d::rect<float>{windowFrame.pos,windowFrame.size},backCol.at(Color(currentTheme)),t);
	DrawRotatingGradientRectDecal(geom2d::rect<float>{windowFrame.pos+vf2d{2,2},windowFrame.size-vf2d{4,4}},BLACK,t,false);
	game.FillRectDecal(window.pos,window.size,backCol.at(Color(currentTheme)));
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