#!/bin/bash 

rm *.*~
rm viennadata/*.*~
rm viennadata/traits/*.*~
rm viennadata/config/*.*~
rm tests/*.*~
rm examples/*.*~
rm examples/benchmarks/*.*~
rm examples/tutorials/*.*~
rm doc/*.*~
rm doc/manual/*.*~
cd tests
./clean.sh
