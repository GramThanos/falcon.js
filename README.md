# falcon.js

Fast-Fourier Lattice-based Compact Signatures over NTRU (FALCON) in JavaScript.

FALCON is a NIST Post-Quantum Cryptography Competition round three finalist.

*Based on official [Falcon](https://falcon-sign.info/) implementation as submitted on NIST competition round 3 code.*
*Compiled with [emscripten](https://emscripten.org/) and methods exposed through custom C wrappers.*
*Wrapper influenced from [ntru.js](https://github.com/cyph/ntru.js)'s wrapper.*

___

## Usage

Add library on your project:

```html
<script type="text/javascript" src="falcon.js"></script>
```

Generate key pair:

```javascript
falcon.keypair(1024).then((pair) => {
	console.log('Public Key', pair.publicKey);
	console.log('Private Key', pair.privateKey);
}).catch((error) => {
	console.log('Failed to create keypair', error);
});
```

Sign a message:

```javascript
falcon.sign('My message', pair.privateKey, 1024).then((signature) => {
	console.log('Signature', signature);
}).catch((error) => {
	console.log('Failed to generate signature', error);
});
```

Verify a signature:

```javascript
falcon.verify('My message', signature, pair.publicKey, 1024).then((signature) => {
	console.log('Signature is valid');
}).catch((error) => {
	console.log('Failed to validate signature', error);
});
```

___

## Build from source

Get FALCON C source:
```bash
rm -r ./falcon-impl
bash ./get.sh
```

Build falcon.js (requires Emscripten installed):
```bash
make clean
make
```

___

### License

This project is under [The MIT license](https://opensource.org/licenses/MIT).
I do although appreciate attribute.

Copyright (c) 2017-2023 Grammatopoulos Athanasios-Vasileios

This project make use of the Falcon implementation that is also under an MIT license.

___

[![GramThanos](https://avatars2.githubusercontent.com/u/14858959?s=42&v=4)](https://github.com/GramThanos)
[![DinoDevs](https://avatars1.githubusercontent.com/u/17518066?s=42&v=4)](https://github.com/DinoDevs)
