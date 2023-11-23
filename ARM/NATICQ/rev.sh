#!/bin/bash
rev=$(git rev-parse --short HEAD)
echo '#define __SVN_REVISION__ "'$rev'"' > revision.h
