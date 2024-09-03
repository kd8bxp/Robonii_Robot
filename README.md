
The RobonII was a programmable toy from (I think) 2013.  It has a ATXMEGA and a "special" version of the
Arduino IDE. Unfortuntally it was for Windows Only.  In 2024 I attempted to just copy the library and board core over to a linux box. And I think I finally got something to compile, but it didn't have the correct programmer, and I couldn't find a programmer that would work....  

I also tried to use Wine but couldn't get Wine to use a serial port. I thought about using a VM, but that didn't work for me either.  

Sadly I booted up a Windows partition for the first time since probably 2015 (or so).  I found that already had this special version of the IDE installed. AND I found some limited code that someone made called TIANchibi_Robonii - it uses a couple of special libraries that probably even back I couldn't find those libraries. So that code will need to be ported and changed to work with the library that is included with the board core and the IDE.  

I wrote the Green_Cylon sketch just to make sure it worked and I could program the robot. And it did.  

I used Archive.org to find the original roboni website (the last real update was in 2016).
https://web.archive.org/web/20160317143357/http://www.robonii.co.za/  

And the Arduino Download Page was (amazingly) still active from google docs. (I can't promise that it will be forever) Maybe I should upload the IDE to the Archive (?) It is of course too large to upload to github.  

So this is just my attempt to document that this can still be used and programmed, even thou there is very very little information left on the internet about it.  
I have no plans for this robot.  
 
* I have included the Tianchibi Robonii project files, I honestly have no idea where they came from originally, I tried to find them again, and didn't have any luck.  

