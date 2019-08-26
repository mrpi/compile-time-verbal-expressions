#!/bin/bash

python3 -m quom include/ctve.hpp ctve.hpp.tmp
echo "/*" > single-header/ctve.hpp
cat LICENSE >> single-header/ctve.hpp
echo "*/" >> single-header/ctve.hpp 
echo "" >> single-header/ctve.hpp 
cat ctve.hpp.tmp >> single-header/ctve.hpp
rm ctve.hpp.tmp
