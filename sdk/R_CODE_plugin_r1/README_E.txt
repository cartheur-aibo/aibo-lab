The "R-CODE Plug-In for the OPEN-R SDK" is covered
by the OPEN-R SDK's END-USER LICENSE AGREEMENT.

===============================
[Development kit]
Files that cannot be redistributed
===============================
   +--OPEN_R_SDK
      +--OPEN-R
         +--MS_ERS7
         |  +--RCODE
         |     +--Jpn
         |     |  +--OPEN-R  (MS Image for R-CODE Plug-In Japanese edition)
         |     |
         |     +--Eng
         |        +--OPEN-R  (MS Image for R-CODE Plug-In English edition)
         |
         +--include
            +--OPENR         (header files for RMain & RPlugIn)

===============================
[Sample program]
Files that can be redistributed 
===============================
   +--sample
      +--RMain
      |  +--src
      |
      +--RPlugIn
         |  Makefile
         |  README.txt
         |
         +--MS
         +--RPlugIn
         +--echo_client 
           
Note 1:
 Sample includes RMain (R-CODE main object) source code.
 But you cannot make the RMain object by using the OPEN-R SDK.
 Disclosing the RMain source code will help you to make
 R-CODE Plug-Ins.

Note 2:
 Sony will not answer any questions regarding
 "R-CODE Plug-In for the OPEN-R SDK."


===================================================================
  How to use the "R-CODE Plug-In the OPEN-R SDK" sample MS image. 
===================================================================

  Step1. Copy OPEN_R_SDK/OPEN_R/MS_ERS7/RCODE/{Jpn,Eng}/OPEN-R
         to your AIBO Programming Memory Stick .

  Step2. Copy sample/RPlugIn/MS/OPEN-R
         to that AIBO Programming Memory Stick.

  Step3. Boot AIBO with that AIBO Programming Memory Stick.

  (also see sample/RPlugIn/README.TXT)
