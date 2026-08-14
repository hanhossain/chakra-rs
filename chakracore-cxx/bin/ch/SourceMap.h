#pragma once
#include "AutoString.h"

struct FileNode
{
    AutoString data;
    AutoString path;
    FileNode * next;
    FileNode(AutoString &path_, AutoString &data_):
        path(path_), data(data_), next(nullptr) {
        path_.MakePersistent();
        data_.MakePersistent();
    }
};

class SourceMap
{
    static FileNode *root;

public:
    static void Add(AutoString &path, AutoString &data)
    {
        // SourceMap lifetime == process lifetime
        FileNode *node = new FileNode(path, data);
        if (root != nullptr)
        {
            node->next = root;
        }
        root = node;
    }

    static bool Find(AutoString &path, AutoString **out) { return Find(path.GetString(), path.GetLength(), out); }

    static bool Find(const char *path, size_t pathLength, AutoString **out)
    {
        FileNode *node = root;
        while (node != nullptr)
        {
            if (strncmp(node->path.GetString(), path, pathLength) == 0)
            {
                *out = &(node->data);
                return true;
            }
            node = node->next;
        }
        return false;
    }

    static bool Find(const std::filesystem::path &path, AutoString **out)
    {
        FileNode *node = root;
        while (node != nullptr)
        {
            if (node->path.GetString() == path.native())
            {
                *out = &(node->data);
                return true;
            }
            node = node->next;
        }
        return false;
    }
};
