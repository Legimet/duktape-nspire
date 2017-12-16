# Duktape-nspire 0.1.1 #

Duktape-nspire provides a JavaScript (ECMAScript) interpreter for the TI-Nspire handheld calculator.
It combines the [Duktape](https://github.com/svaarala/duktape) Javascript engine with a set of
Nspire-specific modules and module support, and a REPL (using
[Nspire I/O](https://github.com/compujuckel/nspire-io.git) for its interface).

## Usage
The calculator must have Ndless installed. To get to the interactive Nspire I/O console, run Duktape
directly from the document browser. Type `.exit` to exit.

You can also run scripts with a .js extension from the document browser. The extension should be
registered with `duktape`. This is done automatically the first time you run Duktape from the
document browser, but you can also register it manually by editing `ndless.cfg.tns`.

## Modules

The following modules are available right now:

* **fs**: Filesystem functions, compatible with the fs module of Node.js
* **nsp/keys**: Functions for keyboard input
* **nsp/texture**: Graphics support, compatible with the Texture class of the Nspire port of Micro
Python

These modules are not complete, and more functions and modules will be added in the future.

## License

Duktape-nspire is licensed under the GNU LGPL, version 3 or above:

> Copyright (C) 2015 Legimet
>
> This file is part of Duktape-nspire.
>
> Duktape-nspire is free software: you can redistribute it and/or modify
> it under the terms of the GNU Lesser General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.
>
> Duktape-nspire is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
> GNU Lesser General Public License for more details.
>
> You should have received a copy of the GNU Lesser General Public License
> along with Duktape-nspire. If not, see <http://www.gnu.org/licenses/>.

Duktape itself is licensed under the MIT License (Expat), which can be found in the file `duktape.h`.
