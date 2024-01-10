#include "Core.hpp"

class PositionDebug : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Update() override {
        std::cout << tag << " " << transform.position.x << std::endl;
    }
};

int main() {
    GameObject Parent("Parent");
    Parent.AddComponent<PositionDebug>();
    Parent.tag = "Parent";

    GameObject Child("Child");
    Child.AddComponent<PositionDebug>();
    Child.tag = "Child";

    Child.transform.SetParent(Parent.transform);

    Parent.transform.DetachChildren();

    Child.transform.Translate(Vector2D(1.0, 1.0));
    Parent.transform.Translate(Vector2D(1.0, 1.0));

    Parent.Update();
    Child.Update();

    return 0;
}