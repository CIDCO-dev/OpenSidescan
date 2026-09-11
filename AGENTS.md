# AGENTS.md

## Purpose
This document summarizes the technical baseline of `OpenSidescan`, the CI migration status, and maintenance rules for repository-level changes.

## Maintenance Rule
- Keep `version.md` updated for every planned and completed change.
- Any CI, build, test, packaging, or documentation modification must be recorded in `version.md`.

## Repository Snapshot
- Main language: C++
- Domain: Side-scan sonar processing, detection, visualization, and installer packaging
- Main build system: CMake + platform scripts (`Scripts/*.sh`, `Scripts/*.bat`)
- Third-party code strategy: git submodules (`MBES-lib`, `QDarkStyleSheet`)

## CI Baseline
- Legacy CI: `Jenkinsfile`
- New CI target: GitHub Actions workflow in `.github/workflows/ci.yml`
- Migrated flows:
  - Linux file lock tests
  - Linux unit test build/run
  - Linux application build
  - macOS application build (`macos-latest`)
  - Windows file lock tests (`windows-latest`)
  - Windows unit tests (`windows-latest`)
  - Windows application build (`windows-latest`)
  - In-CI signing is disabled by policy note (HSM service or dedicated signing host recommended)

## Dependencies
### Linux build/test
- `build-essential`, `cmake`
- `libeigen3-dev`, `libopencv-dev`, `qtbase5-dev`

### macOS build (Homebrew, Apple Silicon)
- `qt@5`, `opencv@4`, `eigen@3`, `libomp`, `armadillo`, `boost`, `cereal`, `ensmallen`
- `qt@5` must be unlinked before installing `opencv@4` (its Qt 6 `qtbase` dependency conflicts on linked binaries); the build consumes keg-only prefixes via `CMAKE_PREFIX_PATH` (see `Scripts/build_opensidescan_mac.sh`)
- mlpack 3.4.2 is built from source into `~/opt/mlpack-3.4.2` by `Scripts/build_mlpack3_mac.sh` (Homebrew only ships header-only mlpack 4; OpenSidescan links against mlpack 3)

### Windows build/test
- MSBuild toolchain
- OpenCV and Qt available at paths expected by existing CMake files
- Existing project scripts in `Scripts/*.bat`

### Windows signing
- `signtool.exe` installed on signing runner
- Code-signing certificate available either:
  - in the machine certificate store (auto-select mode `/a`), or
  - through GitHub secrets (`WINDOWS_SIGN_CERT_B64`, `WINDOWS_SIGN_CERT_PASSWORD`)
- Timestamp service: `http://timestamp.digicert.com`

## Known Technical Risks
1. Root `CMakeLists.txt` defines an empty `PROJECT_VERSION`, which triggers a CMake warning.
2. Linux unit-test CMake uses legacy dataset download logic targeting `/opt` and internal URL references.
3. Windows CMake files currently rely on hardcoded local dependency paths (`C:/LIBS`, `C:/Qt`).

## Recommended Follow-up
1. Define a canonical project version in CMake and propagate it to CI and packaging.
2. Replace hardcoded Windows dependency paths with a package-manager/toolchain approach.
3. Decouple test datasets from internal-only hosts and non-portable absolute paths.
