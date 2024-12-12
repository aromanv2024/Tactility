#ifdef ESP_TARGET

#include "QrUrl.h"

#include "kernel/PanicHandler.h"
#include "Log.h"

#include <sstream>
#include <esp_cpu_utils.h>

std::string getUrlFromCrashData() {
    auto* crash_data = getRtcCrashData();
    auto* stack_buffer = (uint32_t*) malloc(crash_data->callstackLength * 2 * sizeof(uint32_t));
    for (int i = 0; i < crash_data->callstackLength; ++i) {
        const CallstackFrame&frame = crash_data->callstack[i];
        uint32_t pc = esp_cpu_process_stack_pc(frame.pc);
        uint32_t sp = frame.sp;
        stack_buffer[i * 2] = pc;
        stack_buffer[(i * 2) + 1] = sp;
    }

    assert(sizeof(CallstackFrame) == 8);

    std::stringstream stream;

    stream << "https://oops.bytewelder.com?";
    stream << "i=1"; // Application id
    stream << "&v=" << TT_VERSION; // Version
    stream << "&a=" << CONFIG_IDF_TARGET; // Architecture
    stream << "&s="; // Stacktrace

    for (int i = crash_data->callstackLength - 1; i >= 0; --i) {
        uint32_t pc = stack_buffer[(i * 2)];
        uint32_t sp = stack_buffer[(i * 2) + 1];
        stream << std::hex << pc << std::hex << sp;
    }

    free(stack_buffer);

    return stream.str();
}

#endif
