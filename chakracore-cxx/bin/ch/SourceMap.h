#pragma once
#include "AutoString.h"

struct FileNode
{
    std::shared_ptr<std::string> data;
    std::string path;
    FileNode * next;
    FileNode(std::string path_, std::shared_ptr<std::string> data_):
        data(std::move(data_)), path(std::move(path_)), next(nullptr) {
    }
};

class SourceMap
{
    static FileNode *root;

public:
    static void Add(std::string &&path, std::string &&data)
    {
        // SourceMap lifetime == process lifetime
        FileNode *node = new FileNode(std::move(path), std::make_shared<std::string>(std::move(data)));
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
