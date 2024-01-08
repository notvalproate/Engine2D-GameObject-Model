#include "Core.hpp"

Component::Component(GameObject& gameObject) : gameObject(gameObject), transform(gameObject.transform) { };

Transform::Transform(GameObject& gameObject) : gameObject(gameObject) { };

GameObject::GameObject() : name({}), tag({}), transform(*this) { 
    m_GlobalGameObjectsList.push_back(this);
};

GameObject::GameObject(const std::string_view objectName) : name(objectName), tag({}), transform(*this) { 
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

// Much rather have this list stored in the scene object as a non-static, to avoid searching through unnecessary objects that arent in the scene
std::vector<GameObject*> GameObject::m_GlobalGameObjectsList{};

std::optional<GameObject*> GameObject::FindObjectByName(const std::string_view searchName) {
    for(auto& object : m_GlobalGameObjectsList) {
        if(object->name == searchName) {
            return object;
        }
    }

    return {};
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