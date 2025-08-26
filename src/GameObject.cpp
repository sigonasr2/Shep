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
#include "GameObject.h"
#include "GameSettings.h"

GameObject::GameObject(const vf3d&pos,const vf3d&scale,const ObjectID&id,const std::string&spriteMeshName,const MeshType&type)
		:pos(pos),scale(scale),id(id),spriteMeshName(spriteMeshName),type(type){};

const MeshType GameObject::GetMeshType()const{
	return type;
}

const GameObject::ObjectID GameObject::GetID()const{
	return id;
}

const hw3d::mesh&GameObject::GetMesh()const{
	switch(GetMeshType()){
		case MeshType::FLOOR:{
			return ShepGame::Game().GetFloorMesh();
		}break;
		case MeshType::SPRITE:{
			return ShepGame::Game().GetSpriteMesh();
		}break;
		case MeshType::OBJ:{
			return ShepGame::Game().GetMesh(spriteMeshName.substr(0,spriteMeshName.length()-4)+".obj");
		}break;
		default:{
			std::cout<<"WARNING! Mesh Type is not any of the expected values! THIS SHOULD NOT BE HAPPENING!"<<std::endl;
			throw;
		}
	}
}

const std::vector<std::array<float,2>>GameObject::GetUVs()const{
	if(anim){
		switch(GetMeshType()){
			case MeshType::FLOOR:
			case MeshType::OBJ:{
				std::cout<<"WARNING! Unimplemented animation code reached!"<<std::endl;
				throw;
			}break;
			case MeshType::SPRITE:{
				geom2d::rect<float>uv{animState.GetFrame(*anim).GetSourceUV()};
				return std::vector<std::array<float,2>>{{
					{{uv.pos.x,uv.pos.y+uv.size.y}},
					{{uv.pos.x+uv.size.x,uv.pos.y+uv.size.y}},
					{{uv.pos.x+uv.size.x,uv.pos.y}},
					{{uv.pos.x,uv.pos.y+uv.size.y}},
					{{uv.pos.x+uv.size.x,uv.pos.y}},
					{{uv.pos.x,uv.pos.y}},
				}};
			}break;
			default:{
				std::cout<<"WARNING! Mesh Type is not any of the expected values! THIS SHOULD NOT BE HAPPENING!"<<std::endl;
				throw;
			}
		}
	}
	else return GetMesh().uv;
}

const vf3d&GameObject::GetPos()const{
	return pos;
}

const vf3d&GameObject::GetScale()const{
	return scale;
}

const Renderable&GameObject::GetSprite()const{
	if(anim)return *animState.GetFrame(*anim).GetSourceImage();
	else return ShepGame::Game().GetSpr(spriteMeshName);
}

void GameObject::SetAutoScale(const vf2d&unitDivision){
	const Renderable&spr{GetSprite()};
	const vf2d imgSize{spr.Sprite()->Size()};
	scale.x=imgSize.x/unitDivision.x;
	scale.z=imgSize.y/unitDivision.y;
}

void GameObject::Update(const float&fElapsedTime){
	if(anim)animState.UpdateState(*anim,fElapsedTime);
	auto&game{ShepGame::Game()};

	switch(id){
		case ObjectID::PLAYER:{
			if(game.GetKey(Key::W).bHeld){
				pos.z-=fElapsedTime*GameSettings::PLAYER_SPD;
				if(anim)animState.ChangeState(*anim,AnimationState::WALK_N);
			}
			if(game.GetKey(Key::A).bHeld){
				pos.x-=fElapsedTime*GameSettings::PLAYER_SPD;
				if(anim)animState.ChangeState(*anim,AnimationState::WALK_W);
			}
			if(game.GetKey(Key::S).bHeld){
				pos.z+=fElapsedTime*GameSettings::PLAYER_SPD;
				if(anim)animState.ChangeState(*anim,AnimationState::WALK_S);
			}
			if(game.GetKey(Key::D).bHeld){
				pos.x+=fElapsedTime*GameSettings::PLAYER_SPD;
				if(anim)animState.ChangeState(*anim,AnimationState::WALK_E);
			}
			if(game.GetKey(Key::W).bReleased){
				if(anim)animState.ChangeState(*anim,AnimationState::STAND_N);
			}
			if(game.GetKey(Key::A).bReleased){
				if(anim)animState.ChangeState(*anim,AnimationState::STAND_W);
			}
			if(game.GetKey(Key::S).bReleased){
				if(anim)animState.ChangeState(*anim,AnimationState::STAND_S);
			}
			if(game.GetKey(Key::D).bReleased){
				if(anim)animState.ChangeState(*anim,AnimationState::STAND_E);
			}
		}break;
	}
}

void GameObject::ApplyCharacterAnimation(const uint8_t animInd,const SkinTone tone){
	std::unordered_map<AnimationState,Animate2D::FrameSequence>&animations{ShepGame::Game().GetCharacterAnimation(animInd,tone)};
	for(AnimationState&state:std::vector<AnimationState>{AnimationState::STAND_S,AnimationState::STAND_W,AnimationState::STAND_E,AnimationState::STAND_N,AnimationState::WALK_S,AnimationState::WALK_W,AnimationState::WALK_E,AnimationState::WALK_N}){
		animState.AddState(state,animations.at(state));
	}
	anim=Animate2D::AnimationState{};
}