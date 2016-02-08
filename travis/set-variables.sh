OBS_BVERSION=$(git for-each-ref refs/tags --sort=-authordate --format='%(refname)' --count=1)
export OBS_VERSION=${BVERSION#"refs/tags/"}
export OBS_REPPATH=$TRAVIS_BUILD_DIR
