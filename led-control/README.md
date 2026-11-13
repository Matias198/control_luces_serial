# Firmware Arduino — Control de luces

Proyecto educativo de baja tensión para Arduino Mega 2560. La aplicación Flutter vive en el proyecto hermano `../control_luces` y se conecta por USB OTG.

## Estructura

- `src/`, `include/`, `platformio.ini`: firmware PlatformIO para `megaatmega2560`.
- `docs/CONEXION_ELECTRICA.md`: cableado propuesto y advertencias.
- `docs/PROTOCOLO_SERIAL.md`: protocolo de texto compartido con Flutter.

## Firmware

```bash
pio run
pio run --target upload
pio device monitor --baud 115200
```

También puede abrirse el proyecto con Arduino IDE seleccionando Mega 2560; PlatformIO es la configuración reproducible recomendada.
