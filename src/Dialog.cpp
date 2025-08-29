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

#include "olcUTIL_Geometry2D.h"
#include "Dialog.h"
#include "Theme.h"
#include "ShepGame.h"
#include <ranges>

Dialog Dialog::instance;

Dialog::Dialog():Trigger({}){};

void Dialog::Create(const std::string_view text,const std::function<void()>onComplete){	
	instance.isActive=true;
	instance.text=std::string(text);
	instance.cursor=0;
	instance.waitForReleaseKey=true;
	instance.Trigger::Trigger([onComplete](){instance.isActive=false;if(onComplete)onComplete();});
}
void Dialog::UpdateAndDraw(){
	auto&game{ShepGame::Game()};
	const bool CursorAtEnd=instance.cursor==instance.text.length();
	if(CursorAtEnd&&!instance.waitForReleaseKey&&game.GetInputPressed(GameSettings::CONFIRM_DIALOG))instance.trigger();
		
	if(instance.waitForReleaseKey&&!game.GetInput(GameSettings::CONFIRM_DIALOG))instance.waitForReleaseKey=false;
	if(!CursorAtEnd&&!instance.waitForReleaseKey&&game.GetInputPressed(GameSettings::CONFIRM_DIALOG)){
		instance.cursor=instance.text.length();
		instance.waitForReleaseKey=true;
	}

	instance.frameTime-=game.GetElapsedTime();
	if(instance.frameTime<=0.f){
		instance.frameTime=GameSettings::FRAME_TIME_PER_CHAR;
		instance.cursor=std::min(instance.text.length(),size_t(instance.cursor+1));
	}

	const vi2d WINDOW_UPPER_LEFT{int(game.GetScreenSize().x*1/6.f),int(game.GetScreenSize().y-192-16)};
	const vi2d WINDOW_SIZE{int(game.GetScreenSize().x*2/3.f),192};
	const geom2d::rect<int>WINDOW{WINDOW_UPPER_LEFT,WINDOW_SIZE};
	Theme::DrawWindow(WINDOW);
	std::string displayText{GetText()};
	game.DrawStringPropDecal(WINDOW_UPPER_LEFT+vi2d{8,0},displayText,WHITE,{4.f,4.f});
}
const bool Dialog::Exists(){
	return instance.isActive;
}
const std::string Dialog::GetText(){
	return instance.text.substr(0,instance.cursor);
}
const void Dialog::AdvanceToEnd(){
	instance.cursor=instance.text.length();
}