#!/usr/bin/env python3
"""Create a new output directory outside the supplied source/build roots."""
import argparse
from pathlib import Path


def create_output_directory(output, roots):
    target = Path(output)
    # is_symlink also rejects dangling links, for which exists is false.
    if target.is_symlink() or target.exists():
        raise ValueError('output must not already exist or be a symbolic link')
    resolved = target.resolve(strict=False)
    for root in roots:
        source = Path(root).resolve(strict=True)
        if not source.is_dir():
            raise ValueError('source/build root must be a directory')
        if resolved == source or resolved.is_relative_to(source):
            raise ValueError('output must be outside every source/build root')
    # Do not reuse a destination created between validation and mkdir.
    resolved.mkdir(parents=True, exist_ok=False)
    return resolved


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--root', required=True, action='append', type=Path)
    args = parser.parse_args()
    try:
        output = create_output_directory(args.output, args.root)
    except (OSError, RuntimeError, ValueError) as error:
        parser.exit(2, f'Invalid output directory: {error}\n')
    print(output)


if __name__ == '__main__':
    main()
