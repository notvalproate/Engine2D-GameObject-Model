#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <type_traits>
#include <memory>
#include <typeindex>
#include <optional>

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

        auto [it, success] = m_ComponentMap.try_emplace(typeid(Last), std::unique_ptr<Last>(new Last(*this)));

        if(success) {
            m_ComponentMap[typeid(Last)]->Start();
        }
    }

    template<typename First, typename Second, typename... Args>
    void AddComponent() {
        AssertParametersAreDerived<First, Second, Args...>();

        auto [it, success] = m_ComponentMap.try_emplace(typeid(First), std::unique_ptr<First>(new First(*this)));

        if(success) {
            m_ComponentMap[typeid(First)]->Start();
        }
            
        AddComponent<Second, Args...>();
    }

    template<typename T>
    T* GetComponent() const {
        AssertParametersAreDerived<T>();

        auto it = m_ComponentMap.find(typeid(T));

        return (it != m_ComponentMap.end()) ? dynamic_cast<T*>(it->second.get()) : nullptr;
    }

    std::string name;
    std::string tag;
    Transform transform;

private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_ComponentMap{};
    static std::vector<const GameObject*> m_GlobalGameObjectsList;
    
    template<typename... Args>
    static void AssertParametersAreDerived() {
        static_assert(
            std::conjunction<std::is_base_of<Component, Args>...>::value, 
            "Custom Component provided not derived from Component Class"
        );
    }
};
