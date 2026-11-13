# Control de luces Arduino

Proyecto educativo de baja tensión para controlar la iluminación de una maqueta automotriz mediante una aplicación Flutter para Android y un Arduino Mega 2560 conectado por USB OTG.

La aplicación permite controlar:

- Luz baja y luz alta.
- Guiño izquierdo y derecho.
- Balizas/intermitentes de emergencia.
- Luz de freno.

La app inicia en modo demostración, funciona en orientación horizontal y sincroniza el estado real del Arduino mediante respuestas `ACK` y `STATE`. Si el USB OTG se desconecta durante el funcionamiento, intenta detectar y reconectar automáticamente el Arduino.

## Estructura

```text
control de luces arduino/
├── control_luces/   # Aplicación Flutter para Android
└── led-control/     # Firmware PlatformIO para Arduino Mega 2560
```

## Requisitos

- Arduino Mega 2560.
- Teléfono Android compatible con USB Host/OTG.
- Cable o adaptador USB OTG.
- Flutter SDK para compilar la aplicación.
- PlatformIO para compilar y cargar el firmware.

## Pines del Arduino

| Función | Pin Mega 2560 |
|---|---:|
| Freno de posición | D26 |
| Luz de freno | D27 |
| Guiño izquierdo | D22 |
| Guiño derecho | D23 |
| Luz alta | D24 |
| Luz baja | D25 |

Las salidas de alta y baja están configuradas como activas en `HIGH`. La luz alta y la baja son mutuamente excluyentes, pero ambas pueden funcionar junto con el freno.

> Este proyecto está pensado para una maqueta didáctica. No conectar las salidas directamente al sistema eléctrico de un automóvil real ni utilizar cargas de potencia sin transistores/MOSFET, fuente adecuada y protección eléctrica.

## Firmware

Desde el directorio `led-control/`:

```bash
pio run
pio run --target upload
pio device monitor --baud 115200
```

El firmware utiliza comunicación serial a `115200` baudios, 8 bits, sin paridad y 1 bit de parada.

## Aplicación Android

Desde el directorio `control_luces/`:

```bash
flutter pub get
flutter run
```

Para generar un APK:

```bash
flutter build apk --release
```

La aplicación requiere permiso USB para acceder al Arduino. Con el dispositivo conectado, abrir la app y pulsar el botón de conexión. La interfaz está bloqueada en modo landscape.

## Protocolo serial

Flutter envía órdenes completas al firmware:

```text
SET;low=1;high=0;left=0;right=0;hazard=0;brake=0
```

El firmware responde con el estado aplicado:

```text
ACK;low=1;high=0;left=0;right=0;hazard=0;brake=0
STATE;low=1;high=0;left=0;right=0;hazard=0;brake=0
```

También se puede enviar `GET` para solicitar el estado actual. Los mensajes aceptan terminadores `LF`, `CR` o `CRLF`.

Documentación detallada:

- [Firmware Arduino](./led-control/README.md)
- [Instalación Android y USB OTG](./control_luces/docs/INSTALACION_ANDROID.md)
- [Conexión eléctrica](./led-control/docs/CONEXION_ELECTRICA.md)
- [Protocolo serial](./led-control/docs/PROTOCOLO_SERIAL.md)
- [Pruebas](./led-control/docs/PRUEBAS.md)
