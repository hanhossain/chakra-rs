#pragma once
#include "AutoString.h"

struct FileNode
{
    AutoString data;
    std::string path;
    FileNode * next;
    FileNode(std::string path_, AutoString &data_):
        path(std::move(path_)), data(data_), next(nullptr) {
        data_.MakePersistent();
    }
};

class SourceMap
{
    static FileNode *root;

public:
    static void Add(std::string path, AutoString &data)
    {
        // SourceMap lifetime == process lifetime
        FileNode *node = new FileNode(std::move(path), data);
        if (root != nullptr)
        {
            node->next = root;
        }
        root = node;
    }

    static bool Find(const std::string_view path, AutoString **out)
    {
        FileNode *node = root;
        while (node != nullptr)
        {
            if (static_cast<std::string_view>(node->path) == path)
            {
                *out = &(node->data);
                return true;
            }
            node = node->next;
        }
        return false;
    }
};
