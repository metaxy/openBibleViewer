BVERSION=$(git for-each-ref refs/tags --sort=-authordate --format='%(refname)' --count=1)
VERSION=${BVERSION#"refs/tags/"}
REPPATH=/home/paul/c++/openBibleViewer
BUILDPATH=/home/paul/build/home:metaxy:unstable/openbibleviewer
TAR=openbibleviewer-$VERSION.tar.gz
cd $REPPATH
git archive --prefix=openBibleViewer-$VERSION/ HEAD | gzip >$TAR
cp -u $TAR $BUILDPATH
cp -u $REPPATH/deb/* $BUILDPATH -r
