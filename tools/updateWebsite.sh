REPPATH=/home/paul/c++/openBibleViewer
BUILDPATH=/home/paul/coding/websites/github

cd $BUILDPATH
git pull --rebase
cp $REPPATH/modules/ $BUILDPATH/obv/ -r
cp $REPPATH/UPDATES  $BUILDPATH/UPDATES_OBV.xml
git add .
git commit -a -m"site update"
git push origin master

