#include "nozzle_lib.h"

int main()
{
    try
    {
        std::string file_name;
        std::cout << "Input data file name:\n> ";
        std::cin >> file_name;
        Nozzle nozzle(file_name);
        std::cout << "Result file name:\n> ";
        std::cin >> file_name;
        nozzle.output(file_name);
        return 0;
    }

    catch (property::Invalid)
    {
        err("ERROR: can't read fuel file!\n");//если неверно считались свойства газа
    }
    catch (Nozzle::Invalid)
    {
        err("ERROR: Can't calculate nozzle!\n");//если произошла ошибка при формировании свойств сопла
    }
    catch (...)
    {
        err("Unknown ERROR!\n");//если произошла ошибка при формировании свойств сопла
    }
    return -1;
}
