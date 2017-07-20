#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include "logger.hpp"

using namespace laji;

int main() {

    Logger::init("./");
    Logger::log("asdasdasdasd", 6);

}