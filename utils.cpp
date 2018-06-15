#include "utils.h"

utils::utils()
{

}

QString utils::toString(int input)
{
    char buffer[1000];
    itoa(input, buffer, 10);
    return QString(buffer);
}
