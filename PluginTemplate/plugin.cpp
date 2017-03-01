#include "plugin.h"
#include <vector>

#define MAX_SIZE 300

using namespace std;

struct MyString {
	char *text;
	unsigned int code;
};

std::vector<MyString> protector;

static bool delString(int argc, char* argv[])
{
	for (std::vector<MyString>::iterator it = protector.begin(); it != protector.end(); ++it) {
		if (it->code == atoi(argv[1])) {
			_plugin_logprintf("[" PLUGIN_NAME "] %s is being remove\n", it->text);
			protector.erase(it);
			return true;
		}
	}
	return false;
}

static bool setString(int argc, char* argv[])
{
	/*
	@param index: number to identifie the string.
	@param text: the string to compare. 
	*/
	
	MyString custody;

	if (argc != 3) {
		_plugin_logputs("[" PLUGIN_NAME "] Help: setString index, \"string\"");
		return false;
	}
	
	custody.text = (char *)malloc(sizeof(char)*strlen(argv[2]));
	strcpy(custody.text, argv[2]);
	custody.code = atoi(argv[1]);

	protector.push_back(custody);
	
	_plugin_logprintf("[" PLUGIN_NAME "] %s was saved\n", protector.back().text);
	return true;
}

static duint ismystring(int argc, duint* argv, void* userdata)
{
	/*
	@param memory: memory address with a string.
	@param type: $ANSI or $UNICODE (this variables was setted for the plugin).
	@param index: identifier of the string setted (it's decimal, so.. U need to use a dot in the end of the number).
	
	@return: true if the memory address is a pointer to the string stored.
	*/
	
	char *argparse;
	duint step;
	duint va;

	if (!DbgIsDebugging()) {
		_plugin_logputs("[" PLUGIN_NAME "] You need to be debugging to use this expression functions\n");
		return false;
	} else if (protector.empty()) {
		_plugin_logprintf("[" PLUGIN_NAME "] You need to set a string with code %d (use setString)\n", argv[1]);
		return false;
	} else if (argv[1] < 1 && argv[1] > 2) {
		_plugin_logprintf("[" PLUGIN_NAME "] Second argument only supports 1 or 2 (or $ANSI and $UNICODE) to set the string type.\n");
		return false;
	}
	
	argparse = (char *)malloc(sizeof(char)*MAX_SIZE);
	va = argv[0];
	
	step = argv[1];
	for (int i = 0; i < MAX_SIZE; i++) {
		if (!DbgMemIsValidReadPtr(va+(i*step))) {
			_plugin_logprintf("[" PLUGIN_NAME "] Error reading [0x%x+0x%.2x*2]\n", va, i);
			return 0;
		}

		DbgMemRead(va+(i*step), argparse+i, sizeof(char));
		if (*(argparse+i) == '\x00')
			break;
	}
	
	for (std::vector<MyString>::iterator it = protector.begin(); it != protector.end(); ++it) {

		if (it->code == argv[2] && strcmp(it->text, argparse) == 0) {
			_plugin_logputs("[" PLUGIN_NAME "] Condition successful");
			return true;
		}
	}
	_plugin_logprintf("[" PLUGIN_NAME "] Index %d not found\n", argv[2]);
	return false;

}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
	if (!_plugin_registercommand(pluginHandle, "setString", setString, false))
		_plugin_logputs("[" PLUGIN_NAME "] Error registering the \"setString\" command!");

	if (!_plugin_registercommand(pluginHandle, "delString", delString, false))
		_plugin_logputs("[" PLUGIN_NAME "] Error registering the \"delString\" command!");
	
    if (!_plugin_registerexprfunction(pluginHandle, PLUGIN_NAME ".ismystring", 3, ismystring, nullptr))
		_plugin_logputs("[" PLUGIN_NAME "] Error register the \"" PLUGIN_NAME ".ismystring\" expression function!");
	
	DbgCmdExecDirect("$ANSI=1");
	DbgCmdExecDirect("$UNICODE=2");

    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here (clearing menus optional).
bool pluginStop()
{
    _plugin_unregistercommand(pluginHandle, "setString");
	_plugin_unregistercommand(pluginHandle, "delString");
	_plugin_unregisterexprfunction(pluginHandle, PLUGIN_NAME ".ismystring");
    return true;
}