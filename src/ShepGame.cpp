#include "ShepGame.h"
#include "GameSettings.h"
#include "TMXParser.h"
#include <ranges>

ShepGame*ShepGame::game{nullptr};

ShepGame::ShepGame()
{
	sAppName = "Project Shep";
	game=this;
}
	
void ShepGame::InitializeSpriteMesh(){
	meshSpr.pos.push_back({ 0,0,0.5 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 0, 1 }); meshSpr.col.push_back(olc::WHITE);
	meshSpr.pos.push_back({ 1,0,0.5 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 1, 1 }); meshSpr.col.push_back(olc::WHITE);
	meshSpr.pos.push_back({ 1,1,0.5 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 1, 0 }); meshSpr.col.push_back(olc::WHITE);
	meshSpr.pos.push_back({ 0,0,0.5 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 0, 1 }); meshSpr.col.push_back(olc::WHITE);
	meshSpr.pos.push_back({ 1,1,0.5 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 1, 0 }); meshSpr.col.push_back(olc::WHITE);
	meshSpr.pos.push_back({ 0,1,0.5 }); meshSpr.norm.push_back({ 0, 0, -1, 0 }); meshSpr.uv.push_back({ 0, 0 }); meshSpr.col.push_back(olc::WHITE);
}
void ShepGame::InitializeFloorSpriteMesh(){
	meshFloorSpr.pos.push_back({ 0,0,1 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0, 1 }); meshFloorSpr.col.push_back(olc::WHITE);
	meshFloorSpr.pos.push_back({ 1,0,1 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 1, 1 }); meshFloorSpr.col.push_back(olc::WHITE);
	meshFloorSpr.pos.push_back({ 1,0,0 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 1, 0 }); meshFloorSpr.col.push_back(olc::WHITE);
	meshFloorSpr.pos.push_back({ 0,0,1 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0, 1 }); meshFloorSpr.col.push_back(olc::WHITE);
	meshFloorSpr.pos.push_back({ 1,0,0 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 1, 0 }); meshFloorSpr.col.push_back(olc::WHITE);
	meshFloorSpr.pos.push_back({ 0,0,0 }); meshFloorSpr.norm.push_back({ 0, 1, 0, 0 }); meshFloorSpr.uv.push_back({ 0, 0 }); meshFloorSpr.col.push_back(olc::WHITE);
}

void ShepGame::LoadObj(const std::string&filename){
	if(meshes.count(filename)){
		std::cout<<"WARNING! Duplicate model filename detected: "<<filename<<std::endl;
		throw;
	}
	meshes[filename]=std::move(*hw3d::LoadObj("assets/models/"+filename));

	std::string texFilename{filename.substr(0,filename.length()-4)+".png"};

	if(assets.count(texFilename))meshes[filename].tex=GetSpr(texFilename).Decal();
	else assets[texFilename].Load("assets/models/"+texFilename);
}

const Renderable&ShepGame::LoadSprite(const std::string&filename){
	if(assets.count(filename)){
		std::cout<<"WARNING! Duplicate sprite filename detected: "<<filename<<std::endl;
		throw;
	}
	assets[filename].Load("assets/gfx/"+filename);
	return assets.at(filename);
}

GameObject&ShepGame::AddGameObject(const vf3d&pos,const vf3d&scale,const GameObject::ObjectID&id,const std::string&spriteMeshName,const MeshType&type){
	switch(type){
		case MeshType::FLOOR:
		case MeshType::SPRITE:{
			if(!assets.count(spriteMeshName))LoadSprite(spriteMeshName);
		}break;
		case MeshType::OBJ:{
			if(!meshes.count(spriteMeshName.substr(0,spriteMeshName.length()-4)+".obj"))LoadObj("building1.obj");
		}break;
	}
	return objects.emplace_back(pos,scale,id,spriteMeshName,type);
}

void ShepGame::AddLight(const Light&light){
	lights.emplace_back(light);
}

const Renderable&ShepGame::GetSpr(const std::string&filename)const{
	return assets.at(filename);
}

const hw3d::mesh&ShepGame::GetMesh(const std::string&filename)const{
	return meshes.at(filename);
}
const hw3d::mesh&ShepGame::GetFloorMesh()const{
	return meshFloorSpr;
}
const hw3d::mesh&ShepGame::GetSpriteMesh()const{
	return meshSpr;
}

bool ShepGame::OnUserCreate(){
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

	InitializeSpriteMesh();
	InitializeFloorSpriteMesh();
	LoadAnimations();

	LoadMap("Town1.tmx");

	auto&player{AddGameObject({0,0,0},{1,2,1},GameObject::ObjectID::PLAYER,"nico-trapper.png",MeshType::SPRITE)};
	player.ApplyCharacterAnimation(29,SkinTone::TONE_2);

	AddLight({{4,0,10},WHITE});

	Clear(olc::VERY_DARK_BLUE);
	HW3D_Projection(matProject.m);
	HW3D_EnableDepthTest(true);
	HW3D_SetCullMode(olc::CullMode::CCW);

	return true;
}

void ShepGame::LoadMap(const std::string&filename){
	collisionTiles.clear();
	objects.clear();
	lights.clear();
	TMXParser map{"assets/maps/"+filename};
	std::vector<std::vector<int32_t>>tiles{map.GetData().GetLayers()[map.GetData().GetLayers().size()-1].tiles};
	for(int y:std::ranges::iota_view(size_t(0),tiles.size())){
		for(int x:std::ranges::iota_view(size_t(0),tiles[0].size())){
			if(tiles[y][x]!=0)collisionTiles.emplace(x,y);
		}
	}
	if(map.GetData().ZoneData.count("Building")>0){
		const std::vector<ZoneData>&zones{map.GetData().ZoneData.at("Building")};
		for(const ZoneData&zone:zones){
			geom2d::rect<int>buildingZone{zone.zone};
			vi2d spawnCoords{buildingZone.pos/map.GetData().GetMapData().TileSize};
			vi2d size{buildingZone.size/map.GetData().GetMapData().TileSize};
			for(const XMLTag&tag:zone.properties){
				const auto&data{tag.data};
				if(data.at("name")=="Building Type"){
					AddGameObject({float(spawnCoords.x),0,float(spawnCoords.y)},{1,1,1},GameObject::ObjectID::DEFAULT,data.at("value")+".png",MeshType::OBJ);
				}
			}

			for(int y:std::ranges::iota_view(spawnCoords.y,spawnCoords.y+size.y)){
				for(int x:std::ranges::iota_view(spawnCoords.x,spawnCoords.x+size.x)){
					collisionTiles.emplace(x,y);
				}
			}
		}	
	}
	auto&town{AddGameObject({0,0,0},{1,1,1},GameObject::ObjectID::DEFAULT,"../maps/"+filename.substr(0,filename.length()-3)+"png",MeshType::FLOOR)};
	town.SetAutoScale({16,16});
}

std::unordered_map<AnimationState,Animate2D::FrameSequence>&ShepGame::GetCharacterAnimation(const uint8_t animInd,const SkinTone tone){
	return characterAnimations.at((animInd-1)*4+int(tone));
}

void ShepGame::LoadAnimations(){
	for(int i:std::ranges::iota_view(1,42)){
		const Renderable&sheet{LoadSprite(std::format("Character_{:03}.png",i))};
		for(int j:std::ranges::iota_view(0,4)){
			std::unordered_map<AnimationState,Animate2D::FrameSequence>animation;
			(*animation.insert({AnimationState::STAND_N,Animate2D::FrameSequence{0.2f,Animate2D::Style::Repeat}}).first).second.AddFrame(Animate2D::Frame{&sheet,{{24+j*96,24*3},{24,24}}});
			(*animation.insert({AnimationState::STAND_E,Animate2D::FrameSequence{0.2f,Animate2D::Style::Repeat}}).first).second.AddFrame(Animate2D::Frame{&sheet,{{24+j*96,24*2},{24,24}}});
			(*animation.insert({AnimationState::STAND_S,Animate2D::FrameSequence{0.2f,Animate2D::Style::Repeat}}).first).second.AddFrame(Animate2D::Frame{&sheet,{{24+j*96,24*0},{24,24}}});
			(*animation.insert({AnimationState::STAND_W,Animate2D::FrameSequence{0.2f,Animate2D::Style::Repeat}}).first).second.AddFrame(Animate2D::Frame{&sheet,{{24+j*96,24*1},{24,24}}});
			for(uint8_t row{};AnimationState&state:std::vector<AnimationState>{AnimationState::WALK_S,AnimationState::WALK_W,AnimationState::WALK_E,AnimationState::WALK_N}){
				animation.insert({state,Animate2D::FrameSequence{0.2f,Animate2D::Style::Repeat}});
				for(int k:std::ranges::iota_view(0,4)){
					animation[state].AddFrame(Animate2D::Frame{&sheet,{{24*k+j*96,24*row},{24,24}}});
				}
				row++;
			}
			characterAnimations.emplace_back(std::move(animation));
		}
	}
}

bool ShepGame::OnUserUpdate(float fElapsedTime){
	using namespace olc;

	if(GameSettings::DEBUG_CAMERA){
		olc::mf4d m1, m2;
		if(GetKey(olc::Key::RIGHT).bHeld)pos.x+=fElapsedTime*5;
		if(GetKey(olc::Key::LEFT).bHeld)pos.x-=fElapsedTime*5;
		if(GetKey(olc::Key::UP).bHeld)pos.y-=fElapsedTime*5;
		if(GetKey(olc::Key::DOWN).bHeld)pos.y+=fElapsedTime*5;
		if(GetKey(olc::Key::PGUP).bHeld)pos.z-=fElapsedTime*5;
		if(GetKey(olc::Key::PGDN).bHeld)pos.z+=fElapsedTime*5;
		if(GetKey(olc::Key::HOME).bHeld)rot-=fElapsedTime*5;
		if(GetKey(olc::Key::END).bHeld)rot+=fElapsedTime*5;
		m2.rotateX(util::degToRad(rot));
		
		matView = m2 * m1;
	}

	// Clear background
	ClearBuffer(olc::BLANK, true);
		
		
	const std::vector<std::reference_wrapper<utils::hw3d::mesh>>meshes{
		meshSpr,meshFloorSpr
	};

	const auto UpdateMeshLighting=[this](hw3d::mesh&mesh){
		for (size_t i = 0; i < mesh.pos.size(); i += 3)
		{
			const auto& p0 = mesh.pos[i + 0];
			const auto& p1 = mesh.pos[i + 1];
			const auto& p2 = mesh.pos[i + 2];

			// Hand calculate surface normal (i know i know the norms are already there...)
			olc::vf3d vCross = olc::vf3d(p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]).cross(olc::vf3d(p2[0] - p0[0], p2[1] - p0[1], p2[2] - p0[2])).norm();

			// Additive colouring
			mesh.col[i + 0] = olc::BLACK;
			mesh.col[i + 1] = olc::BLACK;
			mesh.col[i + 2] = olc::BLACK;

			olc::Pixel c[] = { olc::RED, olc::GREEN, olc::BLUE };
			for (const Light&light:lights)
			{
				olc::vf3d vLight = -light.pos.norm();
				float illum = std::max(-vCross.dot(vLight), 0.0f) * 0.8f + 0.2f;
				mesh.col[i + 0] += olc::PixelF(illum, illum, illum, 1.0f) * light.col;
				mesh.col[i + 1] += olc::PixelF(illum, illum, illum, 1.0f) * light.col;
				mesh.col[i + 2] += olc::PixelF(illum, illum, illum, 1.0f) * light.col;
			}
		}
	};
	for(auto&[str,mesh]:ShepGame::meshes){
		UpdateMeshLighting(mesh);
	}
	for(utils::hw3d::mesh&meshRef:meshes){
		UpdateMeshLighting(meshRef);
	}
	

	const auto Render3DModel=[this](const GameObject&obj){
		const hw3d::mesh&mesh{obj.GetMesh()};
		matWorld.translate(vf3d{-0.5f,0,-0.5f}+pos+obj.GetPos());
		mf4d scale;
		scale.scale(obj.GetScale());
		HW3D_DrawObject((matView * matWorld * scale).m, obj.GetSprite().Decal(), mesh.layout, mesh.pos, obj.GetUVs(), mesh.col);
	};
	
	for(GameObject&obj:objects){
		Render3DModel(obj);
		obj.Update(fElapsedTime);
	}

	if(GameSettings::LIGHTS_VISIBLE){
		for(Light&light:lights){
			matWorld.translate(vf3d{-0.5f,0,-0.5f}+light.pos+pos);
			std::vector<Pixel>lightCol{};
			std::ranges::copy(std::views::transform(meshSpr.col,[&light](const Pixel&pixel){return light.col;}),std::back_inserter(lightCol));
			HW3D_DrawObject((matView*matWorld).m,nullptr,meshSpr.layout,meshSpr.pos,meshSpr.uv,lightCol);
		}
	}

	return true;
}

ShepGame&ShepGame::Game(){
	if(game==nullptr){
		std::cout<<"WARNING! Trying to reference Game pointer before game has been initialized! THIS IS NOT ALLOWED!"<<std::endl;;
		throw;
	}
	return *game;
}

int main()
{
	ShepGame shep;
	if (shep.Construct(1280, 720, 1, 1, false, false))
		shep.Start();
	return 0;
}