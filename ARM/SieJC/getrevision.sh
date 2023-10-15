#!/bin/sh
echo "#define __GIT_REVISION__ \"$(git rev-parse --short HEAD)\"" > revision.h