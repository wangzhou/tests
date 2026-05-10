#!/bin/sh
# Load HiSilicon ZIP driver and test

echo "Loading QM driver..."
insmod /p9/lib/modules/hisi_qm.ko

echo "Loading ZIP driver..."
insmod /p9/lib/modules/hisi_zip.ko

echo "Checking loaded modules..."
lsmod | grep -E "hisi|hzip"

echo "Checking device..."
lspci | grep -i zip

echo "Checking dmesg..."
dmesg | grep -i hisi_zip

echo "Done."