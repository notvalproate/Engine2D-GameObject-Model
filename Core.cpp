#include "Core.hpp"

Component::Component(GameObject& gameObject) : gameObject(gameObject), transform(gameObject.transform) { };

