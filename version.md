# version.md

## Purpose
Track all repository changes in one place.
This file must be updated whenever a change is planned or completed.

## Change Log

| Date (UTC) | Status | Area | Change |
|---|---|---|---|
| 2026-09-11 | Done | Build | Added macOS (Apple Silicon) build support in root `CMakeLists.txt`: Apple branch for `haralick-analysis` uses `find_package(OpenMP)` + `OpenMP::OpenMP_CXX` (AppleClang/libomp instead of `-fopenmp`/`gomp`) and links mlpack 3 exactly like Linux (`mlpack armadillo`); Linux behavior unchanged. Verified full build and smoke tests against source-built mlpack 3.4.2. |
| 2026-09-11 | Done | Build | Added `Scripts/build_opensidescan_mac.sh` (Homebrew keg-only prefixes via `CMAKE_PREFIX_PATH` plus explicit include/link/`-Wl,-rpath` flags for `$MLPACK3_PREFIX`, default `~/opt/mlpack-3.4.2`) and `Scripts/build_mlpack3_mac.sh` (library-only source build of mlpack 3.4.2, the version the project links against, with CMake 4 and armadillo 15 compatibility patches; Homebrew ships only header-only mlpack 4). |
| 2026-09-11 | Done | CI | Added macOS CI build: `build_macos` job on `macos-latest` installing Homebrew `qt@5`, `eigen@3`, `libomp`, `armadillo`, `boost`, `cereal`, `ensmallen`, then `opencv@4` after `brew unlink qt@5` (Qt 6 `qtbase` binary conflict), cached mlpack 3.4.2 bootstrap via `Scripts/build_mlpack3_mac.sh`, build via `Scripts/build_opensidescan_mac.sh`, binary artifact upload. |
| 2026-09-11 | Done | Build | Anchored the `.gitignore` `build*` pattern to the repository root (`/build*`): the unanchored pattern silently ignored the new `Scripts/build_*.sh` scripts, which would have been skipped by a plain `git add .` and broken the macOS CI job. |
| 2026-02-19 | Done | Docs | Added repository governance and baseline analysis file: `AGENTS.md`. |
| 2026-02-19 | Done | Docs | Added repository version audit file: `versions.md`. |
| 2026-02-19 | Done | Docs | Added change tracking file: `version.md`. |
| 2026-02-19 | Done | CI | Migrated CI from Jenkins to GitHub Actions in `.github/workflows/ci.yml` with Linux and Windows jobs mapped from `Jenkinsfile`. |
| 2026-02-19 | Done | CI | Added conditional Windows signing/packaging flow in GitHub Actions (`sign_and_package_windows`) for executable and installer signing. |
| 2026-02-19 | Done | Security | Added optional certificate-secret signing path (`WINDOWS_SIGN_CERT_B64`, `WINDOWS_SIGN_CERT_PASSWORD`) with fallback to machine certificate store signing. |
| 2026-02-19 | Done | CI | Removed all `self-hosted` runners and moved Windows jobs to `windows-latest`. |
| 2026-02-19 | Done | Security | Disabled in-workflow signing and added CI note to use either a cloud/HSM signing service or a dedicated signing host. |
| 2026-02-19 | Done | CI | Added Windows dependency bootstrap in CI (`Qt 5.15.2` via `install-qt-action`, `OpenCV`/`Eigen` via `vcpkg`) and legacy Eigen path provisioning (`C:\\LIBS\\eigen-3.4.0`). |
| 2026-02-19 | Done | Testing | Fixed Catch2 build failure on modern Linux headers by disabling POSIX signal handling in Catch test entry points (`CATCH_CONFIG_NO_POSIX_SIGNALS`). |
| 2026-02-19 | Done | Build | Updated `test/linuxFileLockTest/CMakeLists.txt` minimum CMake version from `2.8` to `3.10` to remove CI deprecation warning. |
| 2026-02-19 | Done | Testing | Hardened Catch2 workaround at build-system level by adding `CATCH_CONFIG_NO_POSIX_SIGNALS` compile definitions in test CMake targets. |
| 2026-02-19 | Done | Build | Fixed root CMake warning by setting default `PROJECT_VERSION` to `0.1.0` when not provided by CI. |
| 2026-02-19 | Done | CI | Replaced heavy Windows `vcpkg` source builds with prebuilt OpenCV/Eigen download and merged Windows lock/unit/build into a single `windows_ci` job to avoid duplicate dependency setup. |
| 2026-02-19 | Done | CI | Added dependency caching for Windows CI: `actions/cache` for OpenCV/Eigen under `C:\\LIBS` and Qt cache via `install-qt-action` cache mode. |
| 2026-02-19 | Done | CI | Fixed Windows CI compatibility by switching OpenCV prebuilt package to `4.12.0`, adding MSVC environment setup (`ilammy/msvc-dev-cmd`), and auto-detecting OpenCV runtime bin folder (`vc17`/`vc16`/`vc15`). |
| 2026-02-19 | Done | Testing | Made `XTF Support Test` robust in CI: if dataset is unavailable, test is skipped unless `OPENSIDESCAN_REQUIRE_XTF_DATASET=1` is set. |
| 2026-02-19 | Done | Build | Removed internal dataset download attempts (`jenkins.cidco.local`) from `test/CMakeLists.txt`; dataset is now treated as optional external input. |
| 2026-02-19 | Done | CI | Removed fake `/opt/XTF_Support_test_dataset.zip` creation in Linux CI and explicitly run tests with `OPENSIDESCAN_REQUIRE_XTF_DATASET=0`. |
| 2026-02-19 | Planned | Build | Define a non-empty canonical project version in `CMakeLists.txt` and propagate it to packaging metadata. |
| 2026-02-19 | Planned | CI | Replace hardcoded self-hosted runner labels with organization-standard labels after runner registration is finalized. |
| 2026-02-19 | Planned | Testing | Remove internal dataset path assumptions (`/opt`, internal host) from Linux unit-test CMake configuration. |
| 2026-02-19 | Done | Build | Suppressed third-party World Magnetic Model float-truncation warnings (`C4305` / `-Wfloat-conversion`) at the integration point in `inventoryobject.cpp` via compiler-specific diagnostic push/pop around `wmm_calculations.cpp`. |
| 2026-02-19 | Done | Build | Reduced Windows third-party warning noise by adding MSVC-targeted suppression for `dep`, `lockcath-dep`, and `unittest_dep` (`_CRT_SECURE_NO_WARNINGS`, `/wd4996 /wd4244 /wd4267 /wd4200 /wd4305`). |
| 2026-02-19 | Done | Build | Suppressed remaining third-party WMM unused-local warning at integration boundary (`C4101` / `-Wunused-variable`) around `wmm_calculations.cpp` include. |
| 2026-02-19 | Done | Testing | Reduced `win-fileLock-test` warning noise by aligning MSVC suppression on `wincatchLockTest` and avoiding Catch macro redefinition (`C4005`) with a guarded `CATCH_CONFIG_NO_POSIX_SIGNALS` define. |
| 2026-02-19 | Done | Testing | Fixed `Scripts/winlocktest.bat` to preserve working directory and return proper error codes (`pushd/popd` + explicit `exit /b`), preventing post-build path errors in CI. |
| 2026-02-19 | Planned | CI | Re-pin Windows OpenCV dependency from `4.12.0` to `4.5.5` to restore historical unit-test behavior on `windows-latest`. |
| 2026-02-19 | Done | CI | Re-pinned Windows CI OpenCV bootstrap/cache to `4.5.5` (`opencv-4.5.5-vc14_vc15.exe`) while keeping MSVC setup and runtime bin-path auto-detection. |
| 2026-02-19 | Done | Testing | Hardened `Scripts/win-unittest.bat` with idempotent directory creation and explicit CMake/MSBuild error propagation using `pushd/popd` and `exit /b`. |
| 2026-02-19 | Planned | Build | Make Windows OpenCV detection resilient to MSVC toolset changes by auto-selecting available runtime folders (`vc17/vc16/vc15/vc14`) in CMake. |
| 2026-02-19 | Done | Build | Updated Windows CMake configuration in `CMakeLists.txt`, `test/CMakeLists.txt`, and `test/win-fileLock-test/CMakeLists.txt` to set `OpenCV_RUNTIME` explicitly and locate OpenCV from `C:/LIBS/opencv/build` using runtime-folder auto-detection. |
| 2026-02-19 | Done | Build | Replaced hardcoded Windows OpenCV DLL install path (`vc15/opencv_world454.dll`) with runtime-aware globbing of `opencv_world*.dll`. |
| 2026-02-19 | Planned | CI | Ensure NSIS is installed on `windows-latest` before packaging so `cpack -G NSIS` is available in CI. |
| 2026-02-19 | Done | CI | Added explicit Windows CI NSIS bootstrap step in `.github/workflows/ci.yml` (`choco install nsis`, `makensis` PATH export, and version check). |
| 2026-02-19 | Done | Build | Hardened `Scripts/build_installer.bat` with idempotent build directory creation and explicit CMake/CPack error propagation using `pushd/popd` and `exit /b`. |
| 2026-02-19 | Done | CI | Hardened NSIS bootstrap path resolution in `.github/workflows/ci.yml` to find `makensis.exe` in Chocolatey and NSIS `Bin` layouts (`C:\\ProgramData\\chocolatey\\bin`, `Program Files*\\NSIS\\Bin`). |
| 2026-02-26 | Done | CI | Fixed Windows NSIS bootstrap on GitHub Actions by resolving `ProgramFilesX86`/`ProgramFiles` via PowerShell APIs (avoiding `$env:ProgramFiles(x86)` interpolation issues) and adding a recursive `makensis.exe` fallback search under NSIS/Chocolatey locations. |
| 2026-02-26 | Done | Build | Fixed Windows packaging Qt runtime install paths in `CMakeLists.txt` by resolving the Qt prefix from the detected `Qt5` package (`Qt5_DIR`) instead of hardcoded `C:/Qt/.../msvc2019_64`, and added support for both `win64_msvc2019_64` and `msvc2019_64` Qt layouts. |

## Update Rule
When a change starts, add a `Planned` entry.
When it is completed, update that entry to `Done` (or add a new `Done` line with details).
