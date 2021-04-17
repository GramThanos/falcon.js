#! /bin/bash
TARGET_DIR=./falcon-impl
SOURCE_URL=https://falcon-sign.info/Falcon-impl-round3.zip
RENAME_SUBFOLDER=./Falcon-impl-round3
TMP_ZIP_NAME=./temp.zip
if [ ! -d $TARGET_DIR ]; then
	echo "Getting C source code..."
	wget -q --show-progress -O $TMP_ZIP_NAME $SOURCE_URL
	unzip -q $TMP_ZIP_NAME
	mv $RENAME_SUBFOLDER $TARGET_DIR
	rm $TMP_ZIP_NAME
fi
