# arduino-stm32-xdr

-- Code still in development / proof of concept --

XDR/RPC protocol for Arduino STM32Core based on the Sun Microsystems XRD/RPC Source Code. 
Tested with the Arduino 1.8.13, STMCore with the STM32F405 MCU.

Adapted from the source codes of `portablexdr-4.9.1` (https://github.com/Alexpux/portablexdr) and `bsd-xdr-master` (https://github.com/woodfishman/bsd-xdr).

## Examples of using cases

1. transmit & receive (block / stream) information data such as integers, strings, floats, structures in a binary format from one device to another device via USB, TCP, etc.
2. store & load (block / stream) information data such as integers, strings, floats, structures in a binary format into a memory such as Flash, EEprom, etc.
3. protocols such as VXI11 designed for connecting instruments (such as oscilloscopes, multimeters, spectrum analyzers, etc.) to controllers (e.g., PCs) use the underlying XDR/RPC protocol.

## Why reuse the c-code originally written by Sun

It allows to use the xdr/rpc generator `rpcgen` to create c-code files and to implement a Remote Procedure Call (RPC) protocol (Note that the code generated by `rpcgen` may require some little hand-made rewrites in order to get the compiled code).

## References

1. python has it own implementation (`xdrlib`). This can be used to check/debug function outputs.

## License

Except the *.ino files (MIT license), despite the modification, the c source codes are not belong to me. Please refer to the license description for each file.

## More information at

- https://en.wikipedia.org/wiki/External_Data_Representation
- https://datatracker.ietf.org/doc/html/rfc1014

## Future

1. clean up a bit the source code
2. create test functions
3. create a python code to correct the generated code by rpcgen in order to pass a compilation.
