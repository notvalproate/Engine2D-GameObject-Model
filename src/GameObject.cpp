#include "Core.hpp"

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