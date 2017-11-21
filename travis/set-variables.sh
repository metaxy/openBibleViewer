git fetch --unshallow
export OBS_VERSION=$(git describe --abbrev=0 --tags $TRAVIS_BRANCH)
export OBS_REPPATH=$TRAVIS_BUILD_DIR
echo $OBS_VERSION
echo $OBS_REPPATH
