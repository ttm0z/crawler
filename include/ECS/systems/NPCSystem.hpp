#pragma once;
#include "../System.hpp"
#include "../Components.hpp"
#include "../../util/Render.hpp"
#include <unordered_set>
#include <unordered_map>

class NPCSystem : public System {

public:
struct ivec2_hash {
    std::size_t operator()(const glm::ivec2& v) const {
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);
        return h1 ^ (h2 << 1);
    }
};

    size_t cameraEntity;
    float npcCameraHeight = 3.0f;
    
    NPCSystem() {
        setSignature({
            typeid(SpriteComponent).hash_code(),
            typeid(TransformComponent2D).hash_code(),
            typeid(PhysicsComponent2D).hash_code(),
            typeid(NPCComponent).hash_code()
        });
    }
void update(float deltaTime, ECS& ecs) override {
        // Temporarily disabled for 2D conversion
        /*
    auto entities = ecs.getEntitiesBySignature(signature);

    for (auto entity : entities) {
        auto& transform = ecs.getComponent<TransformComponent>(entity);
        auto& npc = ecs.getComponent<NPCComponent>(entity);
        auto& physics = ecs.getComponent<PhysicsComponent>(entity);

        glm::ivec2 currentChunk = worldToChunkCoord(transform.position, 16, 1.0f);
        npc.currentChunk = currentChunk;
        

        if (!npc.hasPath) {
            // Here, you set destination manually or from some AI behavior
            npc.destinationChunk = glm::ivec2(rand() % 16, rand() % 16);

            // Find path
            std::unordered_set<glm::ivec2, ivec2_hash> obstacles; // TODO: fill this in
            auto path = findPathAStar(currentChunk, npc.destinationChunk, obstacles);
            for (const auto& step : path)
                npc.path.push({step.x, 0, step.y});
            npc.hasPath = true;
        }

        if (npc.hasPath && !npc.path.empty()) {
            glm::ivec2 targetChunk = npc.path.front();
            glm::vec3 targetPos = chunkCoordToWorld(targetChunk, 16, 1.0f);

            glm::vec3 direction = glm::normalize(targetPos - transform.position);
            //physics.velocity = direction * npc.movementSpeed;

            if (glm::distance(transform.position, targetPos) < 0.5f) {
                npc.path.pop();
                if (npc.path.empty()) {
                    npc.hasPath = false;
                    physics.velocity = glm::vec3(0.0f);
                }
            }
        }

        //Update NPC direction based on movement
        if (glm::length(physics.velocity) > 0.01f) {
            npc.direction = glm::normalize(physics.velocity);
            npc.right = glm::normalize(glm::cross(npc.direction, glm::vec3(0.0f, 1.0f, 0.0f)));
        }
    }
        */
    }

std::vector<glm::ivec2> findPathAStar(const glm::ivec2& start, const glm::ivec2& goal, const std::unordered_set<glm::ivec2, ivec2_hash>& obstacles) {
auto cmp = [](const std::pair<float, glm::ivec2>& a, const std::pair<float, glm::ivec2>& b) {
    return a.first > b.first; // Priority based only on float
};

std::priority_queue<
    std::pair<float, glm::ivec2>,
    std::vector<std::pair<float, glm::ivec2>>,
    decltype(cmp)
> frontier(cmp);


    std::unordered_map<glm::ivec2, glm::ivec2, ivec2_hash> cameFrom;
    std::unordered_map<glm::ivec2, float, ivec2_hash> costSoFar;

    frontier.emplace(0.0f, start);
    cameFrom[start] = start;
    costSoFar[start] = 0.0f;

    auto heuristic = [](const glm::ivec2& a, const glm::ivec2& b) {
        return glm::length(glm::vec2(a - b));
    };

    const glm::ivec2 directions[4] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    while (!frontier.empty()) {
        auto current = frontier.top().second;
        frontier.pop();

        if (current == goal)
            break;

        for (const auto& dir : directions) {
            glm::ivec2 next = current + dir;

            if (next.x < 0 || next.x >= 16 || next.y < 0 || next.y >= 16)
                continue;
            if (obstacles.count(next)) continue;

            float newCost = costSoFar[current] + 1.0f;
            if (!costSoFar.count(next) || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                float priority = newCost + heuristic(next, goal);
                frontier.emplace(priority, next);
                cameFrom[next] = current;
            }
        }
    }

    std::vector<glm::ivec2> path;
    glm::ivec2 current = goal;
    while (current != start) {
        path.push_back(current);
        current = cameFrom[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

};