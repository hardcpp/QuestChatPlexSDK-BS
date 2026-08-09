# QBS+ Native Stability Change Recommendations

This document converts the findings in `AUDIT_REPORT.md` into an implementation plan and records the implementation status of the accompanying working-tree fixes.

## Implementation status

- Implemented: ChatPlex RPC/schema validation, exception containment, synchronized state/data, cancellable shutdown, CURL header snapshots, response-size overflow checks, and nonblocking `DeleteAsync()`.
- Implemented: atomic `MonoPtr` acquisition, null safety, self-assignment safety, initialized reference counts, and exception-safe monitor handling.
- Implemented: owned, synchronized dispatcher/coroutine queues with explicit shutdown behavior and no raw-array `memcpy`/`memset` relocation.
- Implemented: one-result image callbacks, actual background decoding, image dimension/buffer checks, WebP cleanup and bounds validation, animation input validation, and unused animation cache eviction.
- Implemented: raw-image bounds checks, pool initialization/synchronization, event lock safety, config serialization snapshots, and initialized JSON color channels.
- Deliberately deferred: changing the loading animation's asset, dimensions, or frame count. The author requested that its size remain unchanged until it can be evaluated separately.
- Still required outside this environment: a real Quest/Android build, on-device stress testing, and memory/tombstone telemetry. Local compilation was unavailable because CMake, Ninja, and the Android NDK are not installed.

## Priority 0: Reduce startup memory immediately

Status: deferred by author request. No loading-animation asset, dimension, or frame-count change is included in the source fixes.

### Replace the animated loading-logo pipeline

Recommended approach:

1. Remove runtime decoding of the 72-frame, 512 x 512 loading logo from `UISystem::Init()`.
2. Generate a small atlas during development and ship the atlas plus frame metadata.
3. Prefer 128 x 128 or 256 x 256 frames and reduce the frame count.
4. Load one atlas texture rather than creating a managed byte array and Unity texture for every frame.
5. Mark textures non-readable after upload when CPU access is no longer required.

If runtime WebP decoding must remain:

- Enforce a maximum width and height.
- Enforce a maximum frame count.
- Calculate `width * height * 4 * frameCount` using checked `size_t` arithmetic.
- Reject animations above a conservative decoded-memory budget.
- Decode directly into reusable native storage or the final atlas rather than retaining every full frame.
- Avoid `Texture2D::PackTextures` for untrusted or numerous frames.
- Release source frame arrays before creating the full set of Unity textures where possible.

Suggested initial limits for Quest testing:

- Maximum dimension: 512 pixels.
- Maximum frames: 32.
- Maximum decoded RGBA data: 32 MiB.
- Maximum cached animation memory: a measured global budget rather than an unlimited item count.

Acceptance criteria:

- QBS+ startup peak proportional set size should not increase by more than a measured, agreed budget.
- No single main-thread animation operation should exceed one frame budget under normal conditions.
- Opening and closing the QBS+ UI repeatedly should not grow retained texture memory.

## Priority 0: Make ChatPlex failures non-fatal

### Validate all RPC results and schemas

Before dereferencing a result:

- Check the `JsonRPCResult::Ptr` itself.
- Check `Result` and `Error` documents independently.
- Check document/object type before calling `HasMember()`.
- Check each member type before `GetBool()` or `GetString()`.
- Convert invalid responses into a normal error state.

`ThreadRunner()` should have a top-level exception boundary that logs and transitions to an error/disconnected state without allowing an exception to escape the thread function.

Do not depend on the `il2cpp_aware_thread` catch handler for recovery; that handler aborts on unhandled exceptions.

### Make shutdown cancellable and bounded

Recommended design:

- Replace `m_ThreadCondition` with `std::stop_token`, or at minimum `std::atomic_bool`.
- Use a condition variable instead of periodic sleeps.
- Add a CURL progress/cancellation callback that observes the stop request.
- Configure connect and total timeouts separately.
- Do not perform an unbounded main-thread `join()`.
- Make initialization and release idempotent and safe when partial initialization failed.

Acceptance criteria:

- Offline startup with a saved token does not crash.
- Empty, HTML, malformed, and schema-invalid responses produce a logged error and callback.
- Shutdown completes within a defined short deadline even during DNS failure or an active request.

## Priority 0: Repair `MonoPtr` ownership

### Make acquisition atomic with registration lookup

The wrapper count must be incremented while the holder lock still guarantees that the wrapper cannot be deleted.

Possible designs:

- Have `GetOrRegister()` return an already-acquired wrapper.
- Move `Grab()` inside the locked lookup/registration operation.
- Store wrappers in `std::shared_ptr` and return shared ownership rather than maintaining a separate counter.
- Prefer an established IL2CPP safe-pointer/GC-handle implementation if available.

### Fix null checking

`MonoPtr::Ptr()` should return or throw immediately when either `m_Wrapper` or `m_Wrapper->Ptr` is null. Only then should it inspect Unity cached-pointer state.

### Make assignment safe

- Add `if (this == &p_Other) return *this;` to copy assignment.
- Acquire the new wrapper before dropping the old wrapper, or use copy-and-swap.
- Make move assignment safe for self-move.

### Make monitor handling exception-safe

Wrap IL2CPP monitor acquisition in an RAII guard so every exit path releases it. Ensure managed `List` operations and map allocation failures cannot leave the monitor held.

Acceptance criteria:

- Null `MonoPtr` truth tests never crash.
- Concurrent copy/drop stress testing produces no UAF.
- Self-copy and self-move assignments are safe.
- Holder registration counts return to baseline after stress tests.

## Priority 1: Replace the custom dispatcher arrays

Recommended design:

- Use `std::deque<Action>` or `std::queue<Action>` protected by one mutex.
- Use `std::condition_variable` for the background worker.
- Move/swap a local batch out under the lock, then invoke callbacks without the lock.
- Represent run and destruction state under the same lock or with atomics.
- Reject enqueues after shutdown explicitly and report cancellation to the caller.
- Store actions and `MonoPtr` values as normal C++ objects; do not use `memcpy` or `memset` to relocate them.
- Use RAII locks everywhere.

If a bounded queue is required, overflow must be part of the API contract. Returning a boolean or error is preferable to silently dropping a completion callback.

Shutdown order should be explicit:

1. Stop accepting producers.
2. Cancel or finish outstanding producers.
3. Drain or cancel queued work.
4. Stop and join the worker.
5. Destroy Unity-side dispatch objects.
6. Free queue storage.

Acceptance criteria:

- ThreadSanitizer reports no races in a host-side queue stress test.
- Enqueue racing shutdown cannot access freed memory.
- Every accepted task runs exactly once or receives an explicit cancellation result.
- Queue saturation cannot leave a waiting UI operation unresolved.

## Priority 1: Make animation failure and bounds handling strict

Required corrections:

- Add `co_return` immediately after the null-animation callback.
- Assign `Height = p_Height`.
- Reject zero frames and empty UV arrays.
- Require delays, UVs, and frames to have matching counts.
- Verify the produced WebP frame index is below allocated vector sizes.
- Always call `WebPAnimDecoderDelete()` on every path after decoder creation.
- Use checked arithmetic for all byte counts and pixel offsets.
- Reject unsupported static WebP cleanly instead of throwing through the worker.
- Check every allocation and decoder result.

`GetMaxAtlasTextureSize()` should be rewritten with clear checked integer arithmetic. The current divisibility condition does not round the frame count as its comment describes.

Acceptance criteria:

- Truncated, static, zero-frame, oversized, and malformed WebP inputs return one failure callback.
- No invalid animation input reaches Unity texture creation.
- Every decoder object and temporary frame allocation is released on failure.

## Priority 1: Add an animation cache budget

Replace permanent caching with a bounded policy:

- Track estimated CPU and GPU bytes per animation.
- Maintain last-use time or an LRU list.
- Evict animations with zero active images when above budget.
- Destroy generated sprites and atlas textures on eviction.
- Remove evicted entries from both the map and vector.
- Avoid maintaining a separate count that can diverge from vector size.

Acceptance criteria:

- Loading many unique animations reaches a stable memory plateau.
- Evicted animations can be loaded again safely.
- Destroying the manager releases all atlas textures and sprites.

## Priority 1: Correct image callback and execution behavior

`CreateFromRawThreaded()` must invoke its callback exactly once.

Recommended structure:

1. Validate input.
2. Decode on a bounded worker pool.
3. On failure, schedule one failure callback on the documented callback thread.
4. On success, transfer decoded pixels to the main thread.
5. Create/upload the Unity texture.
6. Invoke one success or failure callback.

Additional requirements:

- Document whether callbacks run on the main thread.
- Do not decode arbitrary avatar/image data on the main thread.
- Enforce compressed input and decoded pixel limits.
- Avoid global STB state such as `stbi_set_flip_vertically_on_load` across concurrent workers; use the thread-local variant if supported or flip explicitly.
- Destroy partially created textures when subsequent operations fail.

Acceptance criteria:

- A callback-count test observes exactly one completion for success and failure.
- Large images are rejected before main-thread texture creation.
- Main-thread frame time remains bounded while processing avatars.

## Priority 2: Harden network clients

Recommended changes:

- Detach/store the `DeleteAsync()` thread consistently, or preferably route all core requests through a bounded worker pool.
- Limit concurrent requests.
- Copy headers under `m_HeadersLock`, then iterate the local copy.
- Reject a null CURL handle.
- Handle empty POST bodies without taking element zero.
- Check multiplication and addition before growing response buffers.
- Configure a maximum response size.
- Catch callback exceptions at the request boundary and return a controlled error.
- Make cancellation stop in-flight CURL transfers rather than only checking before and after `curl_easy_perform()`.
- Ensure CURL global initialization is performed once before concurrent use.

Acceptance criteria:

- Concurrent header mutation and requests pass stress testing.
- Oversized responses fail without excessive allocation.
- `DeleteAsync()` returns promptly.
- Request bursts do not create unbounded native threads.

## Priority 2: Harden raw image utilities

Required corrections:

- Change the null/empty input check in `TextureRaw::Load()` to reject null before dereferencing it.
- Validate positive width, height, target dimensions, and pixel vector pointers.
- Verify `pixelCount == width * height` with checked arithmetic.
- Require `0 <= radius < min(width, height)`, or clamp safely.
- Reject dimensions whose products exceed `size_t` or API integer limits.
- Validate resampling divisors before division.

Acceptance criteria:

- Null and empty inputs return false without crashing.
- Blur fuzz tests cannot index outside the supplied vector.
- Zero and extreme dimensions are rejected predictably.

## Priority 2: Initialize and synchronize pool state

Required corrections:

- Initialize `m_CountAll` to the number of successfully preallocated objects.
- Keep all count reads/writes under the pool mutex in the multithreaded implementation.
- Avoid calling user callbacks under the pool mutex, because callbacks can block, re-enter the pool, or throw.
- Use RAII and define how callback failures affect ownership.

Acceptance criteria:

- Active and inactive counts are correct immediately after construction.
- Concurrent get/release/clear testing has no races or deadlocks.

## Priority 2: Remove uninitialized JSON values

Initialize color channels before optional member reads, preferably from the existing output color or explicit defaults. Decide whether incomplete color objects are valid:

- If valid, preserve/default missing channels.
- If invalid, return an error or throw before modifying the output.

Apply the same rule to vectors, quaternions, and other composite values so partially specified documents cannot leave indeterminate state.

## Validation plan

### Runtime telemetry

For debug builds, record:

- Process RSS/PSS before QBS+ initialization, after WebP decode, after temporary textures, and after atlas creation.
- Managed GC heap size.
- Unity texture/native allocation estimates.
- Main-thread duration of texture creation and `PackTextures`.
- Queue depths and dropped/rejected work.
- Active native thread count.
- Network request and shutdown duration.

### Sanitizers

Use an ARM64 development build where practical with:

- AddressSanitizer or HWAddressSanitizer for UAF and out-of-bounds access.
- UndefinedBehaviorSanitizer for invalid arithmetic, null access, and type issues.
- A host-side ThreadSanitizer harness for `MonoPtr` holder logic, dispatch queues, pools, and other pure C++ components.

Do not ship sanitizer builds to users.

### Fault injection

Test at least:

- Offline startup with and without a saved ChatPlex token.
- DNS timeout and connection timeout.
- Empty, malformed, HTML, and schema-invalid RPC responses.
- Application shutdown during every request phase.
- Invalid, truncated, oversized, static, and high-frame-count WebP files.
- Large PNG/JPEG avatars and decompression-bomb-like dimensions.
- Queue saturation and enqueue-during-shutdown races.
- Repeated UI opening and loading of many unique animations.

### Crash evidence collection

For reported Quest failures, collect and correlate:

- Full `logcat` around the event.
- Tombstone/native backtrace.
- Android low-memory and LMKD messages.
- Watchdog or ANR/liveness text.
- QBS+ version and commit.
- Whether the user has a saved ChatPlex token.
- Whether the crash occurred at startup, UI open, image display, download, or shutdown.

This separation is important: an OOM/LMKD kill, main-thread watchdog, native SIGSEGV, and explicit `SAFE_ABORT()` may all be reported colloquially as a liveness crash but require different fixes.

## Suggested implementation sequence

1. Replace the startup loading animation or ship a prebuilt small atlas.
2. Fix ChatPlex null handling and bounded cancellation-aware shutdown.
3. Fix `MonoPtr` null safety and atomic reference acquisition.
4. Replace dispatcher queues and define shutdown semantics.
5. Fix animation null/bounds handling and callback behavior.
6. Add image and animation memory budgets plus cache eviction.
7. Harden network response limits and concurrency.
8. Fix raw image, pool, and JSON utility defects.
9. Run sanitizer, fault-injection, and long-session memory tests.
