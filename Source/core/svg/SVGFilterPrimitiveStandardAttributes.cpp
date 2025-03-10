/*
 * Copyright (C) 2004, 2005, 2006, 2007 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006 Rob Buis <buis@kde.org>
 * Copyright (C) 2009 Dirk Schulze <krit@webkit.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "core/svg/SVGFilterPrimitiveStandardAttributes.h"

#include "core/SVGNames.h"
#include "core/rendering/svg/RenderSVGResourceContainer.h"
#include "core/rendering/svg/RenderSVGResourceFilterPrimitive.h"
#include "core/svg/SVGLength.h"
#include "platform/graphics/filters/FilterEffect.h"

namespace blink {

SVGFilterPrimitiveStandardAttributes::SVGFilterPrimitiveStandardAttributes(const QualifiedName& tagName, Document& document)
    : SVGElement(tagName, document)
    , m_x(SVGAnimatedLength::create(this, SVGNames::xAttr, SVGLength::create(LengthModeWidth), AllowNegativeLengths))
    , m_y(SVGAnimatedLength::create(this, SVGNames::yAttr, SVGLength::create(LengthModeHeight), AllowNegativeLengths))
    , m_width(SVGAnimatedLength::create(this, SVGNames::widthAttr, SVGLength::create(LengthModeWidth), ForbidNegativeLengths))
    , m_height(SVGAnimatedLength::create(this, SVGNames::heightAttr, SVGLength::create(LengthModeHeight), ForbidNegativeLengths))
    , m_result(SVGAnimatedString::create(this, SVGNames::resultAttr, SVGString::create()))
{
    // Spec: If the x/y attribute is not specified, the effect is as if a value of "0%" were specified.
    m_x->setDefaultValueAsString("0%");
    m_y->setDefaultValueAsString("0%");

    // Spec: If the width/height attribute is not specified, the effect is as if a value of "100%" were specified.
    m_width->setDefaultValueAsString("100%");
    m_height->setDefaultValueAsString("100%");

    addToPropertyMap(m_x);
    addToPropertyMap(m_y);
    addToPropertyMap(m_width);
    addToPropertyMap(m_height);
    addToPropertyMap(m_result);
}

void SVGFilterPrimitiveStandardAttributes::trace(Visitor* visitor)
{
    visitor->trace(m_x);
    visitor->trace(m_y);
    visitor->trace(m_width);
    visitor->trace(m_height);
    visitor->trace(m_result);
    SVGElement::trace(visitor);
}

bool SVGFilterPrimitiveStandardAttributes::isSupportedAttribute(const QualifiedName& attrName)
{
    DEFINE_STATIC_LOCAL(HashSet<QualifiedName>, supportedAttributes, ());
    if (supportedAttributes.isEmpty()) {
        supportedAttributes.add(SVGNames::xAttr);
        supportedAttributes.add(SVGNames::yAttr);
        supportedAttributes.add(SVGNames::widthAttr);
        supportedAttributes.add(SVGNames::heightAttr);
        supportedAttributes.add(SVGNames::resultAttr);
    }
    return supportedAttributes.contains<SVGAttributeHashTranslator>(attrName);
}

void SVGFilterPrimitiveStandardAttributes::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    parseAttributeNew(name, value);
}

bool SVGFilterPrimitiveStandardAttributes::setFilterEffectAttribute(FilterEffect*, const QualifiedName&)
{
    // When all filters support this method, it will be changed to a pure virtual method.
    ASSERT_NOT_REACHED();
    return false;
}

void SVGFilterPrimitiveStandardAttributes::svgAttributeChanged(const QualifiedName& attrName)
{
    if (!isSupportedAttribute(attrName)) {
        SVGElement::svgAttributeChanged(attrName);
        return;
    }

    SVGElement::InvalidationGuard invalidationGuard(this);
    invalidate();
}

void SVGFilterPrimitiveStandardAttributes::childrenChanged(const ChildrenChange& change)
{
    SVGElement::childrenChanged(change);

    if (!change.byParser)
        invalidate();
}

void SVGFilterPrimitiveStandardAttributes::setStandardAttributes(FilterEffect* filterEffect) const
{
    ASSERT(filterEffect);
    if (!filterEffect)
        return;

    if (hasAttribute(SVGNames::xAttr))
        filterEffect->setHasX(true);
    if (hasAttribute(SVGNames::yAttr))
        filterEffect->setHasY(true);
    if (hasAttribute(SVGNames::widthAttr))
        filterEffect->setHasWidth(true);
    if (hasAttribute(SVGNames::heightAttr))
        filterEffect->setHasHeight(true);
}

RenderObject* SVGFilterPrimitiveStandardAttributes::createRenderer(RenderStyle*)
{
    return new RenderSVGResourceFilterPrimitive(this);
}

bool SVGFilterPrimitiveStandardAttributes::rendererIsNeeded(const RenderStyle& style)
{
    if (isSVGFilterElement(parentNode()))
        return SVGElement::rendererIsNeeded(style);

    return false;
}

void SVGFilterPrimitiveStandardAttributes::invalidate()
{
    if (RenderObject* primitiveRenderer = renderer())
        markForLayoutAndParentResourceInvalidation(primitiveRenderer);
}

void SVGFilterPrimitiveStandardAttributes::primitiveAttributeChanged(const QualifiedName& attribute)
{
    if (RenderObject* primitiveRenderer = renderer())
        static_cast<RenderSVGResourceFilterPrimitive*>(primitiveRenderer)->primitiveAttributeChanged(attribute);
}

void invalidateFilterPrimitiveParent(SVGElement* element)
{
    if (!element)
        return;

    ContainerNode* parent = element->parentNode();

    if (!parent)
        return;

    RenderObject* renderer = parent->renderer();
    if (!renderer || !renderer->isSVGResourceFilterPrimitive())
        return;

    RenderSVGResourceContainer::markForLayoutAndParentResourceInvalidation(renderer, false);
}

}
