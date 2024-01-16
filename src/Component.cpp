#include "Core.hpp"

Component::Component(GameObject* gameObject) : gameObject(gameObject), transform(&gameObject->transform), tag(&gameObject->tag) { }

Component::~Component() { }

bool Component::CompareTag(const std::string_view otherTag) const {
    return *tag == otherTag;
}

void Component::AttachGameObject(GameObject* newGameObject) {
    gameObject = newGameObject;
    transform = &newGameObject->transform;
    tag = &newGameObject->tag;
}

std::unique_ptr<Component> Component::Clone() const {
    return std::make_unique<Component>(*this);
};