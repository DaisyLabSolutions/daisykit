#!/bin/bash

set -e
set -x

# OpenMP is not present on macOS by default
if [[ "$RUNNER_OS" == "macOS" ]]; then
    # Make sure to use a libomp version binary compatible with the oldest
    # supported version of the macos SDK as libomp will be vendored into the
    # daisykit wheels for macos. The list of binaries are in
    # https://packages.macports.org/libomp/.  Currently, the oldest
    # supported macos version is: High Sierra / 10.13. When upgrading this, be
    # sure to update the MACOSX_DEPLOYMENT_TARGET environment variable in
    # wheels.yml accordingly. Note that Darwin_17 == High Sierra / 10.13.
    wget https://packages.macports.org/libomp/libomp-13.0.1_1+universal.darwin_17.i386-x86_64.tbz2 -O libomp.tbz2
    sudo tar -C / -xvjf libomp.tbz2 opt

    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
    export CPPFLAGS="$CPPFLAGS -Xpreprocessor -fopenmp"
    export CFLAGS="$CFLAGS -I/opt/local/include/libomp"
    export CXXFLAGS="$CXXFLAGS -I/opt/local/include/libomp"
    export LDFLAGS="$LDFLAGS -Wl,-rpath,/opt/local/lib/libomp -L/opt/local/lib/libomp -lomp"
fi

# The version of the built dependencies are specified
# in the pyproject.toml file, while the tests are run
# against the most recent version of the dependencies

python -m pip install cibuildwheel
python -m cibuildwheel --output-dir wheelhouse
