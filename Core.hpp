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
    virtual ~Component() {};

    virtual void Awake() {};
    virtual void Start() {};
    virtual void Update() {};
    virtual void Render() const {};

    bool CompareTag(const std::string_view otherTag) const;

protected:
    GameObject& gameObject;
    Transform& transform;
    std::string& tag;

private:
    Component(GameObject& gameObject);

    friend class GameObject;
    friend class Behaviour;
};


class Behaviour : public Component {
public:
    bool enabled{true}, isActiveAndEnabled{true};
    std::string& name;

protected:
    Behaviour(GameObject& gameObject);

    friend class GameObject;
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
    Vector2D position{};
    double rotation{};
    Vector2D scale{};

    std::size_t childCount{};

    GameObject& gameObject;

    std::string& tag;
    std::string& name;

    void Translate(const Vector2D& translation);
    void Rotate(const double angle);
    void RotateAround(const Vector2D& point, const double angle);

    void DetachChildren();
    void DetachFromParent();
    Transform* Find(const std::string_view name) const;
    Transform* GetChild(const std::size_t index) const;
    size_t GetSiblingIndex() const;
    void SetAsFirstSibling() const;
    void SetAsLastSibling() const;
    void SetSiblingIndex(const std::size_t index) const;
    bool IsChildOf(Transform& parentTransform) const;
    void SetParent(Transform& parentTransform);

private:
    Transform(GameObject& gameObject);
    ~Transform();

    Transform* m_Parent{nullptr};
    std::vector<Transform*> m_Children{};

    friend class GameObject;

    template<typename... Args>
    static void AssertParametersAreDerived() {
        static_assert(
            std::conjunction<std::is_base_of<Component, Args>...>::value, 
            "Custom Component provided not derived from Component Class"
        );
    }
};


class GameObject final {
public:
    GameObject();
    GameObject(const std::string_view goName);
    ~GameObject();

    void Start();
    void Update();
    void Render() const;

    static std::vector<GameObject*> FindObjectsByTag(const std::string_view searchTag);
    static GameObject* FindObjectByName(const std::string_view searchName);

    void operator delete(void *ptr);

    template<typename T>
    void AddComponent() {
        AssertParametersAreDerived<T>();

        if constexpr (std::is_base_of<Behaviour, T>::value) {
            m_Behaviours.push_back(std::unique_ptr<T>(new T(*this)));
            m_Behaviours.back()->Awake();
        }
        else {
            m_Components.push_back(std::unique_ptr<T>(new T(*this)));
            m_Components.back()->Awake();
        }
    }

    template<typename First, typename Second, typename... Args>
    void AddComponent() {
        AddComponent<First>();
        AddComponent<Second, Args...>();
    }

    template<typename T>
    T* GetComponent() const {
        AssertParametersAreDerived<T>();    

        if constexpr (std::is_base_of<Behaviour, T>::value) {
            for(const auto& behaviour : m_Behaviours) {
                auto ptr = dynamic_cast<T*>(behaviour.get());

                if(ptr != nullptr) {
                    return ptr;
                }
            }
        }
        else {
            for(const auto& component : m_Components) {
                auto ptr = dynamic_cast<T*>(component.get());

                if(ptr != nullptr) {
                    return ptr;
                }
            }
        }

        return nullptr;
    }

    template<typename T>
    T* GetComponentInChildren() const {
        AssertParametersAreDerived<T>();

        T* ptr = GetComponent<T>();

        if(ptr == nullptr) {
            for(auto& childTransform : transform.m_Children) {
                ptr = childTransform->gameObject.GetComponentInChildren<T>();

                if(ptr != nullptr) {
                    break;
                }
            }
        }

        return ptr;
    }

    template<typename T>
    T* GetComponentInParent() const {
        AssertParametersAreDerived<T>();

        T* ptr = GetComponent<T>();

        if(ptr == nullptr && transform.m_Parent) {
            ptr = transform.m_Parent->gameObject.GetComponentInParent<T>();
        }

        return ptr;
    }

    template<typename T>
    std::vector<T*> GetComponents() const {
        AssertParametersAreDerived<T>();

        std::vector<T*> components{};

        if constexpr (std::is_base_of<Behaviour, T>::value) {
            for(const auto& behaviour : m_Behaviours) {
                auto ptr = dynamic_cast<T*>(behaviour.get());

                if(ptr != nullptr) {
                    components.push_back(ptr);
                }
            }
        }
        else {
            for(const auto& component : m_Components) {
                auto ptr = dynamic_cast<T*>(component.get());

                if(ptr != nullptr) {
                    components.push_back(ptr);
                }
            }
        }

        return components;
    }

    template<typename T>
    std::vector<T*> GetComponentsInChildren() const {
        AssertParametersAreDerived<T>();

        std::vector<T*> components = GetComponents<T>();

        for(auto& childTransform : transform.m_Children) {
            auto componentsInChildren = childTransform->gameObject.GetComponentsInChildren<T>();

            components.insert(std::end(components), std::begin(componentsInChildren), std::end(componentsInChildren));
        }

        return components;
    }

    template<typename T>
    std::vector<T*> GetComponentsInParent() const {
        AssertParametersAreDerived<T>();

        std::vector<T*> components = GetComponents<T>();

        if(transform.m_Parent) {
            auto componentsInParent = transform.m_Parent->gameObject.GetComponentsInParent<T>();

            components.insert(std::end(components), std::begin(componentsInParent), std::end(componentsInParent));
        }

        return components;
    }

    std::string name;
    std::string tag;
    Transform transform;
    
private:
    std::vector<std::unique_ptr<Component>> m_Components{};
    std::vector<std::unique_ptr<Behaviour>> m_Behaviours{};
    
    static std::vector<GameObject*> m_GlobalGameObjectsList;
    
    template<typename... Args>
    static void AssertParametersAreDerived() {
        static_assert(
            std::conjunction<std::is_base_of<Component, Args>...>::value, 
            "Custom Component provided not derived from Component Class"
        );
    }
};

class Scene {
public:


private:
    Scene() = default;

    std::vector<std::pair<GameObject, Transform>> m_SceneObjects;
};