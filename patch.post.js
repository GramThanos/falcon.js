// End of emscripten code
	}).catch(function () {
		throw new Error('Failed to load');
	}).then(function (m) {
		Object.keys(m).forEach(function (k) { Module[k] = m[k]; });
	});

	// Check if valid return data
	function dataReturn (returnValue, result) {
		if (returnValue === 0) return result;
		throw new Error('FALCON error: ' + returnValue);
	}

	// Get result from memory
	function dataResult (buffer, bytes) {
		return new Uint8Array(
			new Uint8Array(Module.HEAPU8.buffer, buffer, bytes)
		);
	}

	// Free malloc buffer
	function dataFree (buffer) {
		try {
			Module._xfree(buffer);
		}
		catch (err) {
			setTimeout(function () {throw err;}, 0);
		}
	}

	// Initialize module
	var init = Module.ready.then(function () {
		var res = Module._falconjs_init();
		if (res == 1) {
			console.error('[falcon.js] Initialisation failed!');
		}
		falcon.initialised = res ? false : true;
	});

	// FALCON methods
	var falcon = {

		version: 'v0.2-beta',
		initialised: false,

		isInitialised: function() {
			return this.initialised ? true : false;
		},

		keypair: function (n) {
			return init.then(function () {
				var logn = Math.log2(n);
				var publicKeyBytes = Module._falconjs_pubkey_size(logn);
				var publicKeyBuffer = Module._xmalloc(publicKeyBytes);
				var privateKeyBytes = Module._falconjs_privkey_size(logn);
				var privateKeyBuffer = Module._xmalloc(privateKeyBytes);

				try {
					var returnValue = Module._falconjs_keygen_make(
						publicKeyBuffer,
						privateKeyBuffer,
						logn
					);

					return dataReturn(returnValue, {
						publicKey: dataResult(publicKeyBuffer, publicKeyBytes),
						privateKey: dataResult(privateKeyBuffer, privateKeyBytes)
					});
				}
				finally {
					dataFree(publicKeyBuffer);
					dataFree(privateKeyBuffer);
				}
			});
		},

		sign: function (message, privateKey, n) {
			return init.then(function () {
				var logn = Math.log2(n);

				var data = typeof message === 'string' ? new TextEncoder().encode(message) : message;
				var dataBytes = data.length;
				var dataBuffer = Module._xmalloc(dataBytes);
				var privateKeyBytes = privateKey.length;
				var privateKeyBuffer = Module._xmalloc(privateKeyBytes);
				var signatureKeyBytes = Module._falconjs_sig_compressed_maxsize(logn);
				var signatureKeyBuffer = Module._xmalloc(signatureKeyBytes);
				var signatureKeyBytesByffer = Module._xmalloc(4);

				Module.writeArrayToMemory(data, dataBuffer);
				Module.writeArrayToMemory(privateKey, privateKeyBuffer);
				Module.writeArrayToMemory(new Uint8Array(new Int32Array([signatureKeyBytes]).buffer), signatureKeyBytesByffer);

				try {
					var returnValue = Module._falconjs_sign_dyn(
						signatureKeyBuffer,
						signatureKeyBytesByffer,
						privateKeyBuffer,
						dataBuffer,
						dataBytes,
						logn
					);

					signatureKeyBytes = new Int32Array(dataResult(signatureKeyBytesByffer, 4).buffer)[0];

					return dataReturn(
						returnValue,
						dataResult(signatureKeyBuffer, signatureKeyBytes)
					);
				}
				finally {
					dataFree(dataBuffer);
					dataFree(privateKeyBuffer);
					dataFree(signatureKeyBuffer);
				}
			});
		},

		verify: function (message, signature, publicKey, n) {
			return init.then(function () {
				var logn = Math.log2(n);

				var data = typeof message === 'string' ? new TextEncoder().encode(message) : message;
				var dataBytes = data.length;
				var dataBuffer = Module._xmalloc(dataBytes);
				var publicKeyBytes = publicKey.length;
				var publicKeyBuffer = Module._xmalloc(publicKeyBytes);
				var signatureKeyBytes = signature.length;
				var signatureKeyBuffer = Module._xmalloc(signatureKeyBytes);

				Module.writeArrayToMemory(data, dataBuffer);
				Module.writeArrayToMemory(publicKey, publicKeyBuffer);
				Module.writeArrayToMemory(signature, signatureKeyBuffer);

				try {
					var returnValue = Module._falconjs_verify(
						signatureKeyBuffer,
						signatureKeyBytes,
						publicKeyBuffer,
						dataBuffer,
						dataBytes,
						logn
					);

					return dataReturn(
						returnValue,
						(returnValue === 0)
					);
				}
				finally {
					dataFree(dataBuffer);
					dataFree(publicKeyBuffer);
					dataFree(signatureKeyBuffer);
				}
			});
		}
	};

	return falcon;

}());

if (typeof module !== 'undefined' && module.exports) {
	falcon.falcon = falcon;
	module.exports = falcon;
}
else {
	self.falcon = falcon;
}

// Example use
/*
console.log('Testing...');
console.log('--------------------------------');
let n = 1024;
falcon.keypair(n).then((pair) => {
	console.log('pair', pair);
	console.log('--------------------------------');

	falcon.sign('data', pair.privateKey, n).then((signature) => {
		console.log('signature', signature);
		console.log('--------------------------------');
		
		//signature.set([0],10)

		falcon.verify('data', signature, pair.publicKey, n).then((verified) => {
			console.log('verified', verified);
			console.log('--------------------------------');
		}).catch((error) => {
			console.log('failed to verify signature', error);
		});
	}).catch((error) => {
		console.log('failed to create signature', error);
	});
}).catch((error) => {
	console.log('failed to create keypair', error);
});
*/