#include "Core.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

Component::Component(GameObject& gameObject) : gameObject(gameObject), transform(gameObject.transform), tag(gameObject.tag) { };

bool Component::CompareTag(const std::string_view otherTag) const {
    return tag == otherTag;
}

Behaviour::Behaviour(GameObject& gameObject) : Component(gameObject), name(gameObject.name) { }

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

Transform::Transform(GameObject& gameObject) : gameObject(gameObject), tag(gameObject.tag), name(gameObject.name) { };

Transform::~Transform() {
    DetachFromParent();
    DetachChildren();
}

void Transform::Translate(const Vector2D& translation) {
    position.x += translation.x;
    position.y += translation.y;

    for(auto& child : m_Children) {
        child->Translate(translation);
    }
}

void Transform::Rotate(const double angle) {
    rotation += angle;

    for(auto& child : m_Children) {
        child->Rotate(angle);
    }
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

    for(auto& child : m_Children) {
        child->RotateAround(point, angle);
    }
}

void Transform::DetachChildren() {
    for(auto& child : m_Children) {
        child->m_Parent = nullptr;
    }

    m_Children.clear();
    childCount = 0;
}

void Transform::DetachFromParent() {
    if(!m_Parent) {
        return;
    }

    auto it = std::find(std::begin(m_Parent->m_Children), std::end(m_Parent->m_Children), this);
    m_Parent->m_Children.erase(it);

    m_Parent = nullptr;
}

Transform* Transform::Find(const std::string_view name) const {
    for(const auto& child : m_Children) {
        if(child->gameObject.name == name) {
            return child;
        }
    }

    return nullptr;
}

Transform* Transform::GetChild(const std::size_t index) const {
    if(index >= childCount) {
        return nullptr;
    }

    return m_Children[index];
}

size_t Transform::GetSiblingIndex() const {
    if(!m_Parent) {
        return 0;
    }

    auto it = std::find(std::begin(m_Parent->m_Children), std::end(m_Parent->m_Children), this);

    return std::distance(std::begin(m_Parent->m_Children), it);
}

void Transform::SetAsFirstSibling() const {
    if(!m_Parent) {
        return;
    }

    size_t index = GetSiblingIndex();

    std::swap(m_Parent->m_Children[index], m_Parent->m_Children.front());
}

void Transform::SetAsLastSibling() const {
    if(!m_Parent) {
        return;
    }

    size_t index = GetSiblingIndex();

    std::swap(m_Parent->m_Children[index], m_Parent->m_Children.back());
}

void Transform::SetSiblingIndex(const std::size_t index) const {
    if(!m_Parent) {
        return;
    }

    std::size_t ind = std::clamp(static_cast<std::size_t>(index), static_cast<std::size_t>(0), m_Parent->childCount - 1);
    std::size_t currentIndex = GetSiblingIndex();

    std::swap(m_Parent->m_Children[currentIndex], m_Parent->m_Children[ind]);
}

bool Transform::IsChildOf(Transform& parentTransform) const {
    if(m_Parent == &parentTransform) {
        return true;
    }

    return false;
}

void Transform::SetParent(Transform& parentTransform) {
    m_Parent = &parentTransform;
    m_Parent->m_Children.push_back(this);
    m_Parent->childCount++;
}

GameObject::GameObject() : name({}), tag({}), transform(*this) { 
    m_GlobalGameObjectsList.push_back(this);
}

GameObject::GameObject(const std::string_view goName) : name(goName), tag({}), transform(*this) { 
    m_GlobalGameObjectsList.push_back(this);
}

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
    for (const auto& behaviour : m_Behaviours) {
        if(behaviour->enabled) {
            behaviour->Render();
        }
    }
    for(const auto& component : m_Components) {
        component->Render();
    }
}

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