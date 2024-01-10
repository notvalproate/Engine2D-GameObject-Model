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
    virtual void Awake() {};
    virtual void Start() {};
    virtual void Update() {};
    virtual void Render() const {};

    bool CompareTag(const std::string_view otherTag) const;

    // IMPLMENTATION REQUIRED FOR COMPONENTS


protected:
    Component(GameObject& gameObject);
    virtual ~Component() {};

    GameObject& gameObject;
    Transform& transform;
    std::string& tag;

    friend class GameObject;
};

class Behaviour : public Component {
public:
    using Component::Component;

    bool enabled{true}, isActiveAndEnabled{true};
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
};


class GameObject final {
public:
    GameObject();
    GameObject(const std::string_view goName);
    ~GameObject() = default;

    void Update() const;
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

