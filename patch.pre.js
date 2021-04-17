//
// Falcon.js
// Fast-Fourier Lattice-based Compact Signatures over NTRU in JavaScript
//
// @author   Athanasios Vasileios Grammatopoulos <gramthanos@gmail.com>
// 
// Based on official Falcon implementation
// as submitted on NIST competition round 3
// code from https://falcon-sign.info/
//
// Wrapper influenced from ntru.js's wrapper
// from https://github.com/cyph/ntru.js
// 

var falcon = (function () {

	var Module = {};
	Module.ready = new Promise(function (resolve, reject) {
		var Module = {};
		Module.onAbort = reject;
		Module.onRuntimeInitialized = function () {
			try {
				Module._falconjs_pubkey_size();
				resolve(Module);
			}
			catch (err) {
				reject(err);
			}
		};

// Start of emscripten code
