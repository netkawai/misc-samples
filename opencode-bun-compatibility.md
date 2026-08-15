# OpenCode vs Bun CPU Compatibility Analysis

## Problem
User's system has a CPU that doesn't support AVX2 instructions (Intel i5-3210M with AVX but not AVX2), causing official `bun` package to crash with "Illegal instruction".

## Findings

### CPU Specifications
- **CPU**: Intel i5-3210M (Ivy Bridge)
- **Supports**: SSE4.2, AVX (version 1)
- **Missing**: AVX2, FMA, BMI1, BMI2, LZCNT, MOVBE (x86_64-v3 requirements)

### Binary Analysis
#### System Bun (`/usr/bin/bun`):
- Compiled for x86_64-v3 microarchitecture level
- Requires AVX2 instruction set
- **Crashes** on user's CPU

#### OpenCode (`/usr/bin/opencode`):
- Uses baseline build targeting SSE4.2
- **Works** on user's CPU
- Bundles its own bun runtime (baseline version)

### Package Differences

**Arch Linux Bun Package:**
- Single binary compiled for x86_64-v3 (AVX2 required)
- No baseline version available in official repos
- Installs to `/usr/bin/bun`

**Arch Linux OpenCode Package:**
1. **Downloads pre-built OpenCode binary** (`opencode-linux-x64-baseline`)
2. **Downloads bun baseline** (`bun-linux-x64-baseline.tar.gz`) 
3. **Packages both together**
4. OpenCode binary: `/usr/bin/opencode`
5. Bun baseline: `/usr/share/opencode/`

### Build Process
- **OpenCode PKGBUILD** doesn't compile anything
- Downloads all binaries pre-built from GitHub
- No `makedepends` required for the Arch package
- Uses `--baseline` flag during OpenCode's own build process

### Technical Details
- **x86_64-v3**: Requires AVX2 (Intel Haswell+, AMD Excavator+, 2013+)
- **SSE4.2 baseline**: Works on older CPUs (including Ivy Bridge)
- Both binaries have `cpuid` calls for runtime CPU detection
- No `.note.gnu.property` sections found (ISA requirements not in ELF notes)

## Summary
OpenCode works on older CPUs because it bundles bun's **baseline** (SSE4.2) build, while the official bun package is compiled for **x86_64-v3** (AVX2). OpenCode's Arch package downloads pre-built binaries rather than compiling locally.