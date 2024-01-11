#include "Core.hpp"

class PositionDebug : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Awake() override {
        std::cout << "Awoken" << std::endl;
    }

    void Update() override {
        std::cout << name << " " << transform.position.x << std::endl;
    }
};