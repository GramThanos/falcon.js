#! /bin/bash
TARGET_DIR=./falcon-impl
SOURCE_URL=https://falcon-sign.info/falcon-round3.zip
RENAME_SUBFOLDER=./falcon-round3/Extra/c
TMP_ZIP_NAME=./temp.zip
TMP_FOLDER_NAME=./falcon-round3
if [ ! -d $TARGET_DIR ]; then
	echo "Getting C source code..."
	wget -q --show-progress -O $TMP_ZIP_NAME $SOURCE_URL
	unzip -q $TMP_ZIP_NAME
	mv $RENAME_SUBFOLDER $TARGET_DIR
	rm $TMP_ZIP_NAME
	rm $TMP_FOLDER_NAME
fi
