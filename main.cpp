#include "Core.hpp"
#include "PositionDebug.cpp"

int main() {
    GameObject Parent("Parent");
    Parent.AddComponent<PositionDebug>();
    Parent.tag = "Parent";

    GameObject Child("Child");
    Child.AddComponent<PositionDebug>();
    Child.tag = "Child";

    Child.transform.SetParent(Parent.transform);

    std::cout << std::boolalpha;

    std::cout << Child.transform.IsChildOf(Parent.transform) << std::endl;

    Parent.transform.DetachChildren();

    std::cout << Child.transform.IsChildOf(Parent.transform) << std::endl;

    Child.transform.Translate(Vector2D(1.0, 1.0));
    Parent.transform.Translate(Vector2D(1.0, 1.0));

    Parent.Update();
    Child.Update();

    return 0;
}