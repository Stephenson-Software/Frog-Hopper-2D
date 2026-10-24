# Releasing

A release is a tag on `master` named `v` and three numbers (`v1.1.0`), with a GitHub release on
it. The tag must be `v` followed by the version in [`version.txt`](../version.txt). The earlier
release, `v1.0`, predates this rule.

Pushing the tag, or publishing a release on it, starts
[`.github/workflows/release.yml`](../.github/workflows/release.yml), which builds the game for
Linux x64 and Windows x64 and attaches the downloads. A pushed tag with no release yet gets a
full release created for it, with generated notes.

## Steps

1. **Set the version.** On a branch, set `version.txt` to the version being released (three
   numbers, `1.1.0`) and open a pull request. The game reports this version, and the release
   workflow refuses a tag that does not match it.
2. **Tag the merged commit** with its full SHA, so the tag lands on exactly the commit that was
   checked:

   ```bash
   git fetch origin
   git tag -a v1.1.0 -m "Frog Hopper 1.1.0" "$(git rev-parse origin/master)"
   git push origin v1.1.0
   ```

3. **Wait for the builds.** After a few minutes the release has four assets:

   | Asset | Contents |
   | --- | --- |
   | `FrogHopper2D-<version>-win-x64.zip` | `frogHopper.exe`, the SDL2 DLLs, `resources/`, `README.txt`, `LICENSE.txt` |
   | `FrogHopper2D-<version>-linux-x64.tar.gz` | `frogHopper_executable`, `resources/`, `README.txt`, `LICENSE.txt` |
   | `*.sha256` | the SHA-256 of each archive |

   If the workflow fails, fix the cause and re-run it (`gh run rerun <id>`); uploads replace
   assets of the same name.

## What each build checks

- **Linux** (built on Ubuntu 22.04): the executable links nothing but the C and C++ runtime and
  the system's SDL2 and SDL2_image, which `README.txt` names. Started from another directory with
  SDL's dummy video driver, the packaged game finds its resources and keeps running.
- **Windows** (MSYS2 MinGW-w64): the DLLs the executable needs are copied next to it, and the
  same start-up check runs with only those DLLs on `PATH`.

## Checking the packaging without a release

A pull request that changes `release.yml`, the `Makefile` or `version.txt` runs both builds as a
dry run and keeps the archives as workflow artifacts. So does running it by hand:

```bash
gh workflow run release.yml --ref <branch>               # dry run
gh workflow run release.yml --ref master -f tag=v1.1.0   # build a tag and attach to its release
```
