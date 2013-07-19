#!/bin/bash

rm *.*~
rm viennamath/*.*~
rm viennamath/operations/*.*~
rm viennamath/compiletime/*.*~
rm viennamath/compiletime/operations/*.*~
rm viennamath/runtime/*.*~
rm viennamath/runtime/operations/*.*~
rm viennamath/manipulation/*.*~
rm viennamath/manipulation/detail/*.*~
rm viennamath/old/*.*~
rm tests/*.*~
rm tests/src/*.*~
rm examples/*.*~
rm doc/*.*~

cd tests
./clean.sh
