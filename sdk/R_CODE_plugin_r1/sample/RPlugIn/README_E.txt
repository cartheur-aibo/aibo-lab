R-Code Plug-In sample for the OPEN-R SDK

[Summary]
  Contents
  How to make the MS image.
  Functions


[Contents]
  R_CODE_plugin_r1/sample/RPlugIn/README.TXT
    This file.

  R_CODE_plugin_r1/sample/RPlugIn/RPlugIn
    RPlugIn source code files.
  
  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/PLUGIN.CFG
    The configuration file for RMain and RPlugIn.

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/RPlugIn.cfg
    A sample configuration file for RPlugIn commands and variables.

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/R-Code.R
    A sample R-CODE script file with RPlugIn providing commands.

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/NTP.CFG
    A sample configuration file for RPlugIn NTP.

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/INDEX.HTM
    A sample html file to check the result of RPlugIn commands

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/DATA/P/RCODE/
    The install directory for RPLUGIN.BIN.


[How to make the MS image]
  0. Set up the OPEN-R SDK

  1. Make RPlugIn
      You need "R_CODE_plugin_r1/OPEN_R_SDK" files.
      R_CODE_plugin_r1.tar.gz has that files.

      % cd .../R_CODE_plugin_r1/sample/RPlugIn/
      % make
      % make install

  2. Copy files to your AIBO Programming Memory Stick.

    Step1. Copy OPEN_R_SDK/OPEN_R/MS_ERS7/RCODE/{Jpn,Eng}/OPEN-R
           to your AIBO Programming Memory Stick .

    Step2. Copy sample/RPlugIn/MS/OPEN-R
           to that AIBO Programming Memory Stick.

[Functions]
  1. Save Image to BMP file.
  2. Save Audio to WAV file.
  3. Send NTP Packet.

  ----------
  1. Save Image to BMP file.

   If RPlugIn received this command, it saves OccImageData as a bitmap
   format file .

   [Format]
   SAVE:IMAGE:(filename)
     filename = The file name must be 8.3 format. (ex. IMAGE1.BMP)

   [Output]
   /OPEN-R/APP/PC/AMS/(filename)

  ----------
  2. Save Audio to WAV file.

   If RPlugIn received this command, it starts buffering the OccAudioData
   for about 8[s] and saves the buffered data as a wave formatted file.

   [Format]
   SAVE:AUDIO:(filename)
     filename = The file name must be 8.3 format. (ex. AUDIO1.WAV)

   [Output]
   /OPEN-R/APP/PC/AMS/(filename)

  ----------
  3. Send NTP Packet.

   If RPlugIn received this command, it sends an NTP Packet.
   An NTP sever name is given in /MS/OPEN-R/APP/PC/AMS/NTP.CFG.
   You need to configure the wireless LAN parameters.   

   [Format]
   SEND:NTP

   [Output]
   You can check the result with echo_client.
   If SEND:NTP was successful, you will get date strings echoed back.

   % cd .../RPlugIn/echo_client/
   % make
   % ./echo_client.exe 192.168.XX.XX

