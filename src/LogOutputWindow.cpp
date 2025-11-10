#include <windows.h>
#include <string>

// Local Includes
#include "../include/utils.h"

// This Includes
#include "../include/LogOutputWindow.h"

// Static Variables

// Prototypes

// Implementation

CLogOutputWindow::CLogOutputWindow()
{
}

CLogOutputWindow::~CLogOutputWindow()
{
}

/**
*
* This function is resposible for writing a message.
*
* @param _pcMessage is the message to be written.
* @return Returns void.
*
*/
void
CLogOutputWindow::Write(const Int8 *_pcMessage)
{
    WChar16 *pMessage = StringToWchar_t(_pcMessage);
    OutputDebugStringW(pMessage); // Sends wide output to debug output window.
    delete[] pMessage;
    pMessage = nullptr;
}
