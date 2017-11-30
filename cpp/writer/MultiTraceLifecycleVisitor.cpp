// Copyright 2004-present Facebook. All Rights Reserved.

#include <loom/writer/MultiTraceLifecycleVisitor.h>

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <memory>
#include <sstream>
#include <system_error>

#include <loom/writer/TraceLifecycleVisitor.h>
#include <loom/writer/PacketReassembler.h>

namespace facebook { namespace loom { namespace writer {

using namespace facebook::loom::entries;

MultiTraceLifecycleVisitor::MultiTraceLifecycleVisitor(
  const std::string& folder,
  const std::string& trace_prefix,
  std::shared_ptr<TraceCallbacks> callbacks,
  const std::vector<std::pair<std::string, std::string>>& headers,
  std::function<void(TraceLifecycleVisitor& visitor)> trace_backward_callback):

  folder_(folder),
  trace_prefix_(trace_prefix),
  callbacks_(callbacks),
  trace_headers_(headers),
  visitors_(),
  consumed_traces_(),
  trace_backward_callback_(trace_backward_callback),
  done_(false)
{}

void MultiTraceLifecycleVisitor::visit(const StandardEntry& entry) {
  switch (entry.type) {
    case entries::TRACE_BACKWARDS:
    case entries::TRACE_START: {
      int64_t trace_id = entry.extra;
      visitors_.emplace(
        trace_id,
        TraceLifecycleVisitor(
          folder_, trace_prefix_, callbacks_, trace_headers_, trace_id));

      visitors_.at(trace_id).visit(entry);
      consumed_traces_.insert(trace_id);

      if (entry.type == entries::TRACE_BACKWARDS) {
        trace_backward_callback_(visitors_.at(trace_id));
      }

      break;
    }
    case entries::TRACE_END:
    case entries::TRACE_ABORT:
    case entries::TRACE_TIMEOUT: {
      int64_t trace_id = entry.extra;
      auto visitor = visitors_.find(trace_id);
      if (visitor != visitors_.end()) {
        visitor->second.visit(entry);
        visitors_.erase(trace_id);
      }
      break;
    }
    default: {
      for (auto&& visitor_entry : visitors_) {
        visitor_entry.second.visit(entry);
      }
    }
  }

  if (visitors_.empty()) {
    done_ = true;
  }
}

void MultiTraceLifecycleVisitor::visit(const FramesEntry& entry) {
  for (auto&& visitor_entry : visitors_) {
    visitor_entry.second.visit(entry);
  }
}

void MultiTraceLifecycleVisitor::visit(const BytesEntry& entry) {
  for (auto&& visitor_entry : visitors_) {
    visitor_entry.second.visit(entry);
  }
}

void MultiTraceLifecycleVisitor::abort(AbortReason reason) {
  for (auto&& visitor_entry : visitors_) {
    visitor_entry.second.abort(reason);
  }
  visitors_.clear();
  done_ = true;
}

bool MultiTraceLifecycleVisitor::done() {
  return done_;
}

std::unordered_set<int64_t> MultiTraceLifecycleVisitor::getConsumedTraces() {
  return consumed_traces_;
}

} // namespace writer
} // namespace loom
} // namespace facebook