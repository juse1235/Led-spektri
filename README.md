# Led-spectrum

Tuntuiko normaali haalarisalama liian yksinkertaiselta? Epäiletkö, että juotettavia LEDejä ei voi olla liikaa? Oletko aina haaveillut haalarimerkistä, jonka nähtyään kanssaopiskelijat pyörtyvät ihastuksesta?
Mikäli näin on, olet selvästikin motivoitunut nousemaan seuraavalle tasolle yöllisessä yökerhojen ja pystärien bling-bling-loisteessa. Tämä tanssilattioiden katseenvangitsija tarjoaa ainutkertaisen mahdollisuuden turruttaa itsensä vinoon juottuneille komponenteille ja v*tutukselle. Älä missaa tilaisuutta! Minä en missaisi.

Komponentit
| Tyyppi |	Määrä |	Huom |
|--------|-------| ------|
| Atmega328PB-AU	1	Myös 328P voi toimia, mutta vaatii muutoksia kääntäjään |
| LED, 3 mm, jalallinen	64	Ei värikoodia – käytä mielikuvitusta |
| Transistori, SOT-23	16	Käytössä MMBT3904, ei erityisen tarkkaa mallista |
| Vastus, 0603 – 0805, 1+ kOhm		Transistorien ohjaus |
| Etuvastus, 0805	8	Kirkkauden säätö, 100 – 500 ohmia lienee hyvä, säätää myös virrankulutusta, suunnittelijalla käytössä 300 ohmia |
| Isohko vastus, noin 5 kOhm, 0603	2	 |
| Regulaattori, LP2985 – 3.3	1	|
| Kondensaattori, 100 nF, 0805	1	|
| Kondensaattori, 10 uF, 0805	1	|
| Sulake, 500 mA, 1206	1	|
| Oskillaattori, 16 MHz	1	Käytössä CSTCE16M0V53-R0 |
| MAX4466-mikrofonimoduuli |	1 |	Mikrofoneja löytyy eri herkkyyksillä, voi olla tuurissaan, mikä on käyttökelpoinen. Electret sinisellä |piirilevyllä on toiminut toistaiseksi.|


Yleinen toimintaperiaate
Ledien toimintaperiaate on samankaltainen kuin haalarisalamassakin, eli ledejä vilkutetaan matriisissa sarakkeittain säätämällä, mikä sarake saa yhteyden maahan ja suljettua virtapiirin. 
Mikrofoni kuuntelee ääntä, ja mikrokontrolleri lukee siitä tulevaa analogista signaalia jatkuvalla syötöllä. Näytepätkän ollessa valmis mikrokontrolleri tekee siitä fft-muunnoksen, joka voidaan esittää ledeillä.
Tähän merkkiin ei otettu edes mahdollisuudeksi käyttää 9 voltin paristolla, vaan käyttöjännite otetaan aina 5 voltin jännitelähteestä (lue: USB-kaapelin kautta varavirtalähteestä).
