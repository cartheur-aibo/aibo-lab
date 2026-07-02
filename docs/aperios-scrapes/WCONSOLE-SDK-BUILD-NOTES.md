# WCONSOLE SDK Build Notes

This note distills the local Aperios/Open-R PDFs into the shortest practical
answer to:

- how do we build and run a plain OPEN-R SDK program on ERS-7 with `WCONSOLE`?

The full text scrapes live alongside this note:

- [ProgrammersGuide.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios-scrapes/ProgrammersGuide.txt)
- [InstallationGuide_E.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios-scrapes/InstallationGuide_E.txt)
- [ModelInformation_7_E.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios-scrapes/ModelInformation_7_E.txt)
- [InternetProtocolVersion4.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios-scrapes/InternetProtocolVersion4.txt)

Focused excerpt files:

- [ProgrammersGuide-WCONSOLE-OPENR-section.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios-scrapes/ProgrammersGuide-WCONSOLE-OPENR-section.txt)
- [InstallationGuide-WCONSOLE-HelloWorld-section.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios-scrapes/InstallationGuide-WCONSOLE-HelloWorld-section.txt)
- [InstallationGuide-WCONSOLE-RP-section.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios-scrapes/InstallationGuide-WCONSOLE-RP-section.txt)

## Core Doc Answer

From the Programmer's Guide:

- choose one of:
  - `BASIC/memprot/OPEN-R`
  - `BASIC/nomemprot/OPEN-R`
  - `WLAN/memprot/OPEN-R`
  - `WLAN/nomemprot/OPEN-R`
  - `WCONSOLE/memprot/OPEN-R`
  - `WCONSOLE/nomemprot/OPEN-R`
- copy the chosen `OPEN-R` directory to the root of the AIBO Programming
  Memory Stick
- copy your built object binaries into `/OPEN-R/MW/OBJS/`
- edit `OBJECT.CFG`, `CONNECT.CFG`, and `DESIGNDB.CFG` under
  `/OPEN-R/MW/CONF/` as needed

From the Installation Guide:

- for plain wireless-console execution on ERS-7, the HelloWorld example uses:
  - `/usr/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R`
  - plus the sample's own `MS/OPEN-R` tree
- then:
  - edit `WLANCONF.TXT`
  - insert the wireless LAN card and programming stick
  - boot AIBO
  - connect with `telnet AIBO_IP 59000`

For Remote Processing OPEN-R:

- the guide says the AIBO-side system-object configuration is limited to
  `nomemprot`
- so the directory to copy is:
  - `/usr/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/nomemprot/OPEN-R`

## Minimal ERS-7 WCONSOLE SDK Recipe

This is the clean SDK-native path, without any MIND 2 preservation logic:

1. Build the SDK sample or app.
   Example from the guide:
   ```bash
   cd /mydir/sample/common/HelloWorld
   make install
   ```
2. Copy the SDK ERS-7 WCONSOLE base to the programming stick root.
   For normal console use:
   ```text
   /usr/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R
   ```
   For Remote Processing:
   ```text
   /usr/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/nomemprot/OPEN-R
   ```
3. Copy the sample or app's `MS/OPEN-R` overlay onto the same stick.
4. Edit `/OPEN-R/SYSTEM/CONF/WLANCONF.TXT`.
5. Insert the wireless LAN card and the programming stick.
6. Boot AIBO.
7. Connect to the wireless console with:
   ```bash
   telnet AIBO_IP 59000
   ```

## What This Means For Our Repo

The official SDK flow is indeed simple for plain OPEN-R SDK work:

- build object(s)
- copy one SDK `OPEN-R` base
- copy one app/sample `MS/OPEN-R` overlay
- edit `WLANCONF.TXT`
- boot and use port `59000`

What made our earlier work hard was not the SDK flow itself. It was trying to
layer SDK `WCONSOLE` behavior onto a preserved retail MIND 2 runtime lineage.

That is a different problem from "how do I run an SDK WCONSOLE sample on ERS-7?"

## Practical Guidance

If the goal is to validate plain SDK `WCONSOLE` behavior:

- favor the pure SDK path above
- treat `memprot` as the default for ordinary sample execution
- switch to `nomemprot` when following the Remote Processing OPEN-R path the
  guide describes

If the goal is to preserve MIND 2 runtime behavior while adding SDK services:

- do not assume the simple SDK copy recipe will remain safe
- keep using the boot-risk audit and provenance tools before hardware boots
