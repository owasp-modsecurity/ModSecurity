#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "src/request_body_processor/json_instrumentation.h"

#include <chrono>

namespace modsecurity::RequestBodyProcessor {
namespace {

JsonInstrumentationMetrics &instrumentationMetrics() {
    thread_local JsonInstrumentationMetrics metrics;
    return metrics;
}

std::uint64_t elapsedNanos(
    std::chrono::steady_clock::time_point start_time) noexcept {
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - start_time).count());
}

}  // namespace

void jsonInstrumentationReset() noexcept {
    instrumentationMetrics() = JsonInstrumentationMetrics{};
}

JsonInstrumentationMetrics jsonInstrumentationSnapshot() noexcept {
    return instrumentationMetrics();
}

std::string captureRequestBodySnapshot(const std::ostringstream &request_body) {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const auto start_time = std::chrono::steady_clock::now();
    std::string snapshot = request_body.str();
    instrumentationMetrics().request_body_snapshot_count++;
    instrumentationMetrics().request_body_snapshot_bytes += snapshot.size();
    instrumentationMetrics().request_body_snapshot_ns += elapsedNanos(start_time);
    return snapshot;
#else
    return request_body.str();
#endif
}

void recordJsonProcessChunkAppend(std::size_t bytes,
    std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().json_process_chunk_calls++;
    instrumentationMetrics().json_process_chunk_appended_bytes += bytes;
    instrumentationMetrics().json_process_chunk_ns += elapsed_ns;
#else
    (void) bytes;
    (void) elapsed_ns;
#endif
}

void recordSimdjsonParserConstruction(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().simdjson_parser_constructions++;
    instrumentationMetrics().simdjson_parser_construction_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordSimdjsonPaddedCopy(std::size_t bytes,
    std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().simdjson_padded_copy_bytes += bytes;
    instrumentationMetrics().simdjson_padded_copy_ns += elapsed_ns;
#else
    (void) bytes;
    (void) elapsed_ns;
#endif
}

void recordSimdjsonIterate(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().simdjson_iterate_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordJsonconsCursorInit(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().jsoncons_cursor_constructions++;
    instrumentationMetrics().jsoncons_cursor_init_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordJsonconsTokenCursorInit(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().jsoncons_token_cursor_constructions++;
    instrumentationMetrics().jsoncons_token_cursor_init_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordJsonconsEventLoop(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().jsoncons_event_loop_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordJsonconsTokenSyncStep() noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().jsoncons_token_sync_steps++;
#endif
}

void recordJsonconsTokenExactAdvanceStep() noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    instrumentationMetrics().jsoncons_token_exact_advance_steps++;
#endif
}

}  // namespace modsecurity::RequestBodyProcessor
