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
   int start_frame;

public:  
   SpriteComponent() = default;  
   SpriteComponent(const char* path) {  
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
       texture = Graphics::loadTexture(path);  
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

   void update() override {  
       if (isAnimated) {
           srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed + start_frame) % frames);
       }

       destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
       destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
   }  

   void render() override {  
       Graphics::render(texture, srcRect, destRect, transform->angle, SDL_FLIP_NONE);  
   }  
};