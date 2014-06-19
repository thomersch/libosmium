#ifndef OSMIUM_IO_DETAIL_READ_WRITE_HPP
#define OSMIUM_IO_DETAIL_READ_WRITE_HPP

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

#include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <string>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <system_error>
#include <unistd.h>

#include <osmium/io/overwrite.hpp>

namespace osmium {

    namespace io {

        /**
         * @brief Namespace for Osmium internal use
         */
        namespace detail {

            /**
             * Open file for writing. If the file exists, it is truncated, if
             * not, it is created. If the file name is empty or "-", no file
             * is open and the stdout file descriptor (1) is returned.
             *
             * @param filename Name of file to be opened.
             * @param allow_overwrite If the file exists, should it be overwritten?
             * @return File descriptor of open file.
             * @throws system_error if the file can't be opened.
             */
            inline int open_for_writing(const std::string& filename, osmium::io::overwrite allow_overwrite = osmium::io::overwrite::no) {
                if (filename == "" || filename == "-") {
                    return 1; // stdout
                } else {
                    int flags = O_WRONLY | O_CREAT;
                    if (allow_overwrite == osmium::io::overwrite::allow) {
                        flags |= O_TRUNC;
                    } else {
                        flags |= O_EXCL;
                    }
#ifdef WIN32
                    flags |= O_BINARY;
#endif
                    int fd = ::open(filename.c_str(), flags, 0666);
                    if (fd < 0) {
                        throw std::system_error(errno, std::system_category(), std::string("Open failed for '") + filename + "'");
                    }
                    return fd;
                }
            }

            /**
             * Open file for reading. If the file name is empty or "-", no file
             * is open and the stdin file descriptor (0) is returned.
             *
             * @return File descriptor of open file.
             * @throws system_error if the file can't be opened.
             */
            inline int open_for_reading(const std::string& filename) {
                if (filename == "" || filename == "-") {
                    return 0; // stdin
                } else {
                    int flags = O_RDONLY;
#ifdef WIN32
                    flags |= O_BINARY;
#endif
                    int fd = ::open(filename.c_str(), flags);
                    if (fd < 0) {
                        throw std::system_error(errno, std::system_category(), std::string("Open failed for '") + filename + "'");
                    }
                    return fd;
                }
            }

            /**
             * Reads the given number of bytes into the input buffer.
             * This is basically just a wrapper around read(2).
             *
             * @param fd File descriptor.
             * @param input_buffer Buffer with data of at least size.
             * @param size Number of bytes to be read.
             * @return True when read was successful, false on EOF.
             * @exception std::system_error On error.
             */
            inline bool reliable_read(const int fd, unsigned char* input_buffer, const size_t size) {
                size_t offset = 0;
                while (offset < size) {
                    ssize_t nread = ::read(fd, input_buffer + offset, size - offset);
                    if (nread < 0) {
                        throw std::system_error(errno, std::system_category(), "Read failed");
                    }
                    if (nread == 0) {
                        return false;
                    }
                    offset += nread;
                }
                return true;
            }

            /**
             * Writes the given number of bytes from the output_buffer to the file descriptor.
             * This is just a wrapper around write(2).
             *
             * @param fd File descriptor.
             * @param output_buffer Buffer where data is written. Must be at least size bytes long.
             * @param size Number of bytes to be read.
             * @exception std::system_error On error.
             */
            inline void reliable_write(const int fd, const unsigned char* output_buffer, const size_t size) {
                size_t offset = 0;
                do {
                    ssize_t length = ::write(fd, output_buffer + offset, size - offset);
                    if (length < 0) {
                        throw std::system_error(errno, std::system_category(), "Write failed");
                    }
                    offset += length;
                } while (offset < size);
            }

            inline void reliable_write(const int fd, const char* output_buffer, const size_t size) {
                reliable_write(fd, reinterpret_cast<const unsigned char*>(output_buffer), size);
            }

        } // namespace detail

    } // namespace io

} // namespace osmium

#endif // OSMIUM_IO_DETAIL_READ_WRITE_HPP
