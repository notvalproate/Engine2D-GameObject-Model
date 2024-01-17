#include "Core.hpp"

#include <ranges>

GameObject::GameObject(Scene* scene, const uint32_t id) : name({}), tag({}), transform(this), scene(scene), m_SceneInstanceID(id) { }

GameObject::GameObject(const std::string_view goName, Scene* scene, const uint32_t id) : name(goName), tag({}), transform(this), scene(scene), m_SceneInstanceID(id) { }

void GameObject::Start() {
    for (auto& behaviour : m_Behaviours) {
        behaviour->Start();
    }
    for(auto& component : m_Components) {
        component->Start();
    }
}

void GameObject::Update() {
    for (auto& behaviour : m_Behaviours) {
        if(behaviour->enabled) {
            behaviour->Update();
        }
    }
    for(auto& component : m_Components) {
        component->Update();
    }
}

void GameObject::Render() const {
    for(const auto& component : m_Components) {
        component->Render();
    }
}

std::size_t GameObject::GetComponentIndex(Component* component, bool& isBehaviour) {
    isBehaviour = true;

    if(dynamic_cast<Behaviour*>(component) == nullptr) {
        isBehaviour = false;
    }

    std::size_t i = 0;

    if(isBehaviour) {
        for(const auto& behaviour : m_Behaviours) {
            if(component == behaviour.get()) {
                return i;
            }
            i++;
        }
    }

    for(const auto& comp : m_Components) {
        if(component == comp.get()) {
            return i;
        }
        i++;
    }

    return i;
}