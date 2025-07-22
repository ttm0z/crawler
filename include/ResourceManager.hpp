#pragma once
#include <unordered_map>

#include <memory>
#include <string>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Font.hpp"

template <typename Resource>
class ResourceManager {
public:
    static ResourceManager& getInstance();

    bool load(const std::string& key, const std::string& filepath);
    bool load(const std::string& key, const char* vertexPath, const char* fragmentPath);
    bool load(const std::string& key, const std::string& filepath, unsigned int fontSize);
    bool loadCubemap(const std::string& key, const std::vector<std::string>& faces);
    bool loadTextureArray(const std::string& key, const std::vector<std::string>& layers);
    
    std::shared_ptr<Resource> get(const std::string& key) const;
    void release(const std::string& key);
    void clear();


    bool isResourceLoaded(const std::string& key) const;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    ResourceManager();
    std::unordered_map<std::string, std::shared_ptr<Resource>> resources;
};