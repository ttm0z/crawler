#include <vector>
#include <queue>
#include <cstddef>
#include <stdexcept>

class EntityManager {
private:
    size_t nextEntityID;
    std::vector<size_t> entityGenerations;
    std::queue<size_t> freeEntities;
public:
    EntityManager() : nextEntityID(0) {}
    size_t createEntity() {
        size_t entity;
        if(!freeEntities.empty()){
            entity = freeEntities.front();
            freeEntities.pop();
        }
        else {
            entity = nextEntityID++;
            entityGenerations.push_back(0);
        }
        return entity;
    }

    void destroyEntity(size_t entity) {
        if (entity >= entityGenerations.size()){
            throw std::out_of_range("Attempted to destroy an invalid Entity");
        }
        entityGenerations[entity]++;
        freeEntities.push(entity);
    }
};