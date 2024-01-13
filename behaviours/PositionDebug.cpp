#pragma once
#include "../Core.hpp"
#include "LocalPositionDebug.cpp"

class PositionDebug : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Awake() override {
        std::cout << "Awoken" << std::endl;
    }

    void Update() override {
        std::cout << *name << " " << transform->position.x << std::endl;

        auto childLocalPosDebugger = gameObject->GetComponentInChildren<LocalPositionDebug>();

        if(childLocalPosDebugger != nullptr) {
            childLocalPosDebugger->Update();
        }
    }
};