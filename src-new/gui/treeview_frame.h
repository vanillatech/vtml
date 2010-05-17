#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Odin
{

class GUI;

HWND CreateTreeviewFrame(GUI& gui, HWND mainFrame);

} //Odin
