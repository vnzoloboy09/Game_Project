#pragma once

// learning from https://austinmorlan.com/posts/entity_component_system/

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = unsigned int;

inline ComponentID getComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr unsigned int maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void render() {}

	virtual ~Component() {}
};

class Entity {
public:
	void update() {
		for (auto& c : components) c->update();
	}

	void render() {
		for (auto& c : components) c->render();
	}

	bool isActive() const { return active; }
	void destroy() { active = false; }

	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

private:
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
};

class Manager {
public:
	void update() {
		for (auto& e : entities) e->update();
	}

	void render() {
		for (auto& e : entities) e->render();
	}

	void refresh() {
		entities.erase(std::remove_if(entities.begin(), entities.end(),
			[](const std::unique_ptr<Entity>& mEntity) {
				return !mEntity->isActive();
			}),
			entities.end());
		/*remove_if rearrange entities with all the Active elements move to the front
			and point to the first element not Active
		-> Then .erase remove all the not active to entities.end() */
	}

	Entity& addEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

private:
	std::vector<std::unique_ptr<Entity>> entities;
};