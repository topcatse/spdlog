/*************************************************************************/
/* spdlog - an extremely fast and easy to use c++11 logging library.     */
/* Copyright (c) 2014 Gabi Melman.                                       */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#pragma once
//
// Logger implementation
//

#include "./line_logger.h"


// create logger with given name, sinks and the default pattern formatter
// all other ctors will call this one
template<class It>
inline spdlog::logger::logger(const std::string& logger_name, const It& begin, const It& end) :
    _name(logger_name),
    _sinks(begin, end),
    _formatter(std::make_shared<pattern_formatter>("%+"))
{

    // no support under vs2013 for member initialization for std::atomic
    _level = level::info;
}

// ctor with single sink
inline spdlog::logger::logger(const std::string& logger_name, std::shared_ptr < sinks::sink > single_sink) :
    _name(logger_name),
    _sinks(1, single_sink),
    _formatter(std::make_shared<pattern_formatter>("%+"))
{

    // no support under vs2013 for member initialization for std::atomic
    _level = level::info;
}

inline spdlog::logger::~logger() {}


inline void spdlog::logger::set_formatter(std::shared_ptr<spdlog::formatter> msg_formatter)
{
    _set_formatter(msg_formatter);
}

inline void spdlog::logger::set_pattern(const std::string& pattern)
{
    _set_pattern(pattern);
}

//
// log only if given level>=logger's log level
//


inline spdlog::details::line_logger spdlog::logger::_log_if_enabled(level::level_enum lvl)
{
    return details::line_logger(this, lvl, should_log(lvl));
}

template<typename T>
inline spdlog::details::line_logger spdlog::logger::_log_if_enabled(level::level_enum lvl, const T& msg)
{
    bool msg_enabled = should_log(lvl);
    details::line_logger l(this, lvl, msg_enabled);
    l << msg;
    return l;
}

//
// logger.info(msg) << ".." call style
//
template<typename T>
inline spdlog::details::line_logger spdlog::logger::trace(const T& msg)
{
    return _log_if_enabled(level::trace, msg);
}

template<typename T>
inline spdlog::details::line_logger spdlog::logger::debug(const T& msg)
{
    return _log_if_enabled(level::debug, msg);
}


template<typename T>
inline spdlog::details::line_logger spdlog::logger::info(const T& msg)
{
    return _log_if_enabled(level::info, msg);
}

template<typename T>
inline spdlog::details::line_logger spdlog::logger::notice(const T& msg)
{
    return _log_if_enabled(level::notice, msg);
}

template<typename T>
inline spdlog::details::line_logger spdlog::logger::warn(const T& msg)
{
    return _log_if_enabled(level::warn, msg);
}

template<typename T>
inline spdlog::details::line_logger spdlog::logger::error(const T& msg)
{
    return _log_if_enabled(level::err, msg);
}

template<typename T>
inline spdlog::details::line_logger spdlog::logger::critical(const T& msg)
{
    return _log_if_enabled(level::critical, msg);
}

template<typename T>
inline spdlog::details::line_logger spdlog::logger::alert(const T& msg)
{
    return _log_if_enabled(level::alert, msg);
}

template<typename T>
inline spdlog::details::line_logger spdlog::logger::emerg(const T& msg)
{
    return _log_if_enabled(level::emerg, msg);
}




//
// logger.info() << ".." call  style
//
inline spdlog::details::line_logger spdlog::logger::trace()
{
    return _log_if_enabled(level::trace);
}

inline spdlog::details::line_logger spdlog::logger::debug()
{
    return _log_if_enabled(level::debug);
}

inline spdlog::details::line_logger spdlog::logger::info()
{
    return _log_if_enabled(level::info);
}

inline spdlog::details::line_logger spdlog::logger::notice()
{
    return _log_if_enabled(level::notice);
}

inline spdlog::details::line_logger spdlog::logger::warn()
{
    return _log_if_enabled(level::warn);
}

inline spdlog::details::line_logger spdlog::logger::error()
{
    return _log_if_enabled(level::err);
}

inline spdlog::details::line_logger spdlog::logger::critical()
{
    return _log_if_enabled(level::critical);
}

inline spdlog::details::line_logger spdlog::logger::alert()
{
    return _log_if_enabled(level::alert);
}

inline spdlog::details::line_logger spdlog::logger::emerg()
{
    return _log_if_enabled(level::emerg);
}


//
// name and level
//
inline const std::string& spdlog::logger::name() const
{
    return _name;
}

inline void spdlog::logger::set_level(spdlog::level::level_enum log_level)
{
    _level.store(log_level);
}

inline spdlog::level::level_enum spdlog::logger::level() const
{
    return static_cast<spdlog::level::level_enum>(_level.load(std::memory_order_relaxed));
}

inline bool spdlog::logger::should_log(spdlog::level::level_enum msg_level) const
{
    return msg_level >= _level.load(std::memory_order_relaxed);
}

//
// protected virtual called at end of each user log call (if enabled) by the line_logger
//
inline void spdlog::logger::_log_msg(details::log_msg& msg)
{
    _formatter->format(msg);
    for (auto &sink : _sinks)
        sink->log(msg);
}

inline void spdlog::logger::_set_pattern(const std::string& pattern)
{
    _formatter = std::make_shared<pattern_formatter>(pattern);
}
inline void spdlog::logger::_set_formatter(std::shared_ptr<spdlog::formatter> msg_formatter)
{
    _formatter = msg_formatter;
}


