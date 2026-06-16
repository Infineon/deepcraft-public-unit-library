# DEEPCRAFT™ Studio: Contributing Units

This guide describes the recommended workflow for **developing, testing, and submitting** a unit to the public unit library. It is aimed at external contributors who are not part of the DEEPCRAFT™ product team.

If you have not built a unit before, start with [CreatingUnits.md](CreatingUnits.md), which walks through creating Native and Graph units inside DEEPCRAFT™ Studio. This guide picks up from there and focuses on how to get your finished unit reviewed and merged.

---

## Overview

The recommended workflow is: **develop in Studio, then submit via Git**.

You build and test the unit inside DEEPCRAFT™ Studio, where you can run it against live signals and see results immediately. Once you're happy with how it behaves, you copy the unit files into your fork and open a pull request.

One thing worth knowing upfront: you don't need to worry about whether DEEPCRAFT™ Studio itself compiles with your unit, that's checked by the reviewer as part of the review process. Your job is to make sure the unit works correctly in the graph and that the files are in the right place.

---

## Workflow

### 1. Set up your fork

1. **Fork** [Infineon/deepcraft-public-unit-library](https://github.com/Infineon/deepcraft-public-unit-library) on GitHub.
2. **Clone** your fork locally.

No tooling is required beyond Git and DEEPCRAFT™ Studio.

### 2. Develop the unit in Studio

Create the unit inside a Studio Graph Project, following [CreatingUnits.md](CreatingUnits.md). Iterate on the behavior here:

- Drag the unit into the graph, connect a capture device and data tracks, and run the graph.
- Compare the unit's output against an unaltered signal to confirm it does what you expect.

This is where you catch and fix logic bugs, while you have live signals and immediate visual feedback.

### 3. Copy the unit into your fork

Once the unit behaves correctly, copy the **entire unit folder** from Studio's unit directory into the appropriate category in your fork:

- `Imaginet.Units.Signal/` : signal processing, audio, filters, image processing, IMU, radar, etc.
- `Imaginet.Units.Math/` : element-wise, fixed-point, reductions, and other math operations.

A unit folder typically contains:

- The `.imunit` file
- The `.h` C fragment file
- An optional `.py` reference implementation

### 4. Self-review before opening a PR

Check the following before you submit:

- [ ] The unit folder is placed in the correct category (`Imaginet.Units.Signal/` or `Imaginet.Units.Math/`).
- [ ] The `.imunit` file declares the unit correctly (the `name` attribute follows the existing naming convention).
- [ ] No files outside your unit folder were accidentally modified.

### 5. Open a pull request

Open a PR from your fork against the `main` branch of the upstream repository. In the PR description:

- Summarize what the unit does and its intended use case.

### 6. DEEPCRAFT™ review

A DEEPCRAFT™ developer will:

- Build DEEPCRAFT™ Studio with your unit included and confirm it compiles.
- Test the unit's behavior.
- Either merge, request changes, or make minor fixes directly.
- Add tests for the unit where appropriate.

You do not need to take any action during review unless changes are requested.

---

## Summary

```
Fork ──► Develop & test in Studio ──► Copy unit into fork ──► Self-review ──► PR ──► DEEPCRAFT review
```
