#pragma once  

#include "components.h"  

class SpriteComponent : public Component {  
private:  
   TransformComponent* transform;  
   SDL_Texture* texture;  
   SDL_Rect srcRect, destRect;  

public:  
   SpriteComponent() = default;  
   SpriteComponent(const char* path) {  
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
       destRect.x = static_cast<int>(transform->position.x);  
       destRect.y = static_cast<int>(transform->position.y);  
   }  

   void render() override {  
       Graphics::render(texture, srcRect, destRect, transform->angle, SDL_FLIP_NONE);  
   }  
};