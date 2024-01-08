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
    /**
     * @brief The default constructor for GameObject. Initializes a GameObject without any tag or name.
     */
    GameObject();

    /**
     * @brief The default constructor for GameObject. Initializes a GameObject without any tag or name.
     * @param goName The name to be given to the GameObject.
     */
    GameObject(const std::string_view goName);

    ~GameObject() = default;

    /**
     * @brief Calls the HandleEvents() method on all of its attached Components.
     */
    void HandleEvents() const;

    /**
     * @brief Calls the Update() method on all of its attached Components.
     */
    void Update() const;

    /**
     * @brief Calls the Render() method on all of its attached Components.
     */
    void Render() const;

    /**
     * @brief Finds all the objects in the global list of GameObjects by tag.
     * @param tag The tag of the GameObjects to be searched.
     * @return A vector of pointers to the GameObjects with passed tag. May return an empty list if none exist.
     */
    static std::vector<GameObject*> FindObjectsByTag(const std::string_view searchTag);

    /**
     * @brief Finds the first object in the global list of GameObjects by name.
     * @param name The name of the GameObject to be searched.
     * @return A pointer to the GameObject with passed name. Returns nullptr if GameObject not found.
     */
    static GameObject* FindObjectByName(const std::string_view searchName);

    /**
     * @brief Delete operator that is overloaded to also delete the GameObject from the global list of GameObjects.
     * @param ptr The GameObject pointer to be deleted.
     */
    void operator delete(void *ptr);

    /**
     * @brief Attaches one component to the GameObject.
     * @tparam Last The type of the Component to be attached.
     */
    template<typename Last>
    void AddComponent() {
        AssertParametersAreDerived<Last>();

        m_Components.push_back(std::unique_ptr<Last>(new Last(*this)));
        m_Components.back()->Start();
    }

    /**
     * @brief Attaches a variable number of components to the GameObject.
     * @tparam First The first The type of the Component to be attached.
     * @tparam Second The second The type of the Component to be attached.
     * @tparam Args... The remaining The types of the Components to be attached.
     */
    template<typename First, typename Second, typename... Args>
    void AddComponent() {
        AssertParametersAreDerived<First, Second, Args...>();

        m_Components.push_back(std::unique_ptr<First>(new First(*this)));
        m_Components.back()->Start();
            
        AddComponent<Second, Args...>();
    }

    /**
     * @brief Returns a pointer to the first component of type T attached to this GameObject.
     * @tparam T The type of component to get.
     * @return A pointer to the first component of type T. Returns nullptr if Component does not exist.
     */
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
    static std::vector<GameObject*> m_GlobalGameObjectsList;
    
    template<typename... Args>
    static void AssertParametersAreDerived() {
        static_assert(
            std::conjunction<std::is_base_of<Component, Args>...>::value, 
            "Custom Component provided not derived from Component Class"
        );
    }
};
