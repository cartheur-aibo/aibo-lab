R-Code Plug-In sample for the OPEN-R SDK

[目次]
  同梱するファイル・ディレクトリの概要
  MS イメージの作成方法
  RPlugIn サンプルの機能


[同梱するファイル・ディレクトリの概要]
  R_CODE_plugin_r1/sample/RPlugIn/README.TXT
    このファイル

  R_CODE_plugin_r1/sample/RPlugIn/RPlugIn
    RPlugIn ソースコードファイルを含むディレクトリ
  
  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/PLUGIN.CFG
    RMain と RPlugIn の設定ファイル。

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/RPlugIn.cfg
    RPlugIn が提供するコマンドや変数を登録するファイル。

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/R-Code.R
    R-CODE サンプルスクリプト。RPlugIn サンプルのコマンド使用例。

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/NTP.CFG
    NTP サーバ名設定ファイル。RPlugin サンプルのコマンド SEND:NTP が参照する。

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/INDEX.HTM
    サンプル html ファイル。
    RPlugin サンプルのコマンド実行結果をブラウザで確認する。

  R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/DATA/P/RCODE/
    RPLUGIN.BIN を配置するためのディレクトリ


[MS イメージの作り方]
  0. OPEN-R SDK をセットアップしてください。

  1. RPLUGIN.BIN の作成
      RPLUGIN.BIN の作成には "R_CODE_plugin_r1/OPEN_R_SDK" が必要です。
      "R_CODE_plugin_r1/OPEN_R_SDK" は R_CODE_plugin_r1.tar.gz に含ま
      れます。

      % make
      % make install

  2. MSイメージの作成

      Step1. "R-CODE Plug-In for the OPEN-R SDK" のベース MS イメージ
             OPEN_R_SDK/OPEN_R/MS_ERS7/RCODE/{Jpn,Eng}/OPEN-R を
             AIBO プログラミングメモリースティックにコピーしてください。
      
      Step2. sample/RPlugIn/MS/OPEN-R を Step1 で作った MS に
             コピーしてください。
             

[RPlugIn サンプルの機能]
  1. 画像を BMP ファイルに保存
  2. 音声を WAV ファイルに保存
  3. NTP による時刻問い合わせ

  ----------
  1. 画像を BMP ファイルに保存

   RPlugIn サンプルが SAVE:IMAGE を受け取ると, 受け取り直後の
   OccImageData を BMP ファイルで保存します。

   [書式]
   SAVE:IMAGE:(ファイル名)
     ファイル名 = 保存するファイルの名前
                  DOS形式(8.3文字)で指定すること (例. IMAGE1.BMP)

   [出力先]
   /OPEN-R/APP/PC/AMS/(ファイル名)

  ----------
  2. 音声を WAV ファイルに保存

   RPlugIn サンプルが SAVE:AUDIO を受け取ると、受け取り後から約８秒間の
   音声データ(OccAudioData) を貯蔵し、WAV ファイルで保存します。

   [書式]
   SAVE:AUDIO:(ファイル名)
     ファイル名 = 保存するファイルの名前
                  DOS形式(8.3文字)で指定すること (例. AUDIO1.BMP)

   [出力先]
   /OPEN-R/APP/PC/AMS/(ファイル名)

  ----------
  3. NTP による時刻問い合わせ

   RPlugIn サンプルが SEND::NTP を受け取ると, 指定された NTP サーバーへ
   時刻を問い合わせます。
   NTP サーバーは /MS/OPEN-R/APP/PC/AMS/NTP.CFG で指定します。
   この機能を利用するには無線 LAN の設定が必要です。

   [書式]
   SEND:NTP

   [出力]
   コマンドが成功した場合、同梱の echo_client (RPlugIn/echo_client) を
   使って結果(日付表示)を確認ができます。

   echo_client の使い方 (AIBO の IP アドレスが 192.168.XX.XX の場合)
   % cd .../RPlugIn/echo_client/
   % make
   % ./echo_client.exe 192.168.XX.XX

