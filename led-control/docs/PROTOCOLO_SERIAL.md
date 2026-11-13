# Protocolo serial

Parámetros: `115200`, 8 bits, sin paridad, 1 bit de parada. Los mensajes pueden
terminarse con `LF` (`\n`), `CR` (`\r`) o `CRLF` (`\r\n`).

Flutter envía una orden completa:

```text
SET;low=1;high=0;left=0;right=0;hazard=0;brake=0
```

El firmware responde con confirmación y estado aplicado:

```text
ACK;low=1;high=0;left=0;right=0;hazard=0;brake=0
STATE;low=1;high=0;left=0;right=0;hazard=0;brake=0
```

`GET` solicita el estado actual. Los valores válidos son únicamente `0` y `1`. Una línea incompleta, demasiado larga o con campos desconocidos produce, por ejemplo, `ERR;code=INVALID_COMMAND`, sin cambiar el estado vigente. `brake` controla la luz adicional de freno en D27; la luz de posición de D26 permanece encendida.

`brake` es independiente de `low` y `high`: se puede enviar `brake=1` junto
con cualquiera de las dos luces. La exclusión aplica únicamente entre luz baja
y luz alta.

El estado confirmado es autoritativo: Flutter no presenta una orden como aplicada hasta recibir `ACK`/`STATE`. `left=1` y `right=1` no se ejecutan juntos; `hazard=1` activa ambas salidas y cancela los giros individuales.
