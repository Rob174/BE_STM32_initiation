Quels éléments connectés sur la STM32 ?
- Module RF (Reception) : IN
- Accéléromètre : IN 
- Codeur incrémental : IN : 
- Servo-mot : OUT
- Module RF (Emisison) : OUT en FSK envoi asynchrone 433MHz
- Moteur CC : OUT push pull
1.1 Bordage auto	
Quels périph nécessaires ?
- Ecoute : Servo-moteur (0 port 3 ; 5V port 1; commande port 2 PWM PB8 ou PA8) : push pull 
- Girouette : Codeur incrémental ; 360 périodes pr un tour ; VCC sur port 1 ; CHA : PA6 : port 2 pull up ; CHB : PA7 : 3 pull up ; Index PA5 Port 4 ; GND : port 5 ; 
Quels périph en particulier ?
- Servo-mot : Futaba S3003 en PWM ; 0 5V par batterie ; tension PWM : Timer + GPIO en alternate output pushw
- Codeur incrmental : AEDB-9140 (Agilent) 
1.2. Orientation du voilier
Périphs nécessaires :
- Module RF de communication : RF TELECO : Vcc port 1 ; CH1 PB6 : port 2 ; CH2 PB7 : port 3 ; GND port 4
- Moteur CC : 1 Port PWM en output push pull ; 1 Port sens en push pull aussi (choix) port PA2 ; Port PWM port 2 ; GND port 3
1.3 Système anti chavirement
- Accéléromètre : A6271 K633027 ; Vcc port 1 ; X : PC0 port2 ; Y PC1 port 3 ADC; GND port 4 (tt en ADC)
- Ecoute : cf 1.1
1.4 Transmission d'infos
- Module RF : Emetteur : PA9 : port 2 ; GND port 6 ; PA11 port 8 TX ; Port 9 et 10 à la masse
- RTC DS1307 : communication i2c ; X1 X2 pour placer le crystal ; VBat 
