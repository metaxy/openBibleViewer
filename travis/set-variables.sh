git fetch --unshallow
export OBS_VERSION=$(git describe --tags $TRAVIS_BRANCH)
export OBS_REPPATH=$TRAVIS_BUILD_DIR
echo $OBS_VERSION
echo $OBS_REPPATH