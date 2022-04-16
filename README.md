# susi

Software del cambio di TaurusX

## Build

Per la compilazione su ESP32 sono necessari:

- PlatformIO core 5.2.5+
- mklittlefs 0.2.3+
- nodejs 16+
- python 3+
- git

Clonare il repo e il submodule con:

```shell
git clone --recurse-submodules https://github.com/policumbent/susi
```

Build dell'interfaccia web con nodejs:

```shell
cd susi/ui
npm install && npm run build
cp -rf public/* ../data
```

Flash del progetto sull'esp:

```shell
pio run --target upload -e esp32dev
```

e load del filesystem:

```shell
pio run --target uploadfs -e esp32dev
```

## Usage

La calibrazione si attiva/disattiva premendo per 2 secondi il tasto `marcia su`, 
quando è attiva il led blu della scheda è acceso. Collegarsi alla rete wifi dell'esp
e andare all'indirizzo locale `192.168.4.1` per avviare l'interfaccia.

## Licence

[GPLv3](LICENSE)
