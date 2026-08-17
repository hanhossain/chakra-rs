#pragma once

struct FileNode
{
    std::shared_ptr<std::string> data;
    std::string path;
    FileNode * next;
    FileNode(std::string &&path, std::string &&data_) :
        data(std::make_shared<std::string>(data_)),
        path(std::move(path)),
        next(nullptr)
    {
    }
};

class SourceMap
{
    static FileNode *root;

public:
    static void Add(const rust::String &path, const rust::String &data)
    {
        // SourceMap lifetime == process lifetime
        FileNode *node = new FileNode(static_cast<std::string>(path), static_cast<std::string>(data));
        if (root != nullptr)
        {
            node->next = root;
        }
        root = node;
    }

    static std::optional<std::shared_ptr<std::string>> Find(const std::string_view path)
    {
        FileNode *node = root;
        while (node != nullptr)
        {
            if (node->path == path)
            {
                return node->data;
            }
            node = node->next;
        }
        return {};
    }
};
