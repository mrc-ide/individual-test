#!/usr/bin/env bash
CPUPROFILE="profile.log" R -f profile/profile.R
pprof --text profile.log
