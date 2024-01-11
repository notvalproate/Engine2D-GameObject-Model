#pragma once
#include "Core.hpp"

class LocalPositionDebug : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Update() {
        std::cout << transform.position.x - gameObject.transform.parent->position.x << std::endl;
    }
};