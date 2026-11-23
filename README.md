Acknowledgments
==========

Special thanks to [Alex Hornung](https://github.com/bwalex) for creating
[tc-play](https://github.com/bwalex/tc-play) and for the years he devoted to
maintaining it. Behind the simplicity of opening an encrypted volume with a
single command are countless hours of patient, thoughtful work. This fork owes
its existence to that dedication and to Alex’s generosity in sharing his work.

VeraCrypt has continued to evolve in the years since the last tc-play release.
VCPlay was started to keep pace with those changes while preserving support for
legacy TrueCrypt volumes and the speed of in-kernel disk encryption.
The goal is for a volume created or updated in either
VeraCrypt or VCPlay to work safely in the other.

The fork has its own command, `vcplay`, so both tools can be installed side by
side and existing scripts can keep calling `tcplay`. This matters because
tc-play handles some volume operations differently from VeraCrypt.
VCPlay uses VeraCrypt as the reference for those operations, so compatibility
with every volume created or modified by tc-play cannot be guaranteed.

About
==========
VCPlay is a BSD-licensed implementation of the TrueCrypt and VeraCrypt volume
formats. Its command-line tool is named `vcplay`.

This implementation supports mapping (opening) both system and normal TrueCrypt
volumes, as well as opening hidden volumes and opening an outer volume while
protecting a hidden volume. There is also support to create volumes, including
hidden volumes, etc. Since version 1.1, there is also support for restoring
from the backup header (if present), change passphrase, keyfile and PBKDF2
PRF function.

Since VCPlay uses dm-crypt (or dm_target_crypt on DragonFly) it makes full use
of any available hardware encryption/decryption support once the volume has been
mapped.

It is based solely on the documentation available on the TrueCrypt website,
many hours of trial and error and the output of the Linux' TrueCrypt client.
As it turns out, most technical documents on TrueCrypt contain mistakes, hence
the trial and error approach.


VeraCrypt support
==========
VCPlay supports creating, modifying and mapping VeraCrypt volumes. See the
[`vcplay(8)` manual](vcplay.8) for command-line options.


Implementation notes
==========
DragonFly BSD uses the hybrid OpenSSL + cryptodev(9) approach that can be
found in crypto-dev.c. OpenSSL is only used for the hash/pbkdf2. The
encryption/decryption is performed via cryptodev(9) with enabled cryptosoft.

On Linux gcrypt is used for the encryption and decryption. For the hash/pbkdf2
either gcrypt or OpenSSL can be used. gcrypt only supports pbkdf2 since its
July 2011 release (1.5.0), while OpenSSL has had pbkdf2 since around December
2010, so its easier to find in most distros.

The crypto options can be chosen with make/Makefile parameters. Building on Linux
is as easy as doing

    make -f Makefile.classic SYSTEM=linux

you can even skip the SYSTEM=linux, since that's the default. To choose the
PBKDF backend, you can use either,

    make -f Makefile.classic PBKDF_BACKEND=openssl

or

    make -f Makefile.classic PBKDF_BACKEND=gcrypt

The interface to device mapper is libdevmapper on Linux and libdm on DragonFly.
libdm is a BSD-licensed version of libdevmapper that I hacked together in a few
hours.

On Ubuntu, the following dev packages are needed to build VCPlay:

    apt-get install build-essential libdevmapper-dev libgcrypt11-dev uuid-dev


cmake
----------
VCPlay can be built with CMake:

    mkdir objdir
    cd objdir
    cmake ..
    make

NOTE: building inside the source directory is discouraged, so please do
build inside an "objdir" directory. If you happen to do it anyway, you can
clean up behind cmake using:

    make -f Makefile.classic clean_cmake_mess

Before running `cmake`, make sure you have `pkg-config` installed; e.g. on a
Debian or Ubuntu system you can install it using:

    apt-get install pkg-config

This process will check for dependencies and automatically select whether to
use OpenSSL or gcrypt as PBKDF backend.

The build also generates `vcplay.pc`. After installation,
`pkg-config --cflags --libs vcplay` prints the compiler and linker flags for
`libvcplay`.

The classic single-file Makefile can still be used for building, however, using

    make -f Makefile.classic

Or, if you only want the command line tool:

    make -f Makefile.classic vcplay


Library
==========
The C library is named `libvcplay`. Its public header is `vcplay_api.h`, and
its functions use the `vc_api_` prefix. The [`vcplay(3)` manual](vcplay.3)
describes the available operations and their parameters.


Documentation
==========
The installed manuals are available as `man 8 vcplay` for the command-line
tool and `man 3 vcplay` for the C API.



Download for packaging
==========
The [VCPlay source archive](https://github.com/dennisrazumovsky/VCPlay/archive/refs/heads/master.tar.gz)
contains the current `master` branch. A specific commit can be used for a
reproducible package build.



Bugs
==========
Where practical, bug reports should include a small test volume that
demonstrates the issue in VCPlay.
The reduce_test_vol.sh script in test/ can significantly reduce the size
of a volume when compressed by stripping out all the unnecessary data,
leaving only the headers. After that, just bzip2 it and it should be fairly
tiny.

What would be even better is if you could write a small test case to
reproduce the issue. The README in the test/ directory has information on
how to write tests for VCPlay.



OS Support
==========
VCPlay supports Linux and DragonFly BSD. The original `tcplay` utility is
part of DragonFly BSD and has been packaged by several Linux distributions.



Licensing
==========
The project is under a two-clause BSD license. I would consider dual-licensing
it if required. Drop me an email to discuss the options.



Development
==========
Fixes, new features and ports can be submitted as
[pull requests to VCPlay](https://github.com/dennisrazumovsky/VCPlay/pulls).



Bugs in the TrueCrypt documentation
==========
The TrueCrypt documentation is pretty bad and does not really represent the
actual on-disk format nor the encryption/decryption process.

Some notable differences between actual implementation and documentation:
 - PBKDF using RIPEMD160 only uses 2000 iterations if the volume isn't a system
   volume.
 - The keyfile pool is not XOR'ed with the passphrase but modulo-256 summed.
 - Every field *except* the minimum version field of the volume header are in
   big endian.
 - Some volume header fields (creation time of volume and header) are missing
   in the documentation.
 - All two-way cipher cascades are the wrong way round in the documentation,
   but all three-way cipher cascades are correct.

