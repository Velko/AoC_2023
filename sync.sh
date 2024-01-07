#!/bin/sh

MIRROR=`cat sync.config`

rsync -a inputs puzzles $MIRROR
rsync -a $MIRROR/inputs $MIRROR/puzzles .
