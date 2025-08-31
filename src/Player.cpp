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

#include "ShepGame.h"
#include "GameSettings.h"

void GameObject::Player::Update(GameObject&self,const float&fElapsedTime){
	ShepGame&game{ShepGame::Game()};
	if(game.GetKey(Key::W).bHeld){
		vf2d moveToCoord{self.pos.xz()+vf2d{0.f,-fElapsedTime*GameSettings::PLAYER_SPD}};
		ShepGame::GetAdjustedMovePos(self,moveToCoord);
		self.pos.x=moveToCoord.x;
		self.pos.z=moveToCoord.y;
		if(self.anim)self.animState.ChangeState(*self.anim,AnimationState::WALK_N);
	}
	if(game.GetKey(Key::A).bHeld){
		vf2d moveToCoord{self.pos.xz()+vf2d{-fElapsedTime*GameSettings::PLAYER_SPD,0.f}};
		ShepGame::GetAdjustedMovePos(self,moveToCoord);
		self.pos.x=moveToCoord.x;
		self.pos.z=moveToCoord.y;
		if(self.anim)self.animState.ChangeState(*self.anim,AnimationState::WALK_W);
	}
	if(game.GetKey(Key::S).bHeld){
		vf2d moveToCoord{self.pos.xz()+vf2d{0.f,fElapsedTime*GameSettings::PLAYER_SPD}};
		ShepGame::GetAdjustedMovePos(self,moveToCoord);
		self.pos.x=moveToCoord.x;
		self.pos.z=moveToCoord.y;
		if(self.anim)self.animState.ChangeState(*self.anim,AnimationState::WALK_S);
	}
	if(game.GetKey(Key::D).bHeld){
		vf2d moveToCoord{self.pos.xz()+vf2d{fElapsedTime*GameSettings::PLAYER_SPD,0.f}};
		ShepGame::GetAdjustedMovePos(self,moveToCoord);
		self.pos.x=moveToCoord.x;
		self.pos.z=moveToCoord.y;
		if(self.anim)self.animState.ChangeState(*self.anim,AnimationState::WALK_E);
	}
	if(game.GetKey(Key::W).bReleased){
		if(self.anim)self.animState.ChangeState(*self.anim,AnimationState::STAND_N);
	}
	if(game.GetKey(Key::A).bReleased){
		if(self.anim)self.animState.ChangeState(*self.anim,AnimationState::STAND_W);
	}
	if(game.GetKey(Key::S).bReleased){
		if(self.anim)self.animState.ChangeState(*self.anim,AnimationState::STAND_S);
	}
	if(game.GetKey(Key::D).bReleased){
		if(self.anim)self.animState.ChangeState(*self.anim,AnimationState::STAND_E);
	}
}