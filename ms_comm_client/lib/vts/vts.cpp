#include "vts.h"

#ifdef VOPEN_VLOG
vts::vlog* vts::vlog::obj = nullptr;
vts::vlog::vlog()
{
    vec.push_back("\033[31m[Err");
    vec.push_back("\033[33m[War");
    vec.push_back("\033[32m[Deb");
    vec.push_back("[Inf");
}
#endif //OPEN_VLOG
