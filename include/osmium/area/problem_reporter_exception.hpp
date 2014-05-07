#ifndef OSMIUM_AREA_PROBLEM_REPORTER_EXCEPTION_HPP
#define OSMIUM_AREA_PROBLEM_REPORTER_EXCEPTION_HPP

/*

This file is part of Osmium (http://osmcode.org/libosmium).

Copyright 2014 Jochen Topf <jochen@topf.org> and others (see README).

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

#include <sstream>

#include <osmium/area/problem_reporter.hpp>
#include <osmium/osm/location.hpp>
#include <osmium/osm/ostream.hpp>
#include <osmium/osm/types.hpp>

namespace osmium {

    namespace area {

        class ProblemReporterException : public ProblemReporter {

        public:

            ProblemReporterException() = default;

            virtual ~ProblemReporterException() = default;

            void report_duplicate_node(osmium::object_id_type node_id1, osmium::object_id_type node_id2, osmium::Location location) override {
                std::stringstream s;
                s << "DATA PROBLEM: duplicate node: node_id1=" << node_id1 << " node_id2=" << node_id2 << " location=" << location << "\n";
                throw std::runtime_error(s.str());
            }

            void report_intersection(osmium::object_id_type object_id, osmium::object_id_type way1_id, osmium::Location way1_seg_start, osmium::Location way1_seg_end,
                                                                       osmium::object_id_type way2_id, osmium::Location way2_seg_start, osmium::Location way2_seg_end, osmium::Location intersection) override {
                std::stringstream s;
                s << "DATA PROBLEM: intersection: object_id=" << object_id << " way1_id=" << way1_id << " way1_seg_start=" << way1_seg_start << " way1_seg_end=" << way1_seg_end
                                                                           << " way2_id=" << way2_id << " way2_seg_start=" << way2_seg_start << " way2_seg_end=" << way2_seg_end << " intersection=" << intersection << "\n";
                throw std::runtime_error(s.str());
            }

            void report_ring_not_closed(osmium::object_id_type object_id, osmium::Location end1, osmium::Location end2) override {
                std::stringstream s;
                s << "DATA PROBLEM: ring not closed: object_id=" << object_id << " end1=" << end1 << " end2=" << end2 << "\n";
                throw std::runtime_error(s.str());
            }

            void report_role_should_be_outer(osmium::object_id_type object_id, osmium::object_id_type way_id, osmium::Location seg_start, osmium::Location seg_end) override {
                std::stringstream s;
                s << "DATA PROBLEM: role should be outer: object_id=" << object_id << " way_id=" << way_id << " seg_start=" << seg_start << " seg_end=" << seg_end << "\n";
                throw std::runtime_error(s.str());
            }

            void report_role_should_be_inner(osmium::object_id_type object_id, osmium::object_id_type way_id, osmium::Location seg_start, osmium::Location seg_end) override {
                std::stringstream s;
                s << "DATA PROBLEM: role should be inner: object_id=" << object_id << " way_id=" << way_id << " seg_start=" << seg_start << " seg_end=" << seg_end << "\n";
                throw std::runtime_error(s.str());
            }

        }; // class ProblemReporterException

    } // namespace area

} // namespace osmium

#endif // OSMIUM_AREA_PROBLEM_REPORTER_EXCEPTION_HPP
