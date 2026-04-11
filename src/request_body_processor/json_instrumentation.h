#ifndef SRC_REQUEST_BODY_PROCESSOR_JSON_INSTRUMENTATION_H_
#define SRC_REQUEST_BODY_PROCESSOR_JSON_INSTRUMENTATION_H_

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>

namespace modsecurity {
namespace RequestBodyProcessor {

struct JsonInstrumentationMetrics {
    std::uint64_t request_body_snapshot_count{0};
    std::uint64_t request_body_snapshot_bytes{0};
    std::uint64_t request_body_snapshot_ns{0};

    std::uint64_t json_process_chunk_calls{0};
    std::uint64_t json_process_chunk_appended_bytes{0};
    std::uint64_t json_process_chunk_ns{0};

    std::uint64_t simdjson_parser_constructions{0};
    std::uint64_t simdjson_parser_construction_ns{0};
    std::uint64_t simdjson_padded_copy_bytes{0};
    std::uint64_t simdjson_padded_copy_ns{0};
    std::uint64_t simdjson_iterate_ns{0};

    std::uint64_t jsoncons_cursor_constructions{0};
    std::uint64_t jsoncons_cursor_init_ns{0};
    std::uint64_t jsoncons_token_cursor_constructions{0};
    std::uint64_t jsoncons_token_cursor_init_ns{0};
    std::uint64_t jsoncons_event_loop_ns{0};
    std::uint64_t jsoncons_token_sync_steps{0};
};

void jsonInstrumentationReset() noexcept;
JsonInstrumentationMetrics jsonInstrumentationSnapshot() noexcept;

std::string captureRequestBodySnapshot(const std::ostringstream &request_body);

void recordJsonProcessChunkAppend(std::size_t bytes, std::uint64_t elapsed_ns)
    noexcept;
void recordSimdjsonParserConstruction(std::uint64_t elapsed_ns) noexcept;
void recordSimdjsonPaddedCopy(std::size_t bytes, std::uint64_t elapsed_ns)
    noexcept;
void recordSimdjsonIterate(std::uint64_t elapsed_ns) noexcept;
void recordJsonconsCursorInit(std::uint64_t elapsed_ns) noexcept;
void recordJsonconsTokenCursorInit(std::uint64_t elapsed_ns) noexcept;
void recordJsonconsEventLoop(std::uint64_t elapsed_ns) noexcept;
void recordJsonconsTokenSyncStep() noexcept;

}  // namespace RequestBodyProcessor
}  // namespace modsecurity

#endif  // SRC_REQUEST_BODY_PROCESSOR_JSON_INSTRUMENTATION_H_
