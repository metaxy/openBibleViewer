VERSION=0.7.0
REPPATH=/home/paul/c++/openBibleViewer
BUILDPATH=/home/paul/build/home:metaxy:stable/openBibleViewer
TAR=openBibleViewer-$VERSION.tar.bz2
rm $BUILDPATH/openBibleViewer-*.tar.bz2 -f
rm $BUILDPATH/openBibleViewer.spec
cd $REPPATH
git archive --prefix=openBibleViewer-$VERSION/ HEAD | bzip2 >$TAR
cp -u $TAR $BUILDPATH
cp -u $REPPATH/openBibleViewer.spec $BUILDPATH -f
rm $TAR -f

