#ifndef OSMIUM_VISITOR_HPP
#define OSMIUM_VISITOR_HPP

/*

This file is part of Osmium (http://osmcode.org/libosmium).

Copyright 2013,2014 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <stdexcept>
#include <type_traits>

#include <osmium/io/reader_iterator.hpp>
#include <osmium/memory/buffer.hpp>
#include <osmium/osm/item_type.hpp>

namespace osmium {

    class Object;
    class Node;
    class Way;
    class Relation;
    class Area;
    class Changeset;
    class TagList;
    class WayNodeList;
    class RelationMemberList;
    class OuterRing;
    class InnerRing;

    namespace memory {
        class Item;
    }

    namespace {

        template <typename T, typename U>
        using ConstIfConst = typename std::conditional<std::is_const<T>::value, typename std::add_const<U>::type, U>::type;

        template <class THandler, class TItem>
        inline void apply_item_recurse(TItem& item, THandler& handler) {
            switch (item.type()) {
                case osmium::item_type::node:
                    handler.osm_object(static_cast<ConstIfConst<TItem, osmium::Object>&>(item));
                    handler.node(static_cast<ConstIfConst<TItem, osmium::Node>&>(item));
                    break;
                case osmium::item_type::way:
                    handler.osm_object(static_cast<ConstIfConst<TItem, osmium::Object>&>(item));
                    handler.way(static_cast<ConstIfConst<TItem, osmium::Way>&>(item));
                    break;
                case osmium::item_type::relation:
                    handler.osm_object(static_cast<ConstIfConst<TItem, osmium::Object>&>(item));
                    handler.relation(static_cast<ConstIfConst<TItem, osmium::Relation>&>(item));
                    break;
                case osmium::item_type::area:
                    handler.osm_object(static_cast<ConstIfConst<TItem, osmium::Object>&>(item));
                    handler.area(static_cast<ConstIfConst<TItem, osmium::Area>&>(item));
                    break;
                case osmium::item_type::changeset:
                    handler.changeset(static_cast<ConstIfConst<TItem, osmium::Changeset>&>(item));
                    break;
                case osmium::item_type::tag_list:
                    handler.tag_list(static_cast<ConstIfConst<TItem, osmium::TagList>&>(item));
                    break;
                case osmium::item_type::way_node_list:
                    handler.way_node_list(static_cast<ConstIfConst<TItem, osmium::WayNodeList>&>(item));
                    break;
                case osmium::item_type::relation_member_list:
                case osmium::item_type::relation_member_list_with_full_members:
                    handler.relation_member_list(static_cast<ConstIfConst<TItem, osmium::RelationMemberList>&>(item));
                    break;
                case osmium::item_type::outer_ring:
                    handler.outer_ring(static_cast<ConstIfConst<TItem, osmium::OuterRing>&>(item));
                    break;
                case osmium::item_type::inner_ring:
                    handler.inner_ring(static_cast<ConstIfConst<TItem, osmium::InnerRing>&>(item));
                    break;
                default:
                    throw std::runtime_error("unknown type");
            }
        }

        template <class THandler, class TItem, class ...TRest>
        inline void apply_item_recurse(TItem& item, THandler& handler, TRest&... more) {
            apply_item_recurse(item, handler);
            apply_item_recurse(item, more...);
        }

        template <class THandler>
        inline void done_recurse(THandler& handler) {
            handler.done();
        }

        template <class THandler, class ...TRest>
        inline void done_recurse(THandler& handler, TRest&... more) {
            done_recurse(handler);
            done_recurse(more...);
        }

    } // anonymous namespace

    template <class ...THandlers>
    inline void apply_item(const osmium::memory::Item& item, THandlers&... handlers) {
        apply_item_recurse(item, handlers...);
    }

    template <class ...THandlers>
    inline void apply_item(osmium::memory::Item& item, THandlers&... handlers) {
        apply_item_recurse(item, handlers...);
    }

    template <class TIterator, class ...THandlers>
    inline void apply(TIterator it, TIterator end, THandlers&... handlers) {
        for (; it != end; ++it) {
            apply_item_recurse(*it, handlers...);
        }
        done_recurse(handlers...);
    }

    template <class TContainer, class ...THandlers>
    inline void apply(TContainer& c, THandlers&... handlers) {
        apply(std::begin(c), std::end(c), handlers...);
    }

    template <class ...THandlers>
    inline void apply(const osmium::memory::Buffer& buffer, THandlers&... handlers) {
        apply(buffer.cbegin(), buffer.cend(), handlers...);
    }

} // namespace osmium

#endif // OSMIUM_VISITOR_HPP
