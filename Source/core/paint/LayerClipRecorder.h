// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LayerClipRecorder_h
#define LayerClipRecorder_h

#include "core/rendering/LayerPaintingInfo.h"
#include "core/rendering/PaintPhase.h"
#include "platform/graphics/paint/ClipDisplayItem.h"
#include "platform/graphics/paint/DisplayItem.h"
#include "wtf/Vector.h"

namespace blink {

class ClipRect;
class GraphicsContext;
class RenderLayerModelObject;

class LayerClipRecorder {
public:

    enum BorderRadiusClippingRule { IncludeSelfForBorderRadius, DoNotIncludeSelfForBorderRadius };

    // Set rounded clip rectangles defined by border radii all the way from the LayerPaintingInfo
    // "root" layer down to the specified layer (or the parent of said layer, in case
    // BorderRadiusClippingRule says to skip self). fragmentOffset is used for multicol, to specify
    // the translation required to get from flow thread coordinates to visual coordinates for a
    // certain column.
    // FIXME: The BorderRadiusClippingRule parameter is really useless now. If we want to skip self,
    // why not just supply the parent layer as the first parameter instead?
    // FIXME: The ClipRect passed is in visual coordinates (not flow thread coordinates), but at the
    // same time we pass a fragmentOffset, so that we can translate from flow thread coordinates to
    // visual coordinates. This may look rather confusing/redundant, but it is needed for rounded
    // border clipping. Would be nice to clean up this.
    explicit LayerClipRecorder(const RenderLayerModelObject*, GraphicsContext*, DisplayItem::Type, const ClipRect&, const LayerPaintingInfo* localPaintingInfo, const LayoutPoint& fragmentOffset, PaintLayerFlags, BorderRadiusClippingRule = IncludeSelfForBorderRadius);

    ~LayerClipRecorder();

private:

    void collectRoundedRectClips(RenderLayer&, const LayerPaintingInfo& localPaintingInfo, GraphicsContext*, const LayoutPoint& fragmentOffset, PaintLayerFlags,
        BorderRadiusClippingRule, Vector<FloatRoundedRect>& roundedRectClips);

    GraphicsContext* m_graphicsContext;
    const RenderLayerModelObject* m_renderer;
};

} // namespace blink

#endif // LayerDisplayList_h
