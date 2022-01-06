@echo off

echo building......

if not exist build md build

cd build

if not exist project-build-int md project-build-int


cd project-build-int

cmake ../..

cmake --build .

exit