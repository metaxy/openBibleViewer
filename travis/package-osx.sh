ls $TRAVIS_BUILD_DIR
cpack -G OSXX11
mkdir build_results
cp *.dmg build_results/
