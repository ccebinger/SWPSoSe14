"find function":
- $ muss als erstes Zeichen am Zeilenanfang stehen
- davor dürfen beliebige Zeichen kommen (auch $, wenn es nicht am Zeilenanfang steht)
- wenn nicht $ gelesen wird: wechsele in Zustand "ignore line while find function"
- wenn $ gelesen: wechsele nach "find first semicolon"

"ignore line while find function":
- überspringe jedes Zeichen, außer Zeilenumbruch ('\n')
- bei Zeilenumbruch, wechsele in Zustand "find function"

"find first semicolon":
- lies solange, bis ' geparsed wird, dann wechsele nach "parse function name"

"parse function name":
- lies solange, bis ' geparsed wird, und speichere die gelesenen Zeichen
- bei ' wechsele nach "ignore line after function found"

"ignore line after function found":
- überspringe jedes Zeichen, außer Zeilenumbruch ('\n')
- bei Zeilenumbruch, wechsele in Zustand "parse rail"

"parse rail":
- parse alle Zeichen und speichere sie
- wenn $ gelesen wird, wechsele zu "find first semicolon" (neue Funktion gefunden)
- wenn -1 gelesen wird (end of file), brich ab
