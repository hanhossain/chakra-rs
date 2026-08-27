#pragma once
#include <unordered_map>

class SourceMap
{
    struct string_hash
    {
        using hash_type = std::hash<std::string_view>;
        using is_transparent = void;

        std::size_t operator()(std::string_view str) const { return hash_type{}(str); }
    };

    static inline std::unordered_map<std::string, std::shared_ptr<std::string>, string_hash, std::equal_to<>> store = {};

public:
    static void Add(rust::String &&path, rust::String &&data)
    {
        // SourceMap lifetime == process lifetime
        store.insert(std::make_pair(std::move(path), std::make_shared<std::string>(std::move(data))));
    }

    static std::shared_ptr<std::string> Find(const rust::Str path)
    {
        if (const auto search = store.find(static_cast<std::string_view>(path)); search != store.end())
        {
            return search->second;
        }
        return {};
    }
};
