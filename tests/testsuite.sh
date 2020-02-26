#!/bin/sh

touch tests/expected.txt
touch tests/output.txt
touch tests/diff.txt

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
NC='\033[0m'

TEST=0
PASS=0

echo
echo -e ${BLUE}'------TESTS------'
echo
for file in tests/*.cmd; do
  echo -e '     '${YELLOW}$file${NC}
  echo
  while read -r line; do
    $line &> tests/expected.txt
    TEST=$((TEST + 1))
    LD_PRELOAD=./libmalloc.so $line &> tests/output.txt 
    diff tests/expected.txt tests/output.txt > tests/diff.txt
    if [ $(wc -l < tests/diff.txt) -eq 0 ]; then
      echo -e $line : ${GREEN}passed.${NC}
      PASS=$((PASS + 1))
    else
      echo -e $line : ${RED}failed.${NC}
    fi
  done < $file
  echo
done

echo -e ${BLUE}'-----RESULTS-----'${NC}
echo
echo $PASS / $TEST
