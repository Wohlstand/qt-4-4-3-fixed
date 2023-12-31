// -*- c-basic-offset: 4 -*-
/*
 * Copyright (C) 2006 Apple Computer, Inc.
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
#include "FrameTree.h"

#include "Frame.h"
#include "Page.h"
#include <stdarg.h>
#include <wtf/Platform.h>
#include <wtf/Vector.h>

#include <stdio.h>

using std::swap;

namespace WebCore {

// FIXME: This belongs in some header file where multiple clients can share it.
#if COMPILER(MSVC)
int snprintf(char* str, size_t size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vsnprintf_s(str, size, _TRUNCATE, format, args);
    va_end(args);
    return result;
}
#endif

FrameTree::~FrameTree()
{
    for (Frame* child = firstChild(); child; child = child->tree()->nextSibling())
        child->setView(0);
}

void FrameTree::setName(const AtomicString& name) 
{
    if (!parent()) {
        m_name = name;
        return;
    }
    m_name = AtomicString(); // Remove our old frame name so it's not considered in uniqueChildName.
    m_name = parent()->tree()->uniqueChildName(name);
}

void FrameTree::appendChild(PassRefPtr<Frame> child)
{
    ASSERT(child->page() == m_thisFrame->page());
    child->tree()->m_parent = m_thisFrame;

    Frame* oldLast = m_lastChild;
    m_lastChild = child.get();

    if (oldLast) {
        child->tree()->m_previousSibling = oldLast;
        oldLast->tree()->m_nextSibling = child;
    } else
        m_firstChild = child;

    m_childCount++;

    ASSERT(!m_lastChild->tree()->m_nextSibling);
}

void FrameTree::removeChild(Frame* child)
{
    child->tree()->m_parent = 0;
    child->setView(0);
    if (child->ownerElement())
        child->page()->decrementFrameCount();
    child->pageDestroyed();

    // Slightly tricky way to prevent deleting the child until we are done with it, w/o
    // extra refs. These swaps leave the child in a circular list by itself. Clearing its
    // previous and next will then finally deref it.

    RefPtr<Frame>& newLocationForNext = m_firstChild == child ? m_firstChild : child->tree()->m_previousSibling->tree()->m_nextSibling;
    Frame*& newLocationForPrevious = m_lastChild == child ? m_lastChild : child->tree()->m_nextSibling->tree()->m_previousSibling;
    swap(newLocationForNext, child->tree()->m_nextSibling);
    // For some inexplicable reason, the following line does not compile without the explicit std:: namepsace
    std::swap(newLocationForPrevious, child->tree()->m_previousSibling);

    child->tree()->m_previousSibling = 0;
    child->tree()->m_nextSibling = 0;

    m_childCount--;
}

AtomicString FrameTree::uniqueChildName(const AtomicString& requestedName) const
{
    if (!requestedName.isEmpty() && !child(requestedName) && requestedName != "_blank")
        return requestedName;

    // Create a repeatable name for a child about to be added to us. The name must be
    // unique within the frame tree. The string we generate includes a "path" of names
    // from the root frame down to us. For this path to be unique, each set of siblings must
    // contribute a unique name to the path, which can't collide with any HTML-assigned names.
    // We generate this path component by index in the child list along with an unlikely
    // frame name that can't be set in HTML because it collides with comment syntax.

    const char framePathPrefix[] = "<!--framePath ";
    const int framePathPrefixLength = 14;
    const int framePathSuffixLength = 3;

    // Find the nearest parent that has a frame with a path in it.
    Vector<Frame*, 16> chain;
    Frame* frame;
    for (frame = m_thisFrame; frame; frame = frame->tree()->parent()) {
        if (frame->tree()->name().startsWith(framePathPrefix))
            break;
        chain.append(frame);
    }
    String name;
    name += framePathPrefix;
    if (frame)
        name += frame->tree()->name().domString().substring(framePathPrefixLength,
            frame->tree()->name().length() - framePathPrefixLength - framePathSuffixLength);
    for (int i = chain.size() - 1; i >= 0; --i) {
        frame = chain[i];
        name += "/";
        name += frame->tree()->name();
    }

    // Suffix buffer has more than enough space for:
    //     10 characters before the number
    //     a number (3 digits for the highest this gets in practice, 20 digits for the largest 64-bit integer)
    //     6 characters after the number
    //     trailing null byte
    // But we still use snprintf just to be extra-safe.
    char suffix[40];
    snprintf(suffix, sizeof(suffix), "/<!--frame%u-->-->", childCount());

    name += suffix;

    return AtomicString(name);
}

Frame* FrameTree::child(unsigned index) const
{
    Frame* result = firstChild();
    for (unsigned i = 0; result && i != index; ++i)
        result = result->tree()->nextSibling();
    return result;
}

Frame* FrameTree::child(const AtomicString& name) const
{
    for (Frame* child = firstChild(); child; child = child->tree()->nextSibling())
        if (child->tree()->name() == name)
            return child;
    return 0;
}

Frame* FrameTree::find(const AtomicString& name) const
{
    if (name == "_self" || name == "_current" || name.isEmpty())
        return m_thisFrame;
    
    if (name == "_top")
        return m_thisFrame->page()->mainFrame();
    
    if (name == "_parent")
        return parent() ? parent() : m_thisFrame;

    // Since "_blank" should never be any frame's name, the following just amounts to an optimization.
    if (name == "_blank")
        return 0;

    // Search subtree starting with this frame first.
    for (Frame* frame = m_thisFrame; frame; frame = frame->tree()->traverseNext(m_thisFrame))
        if (frame->tree()->name() == name)
            return frame;

    // Search the entire tree for this page next.
    Page* page = m_thisFrame->page();
    for (Frame* frame = page->mainFrame(); frame; frame = frame->tree()->traverseNext())
        if (frame->tree()->name() == name)
            return frame;

    // Search the entire tree for all other pages in this namespace.
    const HashSet<Page*>* pages = page->frameNamespace();
    if (pages) {
        HashSet<Page*>::const_iterator end = pages->end();
        for (HashSet<Page*>::const_iterator it = pages->begin(); it != end; ++it) {
            Page* otherPage = *it;
            if (otherPage != page)
                for (Frame* frame = otherPage->mainFrame(); frame; frame = frame->tree()->traverseNext())
                    if (frame->tree()->name() == name)
                        return frame;
        }
    }

    return 0;
}

bool FrameTree::isDescendantOf(const Frame* ancestor) const
{
    if (!ancestor)
        return false;

    if (m_thisFrame->page() != ancestor->page())
        return false;

    for (Frame* frame = m_thisFrame; frame; frame = frame->tree()->parent())
        if (frame == ancestor)
            return true;
    return false;
}

Frame* FrameTree::traverseNext(const Frame* stayWithin) const
{
    Frame* child = firstChild();
    if (child) {
        ASSERT(!stayWithin || child->tree()->isDescendantOf(stayWithin));
        return child;
    }

    if (m_thisFrame == stayWithin)
        return 0;

    Frame* sibling = nextSibling();
    if (sibling) {
        ASSERT(!stayWithin || sibling->tree()->isDescendantOf(stayWithin));
        return sibling;
    }

    Frame* frame = m_thisFrame;
    while (!sibling && (!stayWithin || frame->tree()->parent() != stayWithin)) {
        frame = frame->tree()->parent();
        if (!frame)
            return 0;
        sibling = frame->tree()->nextSibling();
    }

    if (frame) {
        ASSERT(!stayWithin || !sibling || sibling->tree()->isDescendantOf(stayWithin));
        return sibling;
    }

    return 0;
}

Frame* FrameTree::traverseNextWithWrap(bool wrap) const
{
    if (Frame* result = traverseNext())
        return result;

    if (wrap)
        return m_thisFrame->page()->mainFrame();

    return 0;
}

Frame* FrameTree::traversePreviousWithWrap(bool wrap) const
{
    // FIXME: besides the wrap feature, this is just the traversePreviousNode algorithm

    if (Frame* prevSibling = previousSibling())
        return prevSibling->tree()->deepLastChild();
    if (Frame* parentFrame = parent())
        return parentFrame;
    
    // no siblings, no parent, self==top
    if (wrap)
        return deepLastChild();

    // top view is always the last one in this ordering, so prev is nil without wrap
    return 0;
}

Frame* FrameTree::deepLastChild() const
{
    Frame* result = m_thisFrame;
    for (Frame* last = lastChild(); last; last = last->tree()->lastChild())
        result = last;

    return result;
}

Frame* FrameTree::top() const
{
    if (Page* page = m_thisFrame->page())
        return page->mainFrame();

    Frame* frame = m_thisFrame;
    while (Frame* parent = frame->tree()->parent())
        frame = parent;
    return frame;
}

} // namespace WebCore
