#pragma once
#include "AutoString.h"

struct FileNode
{
    AutoString data;
    std::string path;
    FileNode * next;
    FileNode(const AutoString &path_, AutoString &data_):
        data(data_), path({path_.GetString(), path_.GetLength()}), next(nullptr) {
        data_.MakePersistent();
    }
};

class SourceMap
{
    static FileNode *root;

public:
    static void Add(const AutoString &path, AutoString &data)
    {
        // SourceMap lifetime == process lifetime
        FileNode *node = new FileNode(path, data);
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
            if (node->path == path)
            {
                *out = &(node->data);
                return true;
            }
            node = node->next;
        }
        return false;
    }

    static std::optional<AutoString> Find(const std::string_view path)
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
