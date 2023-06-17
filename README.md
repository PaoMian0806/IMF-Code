<div align='center'>
<img src='https://github.com/PaoMian0806/IMF-Code/blob/main/appicon.png' height='100px'>
</div>

<div align='center'>
<h1>IoT Smart Fans</h1>
</div>
<p align='center'>A School Project Undertaken During High School.</p>
<p align='center'>Created By Pao Mian (YING, CHIN-WEN) in 2021-2022</p>

Introduction - 介紹
---

因近年來的科技技術日新月異，許多舊有的家電設備被賦予了之前無法達成的便捷功能，且隨著網際網路的發展，物聯網的技術也變得更加成熟。因此本人（或稱本Project）決定以風扇作為開發對象，建構出一個架構簡易且容易移植及應用的模型，不但有著基本的自動操作功能，還同時兼容了手動以及自動操作兩種選擇供使用者挑選，還連結了物聯網技術實現資料傳輸功能，將這些資料去作整理與分析。目前（2023.05）本Project已處於開發中止階段，但不排除會在未來有重啟此項目的可能；故建立此Repository用來儲存曾經開發的Source Code。

Description - 說明
---

placeholder.

<div align='center'>
<img src='https://github.com/PaoMian0806/IMF-Code/blob/main/Screenshot_01.jpg' height='500px'>
</div>
<p align='center'>OpenData 資料查閱介面</p>

How To Contribute - 開發指南
---

若有志同道合者、或欲對本項目之Source Code進行添加、修改等貢獻，建議先使用[Arduino IDE](https://www.arduino.cc/en/software)以及[APP Inventor 2](https://appinventor.mit.edu)對原檔案進行開發作業，以便快速熟悉具體操作方式及程式架構。若在途中有更換更換開發環境之念頭 (諸如使用[Thonny](https://thonny.org)、[Flutter](https://flutter.dev)等)，可依照現有開發資訊及穩定程度來選擇之。

硬體方面的元件需求如下：

- Arduino Nano
- NodeMCU-32S
- Nano IO Shield V1.0
- DHT Sensor
- Brushed DC Motor 130-Size 6V
- TB6612FNG Motor Driver Board
- LCD1602 I2C Blue
- HC-05 Bluetooth Module
- LM2596 DC-DC Buck Adjustable Step-Down Power Supply Converter Module 
- JQC-3FF-S-Z-5V Relay

所有的Source Code所採用的開發環境如下：

- 採用開發板：
    - Arduino Nano
        - 處理器：Atmega328P（Old Bootloader）
    - NodeMCU-32S
        - Upload Speed：115200
        - Flash Frequency：80MHz

- 軟體 & 環境需求：
    - Arduino IDE
    - APP Inventor 2
    - FireBase

Award List - 競賽獎項
---

- 高二-110學年校內發明與專題製作競賽-佳作
- 高二-110學年第二學期校內資訊科專題實作期末成果發表會-特優
- 高二-111年0315梯次之全國高級中等學小論文寫作比賽-優等
> 1110315 > 臺北 > 工程技術類 > 市立內湖高工 > 物聯網辦智慧風扇
> 
> [中學生網站連結](https://www.shs.edu.tw/Customer/Winning/EssayIndex)
- 高二-AEUST-2022年全國高中職電資創意應用競賽-入圍
    - 高二-AEUST-2022年全國高中職電資創意應用競賽-電子電機應用組佳作
- 高二-臺北市第55屆中小學科學展覽會-佳作
    - 高二-臺北市第55屆中小學科學展覽會-探究精神獎
- 高二-2022年第十八屆全國電子設計創意競賽暨學術研討會-入圍
- 高三-2022全國資訊管理前瞻技術研討會暨專題競賽-入圍
    - 高三-2022全國資訊管理前瞻技術研討會暨專題競賽-高中專題組冠軍
- 高三-2022年工程科技技術應用研討會-發表證明

Other Credit - 採用之資源
---

- Arduino Library
    - [FirebaseJson](https://github.com/mobizt/FirebaseJson) - By [Mobizt](https://github.com/mobizt)
    - [政府資料開放平臺 - OpenData](https://data.gov.tw) - By 數位發展部
- ESP32 Node-MCU Library
    - a
    - b
    - c

<p align='center'>感謝侯淇健師、牛暄中師、林傳傑師於這兩年間的細心指導、評點，本研究才能順利完成。</p>

License - 著作權說明
---

MIT 開源許可協議（MIT License）

版權所有 (c) 2023，PaoMian。

特此向任何得到本軟件副本或相關文件的人授權：
被授權人有權使用、複製、修改、 合併、出版、發佈、散佈、
再授權和/或販售軟件及軟件的副本，及授予被供應人同等權利，
只需服從以下義務：

在軟件和軟件的所有副本中都必須包含以上版權聲明和本許可聲明。

該 軟 件 是 「按 原 樣」 提 供 的 ， 沒 有 任 何 形 式 的 明 示 或 暗 示 ， 
包 括 但 不 限 於 為 特 定 目 的 和 不 侵 權 的 適 銷 性 和 適 用 性 的 保 證 擔 保 。 
在 任 何 情 況 下 ， 作 者 或 版 權 持 有 人 ， 都 無 權 要 求 任 何 索 賠 ， 
或 有 關 損 害 賠 償 的 其 他 責 任 。 無 論 在 本 軟 件 的 使 用 上 或 其 他 買 賣 交 易 中 ，
是 否 涉 及 合 同 ， 侵 權 或 其 他 行 為 。
