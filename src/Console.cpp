///////////////////////////////////////////////////////////////////////////////
// Console class
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////

#include "console.h"
#include "MyriadclockConfig.h"

#define PROMPT "$ "
#define arraysize(x)          (int)(sizeof((x))/sizeof(*(x)))

//
// Constructor
//
Console::Console(long baudrate)
{
    _baudrate  = baudrate;
    _started   = false;
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
    if (!_started)
    {
        Serial.begin(_baudrate);
        Serial.println("");
        Serial.println(F("-------------------------------------------------------------"));
        Serial.println(F("---                      MyriadClock                      ---"));
        Serial.println(F("-------------------------------------------------------------"));
        Serial.println(F("            Copyright (c) 2020 - Jochem@myriadbits.com       "));
        Serial.print("Myriadclock firmware version: ");
        Serial.print(FIRMWARE_VERSION);
        Serial.println();
        memset(_commands, 0, sizeof(_commands));
        _prompted = false;
        _started  = true;
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
        if (_commands[i].handler == NULL)
        {
            _commands[i].name     = name;
            _commands[i].handler  = cmd;
            _commands[i].synopsis = synopsis;
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

    if (!_prompted)
    {
        memset(&_command, 0, sizeof(_command));
        while (Serial.available() > 0) Serial.read();       // Flush input
        Serial.print(PROMPT);
        _prompted = true;
    }
    if (Serial.available() > 0)
    {
        c = Serial.read();
        Serial.print(c);

        switch (c)
        {
            case 0x7f:
            case '\b':  
                if (_command.index > 0)
                {
                    _command.line[--_command.index] = '\0';
                }
                break;

            case '\r':  
                Serial.print('\n');
                _process();
                break;

            case '\n':  
                Serial.print('\n');
                _process();
                break;
                
            default:    
                if (_command.index >= COMMAND_BUFFER_SIZE - 1)
                {
                    Serial.println(F("\nline too long"));
                    Serial.print(PROMPT);
                    _prompted = false;
                }
                else
                {
                    _command.line[_command.index++] = c;
                    _command.line[_command.index  ] = '\0';
                }
        }
    }
}

//
// Parse the command line into an argc/argv[] pair and try to execute the command in argv[0]
//
void Console::_process(void)
{
    char *p1, *p2;
    int  i, argc;

    //
    //  Check on builtin "?"
    //
    if (!strcmp(_command.line, "?"))
    {
        _list();
    }
    else
    {
        // Parse the command line
        memset(_argv, 0, sizeof(_argv));
        for (argc = 0, p1 = _command.line; argc < MAXARGUMENTS;)
        {
            while (*p1 == ' ') p1++;                    // remove leading spaces
            if (*p1 == '\0') break;                     // check on EOL
            _argv[argc++] = p1;
            if ((p2 = strchr(p1, ' ')) == NULL) break;  // check on last argument
            *p2 = '\0';
            p1  = p2 + 1;
        }
       
        // Search for and execute the command handler
        for (i = 0; i < arraysize(_commands); i++)
        {
            if (_argv[0])
            {
                if (strcmp(_argv[0], _commands[i].name) == 0)
                {
                    _commands[i].handler(_commands[i].name, argc, _argv);
                    break;
                }
            }
        }
        if (i == sizeof(_commands) / sizeof(cmd_t))
        {
            Serial.print  (F("Unknown command "));
            Serial.println(_command.line);
        }
    }
    _prompted = false;
}

//
// Show all avaiable commands
//
void Console::_list(void)
{
    int i, j;

    Serial.println(F("Command list:"));
    for (i = 0;  i < MAXCOMMANDS; i++)
    {
        if (_commands[i].handler != NULL)
        {
            Serial.print("    ");
            Serial.print(_commands[i].name);
            for (j = strlen(_commands[i].name); j < 12; j++)
            {
                Serial.print(" ");
            }
            Serial.println(_commands[i].synopsis);
        }
    }
}
