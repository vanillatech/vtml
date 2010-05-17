#include <stdexcept>
#include "odin.h"
#include "gui/gui.h"
#include "prozess10/model.h"

using namespace Odin;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	std::string msg;
	try {
		Configuration config;
		Model model(config);
		GUI gui(model);
		model.setCallbacks(gui);
		return gui.run();
	}
	catch (std::runtime_error& e) {
		msg = e.what();
	}
	catch (...) {
		msg = "Unknown exception.";
	}
	MessageBox(0, msg.c_str(), "Odin - Runtime error", MB_OK | MB_ICONERROR);
	return 1;
}
