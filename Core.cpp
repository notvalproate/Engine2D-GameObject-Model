#include "Core.hpp"
#include <cmath>

Component::Component(GameObject& gameObject) : gameObject(gameObject), transform(gameObject.transform) { };

Vector2D::Vector2D(uint32_t x, uint32_t y) : x(x), y(y) {}

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
    x += translation.x;
    y += translation.y;
}

void Transform::Transform::Rotate(const double angle) {
    rotation += angle;
}

void Transform::RotateAround(const Vector2D& point, const double angle) {

}

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