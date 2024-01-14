#include "Core.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

Component::Component(GameObject* gameObject) : gameObject(gameObject), transform(&gameObject->transform), tag(&gameObject->tag) { }

Component::~Component() { }

bool Component::CompareTag(const std::string_view otherTag) const {
    return *tag == otherTag;
}

Behaviour::Behaviour(GameObject* gameObject) : Component(gameObject), enabled(true), isActiveAndEnabled(true), name(&gameObject->name) { }

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

Transform::Transform(GameObject* gameObject) : gameObject(gameObject), parent(nullptr), tag(&gameObject->tag), name(&gameObject->name) { };

Transform::~Transform() { }

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
        child->parent = nullptr;
    }

    m_Children.clear();
    childCount = 0;
}

void Transform::DetachFromParent() {
    if(!parent) {
        return;
    }

    auto it = std::find(std::begin(parent->m_Children), std::end(parent->m_Children), this);
    parent->m_Children.erase(it);

    parent = nullptr;
}

Transform* Transform::Find(const std::string_view searchName) const {
    for(const auto& child : m_Children) {
        if(*child->name == searchName) {
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
    if(!parent) {
        return 0;
    }

    auto it = std::find(std::begin(parent->m_Children), std::end(parent->m_Children), this);

    return std::distance(std::begin(parent->m_Children), it);
}

void Transform::SetAsFirstSibling() const {
    if(!parent) {
        return;
    }

    std::size_t index = GetSiblingIndex();

    std::swap(parent->m_Children[index], parent->m_Children.front());
}

void Transform::SetAsLastSibling() const {
    if(!parent) {
        return;
    }

    size_t index = GetSiblingIndex();

    std::swap(parent->m_Children[index], parent->m_Children.back());
}

void Transform::SetSiblingIndex(const std::size_t index) const {
    if(!parent) {
        return;
    }

    std::size_t ind = std::clamp(static_cast<std::size_t>(index), static_cast<std::size_t>(0), parent->childCount - 1);
    std::size_t currentIndex = GetSiblingIndex();

    std::swap(parent->m_Children[currentIndex], parent->m_Children[ind]);
}

bool Transform::IsChildOf(Transform* parentTransform) const {
    if(parent == parentTransform) {
        return true;
    }

    return false;
}

bool Transform::IsChildOf(Transform& parentTransform) const {
    return IsChildOf(&parentTransform);
}

bool Transform::IsChildOf(GameObject* parentGo) const {
    return IsChildOf(&parentGo->transform);
}

void Transform::SetParent(Transform* parentTransform) {
    DetachFromParent();

    parent = parentTransform;
    parent->m_Children.push_back(this);
    parent->childCount++;
}

void Transform::SetParent(Transform& parentTransform) {
    SetParent(&parentTransform);
}

void Transform::SetParent(GameObject* parentGo) {
    SetParent(&parentGo->transform);
}

GameObject::GameObject(Scene* scene, const uint32_t id) : name({}), tag({}), transform(this), scene(scene), m_SceneInstanceID(id) { }

GameObject::GameObject(const std::string_view goName, Scene* scene, const uint32_t id) : name(goName), tag({}), transform(this), scene(scene), m_SceneInstanceID(id) { }


GameObject* GameObject::Instantiate(GameObject* gameObject) {
    return gameObject;
}

void GameObject::Destroy(GameObject* gameObject) {
    gameObject->scene->m_StagedForDestruction.push_back(gameObject);
}

void GameObject::DestroyImmediate(GameObject* gameObject) {
    for(auto& childTransform : gameObject->transform.m_Children) {
        DestroyImmediate(childTransform->gameObject);
    }   

    std::cout << "Destroying " << gameObject->name << std::endl;

    auto& SceneObjects = gameObject->scene->m_SceneGameObjects;

    for (auto it = SceneObjects.begin(); it != SceneObjects.end(); ++it) {
        if (it->get() == gameObject) {
            SceneObjects.erase(it);
            break;
        }
    }
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
    for(const auto& component : m_Components) {
        component->Render();
    }
}

Scene::Scene() { }

Scene::Scene(const std::string_view name) : name(name) { }

void Scene::Start() {
    for(auto& gameObject : m_SceneGameObjects) {
        gameObject->Start();
    }
}

void Scene::Update() {
    for(auto& gameObject : m_SceneGameObjects) {
        gameObject->Update();
    }

    if(m_StagedForDestruction.size()) {
        for(auto& gO : m_StagedForDestruction) {
            GameObject::DestroyImmediate(gO);
        }
        m_StagedForDestruction.clear();
    }
}

void Scene::Render() const {
    for(auto& gameObject : m_SceneGameObjects) {
        gameObject->Render();
    }
}

GameObject* Scene::CreateGameObject() {
    m_SceneGameObjects.push_back(std::make_unique<GameObject>(this, LatestSceneInstanceID++));
    return m_SceneGameObjects.back().get();
}

GameObject* Scene::CreateGameObject(const std::string_view goName) {
    m_SceneGameObjects.push_back(std::make_unique<GameObject>(goName, this, LatestSceneInstanceID++));
    return m_SceneGameObjects.back().get();
}

GameObject* Scene::FindObjectByName(const std::string_view searchName) {
    for(auto& object : m_SceneGameObjects) {
        if(object.get()->name == searchName) {
            return object.get();
        }
    }

    return nullptr;
}

std::vector<GameObject*> Scene::FindObjectsByTag(const std::string_view searchTag) {
    std::vector<GameObject*> objects{};
    
    for(auto& object : m_SceneGameObjects) {
        if(object.get()->tag == searchTag) {
            objects.push_back(object.get());
        }
    }

    return objects;
}