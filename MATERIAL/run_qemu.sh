#!/bin/bash

qemu-system-aarch64 -cpu cortex-a53 \
                    -machine virt \
                    -m 2048M \
                    -kernel ~/Downloads/kernel-qemu-4.19.50-buster \
                    -append "root=/dev/vda2 panic=1" \
                    -drive file=~/Downloads/raspios-bookworm-arm64-lite.qcow2,format=raw \
                    -serial stdio
