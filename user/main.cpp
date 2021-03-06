#include "main.h"
#include "il2cpp-init.h"

using namespace app;

HMODULE hModule;
HANDLE hUnloadEvent;

void Run(LPVOID lpParam) {
	init_il2cpp();

	if (getGameVersion().compare("2020.12.9") != 0) {
		MessageBox(NULL, L"This Game Version is not supported!", L"AmongUsMenu", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		FreeLibraryAndExitThread((HMODULE)lpParam, 0);
		return;
	}

	hModule = (HMODULE)lpParam;
#if _DEBUG
	hUnloadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	new_console();

	#define DO_APP_CLASS(n, s) if(!n ## __TypeInfo) printf("Unable to locate %s\n", #n "__TypeInfo")
	#include "il2cpp-classes.h"
	#undef DO_APP_CLASS

	#define DO_APP_FUNC(r, n, p, s) if(!n) printf("Unable to locate %s\n", #n)
	#include "il2cpp-functions.h"
	#undef DO_APP_FUNC

	/*auto domain = il2cpp_domain_get();
	auto assembly = il2cpp_domain_assembly_open(domain, "Assembly-CSharp");
	auto klass = il2cpp_class_from_name(assembly->image, "", "ENHLBAECCDF");
	output_class_methods(klass);*/
#endif

	Game::pAmongUsClient = &(app::AmongUsClient__TypeInfo->static_fields->Instance);
	Game::pGameData = &(app::GameData__TypeInfo->static_fields->Instance);
	Game::pGameOptionsData = &(app::PlayerControl__TypeInfo->static_fields->GameOptions);
	Game::pAllPlayerControls = &(app::PlayerControl__TypeInfo->static_fields->AllPlayerControls);
	Game::pLocalPlayer = &(app::PlayerControl__TypeInfo->static_fields->LocalPlayer);
	Game::pShipStatus = &(app::ShipStatus__TypeInfo->static_fields->Instance);

	DetourInitilization();
#if _DEBUG
	DWORD dwWaitResult = WaitForSingleObject(hUnloadEvent, INFINITE);
	if (dwWaitResult != WAIT_OBJECT_0) {
		std::cout << "Failed to watch unload signal! dwWaitResult = " << dwWaitResult << " Error " << GetLastError() << std::endl;
		return;
	}
	
	DetourUninitialization();
	fclose(stdout);
	FreeConsole();
	CloseHandle(hUnloadEvent);
	FreeLibraryAndExitThread(hModule, 0);
#endif
}