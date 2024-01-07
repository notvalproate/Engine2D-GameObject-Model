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

    friend class GameObject;
};


class Transform final {
public:
    uint32_t x{}, y{};
    double rotation{};
    uint16_t sx{}, sy{};

private:
    Transform(GameObject& gameObject);

    std::shared_ptr<Transform> m_Parent;
    std::vector<std::unique_ptr<Transform>> m_Children;

    GameObject& gameObject;

    friend class GameObject;
};

class GameObject final {
public:
    GameObject();
    GameObject(const std::string_view objectName);

    ~GameObject() = default;

    void Update() const;
    void Render() const;
    void HandleEvents() const;

    static std::optional<const GameObject*> FindObjectByName(const std::string_view searchName);
    static std::vector<const GameObject*> FindObjectsByTag(const std::string_view searchTag);

    template<typename Last>
    void AddComponent() {
        AssertParametersAreDerived<Last>();

        m_Components.push_back(std::unique_ptr<Last>(new Last(*this)));
        m_Components.back()->Start();
    }

    template<typename First, typename Second, typename... Args>
    void AddComponent() {
        AssertParametersAreDerived<First, Second, Args...>();

        m_Components.push_back(std::unique_ptr<First>(new First(*this)));
        m_Components.back()->Start();
            
        AddComponent<Second, Args...>();
    }

    template<typename T>
    T* GetComponent() const {
        AssertParametersAreDerived<T>();    

        for(const auto& component : m_Components) {
            auto ptr = dynamic_cast<T*>(component.get());
            
            if(ptr != nullptr) {
                return ptr;
            }
        }

        return nullptr;
    }

    std::string name;
    std::string tag;
    Transform transform;

private:
    std::vector<std::unique_ptr<Component>> m_Components{};
    static std::vector<const GameObject*> m_GlobalGameObjectsList;
    
    template<typename... Args>
    static void AssertParametersAreDerived() {
        static_assert(
            std::conjunction<std::is_base_of<Component, Args>...>::value, 
            "Custom Component provided not derived from Component Class"
        );
    }
};
