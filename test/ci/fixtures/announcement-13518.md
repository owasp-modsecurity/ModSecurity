<!-- Format based on https://github.com/actions/runner-images/issues/13518 -->
### Breaking changes

Retirement of macOS 14 and macOS 14 Arm64 runner images.
Brownouts: October 29, 14:00 UTC and October 30, 14:00 UTC.

### Target date

Deprecation: July 6th, 2026
Retirement: November 2nd, 2026

### Possible impact

The labels `macos-14`, `macos-14-large`, `macos-14-xlarge` will stop running.

### Runner images affected

- [x] macOS 14
- [x] macOS 14 Arm64
- [ ] macOS 15

### Mitigation ways

Use `macos-15` or `macos-26` instead.
