BUILDPATH=$OBS_BUILDPATH/openbibleviewer
TAR=openbibleviewer-$OBS_VERSION.tar.gz
echo "Deploying Debian packages"
cd $BUILDPATH
echo "Remove Old"
rm -f openbibleviewer-*.tar.gz 2> /dev/null 
cd $OBS_REPPATH
echo "Archive File"
git archive --prefix=openBibleViewer-$OBS_VERSION/ HEAD | gzip >$TAR
cp -u $TAR $BUILDPATH/openbibleviewer
cp -u $OBS_REPPATH/deb/* $BUILDPATH -r
cd $BUILDPATH
ls $BUILDPATH
osc ar
osc commit -m "update to $OBS_VERSION"

