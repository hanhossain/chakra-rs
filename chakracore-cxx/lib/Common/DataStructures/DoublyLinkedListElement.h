//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace JsUtil
{
    template<class T, class TAllocator = ArenaAllocator>
    class DoublyLinkedListElement
    {
    private:
        typename WriteBarrierFieldTypeTraits<T*, TAllocator>::Type previous;
        typename WriteBarrierFieldTypeTraits<T*, TAllocator>::Type next;

    public:
        DoublyLinkedListElement();

    public:
        T *Previous() const;
        T *Next() const;

        template<class D> static bool Contains(D *const element,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type const head);
        template<class D> static bool ContainsSubsequence(D *const first, D *const last,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type const head);

        template<class D> static void LinkToBeginning(D *const element,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void LinkToEnd(D *const element,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void LinkBefore(D *const element, D *const nextElement,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void LinkAfter(D *const element, D *const previousElement,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void UnlinkFromBeginning(D *const element,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void UnlinkFromEnd(D *const element,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void UnlinkPartial(D *const element,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void Unlink(D *const element,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void MoveToBeginning(D *const element,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);

        template<class D> static void UnlinkSubsequenceFromEnd(D *const first,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void UnlinkSubsequence(D *const first, D *const last,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);
        template<class D> static void MoveSubsequenceToBeginning(D *const first, D *const last,
            typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const head, typename WriteBarrierFieldTypeTraits<D *,TAllocator>::Type *const tail);

        // JScriptDiag doesn't seem to like the PREVENT_COPY macro
    private:
        DoublyLinkedListElement(const DoublyLinkedListElement &other);
        DoublyLinkedListElement &operator =(const DoublyLinkedListElement &other);
    };
}
