#pragma once
#include "AutoString.h"

struct FileNode
{
    std::shared_ptr<std::string> data;
    std::string path;
    FileNode * next;
    FileNode(const AutoString &path_, const AutoString &data_) :
        data(std::make_shared<std::string>(data_.GetString(), data_.GetLength())),
        path({path_.GetString(), path_.GetLength()}),
        next(nullptr)
    {
    }
};

class SourceMap
{
    static FileNode *root;

public:
    static void Add(const AutoString &path, const AutoString &data)
    {
        // SourceMap lifetime == process lifetime
        FileNode *node = new FileNode(path, data);
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
