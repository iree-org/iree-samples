// Copyright 2022 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_PJRT_PLUGIN_PJRT_COMMON_COMPILER_H_
#define IREE_PJRT_PLUGIN_PJRT_COMMON_COMPILER_H_

#include <memory>
#include <string>

#include "iree/compiler/API2/Embed.h"

namespace iree::pjrt {

class CompilerOutput {
 public:
  virtual ~CompilerOutput() = default;
  virtual void* GetData() = 0;
  virtual size_t GetDataSize() = 0;
};

// A single compilation job.
class CompilerJob {
 public:
  virtual ~CompilerJob() = default;

  // Sets a flag on the compiler job. This should only be done during shared
  // setup of a job (or if the underlying session will not be re-used).
  // Returns false on failure.
  virtual bool SetFlag(const char* flag) = 0;

  // Parses the source buffer. The buffer must remain valid for the life of
  // the job. Some implementations will parse it immediately, while other
  // backends may need to defer processing it.
  // Returns false if parsing failed (diagnostics will be output).
  virtual bool ParseSourceBuffer(const void* buffer, size_t length) = 0;

  // Performs compilation and code emission.
  virtual std::unique_ptr<CompilerOutput> CompileStandardPipeline() = 0;

  // If an operation failed, then an additional error message may be
  // available.
  virtual std::string GetErrorMessage() = 0;
};

// Wraps invocations of the compiler, either in-process or via other means.
class AbstractCompiler {
 public:
  virtual ~AbstractCompiler() = default;

  // Starts a new compiler job.
  virtual std::unique_ptr<CompilerJob> StartJob() = 0;
};

class InprocessCompiler : public AbstractCompiler {
 public:
  std::unique_ptr<CompilerJob> StartJob() override;
};

}  // namespace iree::pjrt

#endif  // IREE_PJRT_PLUGIN_PJRT_COMMON_COMPILER_H_