#include "Core.hpp"

Component::Component(GameObject& gameObject) : gameObject(gameObject), transform(gameObject.transform) { };

Transform::Transform(GameObject& gameObject) : gameObject(gameObject) { };

