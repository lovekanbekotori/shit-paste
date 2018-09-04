#pragma once


// Includes
#include "Utilities.h"
#include <fstream>
#include <PsapI.h>
#include "Interfaces.h"



bool FileLog = false;
std::ofstream logFile;

// --------         U Core           ------------ //
// Opens a debug console
void  U::OpenConsole(std::string Title)
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	SetConsoleTitle(Title.c_str());
}

// Closes the debug console
void  U::CloseConsole()
{
	FreeConsole();
}

// Outputs text to the console
void  U::Log(const char *fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	//Output to console
	if (logBuf[0] != '\0')
	{
		SetConsoleColor(FOREGROUND_INTENSE_RED);
		printf("[%s]", GetTimeString().c_str());
		SetConsoleColor(FOREGROUND_WHITE);
		printf(": %s\n", logBuf);
	}

	if (FileLog)
	{
		logFile << logBuf << std::endl;
	}
}

// Gets the current time as a string
std::string  U::GetTimeString()
{
	//Time related variables
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];

	//Get current time
	time(&current_time);
	time_info = localtime(&current_time);

	//Get current time as string
	strftime(timeString, sizeof(timeString), "%I:%M%p", time_info);
	return timeString;
}

// Sets the console color for upcoming text
void  U::SetConsoleColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Enables writing all log calls to a file
void  U::EnableLogFile(std::string filename)
{
	logFile.open(filename.c_str());
	if (logFile.is_open())
		FileLog = true;
}


// --------         U Memory           ------------ //

DWORD U::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

uintptr_t U::FindSig(std::string moduleName, std::string pattern)
{
	const char* daPattern = pattern.c_str();
	uintptr_t firstMatch = 0;
	uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)moduleBase, &miModInfo, sizeof(MODULEINFO));
	uintptr_t moduleEnd = moduleBase + miModInfo.SizeOfImage;
	for (uintptr_t pCur = moduleBase; pCur < moduleEnd; pCur++)
	{
		if (!*daPattern)
			return firstMatch;

		if (*(PBYTE)daPattern == '\?' || *(BYTE*)pCur == getByte(daPattern))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!daPattern[2])
				return firstMatch;

			if (*(PWORD)daPattern == '\?\?' || *(PBYTE)daPattern != '\?')
				daPattern += 3;

			else
				daPattern += 2;
		}
		else
		{
			daPattern = pattern.c_str();
			firstMatch = 0;
		}
	}
	return 0;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}

uintptr_t U::FindPatternNew(const char* module, const char* pattern_string, const char* mask) {
	MODULEINFO module_info = {};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &module_info, sizeof MODULEINFO);

	uintptr_t module_start = uintptr_t(module_info.lpBaseOfDll);

	const uint8_t* pattern = reinterpret_cast<const uint8_t*>(pattern_string);

	for (size_t i = 0; i < module_info.SizeOfImage; i++)
		if (Compare(reinterpret_cast<uint8_t*>(module_start + i), pattern, mask))
			return module_start + i;

	return 0;
}
DWORD U::FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return DWORD(Address + c);
		}
	}
	return 0;
}

DWORD U::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	int len = strlen(string);
	char* szMask = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		szMask[i] = 'x';
	}
	szMask[len] = '\0';

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

uint64_t U::FindPatternIDA(const char* szModule, const char* szSignature)
{
	//CREDITS: learn_more
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),('A'),('F')) ? ((x&(~0x20)) - ('A') + 0xa) : (INRANGE(x,('0'),('9')) ? x - ('0') : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	DWORD firstMatch = 0;
	for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
		if (!*pat) return firstMatch;
		if (*(PBYTE)pat == ('\?') || *(BYTE*)pCur == getByte(pat)) {
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;
			if (*(PWORD)pat == ('\?\?') || *(PBYTE)pat != ('\?')) pat += 3;
			else pat += 2;    //one ?
		}
		else {
			pat = szSignature;
			firstMatch = 0;
		}
	}
	return NULL;
}

#pragma warning( disable : 4018 )  
#pragma warning( disable : 4348 )  

bool U::bin_match(uint8_t *code, std::vector< uint8_t > &pattern)
{
	for (int j = 0; j < pattern.size(); j++)
	{
		if (!pattern[j] && code[j] != pattern[j])
		{
			return false;
		}
	}

	return true;
}

template< typename T = uintptr_t > static T U::first_match(uintptr_t start, std::string sig, size_t len)
{
	std::istringstream iss(sig);
	std::vector< std::string > tokens{ std::istream_iterator< std::string >{ iss }, std::istream_iterator< std::string >{} };
	std::vector< uint8_t > sig_bytes;

	for (auto hex_byte : tokens)
	{
		sig_bytes.push_back(std::strtoul(hex_byte.c_str(), nullptr, 16));
	}

	if (sig_bytes.empty() || sig_bytes.size() < 2)
	{
		return T{};
	}

	for (size_t i{}; i < len; i++)
	{
		uint8_t *code_ptr = reinterpret_cast< uint8_t * >(start + i);

		if (code_ptr[0] != sig_bytes.at(0))
		{
			continue;
		}

		if (bin_match(code_ptr, sig_bytes))
		{
			return((T)(start + i));
		}
	}




	return T{};
}

template< typename T = uintptr_t > static T U::first_code_match(HMODULE start, std::string sig)
{
	auto ntoskrnl = reinterpret_cast< PIMAGE_DOS_HEADER >(start);

	if (ntoskrnl->e_magic != 0x5a4d)
	{
		return T{};
	}

	auto nt_hdrs = reinterpret_cast< PIMAGE_NT_HEADERS >(reinterpret_cast< uintptr_t >(ntoskrnl) + ntoskrnl->e_lfanew);

	return first_match< T >(reinterpret_cast< uintptr_t >(ntoskrnl) + nt_hdrs->OptionalHeader.BaseOfCode, sig, nt_hdrs->OptionalHeader.SizeOfCode);
}

std::uint8_t* U::pattern_scan(void* module, const char* signature)
{
    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dosHeader = (PIMAGE_DOS_HEADER)module;
    auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = pattern_to_byte(signature);
    auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i) {
        bool found = true;
        for (auto j = 0ul; j < s; ++j) {
            if (scanBytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found) {
            return &scanBytes[i];
        }
    }
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// thanks oneshotgh my nigga for the c+p shit
/// https://www.unknowncheats.me/forum/counterstrike-global-offensive/288763-improved-cvmt-hooking-class.html
/// not tested
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Okay this is a word from Oneshot. This code is not all mine but it wasnt functional until i changed some stuff, if
you wanna know what i changed then go and check the original code from exphck. This should be UD vmthooking
but it might not be that stable because .data area*/
auto table_is_hooked(void* table, const char* module_table_dst) -> const bool
{
	// Because of this probable check you can make a VEH Hook by forcing the pointer to 0
	if (table == nullptr) // Not initialized or invalid table to check
		return false;

	const auto module_base = GetModuleHandleA(module_table_dst);

	// Get module end
	const auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER > (module_base);
	const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS > (reinterpret_cast< std::uint8_t* >(module_base) + dos_header->e_lfanew);

	const auto module_end = reinterpret_cast< std::uintptr_t >(module_base) + nt_headers->OptionalHeader.SizeOfImage - sizeof(std::uintptr_t);

	const auto table_dst = *reinterpret_cast< std::uintptr_t* >(table);

	// Check the destination of the table, if the destination of the table is outside the region of the module, it is because it has been changed(hooked)
	return (table_dst < reinterpret_cast< std::uintptr_t >(module_base) || table_dst > module_end);
}
uintptr_t* vfunc_hook::search_free_data_page(const char* module_name, const std::size_t vmt_size) //Modified code from exphck https://www.unknowncheats.me/forum/2128832-post43.html
{
	auto check_data_section = [&](LPCVOID address, const std::size_t vmt_size)
	{
		const DWORD DataProtection = (PAGE_EXECUTE_READWRITE | PAGE_READWRITE);
		MEMORY_BASIC_INFORMATION mbi = { 0 };

		if (VirtualQuery(address, &mbi, sizeof(mbi)) == sizeof(mbi) && mbi.AllocationBase && mbi.BaseAddress &&
			mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_GUARD) && mbi.Protect != PAGE_NOACCESS)
		{
			if ((mbi.Protect & DataProtection) && mbi.RegionSize >= vmt_size)
			{
				return ((mbi.Protect & DataProtection) && mbi.RegionSize >= vmt_size) ? true : false;
			}
		}
		return false;
	};

	auto module_addr = GetModuleHandleA(module_name);

	if (module_addr == nullptr)
		return nullptr;

	const auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER > (module_addr);
	const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS > (reinterpret_cast< std::uint8_t* >(module_addr) + dos_header->e_lfanew);

	const auto module_end = reinterpret_cast< std::uintptr_t >(module_addr) + nt_headers->OptionalHeader.SizeOfImage - sizeof(std::uintptr_t);

	for (auto current_address = module_end; current_address > (DWORD)module_addr; current_address -= sizeof(std::uintptr_t))
	{
		if (*reinterpret_cast< std::uintptr_t* >(current_address) == 0 && check_data_section(reinterpret_cast< LPCVOID >(current_address), vmt_size))
		{
			bool is_good_vmt = true;
			auto page_count = 0u;

			for (; page_count < vmt_size && is_good_vmt; page_count += sizeof(std::uintptr_t))
			{
				if (*reinterpret_cast< std::uintptr_t* >(current_address + page_count) != 0)
					is_good_vmt = false;
			}

			if (is_good_vmt && page_count >= vmt_size)
				return (uintptr_t*)current_address;
		}
	}
	return nullptr;
}

vfunc_hook::vfunc_hook()
	: class_base(nullptr), vftbl_len(0), new_vftb1(nullptr), old_vftbl(nullptr)
{
}
vfunc_hook::vfunc_hook(void* base)
	: class_base(base), vftbl_len(0), new_vftb1(nullptr), old_vftbl(nullptr)
{
}
vfunc_hook::~vfunc_hook()
{
	unhook_all();
	if (wasAllocated)
		delete[] new_vftb1;
}

bool vfunc_hook::setup(void* base)
{
	std::string moduleName;

	if (base != nullptr)
		class_base = base;

	if (class_base == nullptr)
		return false;

	old_vftbl = *(std::uintptr_t**)class_base;
	vftbl_len = estimate_vftbl_length(old_vftbl) * sizeof(std::uintptr_t);

	if (vftbl_len == 0)
		return false;

	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (VirtualQuery(base, &mbi, sizeof(mbi))) // Grab the path from the alocationbase aka the module and then extract the module name from it.
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA((HMODULE)mbi.AllocationBase, buffer, MAX_PATH);
		std::string temp = buffer;
		const size_t last_slash_idx = temp.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
			temp.erase(0, last_slash_idx + 1), moduleName = temp;
	}
	if (moduleName.size())// If provided a module name then we will find a place in that module				
	{
		new_vftb1 = search_free_data_page(moduleName.c_str(), vftbl_len + sizeof(std::uintptr_t));
		std::memset(new_vftb1, NULL, vftbl_len + sizeof(std::uintptr_t));
		std::memcpy(&new_vftb1[1], old_vftbl, vftbl_len);
		new_vftb1[0] = old_vftbl[-1];
		try {
			auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };

			*(std::uintptr_t**)class_base = &new_vftb1[1];
			wasAllocated = false;
			if (table_is_hooked(base, moduleName.c_str()))
			{
				Beep(500, 500);
			}
		}
		catch (...) {
			delete[] new_vftb1;
			return false;
		}
	}
	else // If not then we do regular vmthookinh
	{
		new_vftb1 = new std::uintptr_t[vftbl_len + 1]();
		std::memcpy(&new_vftb1[1], old_vftbl, vftbl_len);
		try {
			auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
			new_vftb1[0] = old_vftbl[-1];
			*(std::uintptr_t**)class_base = &new_vftb1[1];
			wasAllocated = true;
		}
		catch (...) {
			delete[] new_vftb1;
			return false;
		}
	}


	return true;
}

std::size_t vfunc_hook::estimate_vftbl_length(std::uintptr_t* vftbl_start)
{
	MEMORY_BASIC_INFORMATION memInfo = { NULL };
	int m_nSize = -1;
	do {
		m_nSize++;
		VirtualQuery(reinterpret_cast<LPCVOID>(vftbl_start[m_nSize]), &memInfo, sizeof(memInfo));

	} while (memInfo.Protect == PAGE_EXECUTE_READ || memInfo.Protect == PAGE_EXECUTE_READWRITE);

	return m_nSize;
}


std::wstring U::StringToWstring(std::string str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	try
	{
		return converter.from_bytes(str);
	}
	catch (std::range_error)
	{
		std::wostringstream s;
		s << str.c_str();
		return s.str();
	}
}

long U::GetEpochTime()
{
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);

	return ms.count();
}

const char* U::PadStringRight(std::string text, size_t value)
{
	text.insert(text.length(), value - text.length(), ' ');

	return text.c_str();
}





























































































































































































































































































































































































































































































































































