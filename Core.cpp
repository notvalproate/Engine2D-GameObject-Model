#include "Core.hpp"

Component::Component(GameObject& gameObject) : gameObject(gameObject), transform(gameObject.transform) { };

Transform::Transform(GameObject& gameObject) : gameObject(gameObject) { };

GameObject::GameObject() : name({}), tag({}), transform(*this) { 
    // m_GlobalGameObjectsList.push_back(*this);
};

GameObject::GameObject(const std::string_view objectName) : name(objectName), tag({}), transform(*this) { 
    // m_GlobalGameObjectsList.push_back(*this);
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
// std::vector<std::reference_wrapper<GameObject>> GameObject::m_GlobalGameObjectsList{};

// GameObject& GameObject::FindObjectByName(const std::string_view searchName) {
//     for(auto& object : m_GlobalGameObjectsList) {
//         if(object.get().name == searchName) {
//             return object.get();
//         }
//     }
// }

// std::vector<std::reference_wrapper<GameObject>> GameObject::FindObjectsByTag(const std::string_view searchTag) {
//     std::vector<std::reference_wrapper<GameObject>> objects{};

//     for(auto& object : m_GlobalGameObjectsList) {
//         if(object.get().name == searchTag) {
//             objects.push_back(object);
//         }
//     }

//     return objects;
// }