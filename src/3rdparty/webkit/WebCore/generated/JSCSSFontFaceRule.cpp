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

#include "JSCSSFontFaceRule.h"

#include <wtf/GetPtr.h>

#include "CSSFontFaceRule.h"
#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "JSCSSStyleDeclaration.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSCSSFontFaceRuleTableEntries[] =
{
    { "style", JSCSSFontFaceRule::StyleAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "constructor", JSCSSFontFaceRule::ConstructorAttrNum, DontDelete|DontEnum|ReadOnly, 0, 0 }
};

static const HashTable JSCSSFontFaceRuleTable = 
{
    2, 2, JSCSSFontFaceRuleTableEntries, 2
};

/* Hash table for constructor */

static const HashEntry JSCSSFontFaceRuleConstructorTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSCSSFontFaceRuleConstructorTable = 
{
    2, 1, JSCSSFontFaceRuleConstructorTableEntries, 1
};

class JSCSSFontFaceRuleConstructor : public DOMObject {
public:
    JSCSSFontFaceRuleConstructor(ExecState* exec)
    {
        setPrototype(exec->lexicalInterpreter()->builtinObjectPrototype());
        putDirect(exec->propertyNames().prototype, JSCSSFontFaceRulePrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    JSValue* getValueProperty(ExecState*, int token) const;
    virtual const ClassInfo* classInfo() const { return &info; }
    static const ClassInfo info;

    virtual bool implementsHasInstance() const { return true; }
};

const ClassInfo JSCSSFontFaceRuleConstructor::info = { "CSSFontFaceRuleConstructor", 0, &JSCSSFontFaceRuleConstructorTable, 0 };

bool JSCSSFontFaceRuleConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSCSSFontFaceRuleConstructor, DOMObject>(exec, &JSCSSFontFaceRuleConstructorTable, this, propertyName, slot);
}

JSValue* JSCSSFontFaceRuleConstructor::getValueProperty(ExecState*, int token) const
{
    // The token is the numeric value of its associated constant
    return jsNumber(token);
}

/* Hash table for prototype */

static const HashEntry JSCSSFontFaceRulePrototypeTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSCSSFontFaceRulePrototypeTable = 
{
    2, 1, JSCSSFontFaceRulePrototypeTableEntries, 1
};

const ClassInfo JSCSSFontFaceRulePrototype::info = { "CSSFontFaceRulePrototype", 0, &JSCSSFontFaceRulePrototypeTable, 0 };

JSObject* JSCSSFontFaceRulePrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSCSSFontFaceRulePrototype>(exec, "[[JSCSSFontFaceRule.prototype]]");
}

const ClassInfo JSCSSFontFaceRule::info = { "CSSFontFaceRule", &JSCSSRule::info, &JSCSSFontFaceRuleTable, 0 };

JSCSSFontFaceRule::JSCSSFontFaceRule(ExecState* exec, CSSFontFaceRule* impl)
    : JSCSSRule(exec, impl)
{
    setPrototype(JSCSSFontFaceRulePrototype::self(exec));
}

bool JSCSSFontFaceRule::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSCSSFontFaceRule, JSCSSRule>(exec, &JSCSSFontFaceRuleTable, this, propertyName, slot);
}

JSValue* JSCSSFontFaceRule::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case StyleAttrNum: {
        CSSFontFaceRule* imp = static_cast<CSSFontFaceRule*>(impl());

        return toJS(exec, WTF::getPtr(imp->style()));
    }
    case ConstructorAttrNum:
        return getConstructor(exec);
    }
    return 0;
}

JSValue* JSCSSFontFaceRule::getConstructor(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSCSSFontFaceRuleConstructor>(exec, "[[CSSFontFaceRule.constructor]]");
}

}
