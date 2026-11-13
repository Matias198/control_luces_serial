# Conexión eléctrica propuesta

Esta tabla es una asignación inicial configurable, no una confirmación del circuito construido.

| Función | Mega 2560 | Conexión de maqueta |
|---|---:|---|
| Freno de posición (siempre encendido) | D26 | LED rojo + resistencia limitadora hacia GND |
| Luz de freno al presionar botón | D27 | LED rojo + resistencia limitadora hacia GND |
| Giro izquierdo | D22 | LED ámbar + resistencia limitadora hacia GND |
| Giro derecho | D23 | LED ámbar + resistencia limitadora hacia GND |
| Luz alta izquierda y derecha | D24 | LEDs + resistencias limitadoras hacia GND |
| Luz baja izquierda y derecha | D25 | LEDs + resistencias limitadoras hacia GND |

La luz de posición en D26 queda siempre encendida. La luz adicional de freno en D27 se activa desde el botón de freno de Flutter mediante `brake=1`.

Las salidas de luz alta y baja están configuradas como activas en `HIGH`: D24 o D25 quedan en `HIGH` cuando la luz correspondiente está encendida y en `LOW` cuando está apagada.

Cada LED requiere su propia resistencia. No conectar tiras, lámparas o cargas de potencia directamente a los pines: usar transistor/MOSFET y una fuente adecuada. Compartir GND cuando corresponda. Es una maqueta didáctica, no debe conectarse al cableado, batería, ECU ni iluminación de un automóvil real.

El USB del Mega ocupa el puerto serial principal asociado a D0/D1; no usar esos pines para LEDs.
