#pragma once

#define MAXCOMMANDS          15       // Maximum # commands
#define MAXARGUMENTS          3       // Maximum # command line arguments
#define COMMAND_BUFFER_SIZE  40       // Character buffer size 

#ifndef NULL
#define NULL    (void *)0
#endif

///////////////////////////////////////////////////////////////////////////////
// Console class
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class Console
{
    public:
        Console (long baudrate);
        ~Console(void);
        void Start();
        void Tick(void);
        int  Add(const char *name, void (*cmd)(const char*, int, char **), const char *synopsis);

    private:
        typedef struct
        {
            const char      *name;
            void           (*handler)(const char*, int, char **);
            const char      *synopsis;
        } cmd_t;

        typedef struct
        {
            char            line[COMMAND_BUFFER_SIZE];
            unsigned char   index;
        } line_t;

        // private variables
        cmd_t           m_commands[MAXCOMMANDS];
        char*           m_argv    [MAXARGUMENTS];
        line_t          m_command;
        bool            m_prompted;
        bool            m_started;
        long            m_baudrate;
        bool            m_espmon_on;

        // private functions
        void Process(void);
        void List(void);
};
