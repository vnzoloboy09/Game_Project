#pragma once  

#include "components.h"
#include "game.h"

class SpriteComponent : public Component {  
private:  
   TransformComponent* transform;  
   SDL_Texture* texture;  
   SDL_Rect srcRect, destRect;

   int frames;
   int speed;
   bool isAnimated = false;
   bool blend_mode = false;
   int start_frame;
   int transparency = 0;

public:  
   SpriteComponent() = default;  
   SpriteComponent(const char* path) {  
       setTex(path);  
   }  

   SpriteComponent(const char* path, int nFrames, int startFrame,int mSpeed) {
       isAnimated = true;
       frames = nFrames;
       speed = mSpeed;
       start_frame = startFrame; // pass in random start frame to make the animation not weird
       setTex(path);
   }

   SpriteComponent(const char* path, int nFrames, int mSpeed) {
       isAnimated = true;
       frames = nFrames;
       speed = mSpeed;
       start_frame = rand() % frames;
       setTex(path);
   }

   ~SpriteComponent() {  
       SDL_DestroyTexture(texture);  
   }  

   void setTex(const char* path) {  
       SDL_DestroyTexture(texture);
       texture = Graphics::loadTexture(path);  
   }  

   void setTransparency(int tr) {
       transparency = tr;
   }

   void init() override {  
       transform = &entity->getComponent<TransformComponent>(); 

       srcRect.x = 0;  
       srcRect.y = 0;  
       srcRect.w = transform->width;
       srcRect.h = transform->height;  

       destRect.x = static_cast<int>(transform->position.x);  
       destRect.y = static_cast<int>(transform->position.y);
       destRect.w = srcRect.w;  
       destRect.h = srcRect.h;  
   }  

   void setBlend(bool bl) {
       blend_mode = bl;
   }

   void update() override {  
       if (isAnimated) {
           srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed + start_frame) % frames);
       }

       destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
       destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
   }  

   void render() override { 
       if (blend_mode) {
           Graphics::makeTransparent(texture, transparency);
       }
       Graphics::render(texture, srcRect, destRect, transform->angle, SDL_FLIP_NONE);  
   }  
};