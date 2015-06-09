# Build libcapt at windows #

  1. you can find visual c++ solution file at "builds/win32" fold, like "vc2005"
  1. just open and build it,it is simple!
  1. the execute file of captUtil and sample is build in "bin" fold

# Build libcapt at linux #
  1. make file for linux like system is put in "builds/linux" fold.
  1. type "make" to build libcapt.a and captGen

# HelloWorld #
  1. try "captGen" fist, this program can generate a captcha image with tga format
  1. run captGen at console mode at either windows or linux system
```
   captGen letters.cpf captcha.tga
```
  1. you can find letters.cpf file at "examples" fold, It's a simple cpf file include 26 letters
  1. open "captcha.tga"
> > ![http://www.thecodeway.com/files/libcapt_demo1.gif](http://www.thecodeway.com/files/libcapt_demo1.gif)