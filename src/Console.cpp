///////////////////////////////////////////////////////////////////////////////
// Console class
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////

#include "console.h"
#include "MyriadclockConfig.h"
#include "esp.h"

#define PROMPT ""
#define arraysize(x)          (int)(sizeof((x))/sizeof(*(x)))

using namespace std;

//
// @brief  Start the console
//
void Console::start(long baudrate)
{
    if (!m_started)
    {
        m_baudrate = baudrate;
        Serial.begin(m_baudrate);
        Serial.printf("\n");
        Serial.printf("-------------------------------------------------------------\n");
        Serial.printf("---                      MyriadClock                      ---\n");
        Serial.printf("-------------------------------------------------------------\n");
        Serial.printf("--- Copyright (c) 2020 - Jochem@myriadbits.com            ---\n");
        Serial.printf("--- Firmware version: %d                                   ---\n", FIRMWARE_VERSION);
        Serial.printf("-------------------------------------------------------------\n");
        m_prompted = false;
        m_started  = true;
    }
}

//
// Add a command to the command list
//
void Console::add(string name, ConsoleCallbacks *phandler, string synopsis)
{
    cmd_t cmd = { name, synopsis, phandler };
    m_commands.push_back(cmd);
}

//
// Check and handle any new characters
//
void Console::tick(void)
{
    char c;

    if (!m_prompted)
    {
        m_line.clear();
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
            if (m_line.size() > 0)
                m_line.pop_back();
            break;

        case '\r':  
            Serial.print('\n');
            process();
            break;

        case '\n':  
            Serial.print('\n');
            process();
            break;
            
        default:    
            m_line += c;
        }
    }
}

//
// Parse the command line into an argc/argv[] pair and try to execute the command in argv[0]
//
void Console::process()
{
    //
    //  Check on builtin "?"
    //
    if (m_line == "?")
    {
        list();
    }
    else if (m_line == "\n")
    {
        // Do nothing
    }
    else
    {
        // Parse the command line
        m_args.clear();

        string arg;
        for (auto c : m_line) 
        {
            if (c == ' ')
            {
                m_args.push_back(arg);                
                arg.clear();
            }
            else 
            {
                arg = arg + c;
            }           
        }
        m_args.push_back(arg);
      
        // Search for and execute the command handler
        if (m_args.size() > 0)
        {
            bool commandFound = false;
            for (auto &cmd : m_commands) // access by reference to avoid copying
            {  
                if (cmd.name == m_args[0])
                {
                    commandFound = true;
                    cmd.handler->commandHandler(cmd.name, m_args);
                    break;
                }
            }
            if (!commandFound)
            {
                Serial.print(F("Unknown command "));
                Serial.println(m_line.c_str());
            }
        }
    }
    m_prompted = false;
}

//
// Show all avaiable commands
//
void Console::list()
{
    Serial.println(F("Command list:"));
    for (auto &cmd : m_commands) // access by reference to avoid copying
    {       
        Serial.print("    ");
        Serial.print(cmd.name.c_str());
        for (int j = strlen(cmd.name.c_str()); j < 20; j++)
        {
            Serial.print(" ");
        }
        Serial.println(cmd.synopsis.c_str());
    }
}
