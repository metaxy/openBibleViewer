VERSION=0.5.1
REPPATH=/home/paul/c++/openBibleViewer
BUILDPATH=/home/paul/build/home:metaxy/openBibleViewer
TAR=openBibleViewer-$VERSION.tar.bz2
cd $REPPATH
git archive --prefix=openBibleViewer-$VERSION/ HEAD | bzip2 >$TAR
cp -u $TAR $BUILDPATH
cp -u $REPPATH/openBibleViewer.spec $BUILDPATH
