#!/bin/sh
LIB="./lib/ojdbc.jar"
OUT="./out/"
BIN="./bin/*.java"
MAIN="OracleTest"

javac -cp ${LIB} ${BIN} -d ${OUT}
java -cp ${LIB}:${OUT} ${MAIN}
