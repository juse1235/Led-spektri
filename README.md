# Led-spektri
![GitHub Logo](/Kuvat/Kuva1.png)

Tuntuiko normaali haalarisalama liian yksinkertaiselta? Epäiletkö, että juotettavia LEDejä ei voi olla liikaa? Oletko aina haaveillut haalarimerkistä, jonka nähtyään kanssaopiskelijat pyörtyvät ihastuksesta?

Mikäli näin on, olet selvästikin motivoitunut nousemaan seuraavalle tasolle yöllisessä yökerhojen ja pystärien bling-bling-loisteessa. Tämä tanssilattioiden katseenvangitsija tarjoaa ainutkertaisen mahdollisuuden turruttaa itsensä vinoon juottuneille komponenteille ja v*tutukselle. Älä missaa tilaisuutta! Minä en missaisi.

Komponentit

| Tyyppi          |	Määrä  |	Huom                                                    |
| --------------- | ------ | -------------------------------------------------------- |
| Atmega328PB-AU  |1       |	Myös 328P voi toimia, mutta vaatii muutoksia kääntäjään |
| LED, 3 mm, jalallinen	|64 |	Ei värikoodia – käytä mielikuvitusta |
| Transistori, SOT-23|16 |	Käytössä MMBT4401, ei erityisen tarkkaa mallista, hyvä kuitenkin kestää vähintään 20 mA < virtaa |
| Vastus, 0603 – 0805, 1+ kOhm | 16 |		Transistorien ohjaus |
| Etuvastus, 0805	|8	| Kirkkauden säätö, 100 – 500 ohmia lienee hyvä, säätää myös virrankulutusta, tekijällä käytössä 300 ohmia |
| Isohko vastus, noin 5 kOhm, 0603 |2	 | |
| Regulaattori, LP2985 – 3.3 |	1	| |
| Kondensaattori, 100 nF, 0805|	1	| |
| Kondensaattori, 10 uF, 0805|	1	| |
| Sulake, 500 mA, 1206	|1	| |
| Oskillaattori, 16 MHz |	1 |	Käytössä CSTCE16M0V53-R0 |
| MAX4466-mikrofonimoduuli |	1 |	Mikrofoneja löytyy eri herkkyyksillä, voi olla tuurissaan, mikä on käyttökelpoinen. Electret sinisellä piirilevyllä on toiminut toistaiseksi.|


## Yleinen toimintaperiaate
Ledien toimintaperiaate on samankaltainen kuin haalarisalamassakin, eli ledejä vilkutetaan matriisissa sarakkeittain säätämällä, mikä sarake saa yhteyden maahan ja suljettua virtapiirin. 

Mikrofoni kuuntelee ääntä, ja mikrokontrolleri lukee siitä tulevaa analogista signaalia jatkuvalla syötöllä. Näytepätkän ollessa valmis mikrokontrolleri tekee siitä fft-muunnoksen, joka voidaan esittää ledeillä.

Tähän merkkiin ei otettu edes mahdollisuudeksi käyttää 9 voltin paristolla, vaan käyttöjännite otetaan aina 5 voltin jännitelähteestä (lue: USB-kaapelin kautta varavirtalähteestä).

## Tarkempi piirikaavion analyysi
Piirikaavion olisi voinut päivittää paljon helpommin luettavaksi ja oikeille arvoille, jos Kicad 5.0 ei olisi niin epäyhteensopiva Kicad 4 -versioiden kanssa. 

Käyttöjännite tulee mikrokontrollerille ja 3,3 voltin regulaattorille sulakkeen kautta. Sulakkeen tehtävänä on luonnollisesti edes hieman suojata komponentteja oikosuluilta sun muilta lapsuksilta. Tässä otettiin mallia Arduinosta. Regulaattorin tarkoituksena on vain ja ainoastaan tuottaa tasaista käyttöjännitettä mikrofonimoduulille ja asettaa analoginen referenssitaso mikrokontrollerille mikrofonimoduulin analogisen signaalin lukua varten. 10 mikrofaradin kondensaattori C1 tasoittaa käyttöjännitteen pahimmat häiriöt, ja kondensaattori C_small (oikea arvo 100 nF) tekee saman 3,3 voltin linjalle.

Toisin kuin haalarisalamassa, tässä projektissa tarvitaan transistoreita rajoittamaan mikrokontrollerin kuormaa. Riittävän kirkkauden takaamiseksi ledin virraksi on hyvä määritellä noin 15 mA. Transistoreiksi päätyivät MMBT3904-transistorit, jotka perustuvat NPN-teknologiaan. Lähtökohtaisesti MOSFET-transistorienkin pitäisi toimia ihan yhtä hyvin, kunhan paketointi on sama. Transistorit vaativat vastukset kantavirran rajoittamiseksi (1 kOhmin vastukset piirikaaviossa ja jäljempänä esitetyssä piirilevyssä). Todennäköisesti arvoltaan suuremmatkin vastukset toimisivat aivan yhtä hyvin. 

##### HUOM!Projektia kokeiltiin toteuttaa myös puolella transistorimäärällä, mutta mikrokontrolleri ei kyennyt antamaan riittävästi virtaa yhden sarakkeen ledeille yhtä aikaa. Tämän korjaaminen vaatii joko todella herkkiä ja kirkkaita ledejä ja/tai koodimuutoksia, jolloin ainoastaan yksi led olisi samaan yhtä aikaa päällä (nyt maksimissaan kahdeksan).

Oskillaattori on geneerinen 16 megahertsin malli sisäänrakennetulla kuormakapasitanssilla, jollaisia näkee muun muassa Arduino Nanossa. Myös 20 megahertsin kidettä voi käyttää, mutta se vaatii muutoksia koodiin ja Atmegan… ööö, fuse… sulake ei liene oikea termi. Joka tapauksessa ”fuse”:ille pitää löytää oikeat asetukset.

Yksinkertaisuuden vuoksi mikrofonin kanssa päädyttiin vahvistimen sisältävään valmiiseen moduuliin. Moduuli tuottaa analogista signaalia, joka välitetään Atmegan ADC-pinniin, ja josta Atmega ottaa näytepätkän.

![GitHub Logo](/Kuvat/Kuva2.png)
Kuten tarkimmat jo saattavat huomata, tekijä ei osannut tekovaiheessa tehdä ihmisystävällisiä kytkentöjä.

## Piirilevy
Tilanpuute oli valtava ongelma varsinaisen piirilevyn suunnittelun kanssa. Tästä syystä kaikkia komponentteja ei voitu sijoittaa levyn takapuolelle helpon korjattavuuden takaamiseksi, vaan osa komponenteista ja mikrofonimoduuli oli juotettava etupuolelle.

Yleinen ohje juottamiseen: juota ensin yksi komponentin nurkka kiinni niin, että komponentin muutkin juotoskohdat ovat paikallaan ja komponentti on oikeassa asennossa. Tämän jälkeen voi helposti juottaa loput paikat.

Juottaminen on hyvä aloittaa mikrokontrollerista. Tekijä juotti piirin reippaalla juoksutteella ja reippaalla tinankäytöllä, jolloin myös kontrollerin jalat olivat juottuneet yhteen. Tämän jälkeen ylimääräinen tina oli helppoa imeä tinaimunauhalla.

Muiden pintaliitoskomponenttien juottamisessa ei ole varsinaisesti mitään mainittavaa. U2 alemmassa kuvassa on 3,3 voltin regulaattori.
Kun pintaliitoskomponentit on juotettu, J7 ja J6 on yhdistettävä hyppylangalla. Miksi näin? Tilanpuutteen takia maatasoa ei ole muuten yhdistetty kontrollerin yläpuolelle ollenkaan. Onneksi tämä on matalan taajuuden sovellutus, joten tämmöiset temput ovat sallittuja...

Etupuolella on mainittava ledien juottamisesta sen verran, että pitkä jalka (anodi) menee ympyrän muotoiseen juotospisteeseen, ja lyhyt jalka (katodi) menee neliön muotoiseen juotospisteeseen. Kokemuksen jykevällä rintaäänellä voi sanoa, että juottakaa ledit myös mieluummin etupuolelle kuin takapuolelle. Mikrofonimoduulin ja piirilevyn väliin on suositeltavaa laittaa jotakin eristettä oikosulun välttämiseksi. Tantaalikondensaattorin viivalla merkitty pääty menee aina positiiviseen jännitteeseen kaiken järjen vastaisesti (etupuolen 10 mikrofaradin konkka). 

![GitHub Logo](/Kuvat/Kuva3.png)
Levyn takapuoli

![GitHub Logo](/Kuvat/Kuva4.png)
Levyn etupuoli


## Ohjelmointi
Ohjelmointi tapahtuu ISP-väylän kautta. ArduinoISP on tässä suositeltava ohjelmoija. Piirilevylle jätettiin ohjelmointipaikat myös UART-ohjelmointia varten, mutta niitä ei tässä tapauksessa tarvita ollenkaan. Tulevaisuuden generaatioista ne voikin poistaa tilaa säästääkseen.

![GitHub Logo](/Kuvat/Kuva5.png)

##### Komennot Windowsille:

avrdude -C "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf" -c avrisp -p m328pb -B3 -P COM7 -b 19200 -U flash:w:merkki.hex

avrdude -C "C:\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf" -c avrisp -p m328pb -B3 -P COM7 -b 19200 -U lfuse:w:0xff:m -U hfuse:w:0xde:m efuse:w:0x05:m

Jos avrdude.exe:ä ei ole asetettu Windowsin ympäristömuuttujiin, on edellä mainitut komennot ajettava ”Arduino\hardware\tools\avr\bin”-kansiosta. Polut ja oikea sarjaportti pitää toki tarkistaa tapauskohtaisesti.

##### Testaamattomat komennot Linuxille ja Macille:

avrdude -c avrisp -p m328pb -B3 -P /dev/ttyUSB0 -b 19200 -U flash:w:merkki.hex

avrdude -c avrisp -p m328pb -B3 -P /dev/ttyUSB0 -b 19200 -U lfuse:w:0xff:m -U hfuse:w:0xde:m efuse:w:0x05:m

Atmel Studio 7:llä tehty koodi ei ole mitään kauneinta katseltavaa, mutta joka tapauksessa se toimii. ChaN AVR FFT -kirjasto on otettu lievästi editoituna AVRFreaks-foorumilta ( https://www.avrfreaks.net/forum/64-point-fft-elm-chan-library), ja kirjasto löytyy myös erilaisina variaatioina eri puolilta internetiä. Alun perin koodi on oletettavasti julkaistu Elm-chanin kotisivulla (http://elm-chan.org/works/akilcd/report_e.html)

## Fyysinen rakenne
Kuumaliimaa kannattaa hyödyntää ainakin varsinaisen mikrofonielementin ja mikrofonimoduulipiirilevyn liitoskohdassa, sillä isku taittaa mikrofonin helposti. Lisäksi käyttöjännitetuloon on hyvä tehdä liimalla vedonpoisto.
Haalarimerkkiin integroimiseen ei ole suoria ohjeita, mutta on suositeltavaa tehdä painettuun merkkiin valmiit reiät. Mielikuvituksen ja insinööritaitojen käyttö on aiheellista, sillä ledien asentaminen merkin läpi haalarisalaman mukaisesti ei ole huollettavuuden takia järkevää.

