BVERSION=$(git for-each-ref refs/tags --sort=-authordate --format='%(refname)' --count=1)
VERSION=${BVERSION#"refs/tags/"}
REPPATH=/home/paul/coding/openBibleViewer
BUILDPATH=/home/paul/build/home:metaxy:stable/openbibleviewer
TAR=openbibleviewer-$VERSION.tar.gz
cd $BUILDPATH
echo "Remove Old"
rm -f openbibleviewer-*.tar.gz 2> /dev/null 
cd $REPPATH
echo "Archive File"
git archive --prefix=openBibleViewer-$VERSION/ HEAD | gzip >$TAR
cp -u $TAR $BUILDPATH
cp -u $REPPATH/deb/* $BUILDPATH -r
cd $BUILDPATH
ls $BUILDPATH
osc ar
osc commit -m"update"
