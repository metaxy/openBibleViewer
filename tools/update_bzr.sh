cd ../../
cp openBibleViewer/src bzr_obv -R
cp openBibleViewer/openBibleViewer.pro bzr_obv
cp openBibleViewer/INSTALL bzr_obv
cp openBibleViewer/LICENSE bzr_obv
cp openBibleViewer/README bzr_obv
cp openBibleViewer/TRANSLATION bzr_obv
cp openBibleViewer/doc.config bzr_obv
cp openBibleViewer/openBibleViewer.spec bzr_obv
cp openBibleViewer/openBibleViewer.desktop bzr_obv
cd bzr_obv/src

rm -rf openbibleviewer
mkdir openbibleviewer
lconvert -i locales/obv_en.ts -o openbibleviewer/openbibleviewer.pot --of pot


bzr add
bzr commit -m "update"
bzr push lp:~metaxy/openbibleviewer/trunk



