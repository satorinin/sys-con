#!/bin/bash
#
# This script is used to simplify nintendo switch developement
# You need homebrew ftpd installed
# Boot the switch, enter in ftpd, use this script to do basic actions:
# "ftp upload" to upload the binary (You just built) directly to the switch
# "ftp logs" to display the logs
# Same commands are available with "sd", usefull when the nintendo switch crash on boot (You plug the sdcard to your laptop and debug from there)

# IP of your switch on your network:
FTP_URL=ftp://192.168.10.238:5000

#Below variable need to stay unchanged, they are correct whatever the developer
NSP_FILE=./out/atmosphere/contents/690000000000000D/exefs.nsp
DST_NSP_FILE=atmosphere/contents/690000000000000D/exefs.nsp
NRO_FILE=./out/switch/sys-con.nro
DST_NRO_FILE=switch/sys-con.nro
LOG_FILE=config/sys-con/log.txt

usage() {
  echo "Usage: "
  echo "       $0 ftp upload"
  echo "       $0 ftp logs"
  echo "       $0 sd upload"
  echo "       $0 sd logs"
  echo "       $0 build"
  exit 1
}

display_logs() {
  BLUE='\033[0;34m'
  RED='\033[0;31m'
  YELLOW='\033[0;33m'
  NC='\033[0m'

  echo "---- LOGS ----"
  while IFS= read -r line; do
    if [[ $line == "|I|"* ]]; then
      echo -e "${BLUE}${line}${NC}"
    elif [[ $line == "|E|"* ]]; then
      echo -e "${RED}${line}${NC}"
    elif [[ $line == "|W|"* ]]; then
      echo -e "${YELLOW}${line}${NC}"
    else
      echo "$line"
    fi
  done <./log.txt
  echo "---- END ----"
}

if [ "$1" == "ftp" ]; then
  if [ "$2" == "upload" ]; then
    echo "FTP upload"
    curl -T $NSP_FILE "$FTP_URL/$DST_NSP_FILE" || exit 1
    curl -T $NRO_FILE "$FTP_URL/$DST_NRO_FILE" || exit 1
    exit 0
  fi

  if [ "$2" == "logs" ]; then
    curl "$FTP_URL/$LOG_FILE" -o ./log.txt || exit 1
    display_logs
    exit 0
  fi
fi

if [ "$1" == "sd" ]; then
  if [ "$2" == "upload" ]; then
    echo "SD card copy"
    cp $NSP_FILE /Volumes/SWITCH\ SD/$DST_NSP_FILE || exit 1
    cp $NRO_FILE /Volumes/SWITCH\ SD/$DST_NRO_FILE || exit 1
    sync
    echo "OK"
    exit 0
  fi

  if [ "$2" == "logs" ]; then
    cp /Volumes/SWITCH\ SD/$LOG_FILE ./log.txt || exit 1
    display_logs
    exit 0
  fi
fi

if [ "$1" == "build" ]; then
  rm *.zip
  make distclean || exit 1
  exit 0
fi

usage
