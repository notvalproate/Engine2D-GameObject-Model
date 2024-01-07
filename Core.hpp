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

