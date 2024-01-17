#include "Core.hpp"

GameObject* Object::Instantiate(GameObject* gameObject) {
    std::string newName = gameObject->name + " #" + std::to_string(gameObject->scene->LatestSceneInstanceID);
    GameObject* newGameObject = gameObject->scene->CreateGameObject(newName);

    for(auto& childTransform : gameObject->transform.m_Children) {
        Instantiate(childTransform->gameObject, &newGameObject->transform);
    }

    for(auto& behaviour : gameObject->m_Behaviours) {
        auto componentClone = behaviour.get()->Clone();
        std::unique_ptr<Behaviour> behaviourCasted(static_cast<Behaviour*>(componentClone.release()));

        behaviourCasted.get()->AttachGameObject(newGameObject);

        newGameObject->m_Behaviours.push_back(std::move(behaviourCasted));
    }

    for(auto& component : gameObject->m_Components) {
        auto componentClone = component.get()->Clone();

        componentClone.get()->AttachGameObject(newGameObject);

        newGameObject->m_Components.push_back(std::move(componentClone));
    }

    return newGameObject;
}

GameObject* Object::Instantiate(GameObject* gameObject, Transform* parentTransform) {
    GameObject* newGameObject = Instantiate(gameObject);
    newGameObject->transform.SetParent(parentTransform);

    return newGameObject;
}

GameObject* Object::Instantiate(GameObject* gameObject, const Vector2D& position, const double rotation) {
    GameObject* newGameObject = Instantiate(gameObject);
    newGameObject->transform.position = position;
    newGameObject->transform.rotation = rotation;

    return newGameObject;
}

void Object::Destroy(GameObject* gameObject) {
    gameObject->scene->m_StagedForDestruction.push_back(gameObject);
}

void Object::DestroyImmediate(GameObject* gameObject) {
    for(auto& childTransform : gameObject->transform.m_Children) {
        DestroyImmediate(childTransform->gameObject);
    }   

    std::vector<std::unique_ptr<GameObject>>& SceneObjects = gameObject->scene->m_SceneGameObjects;

    for (auto it = SceneObjects.begin(); it != SceneObjects.end(); ++it) {
        if (it->get() == gameObject) {
            SceneObjects.erase(it);
            break;
        }
    }
}

Behaviour* Object::Instantiate(Behaviour* behaviour) {
    GameObject* newGameObject = Instantiate(behaviour->gameObject);
}

Behaviour* Object::Instantiate(Behaviour* behaviour, Transform* parentTransform) {
    GameObject* newGameObject = Instantiate(behaviour->gameObject, parentTransform);
}

Behaviour* Object::Instantiate(Behaviour* behaviour, const Vector2D& position, const double rotation) {
    GameObject* newGameObject = Instantiate(behaviour->gameObject, position, rotation);
}

void Object::Destroy(Behaviour* behaviour) {

}

void Object::DestroyImmediate(Behaviour* behaviour) {
    
}