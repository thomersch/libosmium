#ifndef OSMIUM_INDEX_MAP_STD_MAP_HPP
#define OSMIUM_INDEX_MAP_STD_MAP_HPP

/*

This file is part of Osmium (http://osmcode.org/osmium).

Copyright 2013 Jochen Topf <jochen@topf.org> and others (see README).

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

#include <map>

#include <osmium/index/map.hpp>

namespace osmium {

    namespace index {

        namespace map {

            /**
             * This implementation uses std::map internally. It uses rather a
             * lot of memory, but might make sense for small maps.
             */
            template <typename TValue>
            class StdMap : public osmium::index::map::Map<TValue> {

                // this is a rough estimate (pointers to left, right, and parent
                // plus some overhead for color of red-black-tree or similar.)
                static constexpr size_t element_overhead = sizeof(void*) * 4;

                std::map<uint64_t, TValue> m_map;

            public:

                StdMap() :
                    Map<TValue>() {
                }

                ~StdMap() override final {
                }

                void set(const uint64_t id, const TValue value) const override final {
                    m_map[id] = value;
                }

                const TValue operator[](const uint64_t id) const override final {
                    return m_map.at(id);
                }

                size_t size() const override final {
                    return m_map.size();
                }

                size_t used_memory() const override final {
                    return (sizeof(uint64_t) + sizeof(TValue) + element_overhead) * m_map.size();
                }

                void clear() const override final {
                    m_map.clear();
                }

            }; // class StdMap

        } // namespace map

    } // namespace index

} // namespace osmium

#endif // OSMIUM_INDEX_MAP_STD_MAP_HPP