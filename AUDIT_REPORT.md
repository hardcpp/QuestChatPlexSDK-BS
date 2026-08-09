# QBS+ Native Stability Audit

Audit target: commit `aa318a4` (`dev` branch)

Audit type: static C++ review focused on liveness failures, use-after-free, memory corruption, uninitialized values, thread safety, bounds safety, and resource exhaustion.

Remediation status (working tree, 2026-08-09): QBS-002 through QBS-017 have corresponding source fixes. QBS-001 is intentionally unchanged at the author's request: the loading animation asset, dimensions, and frame count were not resized, replaced, or removed. Animation input validation and cache eviction were still fixed without changing that asset.

## Executive summary

The audit found multiple credible native crash and liveness hazards. The strongest explanation for reports increasing when QBS+ is installed is the animated loading logo decoded during normal startup. It creates a memory peak of at least approximately 160 MiB before Unity GPU copies, decoder buffers, delayed object destruction, and GC overhead are included.

Other high-risk findings include a concurrent use-after-free in the custom `MonoPtr` holder, null dereferences in common error paths, unsynchronized thread state, unsafe dispatcher teardown, and a ChatPlex network failure path capable of aborting the process.

The highest-priority areas identified in the audited revision were:

1. Startup animation memory use.
2. ChatPlex failure and shutdown handling.
3. `MonoPtr` lifetime and null safety.
4. Cross-thread dispatcher synchronization.

The source changes accompanying this report address items 2-4. The startup animation memory estimate remains an open, explicitly deferred risk pending device measurements and an author decision.

## Critical findings

### QBS-001: Startup loading animation can consume well over 160 MiB

Severity: Critical

Confidence: High

Relevant code:

- `src/CP_SDK/UI/UISystem.cpp:132-141`
- `src/CP_SDK/Animation/AnimationInfo.cpp:11-20`
- `src/CP_SDK/Animation/WEBP/WEBPDecoder.cpp:117-164`
- `src/CP_SDK/Animation/AnimationLoader.cpp:64-88`
- `assets/ChatPlexLogoLoading.webp`

QBS+ always starts decoding `ChatPlexLogoLoading.webp` during UI initialization. The asset is 512 x 512 with 72 animation frames.

The current pipeline:

- Allocates a managed RGBA array for every decoded frame: approximately 72 MiB.
- Copies every frame into those arrays.
- Creates 72 readable Unity `Texture2D` instances: at least another 72 MiB.
- Packs the textures into a 2048 x 2048 atlas: approximately another 16 MiB.
- Destroys temporary Unity textures only after packing. Unity destruction is delayed, and the managed arrays remain alive until the coroutine completes.

This gives a minimum transient footprint of roughly 160 MiB for one loading logo. GPU-side texture copies, WebP decoder buffers, temporary atlas packing data, managed-array overhead, and delayed GC can make the real peak substantially higher.

Likely effects include:

- Process OOM or low-memory termination.
- Long garbage-collection pauses.
- GPU allocation or upload stalls.
- A long main-thread `PackTextures` call.
- Android/Quest watchdog liveness failure.

The path runs during normal startup even if the user never opens the QBS+ menu.

### QBS-002: Network or JSON failure can crash the ChatPlex worker

Severity: Critical

Confidence: High

Relevant code:

- `src/CP_SDK/Network/JsonRPCClient.cpp:150-187`
- `src/CP_SDK/ChatPlexService.cpp:169-201`
- `src/CP_SDK/ChatPlexService.cpp:263-273`
- `extern/includes/beatsaber-hook/shared/utils/il2cpp-utils.hpp:563-592`

`JsonRPCClient::HandleWebResponse()` can return `nullptr` when a response is empty or contains invalid JSON. `ChatPlexService::IsRPCSuccess()` immediately dereferences the result without checking it.

Possible triggers include:

- No internet connection.
- DNS failure or timeout.
- Empty server response.
- HTML proxy/captive-portal response.
- Changed or malformed API response.
- Server outage.

The service is started at normal Unity initialization. Users with a saved ChatPlex token can enter the request path automatically.

Unhandled C++ exceptions in `il2cpp_aware_thread` lead to `SAFE_ABORT()`. A raw null dereference instead produces a native segmentation fault.

### QBS-003: `MonoPtr` reference acquisition has a concurrent use-after-free race

Severity: Critical

Confidence: High

Relevant code:

- `shared/CP_SDK/Utils/MonoPtr.hpp:15-34`
- `src/CP_SDK/Unity/MonoPtrHolder.cpp:48-58`
- `src/CP_SDK/Unity/MonoPtrHolder.cpp:63-93`
- `src/CP_SDK/Unity/MonoPtrHolder.cpp:94-116`

`MonoPtrHolder::GetOrRegister()` releases the holder lock before the `MonoPtr` constructor calls `Wrapper::Grab()`.

A possible interleaving is:

1. Thread A finds an existing wrapper whose reference count is one.
2. Thread A unlocks and returns the wrapper.
3. Thread B drops the final reference and deletes the wrapper.
4. Thread A calls `Grab()` on freed memory.

This is a genuine use-after-free affecting code that moves managed object references between network, decoder, dispatcher, and Unity threads.

### QBS-004: A null `MonoPtr` can dereference its null wrapper

Severity: Critical

Confidence: High

Relevant code:

- `shared/CP_SDK/Utils/MonoPtr.hpp:38-55`

`MonoPtr::Ptr()` calculates whether the wrapper is dead, but the Unity-object branch accesses `m_Wrapper->Ptr` before acting on that result. When `m_Wrapper` is null, normal operations such as `if (!p_Texture)` can crash inside `operator bool()`.

Null `MonoPtr` values are used throughout normal error handling, so this can turn recoverable failures into native crashes.

## High-severity findings

### QBS-005: Failed animation loading continues and dereferences null

Severity: High

Confidence: High

Relevant code:

- `src/CP_SDK/Animation/AnimationLoader.cpp:54-88`
- `src/CP_SDK/Animation/WEBP/WEBPDecoder.cpp:20-35`

When `Coroutine_ProcessLoadedAnimation()` receives a null animation, it invokes the failure callback but does not return. It immediately calls `GetMaxAtlasTextureSize()` with the null pointer.

Invalid, unsupported, truncated, or failed WebP input can reach this path.

Related bounds and correctness issues include:

- `AnimationInfo` assigns `Height = p_Width` instead of `p_Height` in `AnimationInfo.cpp:14`.
- `AnimationControllerInstance` reads `Frames[0]` without rejecting an empty frame list.
- Delay entries are indexed without verifying the delay count matches the UV/frame count.
- The WebP frame loop assumes decoder-reported frame count and produced frame count always agree.

### QBS-006: ChatPlex service state contains multiple data races

Severity: High

Confidence: High

Relevant code:

- `shared/CP_SDK/ChatPlexService.hpp:48-60`
- `src/CP_SDK/ChatPlexService.cpp:32-55`
- `src/CP_SDK/ChatPlexService.cpp:73-80`
- `src/CP_SDK/ChatPlexService.cpp:101-140`
- `src/CP_SDK/ChatPlexService.cpp:169-257`

The Unity thread and ChatPlex worker share `m_ThreadCondition`, `m_State`, strings, vectors, and configuration data without atomics or a shared mutex.

The build uses `-O3`. Unsynchronized access is undefined behavior, and the compiler is allowed to cache or reorder the stop flag and other state.

`Release()` also calls `join()` from the Unity/main thread. If the service is inside CURL timeout or retry processing, shutdown can block long enough for a watchdog failure. The stop condition does not cancel an in-flight CURL operation.

### QBS-007: Dispatcher queues have data races and teardown use-after-free hazards

Severity: High

Confidence: High

Relevant code:

- `src/CP_SDK/Unity/MTThreadInvoker.cpp`
- `src/CP_SDK/Unity/MTMainThreadInvoker.cpp`
- `src/CP_SDK/Unity/MTCoroutineStarter.cpp`

All three dispatcher implementations read `m_Queued` without holding the mutex while producer threads write it. `MTThreadInvoker` also reads `m_RunCondition` without synchronization.

Additional hazards:

- `Destroy()` frees queue arrays without excluding concurrent producers.
- An enqueue racing destruction can access freed queue memory.
- A `do...while` loop processes one entry even when the observed count is zero.
- Queue overflow silently drops work after logging an error.
- Dropped completion callbacks can leave callers permanently waiting.
- `MTCoroutineStarter` clears non-trivial `MonoPtr` objects with `memset`, bypassing reference-count cleanup on exceptional paths.
- Allocated `Queue` objects are not deleted.

### QBS-008: Static image decoding can report failure and success for one operation

Severity: High

Confidence: High

Relevant code:

- `src/CP_SDK/Unity/Texture2DU.cpp:40-107`
- `src/CP_SDK/Unity/SpriteU.cpp:60-75`
- `src/CP_SDK/Unity/EnhancedImage.cpp:97-105`

After a successful decode schedules Unity texture creation, `CreateFromRawThreaded()` falls through and calls `p_Callback(nullptr)`. Later, the queued operation invokes the callback again with the successful texture.

This can make callers run fallback, destruction, or failure-state logic before a delayed success callback uses the same state.

Despite the method name, STB decoding occurs synchronously on the calling thread. Avatar processing also invokes Unity image decoding on the main thread in `PlayerAvatarPicture.cpp:129-140`. Large or malformed images can therefore cause visible stalls or watchdog failures.

### QBS-009: Animation controllers retain resources indefinitely

Severity: High

Confidence: Medium

Relevant code:

- `src/CP_SDK/Animation/AnimationControllerManager.cpp:31-59`
- `shared/CP_SDK/Animation/AnimationControllerManager.hpp:31-33`

Every unique animation identifier is retained in both a map and vector. There is no eviction, unregister-by-ID, memory budget, or least-recently-used policy.

In workloads that display many animated chat images or emotes, every atlas and its generated frame sprites remain live for the rest of the session. This creates monotonic native/GPU memory growth.

## Medium-severity findings

### QBS-010: `WebClientCore::DeleteAsync()` is actually blocking

Severity: Medium

Confidence: High

Relevant code:

- `src/CP_SDK/Network/WebClientCore.cpp:397-411`
- `extern/includes/beatsaber-hook/shared/utils/il2cpp-utils.hpp:622-625`

`DeleteAsync()` creates a temporary `il2cpp_aware_thread` but does not detach it. The custom thread destructor joins when the temporary is destroyed, making the method synchronous. Calling it from the main thread can block for the complete network timeout.

The other asynchronous core operations create one detached native thread per request without concurrency limits, which can also cause thread and memory pressure during bursts.

### QBS-011: CURL header access races map mutation

Severity: Medium

Confidence: High

Relevant code:

- `src/CP_SDK/Network/WebClientCore.cpp:89-121`
- `src/CP_SDK/Network/WebClientCore.cpp:471-480`

Header getters and mutations use `m_HeadersLock`, but `DoRequest()` iterates `m_Headers` without the lock. Concurrent `SetHeader()` or `RemoveHeader()` can invalidate iterators and corrupt the map.

Other network bounds issues include:

- Taking `&content->Bytes->_values[0]` without handling an empty body.
- No overflow check for `size * nmemb` or accumulated response size.
- No configured maximum response body size.

### QBS-012: `TextureRaw::Load()` dereferences null input

Severity: Medium

Confidence: High

Relevant code:

- `src/CP_SDK/Unity/TextureRaw.cpp:23-45`

The input check uses:

```cpp
if (p_Bytes == nullptr && p_Bytes->get_Length() == 0)
```

When `p_Bytes` is null, the right side is evaluated because the left side is true. The condition should reject null independently.

### QBS-013: Gaussian blur can index outside the pixel vector

Severity: Medium

Confidence: High

Relevant code:

- `src/CP_SDK/Unity/TextureRaw.cpp:173-259`

The public blur entry point does not validate width, height, radius, pointer validity, or vector length. When the radius is at least the width or height, the horizontal and vertical passes index beyond the vector bounds.

Invalid dimensions or multiplication overflow can also cause the supplied pixel vector size to disagree with the indexing arithmetic.

### QBS-014: Object pool counters are uninitialized

Severity: Medium

Confidence: High

Relevant code:

- `shared/CP_SDK/Pool/ObjectPool.hpp:31-33`
- `shared/CP_SDK/Pool/ObjectPool.hpp:38-66`
- `shared/CP_SDK/Pool/MTObjectPool.hpp:33-35`
- `shared/CP_SDK/Pool/MTObjectPool.hpp:40-68`

`m_CountAll` is never initialized in either pool constructor. `CountActive()` therefore reads an indeterminate value. The default-capacity creation path also does not increment it.

The multithreaded pool exposes count functions without locking, and resets `m_CountAll` outside the mutex in `Clear()`.

### QBS-015: JSON color parsing uses uninitialized channel values

Severity: Medium

Confidence: High

Relevant code:

- `src/CP_SDK/Utils/Json.cpp:225-235`

`UnserializeColor32()` declares four uninitialized integers. Missing JSON channel members cause the individual unserializers to return without assigning them, after which the uninitialized values are copied into the output color.

### QBS-016: `MonoPtr` copy assignment is not self-assignment safe

Severity: Medium

Confidence: High

Relevant code:

- `shared/CP_SDK/Utils/MonoPtr.hpp:87-101`

Copy assignment drops the existing wrapper before reading and grabbing the source wrapper. For `x = x`, dropping the last reference can delete the wrapper, after which the assignment stores and grabs a dangling pointer.

### QBS-017: Manual monitor and mutex management is not exception-safe

Severity: Medium

Confidence: Medium

Relevant code:

- `src/CP_SDK/Unity/MonoPtrHolder.cpp:76-92`
- `src/CP_SDK/Unity/MonoPtrHolder.cpp:101-115`
- `shared/CP_SDK/Utils/Event.hpp:62-85`
- `src/CP_SDK/ChatPlexService.cpp:143-164`

Several locks are acquired and released manually. Allocation, container copies, managed calls, or callbacks can throw between acquisition and release, leaving a monitor or mutex permanently locked.

## Most likely relationship to reported liveness crashes

The likely contributors, in descending order, are:

1. The normal startup decode and atlas packing of the 72-frame, 512 x 512 loading animation.
2. Main-thread waiting in ChatPlex shutdown while a network operation is still active.
3. Main-thread image decode or texture packing of large images.
4. Monotonic animation atlas retention causing late-session memory pressure.
5. Queue data races, missed callbacks, or queue overflow leaving workflows stuck.
6. Saved-token startup networking encountering an invalid/offline response and crashing the worker.
7. `MonoPtr` concurrency bugs exposed by background image and network processing.

## Audit limitations

This was a static source audit. It did not execute the mod on Quest or collect tombstones, Android watchdog traces, memory telemetry, or sanitizer output. Some findings are deterministic source defects; others require particular timing, data, or memory pressure.

Correlating this report with tombstones and `logcat` would help distinguish OOM/watchdog failures from native segmentation faults or explicit aborts.
