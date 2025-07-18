# Branch Protection Configuration
# Use this as a reference to configure branch protection rules in GitHub

## Required Status Checks for Main Branch

To prevent merging to main if builds fail, configure the following in your GitHub repository settings:

### Go to: Settings → Branches → Add rule (for main branch)

**Branch name pattern:** `main`

**Protect matching branches:**
- ✅ Require a pull request before merging
- ✅ Require status checks to pass before merging
- ✅ Require branches to be up to date before merging

**Required status checks:**
- `build-firmware (ubuntu-latest, megaatmega2560)`
- `build-firmware (ubuntu-latest, nanoatmega328)`
- `build-firmware (windows-latest, megaatmega2560)`
- `build-firmware (windows-latest, nanoatmega328)`
- `build-firmware (macos-latest, megaatmega2560)`
- `build-firmware (macos-latest, nanoatmega328)`
- `build-status`

**Additional recommended settings:**
- ✅ Restrict pushes that create files that do not match patterns
- ✅ Require linear history (optional)
- ✅ Include administrators (recommended)

## What this protects against:

1. **Build failures** - Code that doesn't compile on any platform will be blocked
2. **Platform-specific issues** - Problems that only occur on Windows, Linux, or macOS
3. **Environment-specific issues** - Problems specific to Arduino Mega or Nano configurations
4. **Dependency issues** - Missing or incompatible dependencies

## Workflow Features:

- **Multi-platform builds**: Tests on Ubuntu, Windows, and macOS
- **Multi-environment builds**: Tests both Arduino Mega 2560 and Nano 328 configurations
- **Artifact preservation**: Saves compiled firmware files for 7 days
- **Smart triggering**: Only runs when firmware code or CI config changes
- **Fail-fast disabled**: All combinations are tested even if one fails
