VERSION=0.7.1
REPPATH=/home/paul/c++/openBibleViewer
BUILDPATH=/home/paul/build/home:metaxy:stable/openbibleviewer
TAR=openbibleviewer-$VERSION.tar.gz
rm $BUILDPATH/* -f
cd $REPPATH
git archive --prefix=openBibleViewer-$VERSION/ HEAD | gzip >$TAR
cp -u $TAR $BUILDPATH
cp -u $REPPATH/deb/* $BUILDPATH -r
cd $BUILDPATH
osc ar
osc commit -m"update"

