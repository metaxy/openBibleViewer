sh build-obv-deb-unstable.sh
sh build-obv-unstable.sh

cd /home/paul/build/home:metaxy:unstable
cd openbibleviewer
osc ar
osc commit -m"update"
cd ..
cd openBibleViewer
osc ar
osc commit -m"update"
