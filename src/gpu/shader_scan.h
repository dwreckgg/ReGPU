#pragma once
#include <chrono>
#include <cstdint>
#include <vector>

#include "regpu/regpu.h"
using ScannedShader = regpu::ScannedShader;

// Scans guest physical memory for shader objects not reported before; results queue up
// for ShaderScanTake. Synchronous, with `threads` workers.
void ShaderScanNow(unsigned threads);
// Starts a low-priority thread that rescans every 1.5 s.
void ShaderScanStart();
// For regpu::StopInterrupts: from here on no pass starts (ShaderScanNow returns at once, the periodic
// thread ends) and a pass in flight leaves within a few megabytes, reading nothing more.
void ShaderScanStop();
// After ShaderScanStop: waits until no pass is in flight, at most until deadline; whether none was.
bool ShaderScanWaitStopped(std::chrono::steady_clock::time_point deadline);
// Moves up to maxCount queued shaders into out; false when none were queued.
bool ShaderScanTake(std::vector<ScannedShader>& out, size_t maxCount);
// Shaders found so far.
uint64_t ShaderScanTotal();
