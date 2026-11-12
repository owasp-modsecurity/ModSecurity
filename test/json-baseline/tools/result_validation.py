"""Strict case accounting shared by the JSON profile runner and its tests."""

STATUSES = ('PASS', 'FAIL', 'SKIP', 'BLOCKED', 'XFAIL')


def zeros():
    return {status: 0 for status in STATUSES}


def validated_counts(report):
    if not isinstance(report, dict):
        raise ValueError('case report must be a JSON object')
    for field in ('cases', 'sensitivity'):
        if not isinstance(report.get(field, []), list):
            raise ValueError('case report ' + field + ' must be an array')
    if not report.get('cases'):
        raise ValueError('case report contains no behavior cases')
    counts = report.get('counts')
    if not isinstance(counts, dict) or set(counts) != set(STATUSES):
        raise ValueError('report lacks a complete count summary')
    if any(type(number) is not int or number < 0 for number in counts.values()):
        raise ValueError('report contains invalid counts')
    entries = report.get('cases', []) + report.get('sensitivity', [])
    if not entries:
        raise ValueError('report contains no actual cases or assertion controls')
    actual = zeros()
    for entry in entries:
        if not isinstance(entry, dict) or entry.get('status') not in STATUSES:
            raise ValueError('case report contains an invalid case/status')
        actual[entry['status']] += 1
    if actual != counts:
        raise ValueError('summary counts do not match individual report records')
    if counts['XFAIL']:
        raise ValueError('this package does not mask requirements with XFAIL')
    return counts.copy()


def native_counts(cases, total, failed, skipped, disabled):
    """Account for every fixture, including disabled cases, without masking it."""
    if not isinstance(cases, list) or not cases:
        raise ValueError('Native fixture must be a nonempty array')
    if not all(isinstance(case, dict) for case in cases):
        raise ValueError('Native fixture entries must be objects')
    if any(type(n) is not int or n < 0 for n in (total, failed, skipped, disabled)):
        raise ValueError('Native runner contains invalid counts')
    if failed > total:
        raise ValueError('Native failures exceed executed tests')
    if total + skipped + disabled != len(cases):
        raise ValueError('Native runner did not account for every fixture')
    counts = zeros()
    counts.update(PASS=total-failed, FAIL=failed, SKIP=skipped, BLOCKED=disabled)
    return counts
