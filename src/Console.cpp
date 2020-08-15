///////////////////////////////////////////////////////////////////////////////
// Console class
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////

#include "console.h"
#include "MyriadclockConfig.h"
#include "esp.h"

#define PROMPT ""
#define arraysize(x)          (int)(sizeof((x))/sizeof(*(x)))

//
// Constructor
//
Console::Console(long baudrate)
{
    m_baudrate  = baudrate;
    m_started   = false;
    Start();
}

//
// Destructor
//
Console::~Console(void)
{
}

//
// @brief  Start the console
//
void Console::Start()
{
    if (!m_started)
    {
        Serial.begin(m_baudrate);
        Serial.printf("\n");
        Serial.printf("-------------------------------------------------------------\n");
        Serial.printf("---                      MyriadClock                      ---\n");
        Serial.printf("-------------------------------------------------------------\n");
        Serial.printf("--- Copyright (c) 2020 - Jochem@myriadbits.com            ---\n");
        Serial.printf("--- Firmware version: %d                                   ---\n", FIRMWARE_VERSION);
        Serial.printf("-------------------------------------------------------------\n");
        memset(m_commands, 0, sizeof(m_commands));
        m_prompted = false;
        m_started  = true;
    }
}

//
// Add a command to the command list
//
int Console::Add(const char *name, void (*cmd)(const char*, int, char **), const char *synopsis)
{
    int i;

    for (i = 0; i < MAXCOMMANDS; i++)
    {
        if (m_commands[i].handler == NULL)
        {
            m_commands[i].name     = name;
            m_commands[i].handler  = cmd;
            m_commands[i].synopsis = synopsis;
            break;
        }
    }
    if (i == MAXCOMMANDS)
    {
        Serial.print("Cannot add command ");
        Serial.println(name);
        return -1;
    }
    return 0;
}

//
// Check and handle any new characters
//
void Console::Tick(void)
{
    char c;

    if (!m_prompted)
    {
        memset(&m_command, 0, sizeof(m_command));
        while (Serial.available() > 0) Serial.read();       // Flush input
        Serial.print(PROMPT);
        m_prompted = true;
    }
    if (Serial.available() > 0)
    {
        c = Serial.read();
        Serial.print(c);

        switch (c)
        {
            case 0x7f:
            case '\b':  
                if (m_command.index > 0)
                {
                    m_command.line[--m_command.index] = '\0';
                }
                break;

            case '\r':  
                Serial.print('\n');
                Process();
                break;

            case '\n':  
                Serial.print('\n');
                Process();
                break;
                
            default:    
                if (m_command.index >= COMMAND_BUFFER_SIZE - 1)
                {
                    Serial.println(F("\nline too long"));
                    Serial.print(PROMPT);
                    m_prompted = false;
                }
                else
                {
                    m_command.line[m_command.index++] = c;
                    m_command.line[m_command.index  ] = '\0';
                }
        }
    }
}

//
// Parse the command line into an argc/argv[] pair and try to execute the command in argv[0]
//
void Console::Process(void)
{
    char *p1, *p2;
    int  i, argc;

    //
    //  Check on builtin "?"
    //
    if (!strcmp(m_command.line, "?"))
    {
        List();
    }
    else if (!strcmp(m_command.line, "\n"))
    {
        // Do nothing
    }
    else
    {
        // Parse the command line
        memset(m_argv, 0, sizeof(m_argv));
        for (argc = 0, p1 = m_command.line; argc < MAXARGUMENTS;)
        {
            while (*p1 == ' ') p1++;                    // remove leading spaces
            if (*p1 == '\0') break;                     // check on EOL
            m_argv[argc++] = p1;
            if ((p2 = strchr(p1, ' ')) == NULL) break;  // check on last argument
            *p2 = '\0';
            p1  = p2 + 1;
        }
       
        // Search for and execute the command handler
        for (i = 0; i < arraysize(m_commands); i++)
        {
            if (m_argv[0])
            {
                if (strcmp(m_argv[0], m_commands[i].name) == 0)
                {
                    m_commands[i].handler(m_commands[i].name, argc, m_argv);
                    break;
                }
            }
        }
        if (i == sizeof(m_commands) / sizeof(cmd_t))
        {
            Serial.print(F("Unknown command "));
            Serial.println(m_command.line);
        }
    }
    m_prompted = false;
}

//
// Show all avaiable commands
//
void Console::List(void)
{
    int i, j;

    Serial.println(F("Command list:"));
    for (i = 0;  i < MAXCOMMANDS; i++)
    {
        if (m_commands[i].handler != NULL)
        {
            Serial.print("    ");
            Serial.print(m_commands[i].name);
            for (j = strlen(m_commands[i].name); j < 12; j++)
            {
                Serial.print(" ");
            }
            Serial.println(m_commands[i].synopsis);
        }
    }
}
