#include "Core.hpp"

class PositionDebug : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Update() override {
        std::cout << tag << transform.position.x << std::endl;
    }
};

int main() {
    GameObject Parent("Parent");
    Parent.AddComponent<PositionDebug>();
    Parent.tag = "Parent";

    GameObject Child("Child");
    Child.AddComponent<PositionDebug>();
    Child.tag = "Child";

    

    return 0;
}