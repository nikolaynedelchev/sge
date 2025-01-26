#pragma once

#include <stdexcept>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include <fmt/format.h>

#include <any>
#include "TypeName.h"

namespace ndn::tools
{

class Runtime final
{
public:
    template<typename T>
    using NamedObjectsMap = std::unordered_map<std::string, T>;

    template<typename T>
    void Add(const T& object, bool overwrite = false);

    template<typename T>
    bool Has() const;

    template<typename T>
    T& Get();

    template<typename T>
    const T& Get() const;

    template<typename T>
    void Remove();

    template<typename T>
    void Add(const std::string& name, const T& object, bool overwrite = false);

    template<typename T>
    bool Has(const std::string& name) const;

    template<typename T>
    T& Get(const std::string& name);
    template<typename T>
    NamedObjectsMap<T>& GetAll();

    template<typename T>
    const T& Get(const std::string& name) const;
    template<typename T>
    const NamedObjectsMap<T>& GetAll() const;

    template<typename T>
    void Remove(const std::string& name);
    template<typename T>
    void RemoveAll();

    void RemoveAll() noexcept;

private:
    template<typename T>
    struct NamedObjectsContainer
    {
        NamedObjectsMap<T> map;
    };


    std::unordered_map<std::type_index, std::any> m_objects;
};

template<typename T>
void Runtime::Add(const T& object, bool overwrite)
{
    std::type_index key(typeid(T));
    std::any any = object;

    if(overwrite)
    {
        m_objects[key] = any;
    }
    else
    {
        auto emplaceResult = m_objects.emplace(key, any);
        auto ok = emplaceResult.second;
        if (!ok)
        {
            throw std::runtime_error(
                fmt::format("runtime already contains {} object", TypeName<T>()));
        }
    }
}

template<typename T>
bool Runtime::Has() const
{
    std::type_index key(typeid(T));
    return m_objects.find(key) != m_objects.end();
}

template<typename T>
T& Runtime::Get()
{
    std::type_index key(typeid(T));
    auto it = m_objects.find(key);
    if (it == m_objects.end())
    {
        throw std::runtime_error(
            fmt::format("runtime does not contain {} object", TypeName<T>()));
    }
    return std::any_cast<T&>(it->second);
}

template<typename T>
const T& Runtime::Get() const
{
    std::type_index key(typeid(T));
    auto it = m_objects.find(key);
    if (it == m_objects.end())
    {
        throw std::runtime_error(
            fmt::format("runtime does not contain {} object", TypeName<T>()));
    }
    return std::any_cast<const T&>(it->second);
}

template<typename T>
void Runtime::Remove()
{
    std::type_index key(typeid(T));
    m_objects.erase(key);
}

template<typename T>
void Runtime::Add(const std::string& name, const T& object, bool overwrite)
{
    if (!Has<NamedObjectsContainer<T>>())
    {
        Add<NamedObjectsContainer<T>>({});
    }
    auto& c = Get<NamedObjectsContainer<T>>();

    if(overwrite)
    {
        c.map[name] = object;
    }
    else
    {
        auto emplaceResult = c.map.emplace(name, object);
        auto ok = emplaceResult.second;
        if (!ok)
        {
            throw std::runtime_error(fmt::format("runtime already contains {} object named \"{}\"",
                                                 TypeName<T>(), name));
        }
    }
}

template<typename T>
bool Runtime::Has(const std::string& name) const
{
    if (!Has<NamedObjectsContainer<T>>())
    {
        return false;
    }
    auto& c = Get<NamedObjectsContainer<T>>();
    return c.map.find(name) != c.map.end();
}

template<typename T>
T& Runtime::Get(const std::string& name)
{
    if (!Has<NamedObjectsContainer<T>>())
    {
        throw std::runtime_error(
            fmt::format("runtime does not contain named {} objects", TypeName<T>()));
    }
    auto& c = Get<NamedObjectsContainer<T>>();
    auto it = c.map.find(name);
    if (it == c.map.end())
    {
        throw std::runtime_error(fmt::format("runtime does not contain {} object named \"{}\"",
                                             TypeName<T>(), name));
    }
    return it->second;
}

template<typename T>
Runtime::NamedObjectsMap<T>& Runtime::GetAll()
{
    if (!Has<NamedObjectsContainer<T>>())
    {
        throw std::runtime_error(
            fmt::format("runtime does not contain named {} objects", TypeName<T>()));
    }
    auto& c = Get<NamedObjectsContainer<T>>();
    return c.map;
}

template<typename T>
const T& Runtime::Get(const std::string& name) const
{
    if (!Has<NamedObjectsContainer<T>>())
    {
        throw std::runtime_error(
            fmt::format("runtime does not contain named {} objects", TypeName<T>()));
    }
    auto& c = Get<NamedObjectsContainer<T>>();
    auto it = c.map.find(name);
    if (it == c.map.end())
    {
        throw std::runtime_error(fmt::format("runtime does not contain {} object named \"{}\"",
                                             TypeName<T>(), name));
    }
    return it->second;
}

template<typename T>
const Runtime::NamedObjectsMap<T>& Runtime::GetAll() const
{
    if (!Has<NamedObjectsContainer<T>>())
    {
        throw std::runtime_error(
            fmt::format("runtime does not contain named {} objects", TypeName<T>()));
    }
    auto& c = Get<NamedObjectsContainer<T>>();
    return c.map;
}

template<typename T>
void Runtime::Remove(const std::string& name)
{
    if (!Has<NamedObjectsContainer<T>>())
    {
        return;
    }
    auto& c = Get<NamedObjectsContainer<T>>();
    c.map.erase(name);
    if (c.map.empty())
    {
        Remove<NamedObjectsContainer<T>>();
    }
}

template<typename T>
void Runtime::RemoveAll()
{
    if (!Has<NamedObjectsContainer<T>>())
    {
        return;
    }
    auto& c = Get<NamedObjectsContainer<T>>();
    c.map.clear();
    Remove<NamedObjectsContainer<T>>();
}

}
