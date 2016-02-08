BUILDPATH=$OBS_BUILDPATH/openBibleViewer
TAR=openBibleViewer-$OBS_VERSION.tar.bz2

rm $BUILDPATH/openBibleViewer-*.tar.bz2 -f
rm $BUILDPATH/openBibleViewer.spec
cd $OBS_REPPATH
git archive --prefix=openBibleViewer-$OBS_VERSION/ HEAD | bzip2 >$TAR
cp -u $TAR $BUILDPATH
cp -u $OBS_REPPATH/openBibleViewer.spec $BUILDPATH -f
rm $TAR -f
cd $BUILDPATH
osc ar
osc commit -m"update to $OBS_VERSION"
