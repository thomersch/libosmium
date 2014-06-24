#ifndef OSMIUM_GEOM_OGR_HPP
#define OSMIUM_GEOM_OGR_HPP

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

#define OSMIUM_COMPILE_WITH_CFLAGS_OGR `gdal-config --cflags`
#define OSMIUM_LINK_WITH_LIBS_OGR `gdal-config --libs`

#include <cassert>
#include <memory>
#include <utility>

#include <ogr_geometry.h>

#include <osmium/geom/factory.hpp>
#include <osmium/osm/location.hpp>

namespace osmium {

    namespace geom {

        struct ogr_factory_traits {
            typedef std::unique_ptr<OGRPoint>        point_type;
            typedef std::unique_ptr<OGRLineString>   linestring_type;
            typedef std::unique_ptr<OGRPolygon>      polygon_type;
            typedef std::unique_ptr<OGRMultiPolygon> multipolygon_type;
            typedef std::unique_ptr<OGRLinearRing>   ring_type;
        };

        class OGRFactory : public GeometryFactory<OGRFactory, ogr_factory_traits> {

            friend class GeometryFactory;

        public:

            OGRFactory() :
                GeometryFactory<OGRFactory, ogr_factory_traits>() {
            }

        private:

            /* Point */

            point_type make_point(const osmium::Location location) const {
                return point_type(new OGRPoint(location.lon(), location.lat()));
            }

            /* LineString */

            linestring_type m_linestring;

            void linestring_start() {
                m_linestring = std::unique_ptr<OGRLineString>(new OGRLineString());
            }

            void linestring_add_location(const osmium::Location location) {
                assert(!!m_linestring);
                m_linestring->addPoint(location.lon(), location.lat());
            }

            linestring_type linestring_finish() {
                return std::move(m_linestring);
            }

            /* MultiPolygon */

            multipolygon_type m_multipolygon;
            polygon_type      m_polygon;
            ring_type         m_ring;

            void multipolygon_start() {
                m_multipolygon.reset(new OGRMultiPolygon());
            }

            void multipolygon_outer_ring_start() {
                if (m_polygon) {
                    m_multipolygon->addGeometryDirectly(m_polygon.release());
                }
                m_polygon.reset(new OGRPolygon());
                m_ring.reset(new OGRLinearRing());
            }

            void multipolygon_outer_ring_finish() {
                assert(!!m_polygon);
                assert(!!m_ring);
                m_polygon->addRingDirectly(m_ring.release());
            }

            void multipolygon_inner_ring_start() {
                m_ring.reset(new OGRLinearRing());
            }

            void multipolygon_inner_ring_finish() {
                assert(!!m_polygon);
                assert(!!m_ring);
                m_polygon->addRingDirectly(m_ring.release());
            }

            void multipolygon_add_location(const osmium::Location location) {
                assert(!!m_polygon);
                assert(!!m_ring);
                m_ring->addPoint(location.lon(), location.lat());
            }

            multipolygon_type multipolygon_finish() {
                assert(!!m_polygon);
                m_multipolygon->addGeometryDirectly(m_polygon.release());
                return std::move(m_multipolygon);
            }

        }; // class OGRFactory

    } // namespace geom

} // namespace osmium

#endif // OSMIUM_GEOM_OGR_HPP
