/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"


#if ENABLE(SVG)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "JSSVGPathSegCurvetoQuadraticAbs.h"

#include <wtf/GetPtr.h>

#include "SVGPathSegCurvetoQuadratic.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGPathSegCurvetoQuadraticAbsTableEntries[] =
{
    { "y", JSSVGPathSegCurvetoQuadraticAbs::YAttrNum, DontDelete, 0, 0 },
    { "x", JSSVGPathSegCurvetoQuadraticAbs::XAttrNum, DontDelete, 0, 0 },
    { "x1", JSSVGPathSegCurvetoQuadraticAbs::X1AttrNum, DontDelete, 0, 0 },
    { "y1", JSSVGPathSegCurvetoQuadraticAbs::Y1AttrNum, DontDelete, 0, 0 }
};

static const HashTable JSSVGPathSegCurvetoQuadraticAbsTable = 
{
    2, 4, JSSVGPathSegCurvetoQuadraticAbsTableEntries, 4
};

/* Hash table for prototype */

static const HashEntry JSSVGPathSegCurvetoQuadraticAbsPrototypeTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSSVGPathSegCurvetoQuadraticAbsPrototypeTable = 
{
    2, 1, JSSVGPathSegCurvetoQuadraticAbsPrototypeTableEntries, 1
};

const ClassInfo JSSVGPathSegCurvetoQuadraticAbsPrototype::info = { "SVGPathSegCurvetoQuadraticAbsPrototype", 0, &JSSVGPathSegCurvetoQuadraticAbsPrototypeTable, 0 };

JSObject* JSSVGPathSegCurvetoQuadraticAbsPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGPathSegCurvetoQuadraticAbsPrototype>(exec, "[[JSSVGPathSegCurvetoQuadraticAbs.prototype]]");
}

const ClassInfo JSSVGPathSegCurvetoQuadraticAbs::info = { "SVGPathSegCurvetoQuadraticAbs", &JSSVGPathSeg::info, &JSSVGPathSegCurvetoQuadraticAbsTable, 0 };

JSSVGPathSegCurvetoQuadraticAbs::JSSVGPathSegCurvetoQuadraticAbs(ExecState* exec, SVGPathSegCurvetoQuadraticAbs* impl)
    : JSSVGPathSeg(exec, impl)
{
    setPrototype(JSSVGPathSegCurvetoQuadraticAbsPrototype::self(exec));
}

bool JSSVGPathSegCurvetoQuadraticAbs::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegCurvetoQuadraticAbs, JSSVGPathSeg>(exec, &JSSVGPathSegCurvetoQuadraticAbsTable, this, propertyName, slot);
}

JSValue* JSSVGPathSegCurvetoQuadraticAbs::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case XAttrNum: {
        SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

        return jsNumber(imp->x());
    }
    case YAttrNum: {
        SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

        return jsNumber(imp->y());
    }
    case X1AttrNum: {
        SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

        return jsNumber(imp->x1());
    }
    case Y1AttrNum: {
        SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

        return jsNumber(imp->y1());
    }
    }
    return 0;
}

void JSSVGPathSegCurvetoQuadraticAbs::put(ExecState* exec, const Identifier& propertyName, JSValue* value, int attr)
{
    lookupPut<JSSVGPathSegCurvetoQuadraticAbs, JSSVGPathSeg>(exec, propertyName, value, attr, &JSSVGPathSegCurvetoQuadraticAbsTable, this);
}

void JSSVGPathSegCurvetoQuadraticAbs::putValueProperty(ExecState* exec, int token, JSValue* value, int /*attr*/)
{
    switch (token) {
    case XAttrNum: {
        SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

        imp->setX(value->toFloat(exec));
        break;
    }
    case YAttrNum: {
        SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

        imp->setY(value->toFloat(exec));
        break;
    }
    case X1AttrNum: {
        SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

        imp->setX1(value->toFloat(exec));
        break;
    }
    case Y1AttrNum: {
        SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

        imp->setY1(value->toFloat(exec));
        break;
    }
    }
    SVGPathSegCurvetoQuadraticAbs* imp = static_cast<SVGPathSegCurvetoQuadraticAbs*>(impl());

    ASSERT(exec && exec->dynamicInterpreter());
    Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
    if (!activeFrame)
        return;

    SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
    if (extensions && extensions->hasGenericContext<SVGPathSeg>(imp)) {
        const SVGElement* context = extensions->genericContext<SVGPathSeg>(imp);
        ASSERT(context);

        context->notifyAttributeChange();
    }

}


}

#endif // ENABLE(SVG)
