# M5-Unified-NTP-Clock

NTPを使用した時計アプリです

<img src="https://github.com/yankee-08/M5-Unified-NTP-Clock/blob/main/image/img_001.JPG" width="400" />

## Description

- 以前、作成した物を`M5Unified.h`用に一部修正した物をGitHubに載せています
- 起動時、NTPサーバにアクセスし、時刻情報を取得したらその時刻データをディスプレイ上に表示
- NTPへのアクセスを減らすため、一度NTPアクセス後一定時間はソフトウェアタイマーで動作
- NTPへのアクセス頻度は`NTP_ACCESS_MS_INTERVAL`で定義しています（初期値`300000`）

## Requirement
### Hardware

ディスプレイ付きM5Stackシリーズ  
Unifiedライブラリを使用しているため、M5Stackシリーズに書き込みはできますが、画面サイズが320 x 240のモデルを基準に作成しているため、他のモデルだとうまく動作しないと思います。

動作確認済み機種

- M5Stack Core2
- M5Stack Fire

### Software

- Visual Studio Code
- PlatformIO

## Usage

- [config.h](https://github.com/yankee-08/M5-Unified-NTP-Clock/blob/main/include/config.h) 内の`{_SSID_}`,`{_key_}`を自宅の環境に合わせて設定してください
- 背景色を変えたい場合は、`bkground_color`の変数を適宜変更してください
- 予め定義されている色については、以下のページを参照（// New color definitions use for all my librariesの部分）
  https://github.com/m5stack/M5Stack/blob/master/src/utility/In_eSPI.h

## License

[MIT](https://github.com/yankee-08/M5-Unified-NTP-Clock/blob/main/LICENSE)  

## Author

[yankee](https://github.com/yankee-08)  
