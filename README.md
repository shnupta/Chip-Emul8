# Chip-Emul8
<p>My first little emulator of a Chip-8<p>


<h2>The Idea</h2>
<p>So I first decided to code this Chip-8 emulator around a week ago as I've really wanted to get into emulation and as far as I was aware - writing one is the best way to learn about it. And I can definitely agree. I learnt a lot when going through this and searching the net for info, how to do certain things, using libraries etc and it has all been a great experience. The plan was that this would introduce me to the basics of emulation so I can progress to coding a Gameboy DMG-01 emulator... eventually.</p>

<h2>What I Did</h2>
<p>I started where most people probably would: how to code a chip 8 emulator search in Google. I found the multigesture post linked at the bottom and I got going. I modelled the CPU, added the emulators basic functions and it was all good. I then tweaked around with some of the modelling for example using the <stdint.h> header so I could model the registers as uint8_t rather than unsigned char. These little things just made it a bit easier to understand and keep track of everything. After that I went on to begin decoding the opcodes using the Cowgod's technical reference linked down below (as best as I could) and then took a look at a couple of the other emulators down below as reference on some of the instructions I was unsure about. When I finished writing them for the first time and had fixed the small syntax errors etc. I ran the program and it was fine! I couldn't believe that it actually worked (wasn't what I was expecting but then again there are only 36 instructions). I went on to looking at how I was going to draw the graphics. From what I had looked up I was either going to use SDL2 or GLUT. After playing round with both I decided to go with GLUT - primarily because it ships with OSX but also because it just seemed a bit easier to setup and initialise. To draw the graphics on screen was fairly simple as the Chip-8 has monochrome graphics and a 64 x 32 pixel display. Just iterate through my screen array and draw the pixels. On first run the picture was a mess. So I spent around an hour going back through my opcodes optimising some and correcting others and now that is all done so here we have the Chip-Emul8.</p>

<h2>Plans</h2>
<p>The only thing I have planned for now is to implement sounds, that's it.</p>


<h2>Screenshots</h2>
<h3>Mac</h3>
<img src="Screenshots/Pong-Chip8 Mac.png"/>

<h2>How To Use It</h2>
<p>Currently I've only tested this on Mac and I believe you have to download GLUT on other OSs. First off, download the repo and open it up. Navigate to the directory and run</p>
<h3>Mac</h3>
```make build```
<p>After that, to use the emulator type</p>
```./ch8 ./<path to your chip8 rom>```
<p>Controls... I'm not exactly sure as to the controls for all the games but the keys will be one of: 1, 2, 3, 4, q, w, e, r, t, y, a, s, d, f, z, x, c, v. To quit the emulator press esc.</p>

<h3>Windows</h3>
<p>To be done..</p>

<h3>Linux</h3>
<p>In the process of providing a linux source.</p>

<h5>Feel free to use the code so long as you give credit to me.</h5>

<h3>Here are some useful links:</h3>

<h4>About the CHIP-8:</h4> 
<p>https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description</p>

<h4>Chip-8 Opcodes:</h4>
<p>http://devernay.free.fr/hacks/chip8/C8TECH10.HTM</p>

<h4>Writing an emulator:</h4>
<p>http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/</p>
<p>http://mattmik.com/files/chip8/mastering/chip8.html</p>
<p>http://emulator101.com/</p>
<p>http://fms.komkon.org/EMUL8/HOWTO.html</p>

<h4>ROMS:</h4>
<p>http://www.zophar.net/</p>

<h4>Reading in files as byte arrays:</h4>
<p>http://www.linuxquestions.org/questions/programming-9/c-howto-read-binary-file-into-buffer-172985/</p>

<h4>Some other good emulators:</h4>
<p>https://github.com/eshyong/Chip-8-Emulator</p>
<p>https://github.com/bryangarza/chip8</p>
<p>https://github.com/prophittcorey/iC8</p>

<h5>Read my blog to find out more about the project: http://caseywilliams.tk/blog</h5>
