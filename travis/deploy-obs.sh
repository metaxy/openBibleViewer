#!/bin/bash
cat <<EOF > ~/.oscrc
[general]
apiurl = https://api.opensuse.org
[https://api.opensuse.org]
user = ${OBS_USER}
pass = ${OBS_PASSWORD}
EOF
mkdir -p ~/build
cd ~/build

if [$TRAVIS_BRANCH == "master"]; then
  osc co "home:${OBS_USER}:stable"
  OBS_BUILDPATH=~/build/home:${OBS_USER}:stable
fi

if [$TRAVIS_BRANCH == "develop"]; then
  osc co "home:${OBS_USER}:unstable"
  OBS_BUILDPATH=~/build/home:${OBS_USER}:unstable
fi
cd $TRAVIS_BUILD_DIR

. $TRAVIS_BUILD_DIR/travis/deb.sh
. $TRAVIS_BUILD_DIR/travis/rpm.sh