
# tests with CPU backend
foreach(PROG misc)
   add_executable(${PROG}-test src/${PROG}.cpp)
   add_test(${PROG} ${PROG}-test)
endforeach(PROG)


