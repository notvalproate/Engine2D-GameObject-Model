#include "Core.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

Component::Component(GameObject& gameObject) : gameObject(gameObject), transform(gameObject.transform) { };

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

double Vector2D::GetMagnitude() const {
    return std::sqrt(x * x + y * y);
}

void Vector2D::Normalize() {
    if (x == 0 && y == 0) {
		return;
	}

	double k = std::sqrt(x * x + y * y);
	x /= k;
	y /= k;
}

void Vector2D::Scale(const int factor) {
    x *= factor;
	y *= factor;
}

const Vector2D Vector2D::up(0.0, -1.0);
const Vector2D Vector2D::down(0.0, 1.0);
const Vector2D Vector2D::left(-1.0, 0.0);
const Vector2D Vector2D::right(1.0, 0.0);
const Vector2D Vector2D::one(1.0, 1.0);
const Vector2D Vector2D::zero(0.0, 0.0);

Transform::Transform(GameObject& gameObject) : gameObject(gameObject) { };

void Transform::Translate(const Vector2D& translation) {
    position.x += translation.x;
    position.y += translation.y;
}

void Transform::Rotate(const double angle) {
    rotation += angle;
}

void Transform::RotateAround(const Vector2D& point, const double angle) {
    const double angleRadians = angle * 180 / M_PI;

    const double relX = position.x - point.x, relY = position.y - point.y;
    const double radius = std::sqrt(relX * relX + relY * relY);

    const double currentAngleFromPoint = atan2(relY, relX);

    const double totalAngle = angleRadians + currentAngleFromPoint;

    position.x = (cos(totalAngle) * radius) + point.x;
    position.y = (sin(totalAngle) * radius) + point.y;
    rotation += angle;
}

GameObject::GameObject() : name({}), tag({}), transform(*this) { 
    m_GlobalGameObjectsList.push_back(this);
};

GameObject::GameObject(const std::string_view goName) : name(goName), tag({}), transform(*this) { 
    m_GlobalGameObjectsList.push_back(this);
};

void GameObject::Update() const {
    for (const auto& behaviour : m_Behaviours) {
        if(behaviour->enabled) {
            behaviour->Update();
        }
    }
    for(const auto& component : m_Components) {
        component->Update();
    }
};

void GameObject::Render() const {
    for (const auto& behaviour : m_Behaviours) {
        if(behaviour->enabled) {
            behaviour->Render();
        }
    }
    for(const auto& component : m_Components) {
        component->Render();
    }
};

void GameObject::HandleEvents() const {
    for (const auto& behaviour : m_Behaviours) {
        if(behaviour->enabled) {
            behaviour->HandleEvents();
        }
    }
    for(const auto& component : m_Components) {
        component->HandleEvents();
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