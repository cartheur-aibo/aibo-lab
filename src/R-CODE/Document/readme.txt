===========================================================================

	R-CODE110/111 for ERS-110/111

	Copyright (C) 2002 Sony Corporation

	All Rights Reserved.

===========================================================================


Please read the supplied software license agreement (License.txt) before using the software. 

For details on handling AIBO (names of regions, various settings, how to mount "Memory Stick" and the battery, how to recharge the battery, precautions of safe use, etc.), please refer to the instruction manual supplied with AIBO.


/// Introduction ///
R-CODE 110/111 (hereinafter referred to as R-CODE) is a simplified scripting language for programming AIBO.  It allows you to write programs for AIBO easily with the use of sensor information, variables, built-in commands and other elements.

/// Contents of Package ///
1. R-CODE 110/111 Memory Stick Execution Image
2. Readme.txt
3. Programmer's Manual ( Programmers.txt )
4. Reference Manual ( Reference.txt )
5. Musical Scale Recognition List (Tone_List.txt)
6. List of Pattern Names of LED Lights (LED_Light_List.txt)
7. List of Sound Names (Sound_List.txt)
8. Software License Agreement (Licens.txt)
9. ODA File Format (ODA-FFORM-E.txt)
10.MTN File Format (MTN-FFORM-E.txt)
11.Sample Scripts

/// AIBO Performer ///
You can edit motions called from R-CODE using Motion Editor "AIBO Performer Kit ERF-510/511".  For details on how to use AIBO Performer Kit, please refer to the User's Guide supplied with AIBO Performer Kit and the R-CODE Programmer's Manual.

* You can enjoy this software to the fullest extent even if you do not have AIBO Performer Kit.
(AIBO Performer Kit is no longer for sale.)


/// Precautions for Use ///
1. Do not use AIBO where it may fall and/or shaky/unstable places.
2. Do not place objects that may block the movements of AIBO, and do not place fragile items around AIBO.
3. Use AIBO in locations out of reach of small children.
4. Use this software by copying it to an "AIBO Programming Memory Stick".
5. This software is only for AIBO ERS-110/111.


/// How to handle "Memory Stick" ///

Safety Switch -----
Locking the safety switch of "Memory Stick" disallows recording/deleting data.  To use the "Memory Stick" with AIBO, please do not lock this switch.

Note on writing data from a computer -----
1. Do not format the "Memory Stick" using Windows or MS-DOS. Once you format it, the "Memory Stick" may not be properly recognized.
2. If you try to use the "Memory Stick" when the safety switch is locked, the following symptoms may take place.  But, they are not indicative of problems with the "Memory Stick" or the "Memory Stick" drive.
  - On the display of the computer, "Could not write data to drive D*.  Files and data might be lost." appears. (* The drive name "D" varies depending on your computer.)
  - The icon of the data that could not be written remains on the screen of your computer. (It disappears when you eject the "Memory Stick".)
3. In the following cases, the data written to the "Memory Stick" may be deleted or destroyed sometimes during the writing of data.
  - If you remove the "Memory Stick" or turn off your computer while the machine is writing or reading data
  - If you use it in locations where it may be affected by static electricity or electrical noises

To remove the "Memory Stick" from your computer (when you are using a PC card adaptor)-----

When you are using a PC card adaptor, click the icon of the drive that has the "Memory Stick" slot on the task bar of Windows and then click "Stop Standard IDE/ESDI Hardware Controller". 
When the message of "Now, you can remove this device safely." appears, remove the device.
[Note]
1. To remove the "Memory Stick", wait for a while after the completion of writing data to the "Memory Stick".  (Wait for about 15 seconds when your computer has a "Memory Stick" slot.)  If you remove the "Memory Stick" immediately after the completion of writing data to it, the screen may turn blue and the message of "Error in reading disk" may appear.  In that case, reinsert the "Memory Stick" once again, press the Enter key, wait for a while and then remove it.  
2. If you use an AIBO Programming Memory Stick together with one of the following PC card adaptors, problems occur on rare occasions.
   Please read precautions on our Home Page.
   Applicable Models    PC card adaptor for Sony Memory Stick
		-MSAC-PC1
		-MSAC-PC2
		-MSAC-PC2N


2. It is strictly forbidden by copyright protection law to copy the whole or part of the contents of the software and/or the manual without the prior approval of the copyright holder.
3. Sony shall not take any responsibility for inconveniences occurring from the use not described in the manual.
4. This software should be used only with specified equipment.
5. Please note that this software shall be subject to change without prior notice.

OPEN-R is the standard interface of entertainment robot systems that Sony creates for expanding the world of entertainment robots through "Flexible Configuration of Hardware" and "Exchange of Software" for various purposes.
"R-CODE 110/111" adheres to OPEN-R Version 1.

"AIBO", "OPEN-R" and "Memory Stick" are the trademarks or the registered trademarks of Sony Corporation.  For information, the marks such as (R) and TM that indicate the registered trademark and the trademark are omitted in the text of this document.
