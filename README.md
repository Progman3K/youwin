# YouWin

Embeddable c-callable Win32 API.

Graphical or Text (TTY) output,

One set of sources for your app, graphical or text.

any chip architecture, x86, x86_64, Arm, PPC32, etc...

Supported: OS/X, iOS, Android, Linux, additional platforms.



## Build-time prerequisites

* bash - Attention OSX users, you must upgrade bash v3.x to v5.x
* gnu make
* imake
* makedepend
* gccmakedep

* windres (part of binutils and also MinGW)
	- LLVM's windres can be used instead, but clang **must** be present in this case

* MinGW (optional)
* a c++17 compiler

### Target dependencies

#### Android

	* android-ndk

#### iOS

	* iOS Frameworks (xcode)

#### OpenGL

	* Freeglut/glapi

#### X11

	* libX11

#### Linux Framebuffer

	* no dependancies

#### Text

	* a 24 bit colour capable VT100 terminal program

	Note:
	When running a debug text-mode app,
	you must redirect stderr to a file,
	otherwise it will interfere with rendering the app.
	Redirect stderr like so:

	./app 2>debug_output.txt

	You can
	tail -f debug_output.txt
	in another terminal to watch the debug info live.


---
***

### Recommended development strategy:


Go to whichever folder you wish to work in (home is used here), retrieve the repository, and link it to /opt/youwin

```
cd ~
git get youwin
cd youwin/samples/madchatter
../../ywconfigure android|ios|opengl|text|xwin|lxfb [debug/release]
make depend (optional)
make
```

It is preferable to either add the youwin folder to the path or put a symlink to the ywconfigure command in a folder included in the path, but it is not necessary

Tools:

* **dblog**
	- A console application that displays the udptracer debug log information.

* **dbwin**
	- A graphical application that displays the udptracer debug log information.

* **fontnab**

    This tool permits grabbing (nabbing) any font from Windows.

    The situation with fonts:

    The fonts included were captured from a Linux distro and are thought to be copyright-free.

    - Courier
    - Fixedsys
    - MS Sans Serif
    - Small Fonts
    - System

    However, any font you have the legal right to can be used:

    *fontnab -f fontname ...*

    Permits you to extract installed font definitions and convert them into a text format that can be read by **fntc**.

* **fntc**
    - Font compiler, parses text definitions of fonts and turns them into binary objects.

* **genasm**
    - A command-line tool that generates an assembler file the compiler will link as a resource-segment.

___

See the samples folder for an example application.


**Thanks**:

r/bash at reddit


**Log**:

v 0.9 aka Jovette
