// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <fb/fbjni.h>

#include <loom/writer/TraceWriter.h>

namespace fbjni = facebook::jni;

namespace facebook {
namespace loom {
namespace writer {

struct JNativeTraceWriterCallbacks:
  public fbjni::JavaClass<JNativeTraceWriterCallbacks> {

  static constexpr const char* kJavaDescriptor =
    "Lcom/facebook/loom/writer/NativeTraceWriterCallbacks;";

  void onTraceStart(int64_t trace_id, int32_t flags, std::string file);
  void onTraceEnd(int64_t trace_id);
  void onTraceAbort(int64_t trace_id, AbortReason abortReason);
};

class NativeTraceWriter : public fbjni::HybridClass<NativeTraceWriter> {
 public:
  constexpr static auto kJavaDescriptor =
    "Lcom/facebook/loom/writer/NativeTraceWriter;";

  static fbjni::local_ref<jhybriddata> initHybrid(
    fbjni::alias_ref<jclass>,
    std::string trace_folder,
    std::string trace_prefix,
    int bufferSize,
    fbjni::alias_ref<JNativeTraceWriterCallbacks> callbacks
  );

  static void registerNatives();

  void loop();

  void submit(LoomBuffer::Cursor cursor, int64_t trace_id);

 private:
  friend HybridBase;

  NativeTraceWriter(
    std::string trace_folder,
    std::string trace_prefix,
    size_t bufferSize,
    fbjni::alias_ref<JNativeTraceWriterCallbacks> callbacks
  );

  std::shared_ptr<TraceCallbacks> callbacks_;
  writer::TraceWriter writer_;
};

} // namespace writer
} // namespace loom
} // namespace facebook