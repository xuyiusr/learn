#!/bin/sh
LIB="./lib/HiveJDBC41.jar"
OUT="./out/"
BIN="./bin/*.java"
MAIN="MainTest"

javac -cp ${LIB} ${BIN} -d ${OUT}
java -cp ${LIB}:${OUT} ${MAIN}
