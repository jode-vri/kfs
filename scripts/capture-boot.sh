#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
capture_path="${1:-${repo_root}/kfs-boot.ppm}"
qemu_capture_path="/tmp/kfs-boot-$$.ppm"
capture_delay="${KFS_CAPTURE_DELAY:-4}"

cd "${repo_root}"
rm -f "${capture_path}"
rm -f "${qemu_capture_path}"

{
	sleep "${capture_delay}"
	printf 'screendump %s\n' "${qemu_capture_path}"
	sleep 1
	printf 'quit\n'
} | timeout 15s qemu-system-i386 \
	-cdrom kfs.iso \
	-m 128M \
	-display none \
	-monitor stdio \
	-no-reboot \
	-no-shutdown

test -s "${qemu_capture_path}"
cp "${qemu_capture_path}" "${capture_path}"
rm -f "${qemu_capture_path}"
printf 'Capture written to %s\n' "${capture_path}"
