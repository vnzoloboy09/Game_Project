#pragma once

#include "components.h"
#include "stageManager.h"

// decide the enemy's direction according to the target
/* draw line to the target and compare with the enemy's direction 
   to decide whether to turn left or right */

class ChaseComponent : public Component {
private:
	TransformComponent* transform = NULL;
	SDL_FPoint direction;
	SDL_FPoint line_to_target;
	Vector2D* target;
	SDL_FPoint target_center;
	SDL_FPoint self_center;

public:
	ChaseComponent(Vector2D* tar) {
		target = tar;
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		if (transform->stopped) return;

		direction = { transform->corners[0].x - transform->corners[3].x,
			transform->corners[0].y - transform->corners[3].y};
		float nor = sqrt(direction.x * direction.x + direction.y * direction.y);
		direction.x /= nor;
		direction.y /= nor;

		target_center = { target->x + CAR_WIDTH / 2.0f - Game::camera.x, 
			target->y + CAR_HEIGHT / 2.0f - Game::camera.y};

		self_center = { transform->position.x + CAR_WIDTH / 2.0f - Game::camera.x, 
			transform->position.y + CAR_HEIGHT / 2.0f - Game::camera.y };

		line_to_target = { target_center.x - self_center.x, target_center.y - self_center.y };

		float cross = direction.x * line_to_target.y - direction.y * line_to_target.x;
		if (cross > 0) transform->angle += STEER_ANGEL - 0.2f; // turn right
		if (cross <= 0) transform->angle -= STEER_ANGEL - 0.2f; // turn left
		// -0.2 so enemy can turn less than the player

		// 180 * PI to convert angle to radian
		// -90 since the imgs is import 90 degree rather then 0 degree
		float angleRad = (transform->angle - 90) / 180 * M_PI;
		transform->velocity = { cos(angleRad), sin(angleRad) };
	}

    void render() override {
		// use F11 key when play to see the line
		if (StageManager::dev_mode == true) {
			SDL_SetRenderDrawColor(StageManager::renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(StageManager::renderer, static_cast<int>(self_center.x),
				static_cast<int>(self_center.y), static_cast<int>(target_center.x), 
				static_cast<int>(target_center.y));  
			SDL_SetRenderDrawColor(StageManager::renderer, 0, 0, 255, 255);
			SDL_RenderDrawLine(StageManager::renderer, static_cast<int>(self_center.x),
				static_cast<int>(self_center.y), static_cast<int>(self_center.x + 50*direction.x), 
				static_cast<int>(self_center.y + 50*direction.y));  
		}
    }
};