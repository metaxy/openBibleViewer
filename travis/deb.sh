BUILDPATH=$OBS_BUILDPATH/openbibleviewer
TAR=openbibleviewer-$OBS_VERSION.tar.gz
echo "Deploying Debian packages"

cd $BUILDPATH
rm openbibleviewer-*.tar.gz
cd $OBS_REPPATH
git archive --prefix=openBibleViewer-$OBS_VERSION/ HEAD | gzip >$TAR
cp -u $TAR $BUILDPATH/openbibleviewer
cp -u $OBS_REPPATH/deb/* $BUILDPATH -r
cd $BUILDPATH
osc ar
osc commit -m "update to $OBS_VERSION"

