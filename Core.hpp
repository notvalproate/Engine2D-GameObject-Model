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
    
    virtual void Start() {};
    virtual void Update() {};
    virtual void Render() const {};
    virtual void HandleEvents() {};

protected:
    Component(GameObject& gameObject);

    GameObject& gameObject;
    Transform& transform;

private:

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

    void Translate(const Vector2D& translation);
    void Rotate(const double angle);
    void RotateAround(const Vector2D& point, const double angle);

private:
    Transform(GameObject& gameObject);

    GameObject& gameObject;

    friend class GameObject;
};


class GameObject final {
public:
    GameObject();
    GameObject(const std::string_view goName);
    ~GameObject() = default;

    void HandleEvents() const;
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
            m_Behaviours.back()->Start();
        }
        else {
            m_Components.push_back(std::unique_ptr<T>(new T(*this)));
            m_Components.back()->Start();
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
