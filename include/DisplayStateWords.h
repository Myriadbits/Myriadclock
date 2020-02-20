#ifndef   __DISPLAYSTATEWORDS_H__
#define   __DISPLAYSTATEWORDS_H__

#include "Esp.h"
#include "DisplayStateBase.h"

///////////////////////////////////////////////////////////////////////////////
// Base class for all display states
// Command line interpreter
///////////////////////////////////////////////////////////////////////////////
class DisplayStateWords : public DisplayStateBase
{
public:
    void Initialize(CRGB* pLEDs, Timezone* pTZ, MyriadclockSettings* pSettings);
    bool HandleLoop(unsigned long epochTime);  

    void CommandHandler(int argc, char *argv[]);

private:
    uint8_t       m_nWordIndexTop;
    uint8_t       m_nWordIndexBottom;
};

#endif //__DISPLAYSTATEWORDS_H__
