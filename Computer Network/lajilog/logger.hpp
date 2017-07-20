#ifndef __LAJI_LOGGER
#define __LAJI_LOGGER

#include <mutex>
#include <string>

namespace laji {

    namespace Logger {

        static std::mutex __logger_mutex;
        static std::string __logger_path;

        void init(std::string path);
        bool log(const char* buffer, __ssize_t buffersize);
    };

}

#endif /* __LAJI_LOGGER */