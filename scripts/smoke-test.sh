#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
capture_path="${1:-${repo_root}/kfs-smoke-test.ppm}"
qemu_capture_path="/tmp/kfs-smoke-test-$$.ppm"

cd "${repo_root}"
rm -f "${capture_path}" "${qemu_capture_path}"

{
	sleep 4
	printf 'sendkey t\n'
	printf 'sendkey e\n'
	printf 'sendkey s\n'
	printf 'sendkey x\n'
	printf 'sendkey backspace\n'
	printf 'sendkey t\n'
	printf 'sendkey ret\n'
	sleep 1
	printf 'sendkey f2\n'
	sleep 1
	printf 'sendkey t\n'
	printf 'sendkey e\n'
	printf 'sendkey s\n'
	printf 'sendkey t\n'
	printf 'sendkey ret\n'
	sleep 2
	printf 'screendump %s\n' "${qemu_capture_path}"
	sleep 1
	printf 'quit\n'
} | timeout 20s qemu-system-i386 \
	-cdrom kfs.iso \
	-m 128M \
	-display none \
	-monitor stdio \
	-no-reboot \
	-no-shutdown

test -s "${qemu_capture_path}"
cp "${qemu_capture_path}" "${capture_path}"
rm -f "${qemu_capture_path}"
