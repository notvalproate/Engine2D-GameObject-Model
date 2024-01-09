#include "Core.hpp"

Component::Component(GameObject& gameObject) : gameObject(gameObject), transform(gameObject.transform) { };

Transform::Transform(GameObject& gameObject) : gameObject(gameObject) { };

GameObject::GameObject() : name({}), tag({}), transform(*this) { 
    m_GlobalGameObjectsList.push_back(this);
};

GameObject::GameObject(const std::string_view goName) : name(goName), tag({}), transform(*this) { 
    m_GlobalGameObjectsList.push_back(this);
};

void GameObject::Update() const {
    for(const auto& e : m_Components) {
        e->Update();
    }
};

void GameObject::Render() const {
    for(const auto& e : m_Components) {
        e->Render();
    }
};

void GameObject::HandleEvents() const {
    for(const auto& e : m_Components) {
        e->HandleEvents();
    }
};

std::vector<GameObject*> GameObject::m_GlobalGameObjectsList{};

GameObject* GameObject::FindObjectByName(const std::string_view searchName) {
    for(auto& object : m_GlobalGameObjectsList) {
        if(object->name == searchName) {
            return object;
        }
    }

    return nullptr;
}

std::vector<GameObject*> GameObject::FindObjectsByTag(const std::string_view searchTag) {
    std::vector<GameObject*> objects{};

    for(auto& object : m_GlobalGameObjectsList) {
        if(object->tag == searchTag) {
            objects.push_back(object);
        }
    }

    return objects;
}

void GameObject::operator delete(void *ptr) {
    auto it = std::find(std::begin(m_GlobalGameObjectsList), std::end(m_GlobalGameObjectsList), ptr);
    m_GlobalGameObjectsList.erase(it);

    ::delete ptr;
}