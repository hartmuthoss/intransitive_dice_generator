// Utility logger class, logs to std::cout resp. std::cerr and optionally to logfile
#pragma once
#include <iomanip>
#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>

// DiceLoggerStream redirects output to a list of streams (e.g. std::cout, std::cerr and logfiles)
class DiceLoggerStream
{
public:

    // Initializing constructor, redirects all output to a given ostream (e.g. std::cout)
    explicit DiceLoggerStream(std::ostream& os) : m_ostream(os) { }

    // Redirect any output to the list of streams
    template <typename T> DiceLoggerStream& operator<<(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_ostream << data;
        for(int n = 0; n < m_fstreams.size(); n++)
            if (m_fstreams[n] && m_fstreams[n]->is_open())
                *(m_fstreams[n]) << data;
        return *this;
    }

    // Redirect any io manipulators (e.g. std::endl) to the list of streams
    DiceLoggerStream& operator<<(std::ostream& (*manip)(std::ostream&))
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_ostream << manip;
        for (int n = 0; n < m_fstreams.size(); n++)
            if (m_fstreams[n]->is_open())
                *(m_fstreams[n]) << manip;
        return *this;
    }

    // Remove all optional log streams
    void clear(void) { m_fstreams.clear(); }

    // Append an optional log stream
    void push(std::shared_ptr<std::ofstream>& fstream) { m_fstreams.push_back(fstream); }

protected:

    std::ostream& m_ostream; // std::cout or std::cerr 
    std::vector<std::shared_ptr<std::ofstream>> m_fstreams; // optional log streams
    std::mutex m_mutex; // thread protection

}; // DiceLoggerStream

class DiceLogger
{
public:

    // Default constructor using std::cout and std::cerr
    DiceLogger() : m_cout(std::cout), m_cerr(std::cerr) { }
    
    // Initializing constructor with additional logfile
    DiceLogger(const std::string& logfile) : m_cout(std::cout), m_cerr(std::cerr)
    {
        m_fstreams.push_back(std::make_shared<std::ofstream>(std::ofstream(logfile)));
        if (m_fstreams.back()->is_open())
        {
            m_cout.push(m_fstreams.back());
            m_cerr.push(m_fstreams.back());
        }
    }

    // Destructor
    ~DiceLogger()
    {
        m_cout.clear();
        m_cerr.clear();
        m_fstreams.clear();
    }

    // Log to std::cout and optional logfile
	DiceLoggerStream& cout() { return m_cout; }

    // Log to std::cerr and optional logfile
    DiceLoggerStream& cerr() { return m_cerr; }

protected:

	DiceLoggerStream m_cout; // Log to std::cout and optional logfile
	DiceLoggerStream m_cerr; // Log to std::cerr and optional logfile
    std::vector<std::shared_ptr<std::ofstream>> m_fstreams; // optional logfile streams

}; // DiceLogger
