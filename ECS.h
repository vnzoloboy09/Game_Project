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
class Manager;

using ComponentID = unsigned int;
using Group = unsigned int;

inline ComponentID getNewComponentTypeID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr unsigned int maxComponents = 32;
constexpr unsigned int maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
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
private:
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;

public:
    Entity(Manager& mManager) : manager(mManager) {}

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

    bool hasGroup(Group mGroup) {
        return groupBitSet[mGroup];
    }

    void addGroup(Group mGroup);

    void delGroup(Group mGroup) {
        groupBitSet[mGroup] = false;
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
};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
public:
	void update() {
		for (auto& e : entities) e->update();
	}

	void render() {
		for (auto& e : entities) e->render();
	}

	void refresh() {
		for (auto i = 0; i < maxGroups; i++) {
			auto& v = groupedEntities[i];
			v.erase(std::remove_if(v.begin(), v.end(), [i](Entity* mEntity) {
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}), v.end());
		}		

		entities.erase(std::remove_if(entities.begin(), entities.end(),
			[](const std::unique_ptr<Entity>& mEntity) {
				return !mEntity->isActive();
			}),
			entities.end());
	}

	void addToGroup(Entity* mEntity, Group mGroup) {
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup) {
		return groupedEntities[mGroup]; 
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};