#!/usr/bin/env bash

if test $# -lt 3
then
  echo "Usage: $0 <suffixerator-options>"
  exit 1
fi

function checkerror() 
{
  $1
  if test $? -ne 0
  then
    echo "failure: ${1}"
    exit 1
  else
    echo "okay ${1}"
  fi
}

#VALGRIND=valgrind.sh

function checksfxmap()
{
  cmd="${VALGRIND} ${GTDIR}/bin/gt dev sfxmap $1"
  TMPFILE=`mktemp TMP.XXXXXX` || exit 1
  ${cmd} > ${TMPFILE}
  if test $? -ne 0
  then
    echo "failure: ${cmd}"
    exit 1
  else
    echo "success: ${cmd}"
  fi
  grep -v '^#' ${TMPFILE} > $1-sfx.prj
  rm -f ${TMPFILE}
  checkerror "cmp -s $1.prj $1-sfx.prj"
}

checkerror "${VALGRIND} ${GTDIR}/bin/gt suffixerator -tis -suf -indexname /tmp/sfx $*"
checkerror "checksfxmap /tmp/sfx"
