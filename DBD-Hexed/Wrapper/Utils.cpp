#include "Utils.hpp"
#include <vector>
#include <Psapi.h>

std::string Utils::RandomString(const int len)
{
    static const char alphanum[] = "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) 
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

std::string Utils::time_to_string(const char* format, tm* time)
{
    std::vector<char> buf(100, '\0');
    buf.resize(std::strftime(buf.data(), buf.size(), format, time));
    return std::string(buf.begin(), buf.end());
}

bool Utils::Compare(byte* data, byte* sig, uint64_t size)
{
    for (auto i = 0ull; i < size; i++) { if (data[i] != sig[i] && sig[i] != 0x00) { return false; } }
    return true;
}

byte* Utils::FindSignature(byte* start, byte* end, byte* sig, uint64_t size)
{
    for (auto it = start; it < end - size; it++) { if (Compare(it, sig, size)) { return it; }; }
    return nullptr;
}

void* Utils::FindPointer(HMODULE mod, byte* sig, uint64_t size, int addition)
{
    MODULEINFO info;
    if (!K32GetModuleInformation(GetCurrentProcess(), mod, &info, sizeof(MODULEINFO))) { return nullptr; };
    auto base = static_cast<byte*>(info.lpBaseOfDll);
    auto address = FindSignature(base, base + info.SizeOfImage - 1, sig, size);
    if (!address) { return nullptr; }
    auto k = 0;
    for (; sig[k]; k++);
    auto offset = *reinterpret_cast<int*>(address + k);
    return address + k + 4 + offset + addition;
}