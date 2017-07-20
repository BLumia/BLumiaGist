#include <mutex>
#include <string>
#include "logger.hpp"

namespace laji {

    namespace Logger {

        void init(std::string path) {
            __logger_path = path;
            return;
        };

        // Thread safe function
        bool log(const char* buffer, __ssize_t buffersize) {
            std::lock_guard<std::mutex> guard(__logger_mutex);
            puts("asdasdasd");
            // do log
            return true;
        };

    }

}