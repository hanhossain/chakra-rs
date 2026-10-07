//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <rust/cxx.h>
#include <memory>

#include "ChakraCommon.h"
#include "chakra/Logger.h"
#include "chakracore-sys/src/messages.rs.h"

template <typename T>
class SortedList 
{
    template <typename U>
    struct DListNode
    {
        U data;
        DListNode<U>* prev;
        DListNode<U>* next;

    public:
        DListNode(U data) :
            data(std::move(data)),
            prev(nullptr),
            next(nullptr)
        { }
    };

public:
    SortedList():
        head(nullptr)
    {
    }

    ~SortedList()
    {
        while (head != nullptr)
        {
            Remove(head);
        }
    }

    // Scan through the sorted list
    // Insert before the first node that satisfies the LessThan function
    // This function maintains the invariant that the list is always sorted
    void Insert(T data)
    {
        DListNode<T>* curr = head;
        DListNode<T>* node = new DListNode<T>(std::move(data));
        DListNode<T>* prev = nullptr; 

        // Now, if we have to insert, we have to insert *after* some node
        while (curr != nullptr)
        {
            if (T::LessThan(data, curr->data))
            {
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        InsertAfter(node, prev);
    }

    T Pop()
    {
        T data = std::move(head->data);
        Remove(head);
        return data;
    }

    template <typename PredicateFn>
    void Remove(PredicateFn fn)
    {
        DListNode<T>* node = head;

        while (node != nullptr)
        {
            if (fn(node->data))
            {
                Remove(node);
                return;
            }
        }
    }

    template <typename PredicateFn>
    void RemoveAll(PredicateFn fn)
    {
        while (head != nullptr)
        {
            fn(head->data);
            Remove(head);
        }
    }

    bool IsEmpty()
    {
        return head == nullptr;
    }

private:
    void Remove(DListNode<T>* node)
    {
        if (node->prev == nullptr)
        {
            head = node->next;
        }
        else
        {
            node->prev->next = node->next;
        }

        if (node->next != nullptr)
        {
            node->next->prev = node->prev;
        }

        delete node;
    }

    void InsertAfter(DListNode<T>* newNode, DListNode<T>* node)
    {
        // If the list is empty, just set head to newNode
        if (head == nullptr)
        {
            assert(node == nullptr);
            head = newNode;
            return;
        }

        // If node is null here, we must be trying to insert before head
        if (node == nullptr)
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
            return;
        }

        assert(node);
        newNode->next = node->next;
        newNode->prev = node;

        if (node->next)
        {
            node->next->prev = newNode;
        }

        node->next = newNode;
    }

    DListNode<T>* head;
};

class MessageQueue
{
    struct ListEntry
    {
        unsigned int time;
        rust::Box<chakra_rs::Message> message;

        ListEntry(unsigned int time, rust::Box<chakra_rs::Message> message):
            time(time),
            message(std::move(message))
        { }

        static bool LessThan(const ListEntry& first, const ListEntry& second)
        {
            return first.time < second.time;
        }
    };

    SortedList<ListEntry> m_queue;

public:
    void InsertSorted(rust::Box<chakra_rs::Message> message)
    {
        auto span = chakra::Span::create("MessageQueue::InsertSorted");
        message->begin_timer();
        unsigned int time = message->get_time();
        m_queue.Insert(ListEntry(time, std::move(message)));
    }

    rust::Box<chakra_rs::Message> PopAndWait()
    {
        auto span = chakra::Span::create("MessageQueue::PopAndWait");
        assert(!m_queue.IsEmpty());

        ListEntry entry = m_queue.Pop();
        rust::Box<chakra_rs::Message> tmp = std::move(entry.message);

        int waitTime = tmp->get_time() - GetTickCount();
        if(waitTime > 0)
        {
            Sleep(waitTime);
        }

        return tmp;
    }

    bool IsEmpty()
    {
        auto span = chakra::Span::create("MessageQueue::IsEmpty");
        return m_queue.IsEmpty();
    }

    void RemoveById(unsigned int id)
    {
        auto span = chakra::Span::create(std::format("MessageQueue::RemoveById[id={}]", id));
        // Search for the message with the correct id, and delete it. Can be updated
        // to a hash to improve speed, if necessary.
        m_queue.Remove([id](const ListEntry& entry) 
        {
            const auto &msg = entry.message;
            if(msg->get_id() == id)
            {
                return true;
            }

            return false;
        });
    }

    void RemoveAll()
    {
        auto span = chakra::Span::create("MessageQueue::RemoveAll");
        m_queue.RemoveAll([](const ListEntry& _) {});
    }

    int32_t ProcessAll(rust::Str fileName)
    {
        auto span = chakra::Span::create(std::format("MessageQueue::ProcessAll[filename={}]", fileName));
        while(!IsEmpty())
        {
            rust::Box<chakra_rs::Message> msg = PopAndWait();

            // Omit checking return value for async function, since it shouldn't affect others.
            msg->call(fileName);
        }
        return S_OK;
    }

    static std::unique_ptr<MessageQueue> New()
    {
        return std::make_unique<MessageQueue>();
    }
};
