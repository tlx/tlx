#!/bin/bash -x
################################################################################
# misc/mk-doxygen-travis.sh
#
# Script to automatically generate doxyen documentation on Travis and upload it
# to Github Pages.
#
# Based on https://gist.github.com/vidavidorra/548ffbcdae99d752da02
#
# Part of tlx - http://panthema.net/tlx
#
# Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
#
# All rights reserved. Published under the Boost Software License, Version 1.0
################################################################################

set -e

GH_REPO_REF=github.com/tlx/tlx.github.io
# GH_REPO_TOKEN=<set-by-travis>

# Get the current github doxygen repo
rm -rf doxygen-html
git clone https://git@${GH_REPO_REF} doxygen-html

### Clean directory -- works with lots of files
CURRENTCOMMIT=`git -C doxygen-html rev-parse HEAD`
# Reset working tree to initial commit
git -C doxygen-html reset --hard base
# Move HEAD back to where it was
git -C doxygen-html reset --soft $CURRENTCOMMIT

echo 'Generating Doxygen code documentation...'
doxygen 2>&1 | tee doxygen-html/doxygen.log

if [ -f "doxygen-html/index.html" ]; then

    echo 'Uploading documentation to Github Pages...'
    pushd doxygen-html
    git add --all

    git \
        -c user.name="mk-doxygen (Travis CI)" \
        -c user.email="tlx@panthema.net" \
        commit \
        -m "Deploy code docs to GitHub Pages Travis build: ${TRAVIS_BUILD_NUMBER}" \
        -m "Commit: ${TRAVIS_COMMIT}"

    git \
        -c push.default=simple \
        push --force \
        "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" > /dev/null 2>&1

    popd

else
    echo '' >&2
    echo 'Warning: No documentation (html) files have been found!' >&2
    echo 'Warning: Not going to push the documentation to GitHub!' >&2
    exit 1
fi

################################################################################
