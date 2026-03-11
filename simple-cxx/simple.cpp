#include "simple.h"
#include <iostream>
#include <memory>

simple::Simple::Simple()
{
}

void simple::Simple::hello_world() const
{
    std::cout << "hello world from c++" << std::endl;
}

std::unique_ptr<simple::Simple> simple::Simple_new()
{
    return std::make_unique<simple::Simple>();
}