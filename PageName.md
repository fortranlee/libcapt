# How to generate a captcha image with libcapt. #
> You can find a helloworld sample at "examples" fold
  1. load libcapt font with **libCapt::FontFile**
```
      libCapt::FontFile fontFile;
      fontFile.loadFromDataStream(fontStream, fontStreamSize);
```
  1. generate captcha question with **libCapt::Generator**
```
      libCapt::Question question;
      libCapt::Generator generator(&fontFile);
      generator.generateQuestion(question);
```
  1. libCapt::Question include a captcha image with four random character.
```
      unsigned char imageBuf[IMAGE_BUF_LENGTH];  //image data with rle compressed
```
> > ![http://www.thecodeway.com/files/libcapt_demo1.gif](http://www.thecodeway.com/files/libcapt_demo1.gif)
  1. also, libCapt::Question include four answer with one correct.
```
      unsigned short wAnswer0[ANSWER_LENGTH];  //unicode character
      unsigned short wAnswer1[ANSWER_LENGTH];
      unsigned short wAnswer2[ANSWER_LENGTH];
      unsigned short wAnswer3[ANSWER_LENGTH];
      int nCorretAnswer;
```
  1. you can transfer **imageBuf** to client now
  1. decompress imageBuf with **libCapt::rleDecompress** at client
```
       unsigned char bitBuf[IMAGE_BUF_LENGTH];
       if(question.isCompressed())
       {
         rleDecompress(question.imageBuf, question.getSize(), IMAGE_WIDTH, IMAGE_HEIGHT, bitBuf, bufSize);
       }
       else
       {
         memcpy(bitBuf, question.imageBuf, IMAGE_BUF_LENGTH);
       }
```
  1. now, you can show image on device with imageBuf data(4bit format)