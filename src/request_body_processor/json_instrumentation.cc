#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "src/request_body_processor/json_instrumentation.h"

#include <chrono>

namespace modsecurity::RequestBodyProcessor {
namespace {

thread_local JsonInstrumentationMetrics g_metrics;

std::uint64_t elapsedNanos(
    std::chrono::steady_clock::time_point start_time) noexcept {
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now() - start_time).count());
}

}  // namespace

void jsonInstrumentationReset() noexcept {
    g_metrics = JsonInstrumentationMetrics{};
}

JsonInstrumentationMetrics jsonInstrumentationSnapshot() noexcept {
    return g_metrics;
}

std::string captureRequestBodySnapshot(const std::ostringstream &request_body) {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    const auto start_time = std::chrono::steady_clock::now();
    std::string snapshot = request_body.str();
    g_metrics.request_body_snapshot_count++;
    g_metrics.request_body_snapshot_bytes += snapshot.size();
    g_metrics.request_body_snapshot_ns += elapsedNanos(start_time);
    return snapshot;
#else
    return request_body.str();
#endif
}

void recordJsonProcessChunkAppend(std::size_t bytes,
    std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.json_process_chunk_calls++;
    g_metrics.json_process_chunk_appended_bytes += bytes;
    g_metrics.json_process_chunk_ns += elapsed_ns;
#else
    (void) bytes;
    (void) elapsed_ns;
#endif
}

void recordSimdjsonParserConstruction(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.simdjson_parser_constructions++;
    g_metrics.simdjson_parser_construction_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordSimdjsonPaddedCopy(std::size_t bytes,
    std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.simdjson_padded_copy_bytes += bytes;
    g_metrics.simdjson_padded_copy_ns += elapsed_ns;
#else
    (void) bytes;
    (void) elapsed_ns;
#endif
}

void recordSimdjsonIterate(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.simdjson_iterate_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordJsonconsCursorInit(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.jsoncons_cursor_constructions++;
    g_metrics.jsoncons_cursor_init_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordJsonconsTokenCursorInit(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.jsoncons_token_cursor_constructions++;
    g_metrics.jsoncons_token_cursor_init_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordJsonconsEventLoop(std::uint64_t elapsed_ns) noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.jsoncons_event_loop_ns += elapsed_ns;
#else
    (void) elapsed_ns;
#endif
}

void recordJsonconsTokenSyncStep() noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.jsoncons_token_sync_steps++;
#endif
}

void recordJsonconsTokenExactAdvanceStep() noexcept {
#ifdef MSC_JSON_AUDIT_INSTRUMENTATION
    g_metrics.jsoncons_token_exact_advance_steps++;
#endif
}

}  // namespace modsecurity::RequestBodyProcessor
