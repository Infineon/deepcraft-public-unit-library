# .imunit Authoring Guide — C-Backed Units

## Overview

A `.imunit` file is an XML document that declares a signal-processing unit (node) for DEEPCRAFT Studio. It defines what the node looks like in the UI, what parameters it exposes, how inputs flow to outputs, what validation rules apply, and which C implementation fragments to call at runtime. The code-generation pipeline reads these declarations, evaluates expressions, selects the correct C fragment for the user's configuration, and emits optimised embedded C code.

This guide covers **Viper-architecture units backed by C code** — the most common pattern. Units live under `src/Imaginet.Units.*` (e.g. `Imaginet.Units.Math`, `Imaginet.Units.Signal`). A separate archetype — `<Compound>` — defines units as visual sub-graphs of other units with no C code; those are not covered here.

---

## 1. File & Folder Layout

Every unit is a self-contained folder:

```
src/Imaginet.Units.Signal/Filters/LowPassFilter/
├── LowPassFilter.imunit      ← unit declaration
├── lowpassfilter.h            ← C fragment(s)
├── LPF.png                    ← optional image for docs
└── lowpassfilter_test.h       ← optional test fragment
```

Rules:
- The `.imunit` filename does not have to match the unit `name`, but by convention it does.
- C fragment `.h` files sit next to the `.imunit` file in the same directory.
- Images referenced in `<Description>` are relative to the unit folder.

---

## 2. Quick Start — Your First Unit

Based on the real Threshold unit — element-wise thresholding with one input, one output, one option, and one C fragment:

`Threshold.imunit`:

```xml
<?xml version="1.0" encoding="utf-8" ?>
<Imaginet>
  <Unit name="Imaginet.Units.Filter.Threshold">
    <DisplayName>Threshold</DisplayName>
    <DisplayPath>/Machine Learning/Postprocessing</DisplayPath>
    <Description>
      <Header>Description</Header>
      Set values below a threshold to zero while preserving values above.
      <Header>Usage</Header>
      Use after ML inference to suppress weak confidence scores.
    </Description>

    <Parameters>
      <InputSocket name="input" />
      <DoubleOption name="threshold" text="Threshold" default="0.5" />
      <Expression name="count" value="input.shape.flat" />
      <OutputSocket name="output" type="input.type" shape="input.shape" />
    </Parameters>

    <Contracts>
      <Assert test="threshold >= 0" error="Threshold must be in [0, 1]." />
      <Assert test="threshold &lt;= 1" error="Threshold must be in [0, 1]." />
    </Contracts>

    <Implementations>
      <Implementation language="C" fragment="threshold.h:threshold_f32"
                      call="threshold_f32(input, output, count, threshold)">
        <Conditional value="input.type == System.Float32" />
      </Implementation>
    </Implementations>
  </Unit>
</Imaginet>
```

`threshold.h`:

```c
#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "threshold_f32"
static inline void threshold_f32(const float* restrict input,
                                 float* restrict output,
                                 int count, float threshold)
{
    for (int i = 0; i < count; ++i) {
        const float v = input[i];
        output[i] = (v >= threshold) ? v : 0.0f;
    }
}
#pragma IMAGINET_FRAGMENT_END
```

**What to notice:**
- `count` is derived from `input.shape.flat` — total elements regardless of tensor shape.
- `<Conditional>` selects this fragment for Float32 only. Add more `<Implementation>` elements for other types.
- The `call` attribute maps `.imunit` parameter names to C arguments positionally.
- XML escaping: `<=` → `&lt;=` (required because `<` has special meaning in XML).

---

## 3. XML Document Structure

### 3.1 Root Element

```xml
<?xml version="1.0" encoding="utf-8" ?>
<Imaginet version="2.0.0.0">
  <Unit name="Fully.Qualified.UnitId">
    ...
  </Unit>
</Imaginet>
```

Use `<Unit>` as the child of `<Imaginet>`. The legacy name `<Function>` is still accepted but deprecated.

The `name` attribute is the globally unique unit identifier used by the linker and code generator.

### 3.2 Top-Level Sections

Inside `<Unit>`, the following sections are recognised (all optional except `<Parameters>`):

| Section | Purpose |
|---|---|
| `<DisplayName>` | Human-readable name |
| `<DisplayPath>` | Library category (e.g. `/Math/Element Wise`) |
| `<Description>` | Rich-text documentation |
| `<Tags>` | Behavioural flags |
| `<Parameters>` | Inputs, outputs, options, expressions |
| `<Contracts>` | Validation assertions |
| `<Implementations>` | Main processing body (C fragments) |
| `<Init>` | One-time initialisation |
| `<Destructor>` | Cleanup on teardown |
| `<SoftReset>` | Reset persistent state without full re-init |
| `<Enqueue>` | Streaming body — push data into buffer |
| `<Dequeue>` | Streaming producer — pull/read data out |
| `<CanEnqueue>` | Guard: can buffer accept more data? |
| `<CanDequeue>` | Guard: is output data ready? |

---

## 4. Description (Documentation)

The `<Description>` element supports rich content:

```xml
<Description>
    <Header>Description</Header>
    Compute the sine of each element. Input values are in radians.

    <Header>Usage</Header>
    Use the Sin unit for oscillation or coordinate transforms.

    <Image file="images/sin_plot.png" width="400" />

    <Header>Python Reference</Header>
    <Inline fragment="sin.py:sin" language="Python" />
</Description>
```

Supported child elements:

| Element | Attributes | Purpose |
|---|---|---|
| Plain text | — | Paragraph text |
| `<Header>` | — | Section heading |
| `<LineBreak/>` | — | Explicit line break |
| `<Text>` | `bold`, `italic`, `underline` | Styled text span |
| `<Image>` | `file` (required), `width`, `height` | Image (path relative to unit folder) |
| `<Code>` | `language` | Inline code block |
| `<Inline>` | `language`, `fragment` | Rendered source from a C/Python fragment |

---

## 5. Tags

Tags are empty self-closing elements inside `<Tags>`:

```xml
<Tags>
    <HideInLibrary/>
    <ExcludeFromApiCodeGen/>
</Tags>
```

| Tag | Purpose |
|---|---|
| `<HideInLibrary/>` | Don't show in the unit library panel (used for internal/imported layers) |
| `<ExcludeFromApiCodeGen/>` | Exclude from the generated C API header |
| `<RecordingDevice/>` | Mark as a capture device (camera, microphone) |
| `<Experimental/>` | Mark as experimental |
| `<ControlPipe/>` | Special codegen: outer control loop for DataNode sync |
| `<DatasetGen/>` | Dataset generation support |
| `<DebugBuild/>` | Only include in debug builds |
| `<ContextPushData/>` | Context push data behaviour |
| `<Track/>` | Track behaviour |

---

## 6. Parameters

All parameters live inside `<Parameters>`. Every parameter requires a `name` attribute. The `name` is used to reference the parameter in expressions, implementation calls, and conditionals.

### 6.1 InputSocket — Tensor Input

```xml
<InputSocket name="input" text="Input" pipe="data"
             description="Input tensor. Supports float32."
             optional="false" timestamps="false" />
```

| Attribute | Required | Description |
|---|---|---|
| `name` | Yes | Parameter name used in expressions |
| `text` | No | Display name (defaults to capitalised `name`) |
| `description` | No | Tooltip text |
| `pipe` | No | Pipe name for streaming (e.g. `"data"`) |
| `optional` | No | If `"true"`, input may be unconnected (evaluates to `null`) |
| `timestamps` | No | If `"true"`, this is a timestamp socket |

Once declared, the input exposes properties in expressions:
- `input.type` — data type (e.g. `System.Float32`)
- `input.shape` — shape object
- `input.shape.flat` — total element count
- `input.shape.count` — number of dimensions
- `input.shape.size(n)` — size of dimension `n` (0 = rightmost)
- `input.rate` — sample rate (frequency)
- `input.shift` — Q-format shift
- `input.scale` — D-format scale
- `input.offset` — D-format offset

### 6.2 OutputSocket — Tensor Output

```xml
<OutputSocket name="output" type="input.type" shape="input.shape"
              text="Output" pipe="data"
              description="Output tensor." />
```

| Attribute | Required | Description |
|---|---|---|
| `name` | Yes | Parameter name |
| `type` | Yes | Type expression (e.g. `"input.type"`, `"System.Float32"`) |
| `shape` | Yes | Shape expression (e.g. `"input.shape"`, `"System.Shape(10, 3)"`) |
| `text` | No | Display name |
| `description` | No | Tooltip |
| `pipe` | No | Pipe name |
| `rate` | No | Output frequency expression |
| `shift` | No | Q-format shift expression |
| `scale` | No | D-format scale expression |
| `offset` | No | D-format offset expression |
| `conditional` | No | Expression; socket only exists when true |
| `timestamps` | No | If `"true"`, this is a timestamp output |
| `rateIsApprox` | No | If `"true"`, rate is approximate |

### 6.3 Expression — Computed Value

Expressions define derived values that can be used in other expressions, implementation calls, and contracts.

```xml
<!-- Attribute form (single-line) -->
<Expression name="count" value="input.shape.flat" />

<!-- Element body form (multi-line) -->
<Expression name="model">
    System.Load("Imaginet.Units.TensorFlowLite",
                "Imaginet.Units.TensorFlowLite.TfLiteModel.TfLiteModelReader")
        .LoadTfLiteModelCached(file_path)
</Expression>
```

| Attribute | Required | Description |
|---|---|---|
| `name` | Yes | Variable name |
| `value` | No | Expression (alternative: use element body text) |
| `conditional` | No | Expression; only evaluate when true |
| `description` | No | Documentation |

### 6.4 Int32Option — Integer Parameter

```xml
<Int32Option name="mode" text="Mode" default="0" min="0" max="3"
             description="Rounding mode." ui="textbox">
    <OneOf>
        <Item text="Round">0</Item>
        <Item text="Floor">1</Item>
        <Item text="Ceil">2</Item>
        <Item text="Truncate">3</Item>
    </OneOf>
</Int32Option>
```

| Attribute | Required | Default | Description |
|---|---|---|---|
| `name` | Yes | — | Parameter name |
| `default` | No | `0` | Default value |
| `min` | No | `int.MinValue` | Minimum allowed |
| `max` | No | `int.MaxValue` | Maximum allowed |
| `text` | No | Capitalised name | Display name |
| `description` | No | — | Tooltip |
| `json` | No | — | JSON path for model import (e.g. `"config.filters"`) |
| `global` | No | `false` | If `"true"`, shared across all nodes of this type |
| `ui` | No | — | UI hint (e.g. `"textbox"`) |

The `<OneOf>` child creates a dropdown. Each `<Item>` has a `text` attribute (display label) and element value (the actual value).

### 6.5 DoubleOption — Float Parameter

Same attribute pattern as `Int32Option`, with `double` defaults/min/max:

```xml
<DoubleOption name="threshold" text="Threshold" default="0.7"
              min="0.0" max="1.0" description="Confidence threshold." />
```

### 6.6 BoolOption — Boolean Parameter

```xml
<BoolOption name="to_dB" text="Convert to dB" default="true"
            description="Convert to decibels." />
```

Legacy alias: `<BooleanOption>` still works but prefer `<BoolOption>`.

### 6.7 StringOption — String Parameter

```xml
<StringOption name="padding" text="Padding" default="same"
              json="config.padding">
    <OneOf>
        <Item text="Valid">valid</Item>
        <Item text="Causal">causal</Item>
        <Item text="Same">same</Item>
    </OneOf>
</StringOption>
```

When `<OneOf>` has no `<Item>` children (empty `<OneOf/>`), the dropdown is populated dynamically at runtime.

### 6.8 Handle — Persistent State Buffer

A `Handle` allocates a block of bytes that persists across calls (survives between `body` invocations). Used for IIR filters, trackers, rotation matrices, etc.

```xml
<Handle name="state" text="Filter State" size="input.shape.flat * 4"
        description="IIR filter state buffer." />
```

| Attribute | Required | Description |
|---|---|---|
| `name` | Yes | Parameter name |
| `size` | Yes | Size expression in bytes |
| `text` | No | Display name |
| `description` | No | Tooltip |

In C code, the Handle is received as `int8_t*` (or `void*`) and must be cast:

```c
static inline void my_filter(const float* input, int8_t* state_bytes,
                             float* output, int count)
{
    float* state = (float*)state_bytes;
    // ... use state[i] ...
}
```

### 6.9 Stream — IPC Data Pipe

Used by capture devices for streaming data:

```xml
<Stream name="data_pipe" mode="read" />
```

`mode` is either `"read"` or `"write"`.

### 6.10 Other Parameter Types

| Element | Key Attributes | Purpose |
|---|---|---|
| `<FileOption>` | `name`, `default` | File path picker (e.g. model files) |
| `<DirectoryOption>` | `name` | Directory picker |
| `<ShapeOption>` | `name`, `default` (e.g. `"[128,3]"`) | User-configurable tensor shape |

---

## 7. Contracts (Validation)

Contracts validate parameter values and tensor properties at graph-evaluation time. If a contract fails, the error message is shown to the user.

```xml
<Contracts>
    <Assert test="input.type == System.Float32 || input.type == System.Int8"
            error="Input ({input.type}) must be Float32 or Int8" />
    <Assert test="threshold >= 0 &amp;&amp; threshold &lt;= 1"
            error="Threshold must be in [0, 1]" />
    <Assert test="axis &lt; input.shape.count"
            error="Axis must be less than number of dimensions." />
</Contracts>
```

Rules:
- `test` is a boolean expression. If it evaluates to `false`, the error fires.
- `error` supports `{expression}` interpolation for dynamic messages.
- XML requires escaping: `&&` → `&amp;&amp;`, `<` → `&lt;`, `>` → `&gt;`, `"` in expressions → `&quot;`.

---

## 8. Implementations (C Fragments)

### 8.1 The `<Implementations>` Section

This is the main processing body. It contains one or more `<Implementation>` elements. The code generator selects the first implementation whose conditionals all evaluate to `true`.

```xml
<Implementations>
    <Implementation language="C" fragment="abs.h:abs_f32"
                    call="abs_f32(input, count, output)">
        <Conditional value="input.type == System.Float32" />
    </Implementation>
    <Implementation language="C" fragment="abs.h:abs_i8"
                    call="abs_i8(input, count, output)">
        <Conditional value="input.type == System.Int8" />
    </Implementation>
</Implementations>
```

#### `<Implementation>` Attributes

| Attribute | Required | Description |
|---|---|---|
| `language` | Yes | `"C"` or `"Python"` |
| `fragment` | Yes | Fragment reference: `"filename.h:fragment_name"` |
| `call` | Yes | Function call mapping parameter names to C arguments |

#### `<Conditional>` Children

Each `<Conditional>` contains a boolean expression. Multiple conditionals are AND-ed together. The expression can be specified in two ways:

```xml
<!-- Attribute form (preferred) -->
<Conditional value="input.type == System.Float32" />

<!-- Element-body form (useful when the expression contains characters awkward to escape in attributes) -->
<Conditional>i2 == null &amp;&amp; i3 == null</Conditional>
```

Multiple conditionals example:

```xml
<Implementation language="C" fragment="divide.h:divide_f32"
                call="divide_f32(input, count, divisor, output)">
    <Conditional value="input.type == System.Float32" />
    <Conditional value="mode == 0" />
</Implementation>
```

This is equivalent to `input.type == System.Float32 && mode == 0`.

#### The `call` Attribute

The `call` string maps `.imunit` parameter names to C function arguments **positionally**. Each name in the call must match a declared parameter or expression `name`:

```xml
<!-- Parameters declared: input, count, output -->
<Implementation ... call="abs_f32(input, count, output)" />
```

The code generator passes the resolved values in the listed order to the generated C function call.

#### The `fragment` Attribute

Format: `"relative/path/file.h:fragment_name"`

The path is relative to the unit folder. For cross-unit references:
`"[Imaginet.Units.Other]/path/file.h:fragment_name"`

### 8.2 Writing C Fragment Files

Fragment files use pragma directives to delineate reusable code blocks:

```c
#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "my_function_f32"

static inline void my_function_f32(const float* restrict input,
                                   float* restrict output, int count)
{
    for (int i = 0; i < count; i++) {
        output[i] = input[i] * 2.0f;
    }
}

#pragma IMAGINET_FRAGMENT_END
```

#### Pragma Directives — Summary

| Directive | Description |
|---|---|
| `IMAGINET_INCLUDES_BEGIN` / `IMAGINET_INCLUDES_END` | Wraps `#include` statements. Extracted and placed at the top of the generated file. |
| `IMAGINET_FRAGMENT_BEGIN "name"` / `IMAGINET_FRAGMENT_END` | Wraps a named code fragment (placed in generated `.c`). |
| `IMAGINET_FRAGMENT_BEGIN_HEADER "name"` / `IMAGINET_FRAGMENT_END_HEADER` | Wraps a named code fragment placed in the generated `.h` header file. |
| `IMAGINET_FRAGMENT_DEPENDENCY "ref"` | Declares a dependency on another fragment. |
| `IMAGINET_CODEPACKAGE_DEPENDENCY "pkg"` | Declares a dependency on an external code package (e.g. `cmsis-dsp`, `tflite-micro`). |
| `IMAGINET_SYMBOL_CHECK SYMBOL scope "msg"` | Compile-time check that a preprocessor symbol is defined. |
| `IMAGINET_FRAGMENT_TEST "ref"` | Associates a test fragment with a code fragment. |

Each directive is explained in detail below.

#### Includes Block

The `IMAGINET_INCLUDES_BEGIN`/`END` block wraps `#include` directives. The code generator extracts these and places them at the top of the generated file, deduplicated across all fragments:

```c
#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#include <string.h>
#pragma IMAGINET_INCLUDES_END
```

The includes block must be placed **outside** any fragment. Includes inside fragments are inherited by that fragment only.

#### Fragment Blocks

`IMAGINET_FRAGMENT_BEGIN "name"` / `IMAGINET_FRAGMENT_END` wraps a named code block. When the code generator needs this fragment (because an `.imunit` references it), it emits the code between the pragmas into the generated `.c` file:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "abs_f32"
static inline void abs_f32(const float* restrict x, int count,
                           float* restrict output)
{
    for (int i = 0; i < count; i++)
        output[i] = fabs(x[i]);
}
#pragma IMAGINET_FRAGMENT_END
```

Use `IMAGINET_FRAGMENT_BEGIN_HEADER` / `IMAGINET_FRAGMENT_END_HEADER` when the fragment must appear in the generated `.h` header file instead (e.g. for type definitions, macros, or configuration includes needed before the `.c` body):

```c
#pragma IMAGINET_FRAGMENT_BEGIN_HEADER "mtb_model_header"
#include "{{PROJECT_PREFIX_UPPERCASE}}_MTB_ML_MODEL_Object_Detection_config.h"
#pragma IMAGINET_FRAGMENT_END_HEADER
```

#### Multiple Fragments in One File

A single `.h` file can contain multiple fragments. Each is independently selectable by the `.imunit`. Only fragments that are actually referenced (directly or via dependencies) are included in the generated output:

```c
#pragma IMAGINET_INCLUDES_BEGIN
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs_f32"
static inline void abs_f32(const float* restrict x, int count,
                           float* restrict output) {
    for (int i = 0; i < count; i++)
        output[i] = fabs(x[i]);
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs_i8"
static inline void abs_i8(const int8_t* restrict x, int count,
                          int8_t* restrict output) {
    for (int i = 0; i < count; i++)
        output[i] = abs(x[i]);
}
#pragma IMAGINET_FRAGMENT_END
```

#### Fragment Dependencies (`IMAGINET_FRAGMENT_DEPENDENCY`)

When one fragment uses code from another fragment, declare the dependency so the code generator includes both in the correct order. Place the directive **inside** the dependent fragment, before the code.

**Same file, same fragment name (short form):**

When the dependency is in the same `.h` file, you can reference it by fragment name only:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "candidate_detection_t"
typedef struct {
    float x, y, width, height;
    float confidence;
} candidate_detection_t;
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "detectionfilter_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "candidate_detection_t"

static void detectionfilter_f32(const float* detections, ...) {
    candidate_detection_t candidates[MAX_DET];
    // ...
}
#pragma IMAGINET_FRAGMENT_END
```

The parser resolves the short name `"candidate_detection_t"` to `"<current_file_path>:candidate_detection_t"`.

**Relative path (sibling/child files):**

Reference a fragment in another file using a relative path:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "display_bounding_box_f32"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../Drawing/BitmapFont/bitmap_font.h:bitmap_font_helpers"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "../../Drawing/BitmapFont/bitmap_font.h:bitmap_font_text_utils"

static void display_bounding_box_f32(...) { ... }
#pragma IMAGINET_FRAGMENT_END
```

**Cross-unit reference (bracket notation):**

Reference a fragment in a completely different unit project using the `[UnitId]` prefix:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "sync_wait_command"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "[Imaginet.Units.Base.DataNodeIpc.DataNodeSyncControl]/datanode_sync_control.h:sync_protocol"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "[Imaginet.Units.Base.Ipc]/ipc.h:ipc_base"

static inline int sync_wait_command(void* control_pipe) { ... }
#pragma IMAGINET_FRAGMENT_END
```

**Global (file-level) dependencies:**

When a dependency is placed **outside** any fragment (at file level), it applies to **all** fragments in the file. This avoids repeating the same dependency in every fragment:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_t"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "CBuffer/cbuffer.h:cbuffer"
typedef struct {
    cbuffer_t data_buffer;
    cbuffer_t time_buffer;
    int input_size;
} fixwin_time_t;
#pragma IMAGINET_FRAGMENT_END

// Global dependency — all fragments below inherit this
#pragma IMAGINET_FRAGMENT_DEPENDENCY "fixwin_time_t"

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_init"
// Automatically depends on fixwin_time_t (inherited from global)
static int fixwin_time_init(fixwin_time_t* handle, ...) { ... }
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "fixwin_time_body"
// Also inherits the fixwin_time_t dependency
static int fixwin_time_body(fixwin_time_t* handle, ...) { ... }
#pragma IMAGINET_FRAGMENT_END
```

**Dependency reference format summary:**

| Form | Example | Resolves To |
|---|---|---|
| Fragment name only | `"my_struct"` | `<same_file_path>:my_struct` |
| Relative file:fragment | `"../utils.h:helper"` | Resolved relative to current file |
| Absolute unit ref | `"[Unit.Id]/file.h:fragment"` | Cross-unit dependency |

#### Code Package Dependencies (`IMAGINET_CODEPACKAGE_DEPENDENCY`)

When a fragment requires an external library or SDK package (not just another fragment), declare it with `IMAGINET_CODEPACKAGE_DEPENDENCY`. This tells the build system to link the package:

```c
#pragma IMAGINET_CODEPACKAGE_DEPENDENCY "cmsis-dsp"

#pragma IMAGINET_FRAGMENT_BEGIN "hannmul_cmsis_f32"
static inline void hannmul_cmsis_f32(const float* input, float* output, int count) {
    arm_hanning_f32(output, count);
    arm_mult_f32(input, output, output, count);
}
#pragma IMAGINET_FRAGMENT_END
```

Like fragment dependencies, if placed outside any fragment, it applies globally to all fragments in the file. If placed inside a fragment, it applies only to that fragment.

Common code packages: `cmsis-dsp`, `tflite-micro`, `tflite`.

#### Symbol Checks (`IMAGINET_SYMBOL_CHECK`)

Assert that a preprocessor symbol is defined at compile time. If the symbol is missing, the code generator emits the error message to guide the user:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "mtb_init"
#pragma IMAGINET_FRAGMENT_DEPENDENCY "mtb_profiling"
#pragma IMAGINET_SYMBOL_CHECK COMPONENT_ML_TFLM GLOBAL "Add 'COMPONENTS+=ML_TFLM' to your Makefile."

static int mtb_init(const void* handle, ...) { ... }
#pragma IMAGINET_FRAGMENT_END
```

Syntax: `IMAGINET_SYMBOL_CHECK <SYMBOL_NAME> <SCOPE> "<error message>"`

| Scope | Meaning |
|---|---|
| `GLOBAL` | Symbol must be defined globally (checked across the whole project) |
| `LOCAL` | Symbol must be defined locally (checked in the translation unit) |

Multiple symbol checks can be stacked:

```c
#pragma IMAGINET_SYMBOL_CHECK COMPONENT_ML_TFLM_LESS GLOBAL "Add 'COMPONENTS+=ML_TFLM_LESS' to your Makefile."
#pragma IMAGINET_SYMBOL_CHECK TF_LITE_MICRO_USE_OFFLINE_OP_USER_DATA GLOBAL "Add 'DEFINES+=TF_LITE_MICRO_USE_OFFLINE_OP_USER_DATA' to your Makefile."
```

#### Fragment Tests (`IMAGINET_FRAGMENT_TEST`)

Associate test fragments with a code fragment. Test fragments are separate functions (typically in a `_test.h` or `_tests.c` file) that verify the fragment's behaviour:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "cbuffer"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_test"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_full_test"
#pragma IMAGINET_FRAGMENT_TEST "cbuffer_tests.c:cbuffer_enqueue_overflow_test"

typedef struct { ... } cbuffer_t;
static inline int cbuffer_init(...) { ... }
#pragma IMAGINET_FRAGMENT_END
```

The test runner discovers these associations and can run the tests for any fragment that declares them.

#### Coding Conventions for Fragments

- Declare functions `static inline` — the code generator inlines them into a single `.c` file.
- Use `restrict` on pointer parameters to enable compiler optimisations.
- Use `const` on input pointers.
- Type mapping from `.imunit` parameters to C:

| .imunit Type | C Type |
|---|---|
| `InputSocket` (Float32) | `const float*` |
| `InputSocket` (Int8) | `const int8_t*` |
| `InputSocket` (UInt8) | `const uint8_t*` |
| `InputSocket` (Q7) | `const q7_t*` (CMSIS) |
| `InputSocket` (Q15) | `const q15_t*` (CMSIS) |
| `InputSocket` (Q31) | `const q31_t*` (CMSIS) |
| `InputSocket` (D8) | `const int8_t*` |
| `InputSocket` (D16) | `const int16_t*` |
| `InputSocket` (D32) | `const int32_t*` |
| `OutputSocket` (Float32) | `float*` |
| `OutputSocket` (Q15) | `q15_t*` (CMSIS) |
| `OutputSocket` (D8) | `int8_t*` |
| `Expression` (integer) | `int` |
| `Expression` (double) | `float` or `double` |
| `Int32Option` | `int` |
| `DoubleOption` | `float` |
| `BoolOption` | `int` (0/1) |
| `StringOption` | `char*` (null-terminated) |
| `Handle` | `int8_t*` (cast in C code) |
| `Stream` | platform-specific pipe handle |

---

## 9. Lifecycle Sections

### 9.1 Choosing a Lifecycle Pattern

Every C-backed unit falls into one of three patterns. The deciding factor is **whether input rate equals output rate**:

| Pattern | Output rate | Sections needed | When to use |
|---|---|---|---|
| **Stateless** | Same as input (1:1) | `<Implementations>` only | Pure transforms — each input frame produces exactly one output frame, no memory between calls. |
| **Stateful** | Same as input (1:1) | `<Init>` + `<Implementations>` + optional `<Destructor>` / `<SoftReset>` | Still 1:1, but the unit maintains persistent state across calls via a `<Handle>` (filter coefficients, rotation matrices, running counters, etc.). |
| **Streaming** | Different from input (N:M) | Enqueue/Dequeue API (see below) | The unit buffers data internally. Input arrives at one rate, output is produced at a different rate. The runtime uses guard functions to control data flow. |

**Stateless** — use when the operation is a pure function of the current input (e.g. Threshold, Division, type conversion).

**Stateful** — use when you need memory between calls but each call still consumes one input and produces one output (e.g. LowPassFilter remembers previous samples, RotateIMU precomputes a rotation matrix in Init).

**Streaming** — use when the unit changes the data rate. This comes in two variants:

| Variant | Sections | Description | Example |
|---|---|---|---|
| **Data source** | `<CanEnqueue>` + `<Dequeue>` | No `InputSocket`. Reads from hardware or a file via a `<Stream>` pipe. `<CanEnqueue>` checks if data is available; `<Dequeue>` reads and outputs it. | Microphone, Camera, WaveFileReader |
| **Buffering** | `<Enqueue>` + `<Dequeue>` + `<CanEnqueue>` + `<CanDequeue>` | Has an `InputSocket`. Accumulates input chunks into an internal buffer, then outputs windows/blocks at a different rate. | SlidingWindow |

#### How the runtime uses these sections

For **stateless and stateful** units, the runtime simply calls `<Implementations>` once per input frame in a sequential pipeline.

For **streaming** units, the runtime wraps calls in a **poll loop** that checks the guard functions:

```
while (can_enqueue(handle) >= 0 || can_dequeue(handle) >= 0) {
    enqueue(handle, input);     // push data in
    dequeue(handle, output);    // pull data out (may produce 0 or N outputs)
}
```

The `<Dequeue>` section also **forks a new execution domain** — all downstream units after a streaming unit run at the output rate, not the input rate. This is why streaming units are fundamentally different: they are rate-change boundaries in the pipeline.

#### Return codes for streaming functions

Functions in `<Enqueue>`, `<CanEnqueue>`, and `<CanDequeue>` return `int` status codes:

| Code | Macro | Meaning |
|---|---|---|
| `0` | `IPWIN_RET_SUCCESS` | Operation succeeded |
| `-1` | `IPWIN_RET_NODATA` | No data available (buffer empty / not ready) |
| `-2` | `IPWIN_RET_ERROR` | Error |
| `-3` | `IPWIN_RET_STREAMEND` | Stream has ended (EOF) |

The poll loop continues while the return value is `>= -1` (i.e. success or no-data). It terminates on error or stream-end.

### 9.2 Init — One-Time Initialisation

Called once before the first body call. Use for computing rotation matrices, setting up model interpreters, etc.

```xml
<Init returnStatus="true">
    <Implementation language="C" fragment="rotateIMU.h:rotate_imu_init_f32"
                    call="rotate_imu_init_f32(rotation_matrix, rotation_deg, index)">
        <Conditional value="input.type == System.Float32" />
    </Implementation>
</Init>
```

When `returnStatus="true"`, the C function must return `int` (0 = success, non-zero = error).

### 9.3 Implementations — Main Processing

The default processing body. Called every frame/sample for stateless and stateful units.

```xml
<Implementations>
    <Implementation language="C" fragment="rotateIMU.h:rotate_imu_f32"
                    call="rotate_imu_f32(input, rotation_matrix, output)">
        <Conditional value="input.type == System.Float32" />
    </Implementation>
</Implementations>
```

Legacy alias: `<Body>` is accepted but deprecated — use `<Implementations>` for 1:1 units. For streaming units the same parser slot is accessed via `<Enqueue>` (see Section 9.7).

### 9.4 Destructor — Cleanup

Called when the processing pipeline is torn down:

```xml
<Destructor>
    <Implementation language="C" fragment="tfl.h:tfl_free"
                    call="tfl_free(handle)">
        <Conditional value="global_tf_api == 'tfl'" />
    </Implementation>
</Destructor>
```

### 9.5 SoftReset — State Reset

Called to reset persistent state without full re-initialisation (e.g. between recording sessions). Both stateful and streaming units can use this:

```xml
<SoftReset returnStatus="true">
    <Implementation language="C" fragment="mtb_model.h:mtb_model_soft_reset"
                    call="mtb_model_soft_reset(handle)">
        <Conditional value="global_tf_api == 'mtb-ml-middleware'" />
    </Implementation>
</SoftReset>
```

### 9.6 Streaming: Data Source Pattern (CanEnqueue + Dequeue)

For capture devices and file readers that produce data asynchronously. These units have no `InputSocket` and read from hardware or a file via a `<Stream>` pipe:

```xml
<!-- Guard: is data available to read? Returns int status code. -->
<CanEnqueue>
    <Implementation language="C" fragment="mic_normalize.h:mic_can_read"
                    call="mic_can_read(data_pipe, byte_count)" />
</CanEnqueue>

<!-- Producer: read data and write to output buffers -->
<Dequeue>
    <Implementation language="C" fragment="mic_normalize.h:mic_read_u16_f32"
                    call="mic_read_u16_f32(data_pipe, data, channels)">
        <Conditional value="time == null" />
    </Implementation>
</Dequeue>
```

Legacy aliases: `<Status>` and `<NewDomain>` still work but are deprecated — prefer `<CanEnqueue>` and `<Dequeue>` in new code.

### 9.7 Streaming: Buffering Pattern (Enqueue + Dequeue)

For units that change the data rate by buffering input and producing output at a different rate. These units have both input and output sockets:

```xml
<!-- Push one input chunk into the circular buffer. Returns int status. -->
<Enqueue returnStatus="true">
    <Implementation language="C" fragment="fixwin.h:fixwin_enqueue"
                    call="fixwin_enqueue(handle, input)" />
</Enqueue>

<!-- Pop a window/block from the buffer into the output. -->
<Dequeue>
    <Implementation language="C" fragment="fixwin.h:fixwin_dequeue"
                    call="fixwin_dequeue(handle, output, window_count, stride_count)" />
</Dequeue>

<!-- Guard: can the buffer accept more input? -->
<CanEnqueue>
    <Implementation language="C" fragment="fixwin.h:fixwin_can_enqueue"
                    call="fixwin_can_enqueue(handle, input_count)" />
</CanEnqueue>

<!-- Guard: is there enough data in the buffer to produce output? -->
<CanDequeue>
    <Implementation language="C" fragment="fixwin.h:fixwin_can_dequeue"
                    call="fixwin_can_dequeue(handle, input_count)" />
</CanDequeue>
```

### 9.8 Lifecycle Summary

```
STATELESS / STATEFUL                     STREAMING (buffering)
========================                 ====================================
                                      
 <Init>                                   <Init>
   │                                        │
   ▼                                        ▼
 ┌──────────────────┐                    ┌──────────────────────────────────┐
 │ <Implementations>│ ◄── 1:1            │ while(can_enqueue || can_dequeue)│
 │  (once per frame)│     input=output   │   <Enqueue>  ── push data in    │
 └──────────────────┘     rate           │   <Dequeue>  ── pull data out ──┼── forks new
   │                                     │              (new output rate)   │   execution
   ▼                                     └──────────────────────────────────┘   domain
 <SoftReset> (between sessions)            │
 <Destructor> (teardown)                   ▼
                                         <SoftReset> / <Destructor>

STREAMING (data source)
====================================

 <Init>
   │
   ▼
 ┌──────────────────────────────────┐
 │ while(can_enqueue >= 0)          │
 │   <CanEnqueue> ── data ready?    │
 │   <Dequeue>    ── read & output ─┼── forks new execution domain
 └──────────────────────────────────┘
   │
   ▼
 <SoftReset> / <Destructor>
```

---

## 10. Expression Language Reference

Expressions appear in `<Expression>` values, `<OutputSocket>` type/shape/rate attributes, `<Conditional>` values, `<Assert>` tests, `<Handle>` size, and `<Assert>` error messages (as `{interpolation}`).

**Method-call syntax:** Any single-argument function can be called as either `fn(x)` or `x.fn`. This enables chained calls like `input.shape.size(1).log2.ceil` instead of `ceil(log2(input.shape.size(1)))`. Multi-argument functions use `x.fn(y)` which is equivalent to `fn(x, y)`.

### 10.1 Operators (by precedence, highest first)

| Precedence | Operators | Description |
|---|---|---|
| 1 | `-x` `~x` `!x` | Unary negate, bitwise NOT, logical NOT |
| 2 | `*` `/` `%` | Multiply, divide, modulus |
| 3 | `+` `-` | Add, subtract (also string concat) |
| 4 | `<<` `>>` | Bit shift |
| 5 | `<` `>` `>=` `<=` | Relational comparison |
| 6 | `==` `!=` | Equality |
| 7 | `&` `^` `\|` `&&` `\|\|` | Bitwise/logical AND, XOR, OR |
| 8 | `? :` | Ternary conditional |

### 10.2 Literals

| Syntax | Type | Example |
|---|---|---|
| `42` | int | `42` |
| `3.14` | double | `3.14` |
| `'text'` or `"text"` | string | `'same'` |
| `true` / `false` | bool | `true` |
| `null` | null | `null` |

### 10.3 System Types

All types are accessed via `System.` prefix (e.g. `System.Float32`, `System.Q15`). See Section 11 for the full type reference including C type mappings and fixed-point semantics.

### 10.4 System Constructors

| Expression | Returns | Description |
|---|---|---|
| `System.Shape(d0)` | Shape | 1D shape |
| `System.Shape(d1, d0)` | Shape | 2D shape |
| `System.Shape(d2, d1, d0)` | Shape | 3D shape |
| `System.Shape(d3, d2, d1, d0)` | Shape | 4D shape |
| `System.Shape(d4, d3, d2, d1, d0)` | Shape | 5D shape |
| `System.Tensor(type, count)` | Tensor | Temporary buffer |
| `System.Tensor(type, count, true)` | Tensor | Static (persistent) buffer |
| `System.Tensor(type, shape)` | Tensor | Temporary buffer with shape |
| `System.parseType(string)` | Type | Parse `"float32"`, `"int8"`, etc. |
| `System.Load(assembly, class)` | Object | Load .NET class for runtime computation |
| `System.Identifier(string)` | Identifier | Literal C identifier (no quotes in output) |

### 10.5 Tensor Properties

| Property | Returns | Description |
|---|---|---|
| `tensor.type` | Type | Data type |
| `tensor.shape` | Shape | Shape object |
| `tensor.rate` | double | Sample frequency |
| `tensor.shift` | int | Q-format shift value |
| `tensor.scale` | double | D-format scale |
| `tensor.offset` | double | D-format offset |
| `tensor.isTimeCoded` | bool | True if tensor carries irregular timestamps |

### 10.6 Tensor Methods

| Method | Returns | Description |
|---|---|---|
| `tensor.align(n)` | Tensor | Set byte alignment |
| `tensor.useMlMemory()` | Tensor | Place in ML model memory section |

**Fixed-point quantization:**

| Method | Returns | Description |
|---|---|---|
| `value.quantize(type, shift)` | int | Convert a double to a Q-format fixed-point integer |

Used to pre-compute quantized constants at compile time. Example from Clip:

```xml
<Expression name="min_quantized" value="min.quantize(input.type, input.shift)" />
```

**Constant tensor operations:** The following work only on read-only (constant) tensors — typically lookup tables or weight matrices created by `System.Load(...)`:

| Method | Returns | Description |
|---|---|---|
| `tensor.transpose(d0)` ... `(d3,d2,d1,d0)` | Tensor | Permute axes |
| `tensor.reshape(shape)` | Tensor | Reshape without copying |
| `tensor.exp()` | Tensor | Element-wise exponential |
| `tensor.pow(b)` | Tensor | Element-wise power |
| `tensor.log(base)` | Tensor | Element-wise logarithm |
| `tensor.sqrt()` | Tensor | Element-wise square root |
| `tensor.abs()` | Tensor | Element-wise absolute value |
| `tensor + tensor`, `- * /` | Tensor | Element-wise arithmetic between two constant tensors |
| `tensor + scalar`, `- * /` | Tensor | Element-wise arithmetic with a scalar |

### 10.7 Shape Properties & Methods

**Axis ordering:** Axis 0 is always the **innermost (rightmost)** dimension. Shape `[3,5,7]` in the UI is stored internally as `[7, 5, 3]` — axis 0 = 7, axis 1 = 5, axis 2 = 3. Negative indices count from the left: `-1` = leftmost (outermost).

**Core decomposition — step / size / slot:** Most axis-oriented operations decompose a shape into three integers that drive flat-array iteration in C. For any axis, the relationship `step × size × slot == flat` always holds.

Worked example: shape `[3,5,7]` — stored internally as `[7, 5, 3]`:

```
axis  size  step          slot          step × size × slot
  0     7   1             5×3 = 15      1 × 7 × 15 = 105  ✓
  1     5   7             3             7 × 5 × 3  = 105  ✓
  2     3   7×5 = 35      1             35 × 3 × 1 = 105  ✓
```

- `step(axis)` = product of dimensions 0..(axis-1) — the stride between adjacent elements along that axis.
- `size(axis)` = the length of that dimension.
- `slot(axis)` = product of dimensions (axis+1)..(count-1) — the number of independent slices.

| Expression | Returns | Description |
|---|---|---|
| `shape.count` | int | Number of dimensions (rank) |
| `shape.flat` | int | Total element count (product of all dims) |
| `shape.size(axis)` | int | Size of dimension at `axis`. Negative indices count from left: `-1` = leftmost. |
| `shape.step(axis)` | int | Stride at `axis` — product of all dimensions below it |
| `shape.step(start, stop)` | int | Product of dimensions from `start` to `stop-1` |
| `shape.slot(axis)` | int | Number of slices at `axis` — product of all dimensions above it |
| `shape.remove(axis)` | Shape | Remove dimension (reduces rank by 1) |
| `shape.replace(axis, newSize)` | Shape | Replace dimension size (same rank) |
| `shape.insert(axis, size)` | Shape | Insert a new dimension (increases rank by 1) |
| `shape.broadcast(other)` | Broadcast | Compute broadcast decomposition (see Section 12.5) |
| `shape.withLabels(axis, "a,b,c")` | Shape | Attach semantic labels to a dimension |
| `shape.withLabels(axis, "a,b", start)` | Shape | Replace labels starting at offset `start` |
| `shape.getLabels(axis)` | string | Get comma-separated labels |
| `shape.getLabels(axis, start, count)` | string | Get subset of labels |

**Broadcast properties:** The object returned by `shape.broadcast(other)` exposes five integers — `left`, `group1`, `middle`, `group2`, `right` — that decompose the iteration for element-wise operations between shapes that differ by broadcast dimensions (size-1 dims in `other`). See Tutorial 12.5 for a complete example.

### 10.8 Type Properties

| Expression | Returns | Description |
|---|---|---|
| `type.size` | int | Byte size of one element |

### 10.9 Math Functions

Most are available as both `Math.fn(args)` and bare `fn(args)`. Exceptions noted below.

| Function | Description |
|---|---|
| `real(int)` → double | Cast to double |
| `int(double)` → int | Truncate to int |
| `abs(x)` | Absolute value |
| `min(a, b)` / `max(a, b)` | Minimum / maximum |
| `pow(base, exp)` | Power (also `pow(int, int)` → int) |
| `exp(x)` | e^x |
| `sqrt(x)` | Square root |
| `log(x)` | Natural log |
| `log(x, base)` | Log with base |
| `log2(x)` | Log base 2 (bare only — no `Math.log2`) |
| `log10(x)` | Log base 10 |
| `inv(x)` | 1/x |
| `floor(x)` / `ceil(x)` | Floor / ceiling (returns int) |
| `round(x)` / `trunc(x)` | Round / truncate (returns int) |
| `sign(x)` | Sign (-1, 0, 1) |
| `factorial(n)` | n! |
| `ceilstep(x, step)` | Ceiling to next multiple of step (`Math.` prefix only) |
| `sin` `cos` `tan` `asin` `acos` `atan` | Trigonometric (`Math.` prefix only) |
| `sinh` `cosh` `tanh` | Hyperbolic (`Math.` prefix only) |
| `atan2(y, x)` | Two-argument arctangent (`Math.` prefix only) |

Constants: `Math.E`, `Math.PI`, `Math.Epsilon`, `Math.NaN`, `Math.MinValue`, `Math.MaxValue`

Since single-argument functions double as methods (see top of this section), chaining like `d1.log2.ceil` or `value.abs` is idiomatic.

### 10.10 String Functions

| Expression | Description |
|---|---|
| `str1 + str2` | Concatenation |
| `str.length` | Character count |
| `str.toLower()` / `str.toUpper()` | Case conversion |
| `str.trim()` / `str.trimStart()` / `str.trimEnd()` | Whitespace trimming |
| `str.contains(s)` / `str.startsWith(s)` / `str.endsWith(s)` | Search |
| `str.indexOf(s)` / `str.lastIndexOf(s)` | Index lookup |
| `str.replace(old, new)` | String replacement |
| `str.remove(start)` / `str.remove(start, count)` | Substring removal |
| `str.insert(index, value)` | String insertion |
| `str.compare(other)` | int | Lexicographic comparison (< 0, 0, > 0) |

---

## 11. Data Type System

### 11.1 Overview

DEEPCRAFT Studio supports a rich set of data types for tensors. Beyond the standard IEEE floats and integers, it includes two families of fixed-point types designed for efficient deployment on embedded hardware where floating-point units may be absent or slow.

### 11.2 Complete Type Reference

| Type | Expression | C Type | Byte Size | Description |
|---|---|---|---|---|
| Float32 | `System.Float32` | `float` | 4 | IEEE 754 single precision |
| Float64 | `System.Float64` | `float64_t` | 8 | IEEE 754 double precision |
| Int8 | `System.Int8` | `int8_t` | 1 | Signed 8-bit integer |
| Int16 | `System.Int16` | `int16_t` | 2 | Signed 16-bit integer |
| Int32 | `System.Int32` | `int32_t` | 4 | Signed 32-bit integer |
| Int64 | `System.Int64` | `long` | 8 | Signed 64-bit integer |
| UInt8 | `System.UInt8` | `uint8_t` | 1 | Unsigned 8-bit integer |
| UInt16 | `System.UInt16` | `uint16_t` | 2 | Unsigned 16-bit integer |
| UInt32 | `System.UInt32` | `uint32_t` | 4 | Unsigned 32-bit integer |
| UInt64 | `System.UInt64` | `unsigned long` | 8 | Unsigned 64-bit integer |
| Q7 | `System.Q7` | `q7_t` | 1 | Shifted fixed-point 8-bit |
| Q15 | `System.Q15` | `q15_t` | 2 | Shifted fixed-point 16-bit |
| Q31 | `System.Q31` | `q31_t` | 4 | Shifted fixed-point 32-bit |
| D8 | `System.D8` | `int8_t` | 1 | Scaled fixed-point 8-bit |
| D16 | `System.D16` | `int16_t` | 2 | Scaled fixed-point 16-bit |
| D32 | `System.D32` | `int32_t` | 4 | Scaled fixed-point 32-bit |

Note: Q types use CMSIS-DSP type aliases (`q7_t`, `q15_t`, `q31_t`) and require the `cmsis-dsp` code package. D types use standard C integer types.

### 11.3 Q Types — Shifted Fixed Point

Q-format types represent fractional values using integer storage with a power-of-two scale factor. The conversion formula is:

```
real_value = int_value / (2^N >> shift)
```

Where N is the number of fractional bits:

| Type | N | Formula | Range (shift=0) |
|---|---|---|---|
| Q7 | 7 | `real = int_value / (128 >> shift)` | [-1.0, +0.992] |
| Q15 | 15 | `real = int_value / (32768 >> shift)` | [-1.0, +0.99997] |
| Q31 | 31 | `real = int_value / (2147483648 >> shift)` | [-1.0, +0.9999999995] |

The `shift` parameter moves the binary point. Increasing `shift` by 1 doubles the representable range but halves the precision:

| shift | Q15 range | Q15 precision |
|---|---|---|
| 0 | [-1.0, +1.0) | 1/32768 |
| 1 | [-2.0, +2.0) | 1/16384 |
| 2 | [-4.0, +4.0) | 1/8192 |

**Tensor metadata:** Q-type tensors carry a `.shift` property (an integer) set by the producing unit.

**In .imunit files:** Set the `shift` attribute on `<OutputSocket>` and read it via `input.shift`:

```xml
<InputSocket name="input" />
<Expression name="shift" value="input.shift" />
<OutputSocket name="output" type="System.Q15" shape="input.shape"
              shift="shift" />
```

**In C fragments:** The shift value is passed as a regular `int` parameter. Quantization uses saturation arithmetic:

```c
static inline void quantize_f32_to_q15(const float* restrict src,
                                       q15_t* restrict dst,
                                       int count, int shift)
{
    while (count > 0U) {
        *dst++ = (q15_t) __SSAT((q31_t)(*src++ * 32768.0f / (1 << shift)), 16);
        count--;
    }
}
```

Dequantization reverses the operation:

```c
static inline void dequantize_q15_to_f32(const q15_t* restrict src,
                                         float* restrict dst,
                                         int count, int shift)
{
    while (count > 0U) {
        *dst++ = ((float)*src++ / (float)(32768 >> shift));
        count--;
    }
}
```

### 11.4 D Types — Scaled Fixed Point

D-format types (also called "scaled" or "affine quantization") use an arbitrary scale and offset. This is the quantization scheme used by TensorFlow Lite:

```
real_value = (int_value - offset) * scale
```

Or inversely for quantization:

```
int_value = real_value / scale + offset
```

| Type | Storage | Range |
|---|---|---|
| D8 | `int8_t` | Determined by scale and offset |
| D16 | `int16_t` | Determined by scale and offset |
| D32 | `int32_t` | Determined by scale and offset |

Unlike Q types which only need a `shift` integer, D types carry two metadata values:
- **scale** (double) — the step size per integer increment
- **offset** (long) — the integer value that maps to real zero

**Tensor metadata:** D-type tensors carry `.scale` and `.offset` properties.

**In .imunit files:** Set both `scale` and `offset` on `<OutputSocket>`:

```xml
<InputSocket name="input" />
<Expression name="scale" value="input.scale" />
<Expression name="offset" value="input.offset" />
<OutputSocket name="output" type="System.D8" shape="input.shape"
              scale="scale" offset="offset" />
```

**In C fragments:** Scale and offset are passed as `float` and `int` parameters:

```c
static inline void dequantize_d8_to_f32(const int8_t* restrict src,
                                        float* restrict dst,
                                        int count, float scale, int offset)
{
    while (count > 0U) {
        *dst++ = (*src++ - offset) * scale;
        count--;
    }
}
```

### 11.5 Choosing Between Q and D

| Consideration | Q types | D types |
|---|---|---|
| Scale factor | Power-of-two only | Arbitrary |
| Zero point | Always 0 | Configurable offset |
| Computation | Bit shifts (fast on MCU) | Multiply + subtract |
| Ecosystem | CMSIS-DSP native | TensorFlow Lite native |
| Use case | Signal processing, FFT | ML model quantization |

Use **Q types** when working with signal-processing pipelines on ARM Cortex-M where CMSIS-DSP is available. Use **D types** when interfacing with quantized ML models (TFLite, ONNX).

### 11.6 `System.parseType()` for Dynamic Type Selection

When the output type is selected by the user at runtime (e.g. via a `StringOption`), use `System.parseType()` to convert a string to a type:

```xml
<StringOption name="output_type" text="Output Type" default="q15">
    <OneOf>
        <Item text="Q7 (8 bit)">q7</Item>
        <Item text="Q15 (16 bit)">q15</Item>
        <Item text="Q31 (32 bit)">q31</Item>
        <Item text="D8 (8 bit)">d8</Item>
        <Item text="D16 (16 bit)">d16</Item>
        <Item text="D32 (32 bit)">d32</Item>
    </OneOf>
</StringOption>

<OutputSocket name="output" type="System.parseType(output_type)"
              shape="input.shape" shift="shift" scale="scale" offset="offset" />
```

Accepted string values for `System.parseType()`: `float32`, `float64`, `int8`, `int16`, `int32`, `int64`, `uint8`, `uint16`, `uint32`, `uint64`, `q7`, `q15`, `q31`, `d8`, `d16`, `d32`, `bool`, `string`, and several aliases (`f32`, `s8`, `i8`, `float`, `single`, etc.).

### 11.7 Propagating Fixed-Point Metadata

When a unit preserves the data type (pass-through or element-wise operations), propagate the quantization metadata to the output:

```xml
<InputSocket name="input" />
<OutputSocket name="output" type="input.type" shape="input.shape"
              shift="input.shift" scale="input.scale" offset="input.offset" />
```

When a unit changes the scale (e.g. a norm operation that divides by the square root of the axis size), compute the new shift:

```xml
<Expression name="output_shift"
            value="(d1.sqrt).log2.ceil + shift" />
<OutputSocket name="output" type="input.type" shape="input.shape.remove(axis)"
              shift="input.shift + output_shift" />
```

---

## 12. Tutorials — Building Units Step by Step

Each tutorial presents a complete working unit — both `.imunit` and `.h` — building from the Quick Start example to progressively more complex patterns.

### 12.1 Multi-Type + Mode Selection — Division

**What it teaches:** Dispatching to different C fragments based on data type AND user-selected mode. This is the combinatorial conditional pattern.

`Divide.imunit` (key sections):

```xml
<Parameters>
  <InputSocket name="input" />
  <Expression name="count" value="input.shape.flat" />
  <DoubleOption name="divisor" text="Divisor" default="1" />
  <Int32Option name="mode" text="Mode" default="0">
    <OneOf>
      <Item text="Division">0</Item>
      <Item text="Modulo">1</Item>
    </OneOf>
  </Int32Option>
  <OutputSocket name="output" type="input.type" shape="input.shape" />
</Parameters>

<Contracts>
  <Assert test="input.type == System.Float32 || input.type == System.Int8 || input.type == System.UInt8"
          error="Input ({input.type}) must be Float32, Int8, or UInt8" />
  <Assert test="divisor != 0" error="Divisor cannot be 0" />
</Contracts>

<Implementations>
  <!-- Type × Mode = 6 combinations, each gets its own Implementation -->
  <Implementation language="C" fragment="divide.h:divide_f32"
                  call="divide_f32(input, count, divisor, output)">
    <Conditional value="input.type == System.Float32 &amp;&amp; mode == 0" />
  </Implementation>
  <Implementation language="C" fragment="divide.h:divide_i8"
                  call="divide_i8(input, count, divisor, output)">
    <Conditional value="input.type == System.Int8 &amp;&amp; mode == 0" />
  </Implementation>
  <Implementation language="C" fragment="divide.h:divide_u8"
                  call="divide_u8(input, count, divisor, output)">
    <Conditional value="input.type == System.UInt8 &amp;&amp; mode == 0" />
  </Implementation>

  <Implementation language="C" fragment="mod.h:mod_f32"
                  call="mod_f32(input, count, divisor, output)">
    <Conditional value="input.type == System.Float32 &amp;&amp; mode == 1" />
  </Implementation>
  <!-- ... mod_i8, mod_u8 ... -->
</Implementations>
```

`divide.h`:

```c
#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "divide_f32"
static inline void divide_f32(const float* restrict x, int count,
                              float divisor, float* restrict output)
{
    for (int i = 0; i < count; i++)
        output[i] = x[i] / divisor;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "divide_i8"
static inline void divide_i8(const int8_t* restrict x, int count,
                             float divisor, int8_t* restrict output)
{
    for (int i = 0; i < count; i++)
        output[i] = x[i] / divisor;
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "divide_u8"
static inline void divide_u8(const uint8_t* restrict x, int count,
                             float divisor, uint8_t* restrict output)
{
    for (int i = 0; i < count; i++)
        output[i] = x[i] / divisor;
}
#pragma IMAGINET_FRAGMENT_END
```

**Key points:**
- Each type/mode combination is a separate `<Implementation>`. The code generator picks the first match.
- `<OneOf>` creates a dropdown in the UI. The `<Item>` text is the display label, the element content is the value.
- XML escaping: `&&` becomes `&amp;&amp;` inside attribute values.
- The modulo fragments live in a separate file (`mod.h`) but are referenced from the same `.imunit`.

### 12.2 Stateful — Low-Pass Filter with Handle

**What it teaches:** Persistent state across frames using `<Handle>`, and using `input.rate` for sample-rate-aware processing.

`LowPassFilter.imunit`:

```xml
<?xml version="1.0" encoding="utf-8" ?>
<Imaginet version="2.0.0.0">
  <Unit name="Imaginet.Units.Filter.LowPassFilter">
    <DisplayName>Low Pass Filter</DisplayName>
    <DisplayPath>/Signal Processing/Time Domain</DisplayPath>
    <Description>
      <Header>Description</Header>
      First-order IIR low-pass filter using exponential smoothing.
      <Header>Usage</Header>
      Smooth signals or remove high-frequency noise from sensor data.
      <Image file="LPF.png" width="450" />
    </Description>

    <Parameters>
      <InputSocket name="input" pipe="data" />
      <Expression name="count" value="input.shape.flat" />
      <Expression name="freq" value="input.rate" />
      <DoubleOption name="cutoff_freq" text="Cutoff Frequency" default="10" />
      <Handle name="state" size="input.shape.flat * 4" />
      <OutputSocket name="output" pipe="data" type="input.type"
                    shape="input.shape" />
    </Parameters>

    <Contracts>
      <Assert test="cutoff_freq >= 0"
              error="Cutoff Frequency needs to be positive." />
    </Contracts>

    <Implementations>
      <Implementation language="C"
          fragment="lowpassfilter.h:lowpass_filter_f32"
          call="lowpass_filter_f32(input, state, output, cutoff_freq, count, freq)">
        <Conditional value="input.type == System.Float32" />
      </Implementation>
    </Implementations>
  </Unit>
</Imaginet>
```

`lowpassfilter.h`:

```c
#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "lowpass_filter_f32"
static inline void lowpass_filter_f32(const float* restrict input,
                                     int8_t* restrict state_bytes,
                                     float* restrict output,
                                     float cutoff_freq, int count, int freq)
{
    float* state = (float*)state_bytes;
    float alpha = 1.0f - expf(-2.0f * M_PI * cutoff_freq / (float)freq);

    for (int i = 0; i < count; ++i) {
        output[i] = alpha * input[i] + (1.0f - alpha) * state[i];
        state[i] = output[i];
    }
}
#pragma IMAGINET_FRAGMENT_END
```

**Key points:**
- `<Handle>` allocates persistent memory sized by an expression (`input.shape.flat * 4` = one float per element).
- The Handle arrives in C as `int8_t*` — cast it to the type you need (`float*`).
- `input.rate` extracts the sample frequency from the upstream tensor metadata.
- The `freq` expression is passed to C so the filter can compute alpha from the actual sample rate.

### 12.3 Init + Body Lifecycle — Rotate IMU

**What it teaches:** One-time initialisation with `<Init>`, precomputing data into a Handle, and using `returnStatus="true"` for error reporting.

`RotateIMU.imunit`:

```xml
<?xml version="1.0" encoding="utf-8" ?>
<Imaginet version="2.0.0.0">
  <Unit name="Imaginet.Units.Common.RotateIMU">
    <DisplayName>Rotate IMU</DisplayName>
    <DisplayPath>/Signal Processing/Sensor Packages/IMU</DisplayPath>
    <Description>
      <Header>Description</Header>
      Rotate accelerometer and gyroscope readings by applying a 3D rotation
      matrix. Input shape must be [2, 3].
    </Description>

    <Parameters>
      <InputSocket name="input" pipe="data" />
      <Int32Option name="rotation_deg" text="Rotation (Degrees)" default="0" />
      <Int32Option name="index" min="0" max="2" text="Axis" />
      <Handle name="rotation_matrix" size="36" />
      <OutputSocket name="output" pipe="data" type="input.type"
                    shape="input.shape" />
    </Parameters>

    <Contracts>
      <Assert test="input.shape.size(0) == 3"
              error="Rotate IMU requires a [2,3] matrix." />
      <Assert test="input.shape.size(1) == 2"
              error="Rotate IMU requires a [2,3] matrix." />
      <Assert test="index >= 0 &amp;&amp; index &lt; 3"
              error="Axis index must be 0, 1, or 2." />
    </Contracts>

    <Init returnStatus="true">
      <Implementation language="C"
          fragment="rotateIMU.h:rotate_imu_init_f32"
          call="rotate_imu_init_f32(rotation_matrix, rotation_deg, index)">
        <Conditional value="input.type == System.Float32" />
      </Implementation>
    </Init>

    <Implementations>
      <Implementation language="C"
          fragment="rotateIMU.h:rotate_imu_f32"
          call="rotate_imu_f32(input, rotation_matrix, output)">
        <Conditional value="input.type == System.Float32" />
      </Implementation>
    </Implementations>
  </Unit>
</Imaginet>
```

`rotateIMU.h`:

```c
#pragma IMAGINET_INCLUDES_BEGIN
#include <stdint.h>
#include <math.h>
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "rotate_imu_init_f32"
static inline int rotate_imu_init_f32(void* restrict rotation_matrix,
                                      int rotation_deg, int index)
{
    float* matrix = (float*)rotation_matrix;
    float rad = fmodf(rotation_deg, 360.0f) * (float)M_PI / 180.0f;
    float c = cosf(rad), s = sinf(rad);

    if (index == 0) {          // x-axis
        matrix[0]=1; matrix[1]=0; matrix[2]=0;
        matrix[3]=0; matrix[4]=c; matrix[5]=-s;
        matrix[6]=0; matrix[7]=s; matrix[8]=c;
    } else if (index == 1) {   // y-axis
        matrix[0]=c; matrix[1]=0; matrix[2]=s;
        matrix[3]=0; matrix[4]=1; matrix[5]=0;
        matrix[6]=-s; matrix[7]=0; matrix[8]=c;
    } else {                   // z-axis
        matrix[0]=c; matrix[1]=-s; matrix[2]=0;
        matrix[3]=s; matrix[4]=c;  matrix[5]=0;
        matrix[6]=0; matrix[7]=0;  matrix[8]=1;
    }
    return 0;  // 0 = success (required by returnStatus="true")
}
#pragma IMAGINET_FRAGMENT_END

#pragma IMAGINET_FRAGMENT_BEGIN "rotate_imu_f32"
static inline void rotate_imu_f32(const float* restrict input,
                                  void* restrict rotation_matrix,
                                  float* restrict output)
{
    const float* m = (const float*)rotation_matrix;
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 3; col++) {
            float sum = 0.0f;
            for (int k = 0; k < 3; k++)
                sum += m[col * 3 + k] * input[row * 3 + k];
            output[row * 3 + col] = sum;
        }
    }
}
#pragma IMAGINET_FRAGMENT_END
```

**Key points:**
- `<Init>` runs once at startup; `<Implementations>` runs every frame. Both reference fragments in the same `.h` file.
- `returnStatus="true"` means the init function must return `int` (0 = success). Without it, the function returns `void`.
- The Handle (`rotation_matrix`, 36 bytes = 9 floats) stores the precomputed rotation matrix. Init writes it, Body reads it.
- Two fragments in one `.h` file — each independently selected by the `.imunit`.

### 12.4 Streaming Lifecycle — Sliding Window

**What it teaches:** The full streaming lifecycle with `<Init>`, `<SoftReset>`, `<Enqueue>` (body), `<Dequeue>`, `<CanEnqueue>`, and `<CanDequeue>`. Also shows optional inputs, conditional outputs, `<ShapeOption>`, `<Tags>`, and computed output rate.

`SlidingWindow.imunit` (simplified for clarity — full version in `src/Imaginet.Units.Signal/TemporalAnalysis/SlidingWindow/`):

```xml
<?xml version="1.0" encoding="utf-8" ?>
<Imaginet>
  <Unit name="Imaginet.Units.Signal.SlidingWindow">
    <DisplayName>Sliding Window</DisplayName>
    <DisplayPath>/Signal Processing/Time Domain</DisplayPath>
    <Tags><ContextPushData/></Tags>
    <Description>
      <Header>Description</Header>
      Buffer streaming data into fixed-width overlapping windows using a
      circular buffer with configurable stride.
    </Description>

    <Parameters>
      <InputSocket name="input" text="Data Input" />
      <InputSocket name="time_input" timestamps="yes" text="Time Input" />

      <ShapeOption name="window_shape" text="Window shape" default="[128,3]" />
      <Int32Option name="stride" min="1" text="Stride" default="3" />

      <Expression name="input_size" value="input.shape.flat" />
      <Expression name="stride_count" value="stride / input_size" />
      <Expression name="window_count" value="window_shape.flat / input_size" />
      <Expression name="timestamp_count"
                  value="time_input == null ? 0 : time_input.shape.flat" />
      <Expression name="input_byte_size"
                  value="input.shape.flat * input.type.size" />
      <Expression name="data_buffer_byte_size"
                  value="input_byte_size * window_count" />
      <Expression name="time_buffer_byte_size"
                  value="time_input == null ? 0 : (window_count * 2 * time_input.type.size)" />

      <OutputSocket name="output" type="input.type" shape="window_shape"
                    rate="(input.rate * input_size) / Math.real(stride)"
                    rateIsApprox="true" />
      <OutputSocket name="time_output" type="System.Float32"
                    shape="System.Shape(2)" timestamps="yes"
                    conditional="time_input != null" />

      <Handle name="handle"
              size="208 + data_buffer_byte_size + time_buffer_byte_size" />
    </Parameters>

    <Contracts>
      <Assert test="stride &lt;= window_shape.flat"
              error="Stride ({stride}) can't exceed window size ({window_shape.flat})" />
      <Assert test="window_shape.flat % input_size == 0"
              error="Window ({window_shape.flat}) must be a multiple of input ({input_size})" />
      <Assert test="stride % input_size == 0"
              error="Stride ({stride}) must be a multiple of input ({input_size})" />
    </Contracts>

    <Init>
      <Implementation language="C" fragment="fixwin_time.h:fixwin_time_init"
                      call="fixwin_time_init(handle, input_byte_size, window_count)">
        <Conditional value="time_input != null" />
      </Implementation>
      <Implementation language="C" fragment="fixwin.h:fixwin_init"
                      call="fixwin_init(handle, input_byte_size, window_count)">
        <Conditional value="time_input == null" />
      </Implementation>
    </Init>

    <SoftReset>
      <!-- Reuses the same init fragments to reset state -->
      <Implementation language="C" fragment="fixwin_time.h:fixwin_time_init"
                      call="fixwin_time_init(handle, input_byte_size, window_count)">
        <Conditional value="time_input != null" />
      </Implementation>
      <Implementation language="C" fragment="fixwin.h:fixwin_init"
                      call="fixwin_init(handle, input_byte_size, window_count)">
        <Conditional value="time_input == null" />
      </Implementation>
    </SoftReset>

    <Enqueue returnStatus="true">
      <Implementation language="C" fragment="fixwin_time.h:fixwin_time_enqueue"
          call="fixwin_time_enqueue(handle, input, time_input, timestamp_count)">
        <Conditional value="time_input != null" />
      </Implementation>
      <Implementation language="C" fragment="fixwin.h:fixwin_enqueue"
                      call="fixwin_enqueue(handle, input)">
        <Conditional value="time_input == null" />
      </Implementation>
    </Enqueue>

    <Dequeue>
      <Implementation language="C"
          fragment="fixwin_time.h:fixwin_time_dequeue"
          call="fixwin_time_dequeue(handle, output, window_count, stride_count, time_output, merge_time)">
        <Conditional value="time_input != null" />
      </Implementation>
      <Implementation language="C" fragment="fixwin.h:fixwin_dequeue"
          call="fixwin_dequeue(handle, output, window_count, stride_count)">
        <Conditional value="time_input == null" />
      </Implementation>
    </Dequeue>

    <CanEnqueue>
      <Implementation language="C" fragment="fixwin_time.h:fixwin_time_can_enqueue"
                      call="fixwin_time_can_enqueue(handle, input_count)">
        <Conditional value="time_input != null" />
      </Implementation>
      <Implementation language="C" fragment="fixwin.h:fixwin_can_enqueue"
                      call="fixwin_can_enqueue(handle, input_count)">
        <Conditional value="time_input == null" />
      </Implementation>
    </CanEnqueue>

    <CanDequeue>
      <Implementation language="C" fragment="fixwin_time.h:fixwin_time_can_dequeue"
                      call="fixwin_time_can_dequeue(handle, input_count)">
        <Conditional value="time_input != null" />
      </Implementation>
      <Implementation language="C" fragment="fixwin.h:fixwin_can_dequeue"
                      call="fixwin_can_dequeue(handle, input_count)">
        <Conditional value="time_input == null" />
      </Implementation>
    </CanDequeue>
  </Unit>
</Imaginet>
```

**Key points:**
- **Full streaming lifecycle:** `<Init>` → `<CanEnqueue>` → `<Enqueue>` → `<CanDequeue>` → `<Dequeue>` → `<SoftReset>`.
- **`<SoftReset>` reuses Init fragments** — the same fragment reference can appear in multiple lifecycle sections.
- **Optional input branching:** Every lifecycle section dispatches on `time_input != null` vs `time_input == null`, selecting timestamp-aware or plain fragments.
- **Conditional output:** `time_output` only exists when `conditional="time_input != null"` — the socket disappears from the UI when timestamps aren't connected.
- **`<ShapeOption>`** lets the user configure the window shape directly (e.g. `[128,3]`).
- **Computed output rate:** `rate="(input.rate * input_size) / Math.real(stride)"` derives the output frequency from the input rate and stride.
- **`<Tags>`:** `<ContextPushData/>` tells the runtime this unit pushes data into the processing context.

### 12.5 Tensor Operations — Axis Decomposition & Broadcasting

**What it teaches:** Writing units that operate on arbitrary-rank tensors using the `step`/`size`/`slot` decomposition and the `broadcast` system. These patterns let one C function handle any shape without knowing the rank at compile time.

#### 12.5.1 Reduction Pattern — Sum

A reduction collapses one axis. The user picks which axis to reduce; the `.imunit` decomposes the shape; the C code sees only flat pointers and three loop bounds.

`Sum.imunit` (key sections):

```xml
<Parameters>
  <InputSocket name="input" />
  <Int32Option name="axis" min="0" max="9" ui="textbox" text="Axis" />
  <OutputSocket name="output" shape="input.shape.remove(axis)" type="input.type" />

  <Expression name="d0" value="input.shape.step(axis)" />
  <Expression name="d1" value="input.shape.size(axis)" />
  <Expression name="d2" value="input.shape.slot(axis)" />
</Parameters>

<Contracts>
  <Assert test="input.shape.count &gt;= 2"
          error="Sum requires input ({input.shape}) with two or more dimensions." />
  <Assert test="axis &lt; input.shape.count"
          error="Axis ({axis}) must be one of input dimensions (0...{input.shape.count-1})." />
</Contracts>

<Implementations>
  <Implementation language="C" fragment="sum.h:sum_f32"
                  call="sum_f32(input, d0, d1, d2, output)">
    <Conditional value="input.type == System.Float32" />
  </Implementation>
</Implementations>
```

`sum.h`:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "sum_f32"

static inline float __sum_f32(const float* restrict input, int step, int count)
{
    float sum = 0;
    for (int j = 0; j < count; j++) {
        sum += input[j * step];
    }
    return sum;
}

// d0 = step(axis)  — stride between elements along the target axis
// d1 = size(axis)  — number of elements to sum
// d2 = slot(axis)  — number of independent outer slices
static inline void sum_f32(
    const float* restrict input, const int d0, const int d1, const int d2,
    float* restrict output)
{
    const int full_step = d0 * d1;

    for (int j = 0; j < d2; j++) {
        const int j3 = j * full_step;
        const int j0 = j * d0;
        for (int i = 0; i < d0; i++) {
            output[i + j0] = __sum_f32(input + i + j3, d0, d1);
        }
    }
}
#pragma IMAGINET_FRAGMENT_END
```

**How it works for any rank:** If `input` has shape `[3,5,7]` and the user picks `axis=1`:

```
d0 = step(1) = 7       (stride — product of dims below axis 1)
d1 = size(1) = 5       (length of the axis being reduced)
d2 = slot(1) = 3       (outer slices — product of dims above axis 1)
output shape  = [3,7]  (axis 1 removed)
```

The outer loop runs 3 times (one per slice), the inner loop runs 7 times (one per stride element), and the helper sums 5 values spaced 7 apart. The same code handles `[100,3]` with `axis=0` just as well.

#### 12.5.2 Element-Wise Broadcast Pattern — Addition

When two inputs have different shapes, broadcasting lets the smaller tensor be "stretched" to match. The `.imunit` calls `shape.broadcast()` which decomposes the iteration into five segments.

`Add.imunit` (key sections):

```xml
<Parameters>
  <InputSocket name="a" />
  <InputSocket name="b" />
  <OutputSocket name="output" type="a.type" shape="a.shape" />

  <Expression name="bcast" value="a.shape.broadcast(b.shape)" conditional="b != null" />
  <Expression name="left"   value="bcast.left"   conditional="bcast != null" />
  <Expression name="group1" value="bcast.group1" conditional="bcast != null" />
  <Expression name="middle" value="bcast.middle" conditional="bcast != null" />
  <Expression name="group2" value="bcast.group2" conditional="bcast != null" />
  <Expression name="right"  value="bcast.right"  conditional="bcast != null" />
</Parameters>

<Implementations>
  <Implementation language="C" fragment="add.h:add_f32"
      call="add_f32(a, b, left, group1, middle, group2, right, output)">
    <Conditional value="a.type == System.Float32" />
    <Conditional value="b.type == System.Float32" />
  </Implementation>
</Implementations>
```

`add.h`:

```c
#pragma IMAGINET_FRAGMENT_BEGIN "add_f32"
static inline void add_f32(
    const float* restrict a,
    const float* restrict b,
    int l, int g1, int m, int g2, int r,
    float* restrict output)
{
    int index = 0;
    for (int x = 0; x < l; x++) {
        for (int i = 0; i < g1; i++) {
            for (int y = 0; y < m; y++) {
                for (int j = 0; j < g2; j++) {
                    for (int z = 0; z < r; z++) {
                        output[index] = a[index] + b[x * m * r + y * r + z];
                        index++;
                    }
                }
            }
        }
    }
}
#pragma IMAGINET_FRAGMENT_END
```

**How broadcasting works:** The algorithm walks dimensions from innermost to outermost, grouping them into alternating "same-size" and "broadcast" (size-1 in `b`) regions:

```
Dims scanned from axis 0 →   same   bcast   same   bcast   same
Five segments:                right  group2  middle group1  left
```

When both shapes are identical, the broadcast groups collapse to 1 and the five-loop nest degenerates into a single linear pass. When `b` is a scalar (shape `[1]`), `right=1`, `group2=flat`, and the inner loops broadcast `b[0]` across every element of `a`.

Example: `a=[4,3,5]`, `b=[1,3,5]` — axis 2 is broadcast:

```
right  = 5×3 = 15   (axes 0,1 match)
group2 = 4           (axis 2 is broadcast: size 4 in a, size 1 in b)
middle = 1, group1 = 1, left = 1   (no more dimensions)
```

#### 12.5.3 Shape Manipulation — Concatenate

Some units change the shape without reducing or broadcasting. Concat joins tensors along an axis, growing that dimension:

`Concat.imunit` (key sections):

```xml
<Parameters>
  <InputSocket name="i0" />
  <InputSocket name="i1" optional="true" />
  <Int32Option name="axis" min="0" max="9" ui="textbox" text="Axis" />

  <Expression name="size0" value="i0.shape.size(axis)" />
  <Expression name="size1" value="i1 == null ? 0 : i1.shape.size(axis)" />
  <Expression name="output_shape" value="i0.shape.replace(axis, size0 + size1)" />

  <Expression name="step"  value="output_shape.step(axis)" />
  <Expression name="slot"  value="output_shape.slot(axis)" />
  <Expression name="bytes0" value="size0 * step * i0.type.size" />
  <Expression name="bytes1" value="size1 * step * i0.type.size" />

  <OutputSocket name="output" type="i0.type" shape="output_shape" />
</Parameters>

<Contracts>
  <Assert test="i0.shape.remove(axis) == i1.shape.remove(axis)"
          error="All tensors must have the same shape except at given axis" />
</Contracts>
```

The C code uses `memcpy` in a `slot` loop, copying `bytes0` from `i0` then `bytes1` from `i1` for each outer slice — again completely rank-agnostic.

Other shape manipulations follow the same idea:
- **Stack** (add a new axis): `output_shape = i0.shape.insert(axis, count_inputs)`
- **DotT** (matrix multiply): `output = a.shape.remove(0).insert(1, b.shape.size(1))`
- **Select** (slice): `output = input.shape.replace(axis, count)` with offset arithmetic

#### 12.5.4 Design Principle

Shapes and axes are a **compile-time concern** handled entirely by `.imunit` expressions. The C code never knows the tensor rank — it only receives flat `float*` pointers and integer loop bounds. This separation is what makes one C function work for any dimensionality.

When writing a new tensor operation:

1. Decide which axis the operation targets.
2. Decompose with `step(axis)` / `size(axis)` / `slot(axis)`.
3. Compute the output shape with `remove`, `replace`, or `insert`.
4. Pass the integers to C and write loops over `slot` (outer), `step` (inner), `size` (along the axis).
5. For multi-input operations with different shapes, use `broadcast()`.

### 12.6 Tips & Patterns

Short recipes for common situations. These show the `.imunit` XML only — the C side follows the same conventions shown in the tutorials above.

#### Temporary Buffers

Allocate scratch memory for intermediate computation with `System.Tensor()`:

```xml
<Expression name="scratch" value="System.Tensor(System.UInt8, package_size)" />
<Expression name="work_buf" value="System.Tensor(System.Float32, fft_size)" />
```

Pass `true` as the third argument to make the buffer static (persistent across calls, placed in the Static memory segment):

```xml
<Expression name="twiddle"
            value="System.Tensor(System.Int32, Math.int(Math.sqrt(n) + 2), true)" />
```

#### Dynamic Output Shape

Compute output dimensions from input shape and user parameters:

```xml
<Expression name="out_rows"
    value="padding == 'same'
           ? ceil(real(input.shape.size(1)) / real(strides))
           : ceil(real(input.shape.size(1) - kernel_size + 1) / real(strides))" />

<OutputSocket name="output" type="input.type"
              shape="System.Shape(out_rows, filters)" />
```

#### Optional Inputs

Mark an input as optional. It evaluates to `null` when unconnected — dispatch with conditionals:

```xml
<InputSocket name="timestamps" optional="true" timestamps="true" />

<Implementations>
  <Implementation language="C" fragment="op.h:op_with_time"
                  call="op_with_time(input, timestamps, output)">
    <Conditional value="timestamps != null" />
  </Implementation>
  <Implementation language="C" fragment="op.h:op_no_time"
                  call="op_no_time(input, output)">
    <Conditional value="timestamps == null" />
  </Implementation>
</Implementations>
```

#### Global Options

Parameters shared across all units that declare the same global. Useful for project-wide API or backend selection:

```xml
<StringOption name="global_tf_api" text="TF API" default="mtb-ml-middleware"
              global="true">
  <OneOf>
    <Item text="MTB Middleware">mtb-ml-middleware</Item>
    <Item text="TF Lite Micro">tflu</Item>
    <Item text="TF Lite">tfl</Item>
  </OneOf>
</StringOption>
```

Every unit that declares `global_tf_api` with `global="true"` shares the same value.

#### Label Metadata on Outputs

Attach semantic labels to output dimensions for downstream visualisation:

```xml
<Expression name="class_names"
    value="detections.shape.getLabels(confidence_axis, class_offset, num_classes)" />
<Expression name="output_labels"
    value="'x,y,w,h,' + class_names + ',track_id,confidence'" />

<OutputSocket name="output" type="detections.type"
    shape="output_shape.withLabels(confidence_axis, output_labels)" />
```

#### Python Reference Implementation

Provide a Python implementation alongside C for desktop simulation. The Python implementation is used when running on desktop (non-embedded) and acts as a reference. From the Abs unit:

```xml
<Implementations>
  <Implementation language="C" fragment="abs.h:abs_f32"
                  call="abs_f32(input, count, output)">
    <Conditional value="input.type == System.Float32" />
  </Implementation>
  <Implementation language="Python" fragment="abs.py:abs"
                  call="abs(input, output)" />
</Implementations>
```

Python fragments use the same pragma structure as C:

```python
#pragma IMAGINET_INCLUDES_BEGIN
import numpy as np
#pragma IMAGINET_INCLUDES_END

#pragma IMAGINET_FRAGMENT_BEGIN "abs"
def abs(input, output):
    np.absolute(input, out=output)
#pragma IMAGINET_FRAGMENT_END
```

---

## 13. Quick Reference

### Checklist — Creating a New Unit

1. **Create folder** under the appropriate `Imaginet.Units.*` project.
2. **Write the `.imunit` file** — set a unique `name`, add `<DisplayName>` and `<DisplayPath>`, declare parameters, contracts, and implementations.
3. **Write the `.h` fragment file** — wrap includes in `IMAGINET_INCLUDES_BEGIN`/`END`, wrap each function in `IMAGINET_FRAGMENT_BEGIN "name"`/`END`. Use `static inline`, `restrict`, `const`.
4. **Add lifecycle sections** if needed (`<Init>`, `<Handle>`, `<Destructor>`, `<SoftReset>`).
5. **Test** by adding the unit to a graph in DEEPCRAFT Studio and running code generation.

### XML Escaping

| Character | Escape | Example |
|---|---|---|
| `&` | `&amp;` | `a &amp;&amp; b` |
| `<` | `&lt;` | `a &lt; b` |
| `>` | `&gt;` | `a &gt; b` |
| `"` | `&quot;` | `mode == &quot;clamp&quot;` |

Tip: Use single quotes for strings (`padding == 'same'`) to avoid `&quot;`.

### Legacy Aliases — Don't Use in New Code

Many elements have old names that the parser still accepts. Always use the preferred name:

| Legacy (deprecated) | Preferred | Notes |
|---|---|---|
| `<Function>` | `<Unit>` | Root element type |
| `<Body>` | `<Implementations>` | Main processing body |
| `<NewDomain>` | `<Dequeue>` | Streaming data read/produce |
| `<Status>` | `<CanEnqueue>` | Streaming data-available guard |
| `<BooleanOption>` | `<BoolOption>` | Boolean parameter |
| `<Hidden>` | `<Expression>` | Computed value |
| `System.Array(...)` | `System.Tensor(...)` | Allocate temporary buffer |
| `System.ArrayBuffer(...)` | `System.Tensor(...)` | Allocate temporary buffer |

### Template Variables

C fragments support `{{variable}}` substitution during code generation:

| Variable | Expands To |
|---|---|
| `{{PROJECT_PREFIX_UPPERCASE}}` | Project prefix (e.g. `MY_APP`) |
