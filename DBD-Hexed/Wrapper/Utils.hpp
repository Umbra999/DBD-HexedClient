#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

class Utils
{
private:
    static bool Compare(byte*, byte*, uint64_t);

public:
    static std::string RandomString(const int);
    static std::string time_to_string(const char*, tm*);
    static byte* FindSignature(byte*, byte*, byte*, uint64_t);
    static void* FindPointer(HMODULE, byte*, uint64_t, int);
};