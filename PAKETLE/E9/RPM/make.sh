#!/bin/sh

set -e

rm -r -f ~/rpm
sh /LINUX/SCRIPTS/rpm_setup.sh

name="ceren_k9"
ceren_subdir="K9"

. ./build_rpm.sh $1
