#include "ResourceManager.hpp"
#include <iostream>
#include <filesystem>

inline bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

template <typename Resource>
bool ResourceManager<Resource>::isResourceLoaded(const std::string& key) const {
    return resources.find(key) != resources.end();
}

// Get the singleton instance
template <typename Resource>
ResourceManager<Resource>& ResourceManager<Resource>::getInstance() {
    static ResourceManager instance; // Lazy initialization
    return instance;
}

// Private constructor
template <typename Resource>
ResourceManager<Resource>::ResourceManager() {}

// // Load a 2D texture resource
template <>
bool ResourceManager<Texture>::load(const std::string& key, const std::string& filepath) {
    if (resources.find(key) != resources.end()) {
        std::cout << "Resource already loaded: " << key << std::endl;
        return false;
    }

    auto resource = std::make_shared<Texture>(TextureType::TEXTURE_2D);
    if (!resource->loadFromFile(filepath)) {
        std::cerr << "Failed to load resource: " << filepath << std::endl;
        return false;
    }

    resources[key] = resource;
    std::cout << "Loaded resource: " << key << std::endl;
    return true;
}

// Overloaded load function for Shader resources
template <>
bool ResourceManager<Shader>::load(const std::string& key, const char* vertexPath, const char* fragmentPath) {
    if (resources.find(key) != resources.end()) {
        std::cout << "Resource already loaded: " << key << std::endl;
        return false;
    }
    std::shared_ptr<Shader> resource = std::make_shared<Shader>();
    if (!resource->loadFromFile(vertexPath, fragmentPath)) {
        std::cerr << "Failed to load Shader resource: " << key << std::endl;
        return false;
    }
    resources[key] = resource;
    std::cout << "Loaded Shader resource: " << key << std::endl;
    return true;
}



// Load a cubemap texture
template <>
bool ResourceManager<Texture>::loadCubemap(const std::string& key, const std::vector<std::string>& faces) {
    if (resources.find(key) != resources.end()) {
        std::cout << "Resource already loaded: " << key << std::endl;
        return false;
    }

    auto resource = std::make_shared<Texture>(TextureType::CUBEMAP);
    if (!resource->loadCubemap(faces)) {
        std::cerr << "Failed to load cubemap resource: " << key << std::endl;
        return false;
    }

    resources[key] = resource;
    std::cout << "Loaded cubemap resource: " << key << std::endl;
    return true;
}

// Load a texture array
template <>
bool ResourceManager<Texture>::loadTextureArray(const std::string& key, const std::vector<std::string>& layers) {
    if (resources.find(key) != resources.end()) {
        std::cout << "Resource already loaded: " << key << std::endl;
        return false;
    }

    auto resource = std::make_shared<Texture>(TextureType::TEXTURE_ARRAY);
    if (!resource->loadTextureArray(layers)) {
        std::cerr << "Failed to load texture array resource: " << key << std::endl;
        return false;
    }

    resources[key] = resource;
    std::cout << "Loaded texture array resource: " << key << std::endl;
    return true;
}

template <>
bool ResourceManager<Font>::load(const std::string& key, const std::string& filepath, unsigned int fontSize) {
    if (isResourceLoaded(key)) {
        std::cout << "Resource already loaded: " << key << std::endl;
        return false;
    }

    if (!fileExists(filepath)) {
        std::cerr << "File does not exist: " << filepath << std::endl;
        return false;
    }

    auto resource = std::make_shared<Font>();
    if (!resource->loadFromFile(filepath, fontSize)) {
        std::cerr << "Failed to load texture resource: " << filepath << std::endl;
        return false;
    }

    resources[key] = resource;
    std::cout << "Loaded font: " << key << std::endl;
    return true;
}






// Get a resource by key
template <typename Resource>
std::shared_ptr<Resource> ResourceManager<Resource>::get(const std::string& key) const {
    auto it = resources.find(key);
    if (it != resources.end()) {
        return it->second;
    }
    std::cerr << "Resource not found: " << key << std::endl;
    return nullptr;
}

// Release a specific resource
template <typename Resource>
void ResourceManager<Resource>::release(const std::string& key) {
    auto it = resources.find(key);
    if (it != resources.end()) {
        resources.erase(it);
        std::cout << "Released resource: " << key << std::endl;
    }
}

// Clear all loaded resources
template <typename Resource>
void ResourceManager<Resource>::clear() {
    resources.clear();
    std::cout << "All resources cleared." << std::endl;
}

// Explicit template instantiation for Texture and Shader
template class ResourceManager<Texture>;
template class ResourceManager<Shader>;
template class ResourceManager<Font>;
