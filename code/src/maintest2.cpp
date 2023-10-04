#include <iostream>
#include <string>
#include <LuaCpp.hpp>

int main()
{
    LuaCpp::LuaContext lua;
    try
    {
        lua.CompileStringAndRun("print('Easy peasy');");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}