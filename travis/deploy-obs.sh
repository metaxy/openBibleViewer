cat <<EOF > ~/.oscrc
[general]
apiurl = https://api.opensuse.org
[https://api.opensuse.org]
user = ${OBS_USER}
pass = ${OBS_PASSWORD}
EOF
cd $TRAVIS_BUILD_DIR
OBS_BVERSION=$(git for-each-ref refs/tags --sort=-authordate --format='%(refname)' --count=1)
OBS_VERSION=${BVERSION#"refs/tags/"}
OBS_REPPATH=$TRAVIS_BUILD_DIR

mkdir -p ~/build
cd ~/build

If [$TRAVIS_BRANCH == "master"]
then
  osc co "home:${OBS_USER}:stable"
  OBS_BUILDPATH=~/build/home:${OBS_USER}:stable
fi

If [$TRAVIS_BRANCH == "develop"]
then
  osc co "home:${OBS_USER}:unstable"
  OBS_BUILDPATH=~/build/home:${OBS_USER}:unstable
fi

. deb.sh
. rpm.sh