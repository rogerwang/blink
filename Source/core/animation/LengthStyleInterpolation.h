// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LengthStyleInterpolation_h
#define LengthStyleInterpolation_h

#include "core/animation/StyleInterpolation.h"
#include "core/css/CSSPrimitiveValue.h"
#include "platform/Length.h"

namespace blink {

class LengthStyleInterpolation : public StyleInterpolation {
public:
    static PassRefPtrWillBeRawPtr<LengthStyleInterpolation> create(const CSSValue& start, const CSSValue& end, CSSPropertyID id, ValueRange range)
    {
        return adoptRefWillBeNoop(new LengthStyleInterpolation(lengthToInterpolableValue(start), lengthToInterpolableValue(end), id, range));
    }

    static bool canCreateFrom(const CSSValue&);

    virtual void apply(StyleResolverState&) const override;

    virtual void trace(Visitor*) override;

private:
    LengthStyleInterpolation(PassOwnPtrWillBeRawPtr<InterpolableValue> start, PassOwnPtrWillBeRawPtr<InterpolableValue> end, CSSPropertyID id,  ValueRange range)
        : StyleInterpolation(start, end, id)
        , m_range(range)
        { }

    static PassOwnPtrWillBeRawPtr<InterpolableValue> lengthToInterpolableValue(const CSSValue&);
    static PassRefPtrWillBeRawPtr<CSSPrimitiveValue> interpolableValueToLength(const InterpolableValue*, ValueRange);

    ValueRange m_range;

    friend class AnimationLengthStyleInterpolationTest;
    friend class LengthBoxAndBoolStyleInterpolation;
    friend class LengthBoxStyleInterpolation;
    friend class ShadowStyleInterpolation;
    friend class LengthPairStyleInterpolation;
    friend class LengthPoint3DStyleInterpolation;
};

}

#endif
