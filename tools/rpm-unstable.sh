BVERSION=$(git for-each-ref refs/tags --sort=-authordate --format='%(refname)' --count=1)
VERSION=${BVERSION#"refs/tags/"}
REPPATH=/home/paul/c++/openBibleViewer
BUILDPATH=/home/paul/build/home:metaxy:unstable/openBibleViewer
TAR=openBibleViewer-$VERSION.tar.bz2
rm $BUILDPATH/openBibleViewer-*.tar.bz2 -f
rm $BUILDPATH/openBibleViewer.spec
cd $REPPATH
git archive --prefix=openBibleViewer-$VERSION/ HEAD | bzip2 >$TAR
cp -u $TAR $BUILDPATH
cp -u $REPPATH/openBibleViewer.spec $BUILDPATH -f
rm $TAR -f
cd $BUILDPATH
osc ar
osc commit -m"update"
