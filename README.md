# OrtoYun - InternetOfGreens

## Intro

L'ortoYun è la verisone IoG ( Internet of Green ) di un orto tradizionale o idroponico gestito dall'Arduino Yùn.
La versione corrente è evoluzione della verione basata su Arduino Mega2560 ed Ethernet shield

## Caratteristiche/Obiettivi del progetto:

- Ora corrente sincronizzata via ntpd Linux;
- Dati sensori ambiente raccolti in JSON da SmartCitizen project;
- Collezione dati su SD Card come datalogging;
- Accesso e Visaulizzazione dei dati via web;

## Sensori/Attuatori presenti:

- n.4 relay a 220v per il controllo di 2 serie di luci e 2 pompe;
- n.1 sistema di fertilizzazione con elettrovalvola;
- n.1 pH Meter 
- n.2 sensori EC ( Conducibilità elettrica dell'acqua );

## Valori monitorati direttamente da sensori connessi ad Arduno Yùn:

- Controllo Ph dell'acqua;
- Misura del livello EC dell'acqua;
- Misura del livello di acqua;

## Valori monitorati da sensori del progetto Smart Citizen:

- Temperatura
- Umidità
- Co
- No2
- luminosità ambiente
- inquinamento acustico
- stato batteria

## Attività automatiche presenti:

- Gestione cicli di accensione/spegnimento lampade;
- Gestione cicli di irrigazione;
- Gestione ciclo di fertilizzazione dell'acqua;

## Prerequisiti - Package Required

opkg install php5 php5-cgi php5-cli php5-mod-curl php5-mod-json

