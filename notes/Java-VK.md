Java VK name

GRAVE ACCENT `
APOSTROPHE   '
JAVAのキー定義には、GRAVE ACCENTも
APOSTROPHEもない。
しかし、PCでは上記文字はキータイプ可能な文字であり、
実は名前がある。
昔、APOSTROPHEのフォントは垂直ではなかった。そのため、ASCII制定後、
この記号を使用して、'引用部'と表示していた。
テキストのフォントは垂直なので、見た目変ではないが
昔は、’引用部’こんな感じだったらしい。
そして、この記号をなんと、QUOTEと呼んでいた。
つまり、APOSTROPHEキーの位置の文字を慣用としてQUOTEと呼んだ。
しかし、これは少しかっこ悪いので、さらに１文字追加したこれがGRAVE ACCENTになる。
つまり、下記のようになる。
`引用部’
すると、跳ねる向きが逆なので、最初の文字であるGRAVE ACCENTを
BACK QOUTEをとして呼ぶことにした。
Javaの定義は古いPCの定義を使用している。USB UID Tableが定義しているキーと、
VKキーの対応はPlatform仕様に依存する。つまり別途規定する必要がある部分である。
PCのPlatform仕様では、GRAVE ACCENT or Title(0x32 or 0x35)が
VK_BACKQOUTEを生成し、APOSTROPHE or Quotation(0x34)が
VK_QUOTEを生成する使用になっているだけである。