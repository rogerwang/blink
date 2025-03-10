/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef StyleFilterData_h
#define StyleFilterData_h

#include "platform/graphics/filters/FilterOperations.h"
#include "platform/heap/Handle.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"

namespace blink {

// FIXME: Oilpan: resorting to RefCountedGarbageCollected<> here so as to support
// DataRef<StyleFilterData> uses. Once/if DataRef<> is able to move away from
// relying on RefPtr<>, switch to GarbageCollected<>.
class StyleFilterData final : public RefCountedWillBeRefCountedGarbageCollected<StyleFilterData> {
public:
    static PassRefPtrWillBeRawPtr<StyleFilterData> create()
    {
        return adoptRefWillBeNoop(new StyleFilterData);
    }

    PassRefPtrWillBeRawPtr<StyleFilterData> copy() const
    {
        return adoptRefWillBeNoop(new StyleFilterData(*this));
    }

    bool operator==(const StyleFilterData&) const;
    bool operator!=(const StyleFilterData& o) const
    {
        return !(*this == o);
    }

    void trace(Visitor* visitor)
    {
        visitor->trace(m_operations);
    }

    FilterOperations m_operations;

private:
    StyleFilterData();
    explicit StyleFilterData(const StyleFilterData&);
};

} // namespace blink


#endif // StyleFilterData_h
