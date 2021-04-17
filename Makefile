# Build script for the Falconjs
# based on official Falcon implementation submitted on NIST competition
#
# @author   Athanasios Vasileios Grammatopoulos <gramthanos@gmail.com>

.POSIX:

# =====================================================================
#
# Configurable options:
#   CC       C compiler to JavaScript; Probably Emscripten ("emcc")
#   CFLAGS   Compilation flags
#   LD       Linker; normally the same command as the compiler.
#   LDFLAGS  Linker options, not counting the extra libs.
#   LIBS     Extra libraries for linking.

CC = emcc
CFLAGS = -Wall -Wextra -Wshadow -Wundef -O3
LD = emcc
LDFLAGS = 
LIBS = 

# =====================================================================

SOURCE = falcon-impl
OBJ = $(SOURCE)/codec.o $(SOURCE)/common.o $(SOURCE)/falcon.o $(SOURCE)/fft.o $(SOURCE)/fpr.o $(SOURCE)/keygen.o $(SOURCE)/rng.o $(SOURCE)/shake.o $(SOURCE)/sign.o $(SOURCE)/vrfy.o

# =====================================================================

all: getsource falconjs

clean:
	-rm -f $(OBJ) falcon.js falconjs.o falcon.wasm

getsource:
	-bash ./get.sh

falconjs: falconjs.o $(OBJ)
	$(LD) $(LDFLAGS) -o falcon.js falconjs.o $(OBJ) $(LIBS) -s EXPORTED_FUNCTIONS='["_xfree","_xmalloc","_falconjs_init","_falconjs_pubkey_size","_falconjs_privkey_size","_falconjs_expandedkey_size","_falconjs_sig_compressed_maxsize","_falconjs_sig_ct_size","_falconjs_keygen_make","_falconjs_expand_privkey","_falconjs_sign_dyn","_falconjs_verify"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["writeArrayToMemory"]'
	cat patch.pre.js falcon.js patch.post.js > tmp.js; mv tmp.js falcon.js

# =====================================================================

falconjs.o: falconjs.c $(SOURCE)/falcon.h
	$(CC) $(CFLAGS) -c -o falconjs.o falconjs.c -I$(SOURCE)/

codec.o: codec.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/codec.o $(SOURCE)/codec.c

common.o: common.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/common.o $(SOURCE)/common.c

falcon.o: falcon.c falcon.h config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/falcon.o $(SOURCE)/falcon.c

fft.o: fft.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/fft.o $(SOURCE)/fft.c

fpr.o: fpr.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/fpr.o $(SOURCE)/fpr.c

keygen.o: keygen.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/keygen.o $(SOURCE)/keygen.c

rng.o: rng.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/rng.o $(SOURCE)/rng.c -D FALCON_RAND_GETENTROPY=0 -D FALCON_RAND_URANDOM=1 -D FALCON_RAND_WIN32=0

shake.o: shake.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/shake.o $(SOURCE)/shake.c

sign.o: sign.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/sign.o $(SOURCE)/sign.c

vrfy.o: vrfy.c config.h inner.h fpr.h
	$(CC) $(CFLAGS) -c -o $(SOURCE)/vrfy.o $(SOURCE)/vrfy.c
