#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include <typeinfo>
#include <algorithm>
#include <functional>

class Transform;

class GameObject;

class Component {
public:
    Component(GameObject* gameObject);

    virtual ~Component();

    virtual void Awake() {};
    virtual void Start() {};
    virtual void Update() {};
    virtual void Render() const {};

    bool CompareTag(const std::string_view otherTag) const;

    GameObject* gameObject;
    Transform* transform;
    std::string* tag;
};


class Behaviour : public Component {
public:
    Behaviour(GameObject* gameObject);

    bool enabled;
    bool isActiveAndEnabled;
    std::string* name;
};


class Vector2D {
public:
    Vector2D() = default;
    Vector2D(double x, double y);

    double x{}, y{};

	double GetMagnitude() const;
    void Normalize();
	void Scale(const int factor);

    static const Vector2D up;
    static const Vector2D down;
    static const Vector2D left;
    static const Vector2D right;
    static const Vector2D one;
    static const Vector2D zero;
};

class Transform final {
public:
    Transform(GameObject* gameObject);
    ~Transform();

    Vector2D position{};
    double rotation{};
    Vector2D scale{};

    GameObject* gameObject;

    Transform* parent;
    std::vector<Transform*> m_Children{};
    std::size_t childCount{};

    std::string* tag;
    std::string* name;

    void Translate(const Vector2D& translation);
    void Rotate(const double angle);
    void RotateAround(const Vector2D& point, const double angle);

    void DetachChildren();
    void DetachFromParent();
    Transform* Find(const std::string_view searchName) const;
    Transform* GetChild(const std::size_t index) const;
    size_t GetSiblingIndex() const;
    void SetAsFirstSibling() const;
    void SetAsLastSibling() const;
    void SetSiblingIndex(const std::size_t index) const;
    bool IsChildOf(Transform* parentTransform) const;
    void SetParent(Transform* parentTransform);
};

class Scene;

class GameObject final {
public:
    GameObject(Scene* scene);
    GameObject(const std::string_view goName, Scene* scene);
    ~GameObject();
    
    void Start();
    void Update();
    void Render() const;

    template<typename T>
    void AddComponent() {
        AssertParametersAreDerived<T>();

        if constexpr (std::is_base_of<Behaviour, T>::value) {
            m_Behaviours.push_back(std::make_unique<T>(this));
            m_Behaviours.back().get()->Awake();
        }
        else {
            m_Components.push_back(std::make_unique<T>(this));
            m_Components.back().get()->Awake();
        }
    }

    template<typename First, typename Second, typename... Args>
    void AddComponent() {
        AddComponent<First>();
        AddComponent<Second, Args...>();
    }

    template<typename T>
    T* GetComponent() {
        AssertParametersAreDerived<T>();    

        if constexpr (std::is_base_of<Behaviour, T>::value) {
            for(auto& behaviour : m_Behaviours) {
                auto ptr = dynamic_cast<T*>(behaviour.get());

                if(ptr != nullptr) {
                    return ptr;
                }
            }
        }
        else {
            for(auto& component : m_Components) {
                auto ptr = dynamic_cast<T*>(component.get());

                if(ptr != nullptr) {
                    return ptr;
                }
            }
        }

        return nullptr;
    }

    template<typename T>
    T* GetComponentInChildren() {
        AssertParametersAreDerived<T>();

        T* ptr = GetComponent<T>();

        if(ptr == nullptr) {
            for(auto& childTransform : transform.m_Children) {
                ptr = childTransform->gameObject->GetComponentInChildren<T>();

                if(ptr != nullptr) {
                    break;
                }
            }
        }

        return ptr;
    }

    template<typename T>
    T* GetComponentInParent() {
        AssertParametersAreDerived<T>();

        T* ptr = GetComponent<T>();

        if(ptr == nullptr && transform.parent) {
            ptr = transform.parent->gameObject->GetComponentInParent<T>();
        }

        return ptr;
    }

    template<typename T>
    std::vector<T*> GetComponents() {
        AssertParametersAreDerived<T>();

        std::vector<T*> components{};

        if constexpr (std::is_base_of<Behaviour, T>::value) {
            for(auto& behaviour : m_Behaviours) {
                auto ptr = dynamic_cast<T*>(behaviour.get());

                if(ptr != nullptr) {
                    components.push_back(ptr);
                }
            }
        }
        else {
            for(auto& component : m_Components) {
                auto ptr = dynamic_cast<T*>(component.get());

                if(ptr != nullptr) {
                    components.push_back(ptr);
                }
            }
        }

        return components;
    }

    template<typename T>
    std::vector<T*> GetComponentsInChildren() {
        AssertParametersAreDerived<T>();

        std::vector<T*> components = GetComponents<T>();

        for(auto& childTransform : transform.m_Children) {
            auto componentsInChildren = childTransform->gameObject->GetComponentsInChildren<T>();

            components.insert(std::end(components), std::begin(componentsInChildren), std::end(componentsInChildren));
        }

        return components;
    }

    template<typename T>
    std::vector<T*> GetComponentsInParent() {
        AssertParametersAreDerived<T>();

        std::vector<T*> components = GetComponents<T>();

        if(transform.parent) {
            auto componentsInParent = transform.parent->gameObject->GetComponentsInParent<T>();

            components.insert(std::end(components), std::begin(componentsInParent), std::end(componentsInParent));
        }

        return components;
    }

    // OWNED
    std::string name;
    std::string tag;
    Transform transform;

    // SHARED
    Scene* scene;
private:
    std::vector<std::unique_ptr<Component>> m_Components{};
    std::vector<std::unique_ptr<Behaviour>> m_Behaviours{};
    
    template<typename... Args>
    static void AssertParametersAreDerived() {
        static_assert(
            std::conjunction<std::is_base_of<Component, Args>...>::value, 
            "Custom Component provided not derived from Component Class"
        );
    }

    friend class Scene;
};

class Scene {
public:
    Scene();
    Scene(const std::string_view name);

    virtual void SetupScene() = 0;

    void Start();
    void Update();
    void Render() const;

    GameObject* CreateGameObject();
    GameObject* CreateGameObject(const std::string_view goName);

    std::vector<GameObject*> FindObjectsByTag(const std::string_view searchTag);
    GameObject* FindObjectByName(const std::string_view searchName);

    std::string name{};
private:
    std::vector<std::unique_ptr<GameObject>> m_SceneGameObjects{};

    friend class GameObject;
};