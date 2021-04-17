/*
 * Simple functions for FALCON signatures
 *
 * @author   Athanasios Vasileios Grammatopoulos <gramthanos@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * This code uses only the external API.
 */

#include "falcon.h"

void * xmalloc(size_t len) {
	void *buf;

	if (len == 0) {
		return NULL;
	}
	buf = malloc(len);
	if (buf == NULL) {
		fprintf(stderr, "memory allocation error\n");
		exit(EXIT_FAILURE);
	}
	return buf;
}

void xfree(void *buf) {
	if (buf != NULL) {
		free(buf);
	}
}


// Random Number Generator
shake256_context rng;


int falconjs_init () {
	if (shake256_init_prng_from_system(&rng) != 0) {
		// Error
		return 1;
	}
	return 0;
}

// bits = 2 ^ logn
// 		= 2 ^ 8 = 256
// 		= 2 ^ 9 = 512
// 		= 2 ^ 10 = 1024

size_t falconjs_pubkey_size (unsigned logn) {
	return FALCON_PUBKEY_SIZE(logn);
}

size_t falconjs_privkey_size (unsigned logn) {
	return FALCON_PRIVKEY_SIZE(logn);
}

size_t falconjs_expandedkey_size (unsigned logn) {
	return FALCON_EXPANDEDKEY_SIZE(logn);
}

size_t falconjs_sig_compressed_maxsize (unsigned logn) {
	return FALCON_SIG_COMPRESSED_MAXSIZE(logn);
}

size_t falconjs_sig_ct_size (unsigned logn) {
	return FALCON_SIG_COMPRESSED_MAXSIZE(logn);
}



int falconjs_keygen_make(uint8_t *pk, uint8_t *sk, unsigned logn) {

	size_t tmp_len = FALCON_TMPSIZE_KEYGEN(logn);
	uint8_t *tmp = xmalloc(tmp_len);

	int r = falcon_keygen_make(&rng, logn,
		sk, FALCON_PRIVKEY_SIZE(logn),
		pk, FALCON_PUBKEY_SIZE(logn),
		tmp, tmp_len
	);

	xfree(tmp);

	return r;
}

int falconjs_expand_privkey(uint8_t *esk, uint8_t *sk, unsigned logn) {

	size_t tmp_len = FALCON_TMPSIZE_EXPANDPRIV(logn);
	uint8_t *tmp = xmalloc(tmp_len);

	int r = falcon_expand_privkey(
		esk, FALCON_EXPANDEDKEY_SIZE(logn),
		sk, FALCON_PRIVKEY_SIZE(logn),
		tmp, tmp_len
	);

	xfree(tmp);

	return r;
}

int falconjs_sign_dyn(uint8_t *sig, size_t *sig_len, uint8_t *sk, uint8_t *data, size_t data_len, unsigned logn) {

	size_t tmp_len = FALCON_TMPSIZE_SIGNDYN(logn);
	uint8_t *tmp = xmalloc(tmp_len);

	int r = falcon_sign_dyn(
		&rng,
		sig, sig_len, FALCON_SIG_COMPRESSED,
		sk, FALCON_PRIVKEY_SIZE(logn),
		data, data_len, tmp, tmp_len
	);

	xfree(tmp);

	return r;
}

int falconjs_verify(uint8_t *sig, size_t sig_len, uint8_t *pk, uint8_t *data, size_t data_len, unsigned logn) {

	size_t tmp_len = FALCON_TMPSIZE_VERIFY(logn);
	uint8_t *tmp = xmalloc(tmp_len);

	int r = falcon_verify(
		sig, sig_len, FALCON_SIG_COMPRESSED,
		pk, FALCON_PUBKEY_SIZE(logn),
		data, data_len, tmp, tmp_len
	);

	xfree(tmp);

	return r;
}


/*
int secret_key(unsigned logn) {
	// bits = 2 ^ logn
	// 		= 2 ^ 8 = 256
	// 		= 2 ^ 9 = 512
	// 		= 2 ^ 10 = 1024
	
	bench_context bc;

	bc.logn = logn;
	if (shake256_init_prng_from_system(&bc.rng) != 0) {
		fprintf(stderr, "random seeding failed\n");
		exit(EXIT_FAILURE);
	}
	len = FALCON_TMPSIZE_KEYGEN(logn);
	len = maxsz(len, FALCON_TMPSIZE_SIGNDYN(logn));
	len = maxsz(len, FALCON_TMPSIZE_SIGNTREE(logn));
	len = maxsz(len, FALCON_TMPSIZE_EXPANDPRIV(logn));
	len = maxsz(len, FALCON_TMPSIZE_VERIFY(logn));
	bc.tmp = xmalloc(len);
	bc.tmp_len = len;
	bc.pk = xmalloc(FALCON_PUBKEY_SIZE(logn));
	bc.sk = xmalloc(FALCON_PRIVKEY_SIZE(logn));
	bc.esk = xmalloc(FALCON_EXPANDEDKEY_SIZE(logn));
	bc.sig = xmalloc(FALCON_SIG_COMPRESSED_MAXSIZE(logn));
	bc.sig_len = 0;
	bc.sigct = xmalloc(FALCON_SIG_CT_SIZE(logn));
	bc.sigct_len = 0;

	falcon_keygen_make(bc.rng, bc.logn,
		bc.sk, FALCON_PRIVKEY_SIZE(bc.logn),
		bc.pk, FALCON_PUBKEY_SIZE(bc.logn),
		bc.tmp, bc.tmp_len
	);

	falcon_expand_privkey(
		bc.esk, FALCON_EXPANDEDKEY_SIZE(bc.logn),
		bc.sk, FALCON_PRIVKEY_SIZE(bc.logn),
		bc.tmp, bc.tmp_len
	);

	bc.sig_len = FALCON_SIG_COMPRESSED_MAXSIZE(bc.logn);
	falcon_sign_dyn(
		bc.rng,
		bc.sig, bc.sig_len, FALCON_SIG_COMPRESSED,
		bc.sk, FALCON_PRIVKEY_SIZE(bc.logn),
		"data", 4, bc.tmp, bc.tmp_len
	);

	bc.sigct_len = FALCON_SIG_CT_SIZE(bc.logn);
	falcon_sign_dyn(
		bc.rng,
		bc.sigct, bc.sigct_len, FALCON_SIG_CT,
		bc.sk, FALCON_PRIVKEY_SIZE(bc.logn),
		"data", 4, bc.tmp, bc.tmp_len
	);

	bc.sig_len = FALCON_SIG_COMPRESSED_MAXSIZE(bc.logn);
	falcon_sign_tree(
		bc.rng,
		bc.sig, bc.sig_len, FALCON_SIG_COMPRESSED,
		bc.esk,
		"data", 4, bc.tmp, bc.tmp_len
	);

	bc.sigct_len = FALCON_SIG_CT_SIZE(bc.logn);
	falcon_sign_tree(
		bc.rng,
		bc.sigct, bc.sigct_len, FALCON_SIG_CT,
		bc.esk,
		"data", 4, bc.tmp, bc.tmp_len
	);

	pk_len = FALCON_PUBKEY_SIZE(bc.logn);
	falcon_verify(
		bc.sig, bc.sig_len, FALCON_SIG_COMPRESSED,
		bc.pk, pk_len,
		"data", 4, bc.tmp, bc.tmp_len
	);

	pk_len = FALCON_PUBKEY_SIZE(bc.logn);
	falcon_verify(
		bc.sigct, bc.sigct_len, FALCON_SIG_CT,
		bc.pk, pk_len,
		"data", 4, bc.tmp, bc.tmp_len
	);

	return 0;
}
*/
