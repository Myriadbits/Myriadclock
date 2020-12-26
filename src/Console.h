#pragma once

#include <vector>
#include <string>

#define COMMAND_BUFFER_SIZE     256 // Maximum # chars on a line

#ifndef NULL
#define NULL    (void *)0
#endif

//
// Handler for processing console commands
//
class ConsoleCallbacks
{
    public:
    virtual void commandHandler(std::string command, std::vector<std::string> args) {};
};

///////////////////////////////////////////////////////////////////////////////
// Console class
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class Console
{
    public:
        Console()
            : m_started(false)
        {
        }

        Console(Console const&) = delete;
        void operator=(Console const&) = delete;

        static Console& getInstance()
        {
            static Console instance; // Guaranteed to be destroyed. Instantiated on first use.
            return instance;
        }

        void start(long baudrate);
        void tick(void);
        void add(std::string name, ConsoleCallbacks *phandler, std::string synopsis);

    private:
        typedef struct
        {
            std::string          name;
            std::string          synopsis;
            ConsoleCallbacks      *handler;
        } cmd_t;

        // private variables
        std::vector<cmd_t>          m_commands;
        std::vector<std::string>    m_args;
        std::string                 m_line;
        bool            m_prompted;
        bool            m_started;
        long            m_baudrate;
        bool            m_espmon_on;

        // private functions
        void process(void);
        void list(void);
};
