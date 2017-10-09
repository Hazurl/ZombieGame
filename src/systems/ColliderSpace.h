#pragma once

#include "Collider/SAT_Collider.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <limits>
#include <unordered_set>

class ColliderAABBComponent;
class TransformComponent;
class RigidBodyComponent;

struct Raycast {
    static constexpr float max_distance = 10000; // std::numeric_limits<float>::max() 
    Raycast();
    Raycast(sf::Vector2f const& start, sf::Vector2f const& direction, float distance = max_distance);

    bool is_infinite() const;
    sf::Vector2f at(float d) const;
    float at_x(float d) const;
    float at_y(float d) const;
    
    sf::Vector2f start, direction;
    float distance;
};

struct RaycastInfo {
    RaycastInfo();
    RaycastInfo(ColliderAABBComponent* collider, float distance);

    operator bool() const;

    ColliderAABBComponent* collider;
    float distance;
};

struct ColliderOwner {
    TransformComponent* tf;
    RigidBodyComponent* rb;
    SAT_Collider* collider;

    bool operator < (ColliderOwner const& o) const;
    bool operator == (ColliderOwner const& o) const;
    bool operator != (ColliderOwner const& o) const;
};

class ColliderSpace {
public:

    void insert(ColliderOwner const& colider);
    void remove(ColliderOwner const& collider);
    void remove(TransformComponent* const& tf, RigidBodyComponent* const& rb = nullptr);

    void update(sf::Time const& time);

    RaycastInfo raycast(Raycast const& r, std::unordered_set<TransformComponent*> const& ignored = {});
    
private:

    void checkCollision(ColliderOwner& c0, ColliderOwner& c1);
    void checkRaycastCollision(Raycast const& r, ColliderOwner& c, RaycastInfo& info);
    void resolveCollision(ColliderOwner& c0, ColliderOwner& c1, sf::Vector2f const& direction);
    
    std::vector<ColliderOwner> m_colliders;
};