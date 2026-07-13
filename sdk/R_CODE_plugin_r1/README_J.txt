"R-CODE Plug-In for the OPEN-R SDK" は 
OPEN-R SDK ソフトウェア使用許諾契約書に則ります。

===============================
[Development kit]
再配布不可なファイル
===============================
   +--OPEN_R_SDK
      +--OPEN-R
         +--MS_ERS7
         |  +--RCODE
         |     +--Jpn
         |     |  +--OPEN-R  (MS イメージ for R-CODE Plug-In 日本語版)
         |     |
         |     +--Eng
         |        +--OPEN-R  (MS イメージ for R-CODE Plug-In 英語版)
         |
         +--include
            +--OPENR         (header files for RMain & RPlugIn)

===============================
[Sample program]
再配布可能なファイル
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
 Sample ディレクトリには RMain (R-CODE メインパート) のソースコードが含ま
 れますが、OPEN-R SDK を使ってコンパイルすることはできません。RMain の
 ソースコードは、R-CODE Plug-In 開発参考のため公開いたしました。

Note 2:
 ソニー(株)は "R-CODE Plug-In for the OPEN-R SDK" に関して、いかなる質問に
 もお答えいたしません。


====================================================================
  "R-CODE Plug-In for the OPEN-R SDK" サンプル MS イメージの使い方
====================================================================

  Step1. OPEN_R_SDK/OPEN_R/MS_ERS7/RCODE/{Jpn,Eng}/OPEN-R を
         AIBO プログラミングメモリースティックにコピーしてください。

  Step2. Step1 の AIBO プログラミングメモリースティックに
         sample/RPlugIn/MS/OPEN-R を上書きしてください。

  Step3. Step2 の AIBO プログラミングメモリースティックを使って
         アイボを起動します。

  (機能などの詳細については sample/RPlugIn/README.TXT を参照してください。)
