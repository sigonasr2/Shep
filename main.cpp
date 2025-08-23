#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"
#include "util.h"

class ShepGame : public olc::PixelGameEngine
{
public:
	ShepGame()
	{
		// Name your application
		sAppName = "Shep";
	}

public:

	const vi2d SCREEN_SIZE{256,192};

	Renderable space,nico;
	GFX3D::PipeLine renderer;
	GFX3D::vec3d vUp{0,0,1};
	GFX3D::vec3d vEye{0.f,-5.f,2.f};
	GFX3D::mat4x4 camRot{GFX3D::Math::Mat_MakeRotationX(util::degToRad(5))};
	GFX3D::vec3d vLookDir{GFX3D::Math::Mat_MultiplyVector(camRot,{0,0,1})};

	bool OnUserCreate() override
	{
		olc::GFX3D::ConfigureDisplay();
		renderer.SetProjection(90.0f, (float)SCREEN_SIZE.x/(float)SCREEN_SIZE.y, 0.1f, 1000.0f, 0, SCREEN_SIZE.y, 512, SCREEN_SIZE.y);
		space.Load("assets/gfx/space.png");
		nico.Load("assets/gfx/nico-Trapper_512.png");
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		DrawDecal({},space.Decal(),{1.f,1.f},WHITE,olc::GFX3DTransform::TRANSFORM_REQUIRED);
		
		DrawDecal({},nico.Decal(),{1.f,1.f},WHITE,olc::GFX3DTransform::POPUP_SPRITE);


		if(GetKey(LEFT).bHeld)vEye.x-=fElapsedTime*5;
		if(GetKey(RIGHT).bHeld)vEye.x+=fElapsedTime*5;
		if(GetKey(UP).bHeld)vEye.z+=fElapsedTime*5;
		if(GetKey(DOWN).bHeld)vEye.z-=fElapsedTime*5;
		if(GetKey(PGUP).bHeld)vEye.y+=fElapsedTime*5;
		if(GetKey(PGDN).bHeld)vEye.y-=fElapsedTime*5;
		return true;
	}
	virtual void Apply3DTransform(std::vector<DecalInstance>&decals)override{
		std::vector<DecalInstance>oldDecals;
		//std::vector<DecalInstance>foregroundDecals;
		oldDecals.reserve(decals.size());
		std::copy(decals.begin(),decals.end(),std::back_inserter(oldDecals));
		decals.clear();
		GFX3D::vec3d vLookTarget = GFX3D::Math::Vec_Add(vEye, vLookDir);
		renderer.SetCamera(vEye,vLookTarget,vUp);

		GFX3D::mat4x4 matRotateX=GFX3D::Math::Mat_MakeRotationX(0.f);
		GFX3D::mat4x4 matRotateZ=GFX3D::Math::Mat_MakeRotationZ(0.f);

		GFX3D::mat4x4 matWorld=GFX3D::Math::Mat_MultiplyMatrix(matRotateX,matRotateZ);
	
		renderer.SetTransform(matWorld);

		float zIncrementer{0.f};

		for(DecalInstance&decal:oldDecals){
			SetDecalMode(decal.mode);
			if(decal.points==0)continue;
			if(decal.points==3){
				GFX3D::triangle tri{{{decal.pos[0].x,decal.pos[0].y,decal.z[0],1.f},{decal.pos[1].x,decal.pos[1].y,decal.z[1],1.f},{decal.pos[2].x,decal.pos[2].y,decal.z[2],1.f}},{{decal.uv[0].x,decal.uv[0].y,0.f},{decal.uv[1].x,decal.uv[1].y,0.f},{decal.uv[2].x,decal.uv[2].y,0.f}},{decal.tint[0],decal.tint[1],decal.tint[2]}};
				tri.p[0].z+=zIncrementer;
				tri.p[1].z+=zIncrementer;
				tri.p[2].z+=zIncrementer;
				renderer.Render({tri},decal.decal,GFX3D::RENDER_TEXTURED|GFX3D::RENDER_DEPTH);
				if(decal.z[0]>0.1f||decal.z[1]>0.1f||decal.z[2]>0.1f){
					tri.col[0]=tri.col[1]=tri.col[2]={0,0,0,uint8_t(util::lerp(0,160,(1/std::pow(decal.z[0]/10.f+1,4))))};
					tri.p[0].z=tri.p[1].z=tri.p[2].z=0.1f+zIncrementer;
					renderer.Render({tri},decal.decal,GFX3D::RENDER_TEXTURED|GFX3D::RENDER_DEPTH);
				}
			}else if(decal.points==4){
				GFX3D::triangle tri,tri2;
				if(decal.transform==GFX3DTransform::POPUP_SPRITE){ //Orientation is lower-left clockwise
					tri={{{decal.pos[0].x,decal.pos[0].y,decal.z[0],1.f},{decal.pos[1].x,decal.pos[1].y,decal.decal->sprite->height+decal.z[1],1.f},{decal.pos[2].x,decal.pos[2].y,decal.decal->sprite->height+decal.z[2],1.f}},{{decal.uv[0].x,decal.uv[0].y,0.f},{decal.uv[1].x,decal.uv[1].y,0.f},{decal.uv[2].x,decal.uv[2].y,0.f}},{decal.tint[0],decal.tint[1],decal.tint[2]}};
					tri2={{{decal.pos[0].x,decal.pos[0].y,decal.decal->sprite->height+decal.z[0],1.f},{decal.pos[2].x,decal.pos[2].y,decal.decal->sprite->height+decal.z[2],1.f},{decal.pos[3].x,decal.pos[3].y,decal.z[3],1.f}},{{decal.uv[0].x,decal.uv[0].y,0.f},{decal.uv[2].x,decal.uv[2].y,0.f},{decal.uv[3].x,decal.uv[3].y,0.f}},{decal.tint[0],decal.tint[2],decal.tint[3]}};
				}else{
					tri={{{decal.pos[0].x,decal.pos[0].y,decal.z[0],1.f},{decal.pos[1].x,decal.pos[1].y,decal.z[1],1.f},{decal.pos[2].x,decal.pos[2].y,decal.z[2],1.f}},{{decal.uv[0].x,decal.uv[0].y,0.f},{decal.uv[1].x,decal.uv[1].y,0.f},{decal.uv[2].x,decal.uv[2].y,0.f}},{decal.tint[0],decal.tint[1],decal.tint[2]}};
					tri2={{{decal.pos[0].x,decal.pos[0].y,decal.z[0],1.f},{decal.pos[2].x,decal.pos[2].y,decal.z[2],1.f},{decal.pos[3].x,decal.pos[3].y,decal.z[3],1.f}},{{decal.uv[0].x,decal.uv[0].y,0.f},{decal.uv[2].x,decal.uv[2].y,0.f},{decal.uv[3].x,decal.uv[3].y,0.f}},{decal.tint[0],decal.tint[2],decal.tint[3]}};
				}
				tri.p[0].z+=zIncrementer;
				tri.p[1].z+=zIncrementer;
				tri.p[2].z+=zIncrementer;
				tri2.p[0].z+=zIncrementer;
				tri2.p[1].z+=zIncrementer;
				tri2.p[2].z+=zIncrementer;
				renderer.Render({tri,tri2},decal.decal,GFX3D::RENDER_TEXTURED|GFX3D::RENDER_DEPTH);
				if(decal.z[0]>0.1f||decal.z[1]>0.1f||decal.z[2]>0.1f||decal.z[3]>0.1f){
					tri.col[0]=tri.col[1]=tri.col[2]=tri2.col[0]=tri2.col[1]=tri2.col[2]={0,0,0,uint8_t(util::lerp(0,160,(1/std::pow(decal.z[0]/10.f+1,4))))};
					tri.p[0].z=tri.p[1].z=tri.p[2].z=tri2.p[0].z=tri2.p[1].z=tri2.p[2].z=0.1f+zIncrementer;
					renderer.Render({tri,tri2},decal.decal,GFX3D::RENDER_TEXTURED|GFX3D::RENDER_DEPTH);
				}
			}else{
			
			}
			SetDecalMode(DecalMode::NORMAL);
			zIncrementer+=0.000001f;
		}

		std::sort(decals.begin(),decals.end(),[](const DecalInstance&d1,const DecalInstance&d2){return d1.z[0]>d2.z[0];});
	}
};

int main()
{
	ShepGame demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}