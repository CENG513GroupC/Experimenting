rm -r build
mkdir build
cd build
cmake ../
make
cd ..
chmod 777 -R . 
cp hostedx115-latest.rbf build

